#include "global.h"
#include "crt0.h"
#include "malloc.h"
#include "m4a.h"
#include "bg.h"
#include "rtc.h"
#include "scanline_effect.h"
#include "overworld.h"
#include "play_time.h"
#include "random.h"
#include "dma3.h"
#include "load_save.h"
#include "gpu_regs.h"
#include "sound.h"
#include "battle.h"
#include "battle_controllers.h"
#include "text.h"
#include "intro.h"
#include "main.h"
#include "test_runner.h"
#include "constants/rgb.h"
#include "main_menu.h"
#include "AgbAccuracy.h"
#include "palette_effects.h"
#include "palette.h"
#include "trig.h"
#include "constants/event_objects.h"


static void VBlankIntr(void);
static void HBlankIntr(void);
static void VCountIntr(void);
static void SerialIntr(void);
static void IntrDummy(void);
static void GamePakCrashHandler(void);

// Defined in the linker script so that the test build can override it.
extern void gInitialMainCB2(void);
extern void CB2_FlashNotDetectedScreen(void);
extern void CB2_TestResultCallback(void);

const u8 gGameVersion = GAME_VERSION;

const u8 gGameLanguage = GAME_LANGUAGE; // English

const char BuildDateTime[] = "2025 09 28 23:59";

#define RELOAD_OFFSET 31

__attribute__((used))
static const char sSaveTypeId[] __attribute__((aligned(4))) = "SRAM_V130"; // Some emulators and flash carts will look for magic strings to determain save type 

// This won't actually be used
void Timer3Intr(void)
{
    REG_TM3CNT_H &= ~TIMER_ENABLE;
    REG_TM3CNT_L = -197;
}

const IntrFunc gIntrTableTemplate[] =
{
    VCountIntr, // V-count interrupt
    SerialIntr, // Serial interrupt
    Timer3Intr, // Timer 3 interrupt
    HBlankIntr, // H-blank interrupt
    VBlankIntr, // V-blank interrupt
    IntrDummy,  // Timer 0 interrupt
    IntrDummy,  // Timer 1 interrupt
    IntrDummy,  // Timer 2 interrupt
    IntrDummy,  // DMA 0 interrupt
    IntrDummy,  // DMA 1 interrupt
    IntrDummy,  // DMA 2 interrupt
    IntrDummy,  // DMA 3 interrupt
    IntrDummy,  // Key interrupt
    GamePakCrashHandler,  // Game Pak interrupt
};

#define INTR_COUNT ((int)(sizeof(gIntrTableTemplate)/sizeof(IntrFunc)))

COMMON_DATA u16 gKeyRepeatStartDelay = 0;
COMMON_DATA struct Main gMain = {0};
COMMON_DATA u16 gKeyRepeatContinueDelay = 0;
COMMON_DATA bool8 gSoftResetDisabled = 0;
COMMON_DATA IntrFunc gIntrTable[INTR_COUNT] = {0};
COMMON_DATA s8 gPcmDmaCounter = 0;
COMMON_DATA void *gAgbMainLoop_sp = NULL;

static EWRAM_DATA u16 sTrainerId = 0;

EWRAM_DATA u8 gSoftResetFlag;
EWRAM_DATA u8 gTestResult;

asm(".arm\n"
    "SetSoftResetVariable:\n"
    "	ldr	r1, =gSoftResetFlag\n"
    "   mov	r0, #0x1\n"
	"   str	r0, [r1]\n"
    "   b 0x8000000\n"
    "SetSoftResetVariableEnd:");

extern const void *SetSoftResetVariable(void);
extern const void *SetSoftResetVariableEnd(void);

static void InitMainCallbacks(void);
static void CallCallbacks(void);
#ifdef BUGFIX
static void SeedRngWithRtc(void);
#endif
static void ReadKeys(void);
static void CB2_PostSoftResetInit(void);
static void CB2_PostSoftResetLoadSlotInit(void);
void InitIntrHandlers(void);
static void WaitForVBlank(void);
void EnableVCountIntrAtLine150(void);
void InitFastHashRAMLUT(void);

#define B_START_SELECT (B_BUTTON | START_BUTTON | SELECT_BUTTON)

static void GamePakCrashHandler(void)
{
    DebugPrintfLevel(MGBA_LOG_ERROR, "GamePak crash interrupt triggered!");

    if (SlotIsValid(SAVE_AUTO_2))
    {
        ReloadSlot(SAVE_AUTO_2);
    }
    else
    {
        GlobalSaveHeader *hdr = (GlobalSaveHeader *)FRAM_BASE;
        ReloadSlot(hdr->lastSaveSlot);
    }
}

