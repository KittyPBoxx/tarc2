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
void InitIntrHandlers(void);
static void WaitForVBlank(void);
void EnableVCountIntrAtLine150(void);

#define B_START_SELECT (B_BUTTON | START_BUTTON | SELECT_BUTTON)

static void GamePakCrashHandler(void)
{
    DebugPrintfLevel(MGBA_LOG_ERROR, "GamePak crash interrupt triggered!");
    DoSoftReset();
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
    
    // TODO: TARC accuracy tests
    if (!gSoftResetFlag)
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

    gSoftResetDisabled = FALSE;

#ifndef NDEBUG
#if (LOG_HANDLER == LOG_HANDLER_MGBA_PRINT)
    (void) MgbaOpen();
#elif (LOG_HANDLER == LOG_HANDLER_AGB_PRINT)
    AGBPrintInit();
#endif
#endif

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
    
    if(gSoftResetFlag)
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
    TryReceiveLinkBattleData();

    if (!gTestRunnerEnabled && (!gMain.inBattle || !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_RECORDED))))
        AdvanceRandom();

    INTR_CHECK |= INTR_FLAG_VBLANK;
    gMain.intrCheck |= INTR_FLAG_VBLANK;
}

static void HBlankIntr(void)
{
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
    VBlankIntrWait();
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

void ClearPokemonCrySongs(void)
{
    CpuFill16(0, gPokemonCrySongs, MAX_POKEMON_CRIES * sizeof(struct PokemonCrySong));
}

static void CB2_PostSoftResetInit(void)
{
    gSoftResetFlag = 0;

    SetSaveBlocksPointers(GetSaveBlocksPointersBaseOffset());
    LoadGameSave(SAVE_NORMAL);
    SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
    SetMainCallback2(CB2_InitMainMenu);
}