void AgbMain(void)
{
    gSoftResetFlag = *(vu8 *)0x3007FFA;

    *(vu16 *)BG_PLTT = RGB_WHITE; // Set the backdrop to white on startup
    InitGpuRegManager();
    REG_WAITCNT = WAITCNT_PREFETCH_ENABLE
	        | WAITCNT_WS0_S_1 | WAITCNT_WS0_N_3
	        | WAITCNT_WS1_S_1 | WAITCNT_WS1_N_3;
    InitKeys();
    InitIntrHandlers();

    u8 testLocal = 0xFF;

    #ifndef NDEBUG
    #if (LOG_HANDLER == LOG_HANDLER_MGBA_PRINT)
        (void) MgbaOpen();
    #elif (LOG_HANDLER == LOG_HANDLER_AGB_PRINT)
        AGBPrintInit();
    #endif
    #endif
    
    if (gSoftResetFlag == 0)
        testLocal = RunAgbAccuracyTests();

    m4aSoundInit();
    EnableVCountIntrAtLine150();
    RtcInit();
    InitMainCallbacks();
    InitMapMusic();
#ifdef BUGFIX
    SeedRngWithRtc(); // see comment at SeedRngWithRtc definition below
#endif
    ClearDma3Requests();
    ResetBgs();
    SetDefaultFontsPointer();
    InitHeap(gHeap, HEAP_SIZE);
    InitFastHashRAMLUT();

    gSoftResetDisabled = FALSE;



    gTestResult = testLocal;

    gAgbMainLoop_sp = __builtin_frame_address(0);
    AgbMainLoop();
}

void AgbMainLoop(void)
{
    for (;;)
    {
        ReadKeys();

        if (gSoftResetDisabled == FALSE
         && JOY_HELD_RAW(A_BUTTON)
         && JOY_HELD_RAW(B_START_SELECT) == B_START_SELECT)
        {
            DoSoftReset();
        }

        CallCallbacks();

        PlayTimeCounter_Update();
        MapMusicMain();
        WaitForVBlank();
    }
}

static void InitMainCallbacks(void)
{
    gMain.vblankCounter1 = 0;
    gMain.vblankCounter2 = 0;
    gMain.callback1 = NULL;
    
    if (gSoftResetFlag >= RELOAD_OFFSET)
    {
        SetMainCallback2(CB2_PostSoftResetLoadSlotInit);
    }
    else if(gSoftResetFlag == 1)
    {
        SetMainCallback2(CB2_PostSoftResetInit);
    }
    else
    {
        SetMainCallback2(gInitialMainCB2);
    }

    gSaveBlock2Ptr = &gSaveblock2.block;
}

static void CallCallbacks(void)
{
    if (gMain.callback1)
        gMain.callback1();

    if (gMain.callback2)
        gMain.callback2();
}

void SetMainCallback2(MainCallback callback)
{
    gMain.callback2 = callback;
    gMain.state = 0;
}

void StartTimer1(void)
{

    REG_TM2CNT_L = 0;
    REG_TM2CNT_H = TIMER_ENABLE | TIMER_COUNTUP;
    REG_TM1CNT_H = TIMER_ENABLE;
}

void SeedRngAndSetTrainerId(void)
{
    u32 val;

    REG_TM1CNT_H = 0;
    REG_TM2CNT_H = 0;
    val = ((u32)REG_TM2CNT_L) << 16;
    val |= REG_TM1CNT_L;
    SeedRng(val);
    sTrainerId = Random();
}

u16 GetGeneratedTrainerIdLower(void)
{
    return sTrainerId;
}

void EnableVCountIntrAtLine150(void)
{
    u16 gpuReg = (GetGpuReg(REG_OFFSET_DISPSTAT) & 0xFF) | (150 << 8);
    SetGpuReg(REG_OFFSET_DISPSTAT, gpuReg | DISPSTAT_VCOUNT_INTR);
    EnableInterrupts(INTR_FLAG_VCOUNT);
}

// FRLG commented this out to remove RTC, however Emerald didn't undo this!
#ifdef BUGFIX
static void SeedRngWithRtc(void)
{
    #define BCD8(x) ((((x) >> 4) & 0xF) * 10 + ((x) & 0xF))
    u32 seconds;
    struct SiiRtcInfo rtc;
    RtcGetInfo(&rtc);
    seconds =
        ((HOURS_PER_DAY * RtcGetDayCount(&rtc) + BCD8(rtc.hour))
        * MINUTES_PER_HOUR + BCD8(rtc.minute))
        * SECONDS_PER_MINUTE + BCD8(rtc.second);
    SeedRng(seconds);
    #undef BCD8
}
#endif

void InitKeys(void)
{
    gKeyRepeatContinueDelay = 5;
    gKeyRepeatStartDelay = 40;

    gMain.heldKeys = 0;
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = 0;
    gMain.heldKeysRaw = 0;
    gMain.newKeysRaw = 0;
}

static void ReadKeys(void)
{
    u16 keyInput = REG_KEYINPUT ^ KEYS_MASK;
    gMain.newKeysRaw = keyInput & ~gMain.heldKeysRaw;
    gMain.newKeys = gMain.newKeysRaw;
    gMain.newAndRepeatedKeys = gMain.newKeysRaw;

    // BUG: Key repeat won't work when pressing L using L=A button mode
    // because it compares the raw key input with the remapped held keys.
    // Note that newAndRepeatedKeys is never remapped either.

    if (keyInput != 0 && gMain.heldKeys == keyInput)
    {
        gMain.keyRepeatCounter--;

        if (gMain.keyRepeatCounter == 0)
        {
            gMain.newAndRepeatedKeys = keyInput;
            gMain.keyRepeatCounter = gKeyRepeatContinueDelay;
        }
    }
    else
    {
        // If there is no input or the input has changed, reset the counter.
        gMain.keyRepeatCounter = gKeyRepeatStartDelay;
    }

    gMain.heldKeysRaw = keyInput;
    gMain.heldKeys = gMain.heldKeysRaw;

    // Remap L to A if the L=A option is enabled.
    if (gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_L_EQUALS_A)
    {
        if (JOY_NEW(L_BUTTON))
            gMain.newKeys |= A_BUTTON;

        if (JOY_HELD(L_BUTTON))
            gMain.heldKeys |= A_BUTTON;
    }

    if (JOY_NEW(gMain.watchedKeysMask))
        gMain.watchedKeysPressed = TRUE;
}

void InitIntrHandlers(void)
{
    int i;

    for (i = 0; i < INTR_COUNT; i++)
        gIntrTable[i] = gIntrTableTemplate[i];

    INTR_VECTOR = IntrMain;

    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
    SetSerialCallback(NULL);

    REG_IME = 1;

    EnableInterrupts(INTR_FLAG_VBLANK);
}

void SetVBlankCallback(IntrCallback callback)
{
    gMain.vblankCallback = callback;
}

void SetHBlankCallback(IntrCallback callback)
{
    gMain.hblankCallback = callback;
}

void SetVCountCallback(IntrCallback callback)
{
    gMain.vcountCallback = callback;
}

void SetSerialCallback(IntrCallback callback)
{
    gMain.serialCallback = callback;
}

/**
To whom it may concern, forgive me because I have sinned
*********************************************************************************
*/

/*
function gbaToRgb(color) {
  let r = (color & 0x1F) << 3;
  let g = ((color >> 5) & 0x1F) << 3;
  let b = ((color >> 10) & 0x1F) << 3;
  return [r, g, b];
}

function rgbToGba(r, g, b) {
  let rr = Math.round(r / 8);
  let gg = Math.round(g / 8);
  let bb = Math.round(b / 8);
  return (bb << 10) | (gg << 5) | rr;
}

function rgbToHsl(r, g, b) {
  r /= 255; g /= 255; b /= 255;
  let max = Math.max(r, g, b), min = Math.min(r, g, b);
  let h, s, l = (max + min) / 2;
  if (max === min) {
    h = s = 0;
  } else {
    let d = max - min;
    s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
    switch (max) {
      case r: h = (g - b) / d + (g < b ? 6 : 0); break;
      case g: h = (b - r) / d + 2; break;
      case b: h = (r - g) / d + 4; break;
    }
    h /= 6;
  }
  return [h * 360, s, l];
}

function hslToRgb(h, s, l) {
  h /= 360;
  let r, g, b;
  if (s === 0) {
    r = g = b = l;
  } else {
    function hue2rgb(p, q, t) {
      if (t < 0) t += 1;
      if (t > 1) t -= 1;
      if (t < 1/6) return p + (q - p) * 6 * t;
      if (t < 1/2) return q;
      if (t < 2/3) return p + (q - p) * (2/3 - t) * 6;
      return p;
    }
    let q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    let p = 2 * l - q;
    r = hue2rgb(p, q, h + 1/3);
    g = hue2rgb(p, q, h);
    b = hue2rgb(p, q, h - 1/3);
  }
  return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}

let base = [0x162a, 0x15a8, 0x0ecf, 0x0b55, 0x1125, 0x0ce3, 0x08a2];
let palettes = [];
let adjustmentAmount = 8;

for (let shift = 0; shift < 7; shift++) {
  let row = base.map(color => {
    let [r, g, b] = gbaToRgb(color);
    let [h, s, l] = rgbToHsl(r, g, b);
    h = (h + shift * adjustmentAmount) % 360; // shift hue by 15° per row
    let [nr, ng, nb] = hslToRgb(h, s, l);
    return rgbToGba(nr, ng, nb);
  });
  palettes.push(row);
}

// Print nicely formatted for C
console.log("static const u16 baseColors[7][7] = {");
for (let row of palettes) {
  let formatted = row.map(c => "0x" + c.toString(16).padStart(4, "0"));
  console.log("    {" + formatted.join(", ") + "},");
}
console.log("};");
*/

#define BG_PALETTE ((volatile u16*)0x5000000)
#define PALETTE_0_1 ((16 * 0) + 0x01)
#define PALETTE_0_2 ((16 * 0) + 0x02)
#define PALETTE_0_3 ((16 * 0) + 0x05)
#define PALETTE_0_4 ((16 * 0) + 0x06)
#define PALETTE_0_5 ((16 * 0) + 0x03)
#define PALETTE_0_6 ((16 * 0) + 0x04)
#define PALETTE_0_7 ((16 * 0) + 0x09)

#define PALETTE_1_1 ((16 * 1) + 0x02)
#define PALETTE_1_6 ((16 * 1) + 0x06)

#define PALETTE_2_1 ((16 * 2) + 0x02) 
#define PALETTE_2_2 ((16 * 2) + 0x03) 
#define PALETTE_2_3 ((16 * 2) + 0x04) 
#define PALETTE_2_4 ((16 * 2) + 0x05) 
#define PALETTE_2_5 ((16 * 2) + 0x06) 
#define PALETTE_2_6 ((16 * 2) + 0x07) 
#define PALETTE_2_7 ((16 * 2) + 0x08)

#define PALETTE_3_1 ((16 * 3) + 0x01)
#define PALETTE_3_2 ((16 * 3) + 0x02)
#define PALETTE_3_4 ((16 * 3) + 0x04)
#define PALETTE_3_5 ((16 * 3) + 0x05)
#define PALETTE_3_11 ((16 * 3) + 0x0B)

#define PALETTE_4_1 ((16 * 4) + 0x01) 
#define PALETTE_4_7 ((16 * 4) + 0x07) 
#define PALETTE_4_9 ((16 * 4) + 0x09) 

#define PALETTE_5_1  ((16 * 5) + 0x01)
#define PALETTE_5_2  ((16 * 5) + 0x02)
#define PALETTE_5_3  ((16 * 5) + 0x03)
#define PALETTE_5_5  ((16 * 5) + 0x05)
#define PALETTE_5_8  ((16 * 5) + 0x08)
#define PALETTE_5_9  ((16 * 5) + 0x09)
#define PALETTE_5_10 ((16 * 5) + 0x0A)

#define PALETTE_9_9  ((16 * 9) + 0x09)
#define PALETTE_9_10 ((16 * 9) + 0x0A)

#define PALETTE_10_9  ((16 * 10) + 0x09)
#define PALETTE_10_10 ((16 * 10) + 0x0A)


COMMON_DATA u16 baseColorsRAM[7][7] = {0};

static const u16 baseColors[7][7] = {
    {0x162a, 0x15a8, 0x0ecf, 0x0b55, 0x1125, 0x0ce3, 0x08a2}, 
    {0x1628, 0x15a7, 0x0ecd, 0x0b52, 0x1124, 0x10e3, 0x08a2}, 
    {0x1627, 0x15a6, 0x0eca, 0x0b4f, 0x1124, 0x10e3, 0x0ca2}, 
    {0x1625, 0x15a5, 0x0ec7, 0x0b4b, 0x1524, 0x14e3, 0x0ca2}, 
    {0x1a25, 0x19a5, 0x0ec5, 0x0b48, 0x1924, 0x14e3, 0x10a2}, 
    {0x2225, 0x1da5, 0x12c3, 0x0b45, 0x1924, 0x18e3, 0x10a2}, 
    {0x2a25, 0x21a5, 0x1ac3, 0x0b42, 0x1d24, 0x18e3, 0x10a2}, 
};

// One or two calls is fine but this is a bit slow to do multimple times on VBlank so can just make a lut
// static inline uint32_t fast_hash32(uint32_t x)
// {
//     x ^= x >> 15;
//     x *= 0x297a2d39; // We specifically DO want a kinda biased hash function for the wavy look https://nullprogram.com/blog/2018/07/31/ 
//     x ^= x >> 12;
//     return x;
// }

COMMON_DATA uint32_t fastHashRAMLUT[168] = {0};

static const uint32_t fastHashLUT[168] = {
    0x00000000,     0x2978ba9b,     0x52f17537,     0x7c694143,     0xa5e2ea6f,     0xcf6e1433,     0xf8d28286,     0x225519fc,
    0x4bd5d4de,     0x754cc3b8,     0x9ecc2866,     0xc833728c,     0xf1b5050d,     0x1b35f8a1,     0x44aa33f9,     0x6e2e44dd,
    0x97aba9bd,     0xc1111119,     0xea998770,     0x14101a2e,     0x3d8850cc,     0x6703c5f6,     0x9076e518,     0xb9f18fbe,
    0xe37a0a1b,     0x0ceea477,     0x366bf143,     0x5fe73b2f,     0x895467f3,     0xb2dc3204,     0xdc5c89ba,     0x05cb2550,
    0x2f47537a,     0x58ba5fa4,     0x82322232,     0xabbe9589,     0xd5230ee1,     0xfea763b5,     0x2820345d,     0x5199fa61,
    0x7b10a199,     0xa49b7732,     0xce078bec,     0xf78ae0ca,     0x20fdca30,     0x4a7d549a,     0x73f31f7c,     0x9d679b93,
    0xc6e41437,     0xf06da1c3,     0x19dd48ef,     0x4353372b,     0x6cd7e286,     0x96423878,     0xbfce765e,     0xe93124c4,
    0x12b8cfe6,     0x3c37d2f0,     0x65a86409,     0x8f209ea5,     0xb8a91375,     0xe212e759,     0x0b964aa1,     0x35127015,
    0x5e8ea6f4,     0x880dfb2e,     0xb174bf48,     0xdaf47a76,     0x04744464,     0x2deceebe,     0x576d2b13,     0x80ea84e7,
    0xaa561dc3,     0xd3dbd82f,     0xfd5ec76b,     0x26c95300,     0x504068ba,     0x79b801dc,     0xa333f4c2,     0xccbf3fa8,
    0xf6214332,     0x1fa9b58d,     0x4926ee65,     0x729b80b1,     0x9c1f17d9,     0xc59d5afd,     0xef05c195,     0x1884d60a,
    0x41fb9460,     0x6b7f0f4a,     0x94faa934,     0xbe65f5a2,     0xe7e63ef8,     0x11637b93,     0x3adf3727,     0x64508d4b,
    0x8dd8286f,     0xb740562b,     0xe0cb4386,     0x0a3fdefc,     0x33ba91de,     0x5d360440,     0x86a66e56,     0xb023336c,
    0xd9afc50d,     0x031cbda9,     0x2c9470f1,     0x561587dd,     0x7f8cecbd,     0xa90fd111,     0xd2724988,     0xfbf6242e,
    0x25719fcc,     0x4ee918f6,     0x786fa5e0,     0xa1e84ebe,     0xcb50c813,     0xf4d9e667,     0x1e513d4b,     0x47cf79cf,
    0x714226eb,     0x9ab6f404,     0xc435ceb2,     0xedbd6158,     0x172c9542,     0x40ac1e54,     0x6a24e02a,     0x93955681,
    0xbd1d4de9,     0xe69ea0b5,     0x100bf65d,     0x3986bc79,     0x62f97e91,     0x8c71b90a,     0xb5f8f4ec,     0xdf602fd2,
    0x08e888c8,     0x326115a2,     0x5bd9dd7c,     0x855ed89b,     0xaeda5627,     0xd8476ccb,     0x01c509cf,     0x2b3df123,
    0x54bc3b87,     0x7e347e70,     0xa7a7b05e,     0xd12ae5c4,     0xfaad8ed6,     0x241e13e8,     0x4d92a601,     0x7717dd95,
    0xa080d175,     0xca09a741,     0xf37003b9,     0x1cf8ae15,     0x4677e984,     0x6feb452e,     0x996e7f50,     0xc2ee3846,
    0xd9afc50d,     0x031cbda9,     0x2c9470f1,     0x561587dd,     0x7f8cecbd,     0xa90fd111,     0xd2724988,     0xfbf6242e,
}; 

void InitFastHashRAMLUT(void)
{
    for (int i = 0; i < 168; i++)
    {
        fastHashRAMLUT[i] = fastHashLUT[i];
    }
    memcpy(baseColorsRAM, baseColors, sizeof(baseColors));
}


// I'm not really doing palette cycling justice. I recommend this https://www.youtube.com/watch?v=aMcJ1Jvtef0
#define ROTATE_LEN 6 // number of palette entries to rotate
EWRAM_DATA u32 sPalRotationCounter; // the "offset -= ROTATE_LEN;" while loop is a hack that would lag the game over time without this second hack #FIXED 
static inline void RotateObjPalette0(void)
{
    volatile u16 *pal0 = (volatile u16 *)OBJ_PLTT;  // OBJ palette slot 0

    // Div 5, but like, in a way that won't suck that much but still kind of sucks
    u64 temp = (u64)sPalRotationCounter * 0xCCCCCCCDULL;
    u32 offset = temp >> 34;

    while (offset >= ROTATE_LEN) 
        offset -= ROTATE_LEN;

    if (offset == 0)
        return;

    for (u32 j = 0; j < offset; j++)
    {
        u16 temp = pal0[1]; 
        for (int i = 1; i < ROTATE_LEN; i++)
            pal0[i] = pal0[i + 1];  
        pal0[ROTATE_LEN] = temp;
    }
}

EWRAM_DATA u32 sPalWaterCounter; 
static inline u16 ApplyBrightnessToColor(u16 color, s16 delta)
{
    s16 r = (color & 0x1F) + delta;
    s16 g = ((color >> 5) & 0x1F) + delta;
    s16 b = ((color >> 10) & 0x1F) + delta;

    if (r < 0) r = 0; else if (r > 31) r = 31;
    if (g < 0) g = 0; else if (g > 31) g = 31;
    if (b < 0) b = 0; else if (b > 31) b = 31;

    return (b << 10) | (g << 5) | r;
}

#define OBJ_PALETTE ((volatile u16 *)(0x5000000 + 0x200))
#define OBJ_PALETTE_SLOT(n) ((n) * 16)
#define OBJ_PALETTE_INDEX(n, i) (((n) * 16) + (i))

void ApplyVBlankPaletteModifiers()
{
    sPalRotationCounter++;
    if (gMain.hblankPaletteEffect == PALETTE_EFFECT_GRASS) // This is the petals not the grass
    {    
        RotateObjPalette0();    
    } 
    else if (gMain.vblankPaletteEffect == PALETTE_EFFECT_WATER)
    {

        // Increment our slower counter every few frames
        static u8 frameDiv = 0;
        if (++frameDiv >= 3)  // update every 3 frames
        {
            frameDiv = 0;
            sPalWaterCounter++;
        }

        s16 wave = gSineTable[(sPalWaterCounter * 3) & 0xFF] >> 5;

        // Base water edge colors
        u16 lightBase = 0x6291; // lighter blue
        u16 darkBase  = 0x55EA; // darker blue

        // Apply a very small brightness shift
        u16 light = ApplyBrightnessToColor(lightBase,  wave);
        u16 dark  = ApplyBrightnessToColor(darkBase, -wave);

        // outline
        BG_PALETTE[PALETTE_5_3] = light;
        BG_PALETTE[PALETTE_5_5] = dark;

        // some outline + pads
        //BG_PALETTE[PALETTE_4_7] = light;
        BG_PALETTE[PALETTE_4_9] = dark;

        // water flowers + highlights (smaller change, using bitshift)
        //BG_PALETTE[PALETTE_3_4]  = dark;
        BG_PALETTE[PALETTE_3_11] = ApplyBrightnessToColor(0x66D5, wave >> 1);


        // outline
        // BG_PALETTE[PALETTE_5_3] = 0x6291; // 0x6291 lighter blue
        // BG_PALETTE[PALETTE_5_5] = 0x55ea; // 0x55ea darker blue

        // // some outline + pads
        // BG_PALETTE[PALETTE_4_7] = 0x6291; // 0x6291 lighter blue
        // BG_PALETTE[PALETTE_4_9] = 0x55ea; // 0x55ea darker blue

        // // water flowers + highlights
        // BG_PALETTE[PALETTE_3_4] = 0x55ea; // 0x55ea darker blue
        // BG_PALETTE[PALETTE_3_11] = 0x66d5; // 0x66d5 lighter blue
        
        // Cant change this because it also effects the grass (I really need to learn porytile palette overrides)
        //BG_PALETTE[PALETTE_5_2] = 0x15a8;
    }
    else if (gMain.vblankPaletteEffect == PALETTE_EFFECT_RUINS)
    {
        static u16 sCrystalPhase = 0;
        const u16 indexDelta = 2;
        const u8  fracDelta  = 64; // controls interpolation smoothness (64 → 4 substeps)

        s16 palSlot = IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_POOCHYENA);

        sCrystalPhase = (u16)(sCrystalPhase + ((indexDelta << 8) | fracDelta));

        // --- Interpolated sine A ---
        u8 idxA  = (u8)(sCrystalPhase >> 8);
        u8 idxA1 = (u8)(idxA + 1);
        u8 frac  = (u8)(sCrystalPhase & 0xFF);

        s16 v0 = gSineTable[idxA];
        s16 v1 = gSineTable[idxA1];
        s16 waveA = (s16)(v0 + (((s32)(v1 - v0) * frac) >> 8)); // interpolated -255..+255

        // --- Interpolated sine B (phase shifted by 64 indices) ---
        u16 phaseB = (u16)(sCrystalPhase + (64 << 8));
        u8 idxB  = (u8)(phaseB >> 8);
        u8 idxB1 = (u8)(idxB + 1);
        u8 fracB = (u8)(phaseB & 0xFF);

        s16 vb0 = gSineTable[idxB];
        s16 vb1 = gSineTable[idxB1];
        s16 waveB = (s16)(vb0 + (((s32)(vb1 - vb0) * fracB) >> 8));

        // Core wave-to-brightness mapping
        s16 baseBright = waveA >> 6;  // ~ -4..+4
        s16 hueShiftR  =  waveB >> 7; // ~ -2..+2
        s16 hueShiftB  = -(waveB >> 7);

        for (int i = 1; i < 16; i++) // skip transparency index 0
        {
            u16 baseColor = OBJ_PALETTE[OBJ_PALETTE_INDEX(palSlot, i)];

            s16 r = baseColor & 0x1F;
            s16 g = (baseColor >> 5) & 0x1F;
            s16 b = (baseColor >> 10) & 0x1F;

            // Compute perceived brightness (average of RGB roughly)
            s16 avg = (r + g + b) / 3;

            // Scale oscillation depth based on darkness:
            // darker colors (small avg) → larger multiplier (up to ×2)
            // brighter colors (large avg) → smaller multiplier (down to ×0.5)
            //
            // Using an inverse-linear curve: scale = 1.5 - (avg / 64.0)
            // (so darkest colors pulse ~2× more, bright ones ~0.5×)
            s16 depthScale = 96 - (avg * 2); // 0..96 roughly (fixed-point scale /64)
            if (depthScale < 32) depthScale = 32; // clamp min
            if (depthScale > 128) depthScale = 128;

            // Apply scaled brightness oscillation
            s16 brightShift = (baseBright * depthScale) >> 6; // depthScale ~64=1×, 128=2×

            // Apply color modulation
            r += hueShiftR + brightShift;
            g += brightShift;
            b += hueShiftB + brightShift;

            // Clamp to [0, 31]
            if (r < 0) r = 0; else if (r > 31) r = 31;
            if (g < 0) g = 0; else if (g > 31) g = 31;
            if (b < 0) b = 0; else if (b > 31) b = 31;

            OBJ_PALETTE[OBJ_PALETTE_INDEX(palSlot, i)] = (b << 10) | (g << 5) | r;
        }
    }
    if (sPalRotationCounter > 1000)
    {
        sPalRotationCounter = 0;
    }
}

#define NUM_PALETTES 6
#define PALETTE_SIZE 16
#define PALETTE_BYTES (NUM_PALETTES * PALETTE_SIZE)

static inline void applyHBlankPaletteModifiers()
{
    // This is all super time sensitive. It must be inlined <strike>and I suspect only works because of the prefetch buffer</strike> This is now all in ram so the prefetch buffer is no longer used.  
    // I reckon if we could force porytiles to block all the palette slots together 1 or 2 dmas it would be more efficent
    if (gMain.hblankPaletteEffect == PALETTE_EFFECT_GRASS && (REG_VCOUNT < 160 || REG_VCOUNT > 225))
    {
        u16 line = ((REG_VCOUNT + gSaveBlock1Ptr->pos.y) * 160) >> 8; //+ gSaveBlock1Ptr->pos.y;//((REG_VCOUNT + (gSaveBlock1Ptr->pos.y >> 2)) >> BAND_SHIFT) & (NUM_BANDS - 1);//REG_VCOUNT; //^ gMain.vblankCounter1 ;
       
        u16 col1 = baseColorsRAM[fastHashRAMLUT[line] & 0x6][0];
        u16 col2 = baseColorsRAM[fastHashRAMLUT[line + 1] & 0x6][1];
        u16 col3 = baseColorsRAM[fastHashRAMLUT[line + 2] & 0x6][2];
        u16 col4 = baseColorsRAM[fastHashRAMLUT[line + 3] & 0x6][3];
        u16 col5 = baseColorsRAM[fastHashRAMLUT[line + 4] & 0x6][4];
        u16 col6 = baseColorsRAM[fastHashRAMLUT[line + 5] & 0x6][5];
        //u16 col7 = baseColorsRAM[fastHashRAMLUT[line + 6] & 0x6][6];

        // Pack two u16 colors into a u32
        #define PAIR_U16(a, b) (((u32)(b) << 16) | (u16)(a))

        // Palette 0: 7 colors
        ((u32*)&BG_PALETTE[PALETTE_0_1])[0] = PAIR_U16(col1, col2);
        ((u32*)&BG_PALETTE[PALETTE_0_3])[0] = PAIR_U16(col3, col4);
        ((u32*)&BG_PALETTE[PALETTE_0_5])[0] = PAIR_U16(col5, col6);
        // BG_PALETTE[PALETTE_0_7] = col7; // last one by itself

        // Palette 1: 2 colors
        // BG_PALETTE[PALETTE_1_1] = col1;
        // BG_PALETTE[PALETTE_1_6] = col6;

        // Palette 2: 7 colors
        ((u32*)&BG_PALETTE[PALETTE_2_1])[0] = PAIR_U16(col1, col2);
        ((u32*)&BG_PALETTE[PALETTE_2_3])[0] = PAIR_U16(col3, col4);
        ((u32*)&BG_PALETTE[PALETTE_2_5])[0] = PAIR_U16(col5, col6);
        // BG_PALETTE[PALETTE_2_7] = col7;

        // Palette 3: 3 colors
        ((u32*)&BG_PALETTE[PALETTE_3_1])[0] = PAIR_U16(col1, col2);
        //BG_PALETTE[PALETTE_3_5] = col5;

        // Palette 4: 1 color
        // BG_PALETTE[PALETTE_4_1] = col2;

        // Palette 5: 6 colors
        ((u32*)&BG_PALETTE[PALETTE_5_1])[0] = PAIR_U16(col1, col2);
        // BG_PALETTE[PALETTE_5_3] = col3;
        ((u32*)&BG_PALETTE[PALETTE_5_8])[0] = PAIR_U16(col5, col6);
        // BG_PALETTE[PALETTE_5_10] = col7;
    }
}

/**
*********************************************************************************
*/

static void VBlankIntr(void)
{
    gMain.vblankCounter1++;

    if (gMain.vblankCallback)
        gMain.vblankCallback();

    gMain.vblankCounter2++;

    CopyBufferedValuesToGpuRegs();
    ProcessDma3Requests();

    gPcmDmaCounter = gSoundInfo.pcmDmaCounter;

    m4aSoundMain();

    if (!gTestRunnerEnabled && (!gMain.inBattle || !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_RECORDED))))
        AdvanceRandom();

    INTR_CHECK |= INTR_FLAG_VBLANK;
    gMain.intrCheck |= INTR_FLAG_VBLANK;
}

static void HBlankIntr(void)
{
    applyHBlankPaletteModifiers();

    if (gMain.hblankCallback)
        gMain.hblankCallback();

    INTR_CHECK |= INTR_FLAG_HBLANK;
    gMain.intrCheck |= INTR_FLAG_HBLANK;
}

static void VCountIntr(void)
{
    if (gMain.vcountCallback)
        gMain.vcountCallback();

    m4aSoundVSync();
    INTR_CHECK |= INTR_FLAG_VCOUNT;
    gMain.intrCheck |= INTR_FLAG_VCOUNT;
}

static void SerialIntr(void)
{
    if (gMain.serialCallback)
        gMain.serialCallback();

    INTR_CHECK |= INTR_FLAG_SERIAL;
    gMain.intrCheck |= INTR_FLAG_SERIAL;
}

static void IntrDummy(void)
{}

static void WaitForVBlank(void)
{
    gMain.intrCheck &= ~INTR_FLAG_VBLANK;

    if (gPaletteFade.active || PrevPaletteFadeResult() == PALETTE_FADE_STATUS_LOADING)
    {
        VBlankIntrWait();
    }
    else if (gMain.gSpeedupDisabled)
    {
        DebugPrintfLevel(MGBA_LOG_ERROR, "Speedup Disabled");
        VBlankIntrWait();
    }
    else if (!(gMain.heldKeysRaw & R_BUTTON))
    {
        VBlankIntrWait();
    }
}

void SetTrainerHillVBlankCounter(u32 *counter)
{
}

void ClearTrainerHillVBlankCounter(void)
{
}

void DoSoftReset(void)
{
    REG_IME = 0;
    m4aSoundVSyncOff();
    ScanlineEffect_Stop();
    DmaStop(1);
    DmaStop(2);
    DmaStop(3);
    SiiRtcProtect();
    *(vu32 *)0x2000000 = 0xE59F000C; // ldr r0, =0x3007FFA
    *(vu32 *)0x2000004 = 0xE3A01001; // mov r1, #1
    *(vu32 *)0x2000008 = 0xE5C01000; // strb r1, [r0]
    *(vu32 *)0x200000C = 0xE3A00302; // mov r0, #0x8000000
    *(vu32 *)0x2000010 = 0xE12FFF10; // bx r0
    *(vu32 *)0x2000014 = 0x03007FFA; // .pool variable, accessed from 1st instruction write above
    *(vu8 *)0x3007FFA = 1; // now set this address byte to 1 so the GBA knows to jump to RAM instead of ROM.
    SoftReset(RESET_ALL & ~RESET_EWRAM); // Do not reset EWRAM so the code isnt destroyed
}

/**
* This is a softreset except we control which slot we load back into 
*/
void ReloadSlot(u8 slot)
{
    u32 value = RELOAD_OFFSET + slot;
    REG_IME = 0;
    m4aSoundVSyncOff();
    ScanlineEffect_Stop();
    DmaStop(1);
    DmaStop(2);
    DmaStop(3);
    SiiRtcProtect();
    *(vu32 *)0x2000000 = 0xE59F000C; 
    *(vu32 *)0x2000004 = 0xE3A01000 | value; 
    *(vu32 *)0x2000008 = 0xE5C01000; 
    *(vu32 *)0x200000C = 0xE3A00302; 
    *(vu32 *)0x2000010 = 0xE12FFF10; 
    *(vu32 *)0x2000014 = 0x03007FFA; 
    *(vu8 *)0x3007FFA = value; 
    SoftReset(RESET_ALL & ~RESET_EWRAM); 
}

void ClearPokemonCrySongs(void)
{
    CpuFill16(0, gPokemonCrySongs, MAX_POKEMON_CRIES * sizeof(struct PokemonCrySong));
}

static void CB2_PostSoftResetInit(void)
{
    gSoftResetFlag = 0;

    SetSaveBlocksPointers(GetSaveBlocksPointersBaseOffset());
    LoadGameSave(SAVE_NORMAL, LAST_SAVED_SLOT);
    SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
    SetMainCallback2(CB2_InitMainMenu);
}

static void CB2_PostSoftResetLoadSlotInit()
{
    u8 slot = gSoftResetFlag - RELOAD_OFFSET;

    if (slot > SAVE_SLOT_COUNT)
    {
        slot = LAST_SAVED_SLOT;
    }

    gSoftResetFlag = 0;

    DebugPrintfLevel(MGBA_LOG_ERROR, "CB2_PostSoftResetLoadSlotInit");

    SetSaveBlocksPointers(GetSaveBlocksPointersBaseOffset());
    LoadGameSave(SAVE_NORMAL, slot);
    SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
    SetMainCallback2(CB2_ContinueSavedGame);
}