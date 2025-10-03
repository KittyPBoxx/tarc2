#include "global.h"
#include "text.h"
#include "main.h"
#include "malloc.h"
#include "palette.h"
#include "graphics.h"
#include "gpu_regs.h"
#include "bg.h"
#include "decompress.h"
#include "task.h"
#include "window.h"
#include "menu.h"
#include "save.h"
#include "starter_choose.h"
#include "text_window.h"
#include "constants/rgb.h"
#include "string_util.h"
#include "AgbAccuracy.h"

#define MSG_WIN_TOP 12
#define CLOCK_WIN_TOP (MSG_WIN_TOP - 4)

extern const u8 gText_SaveFailedCheckingBackup[];
extern const u8 gText_BackupMemoryDamaged[];
extern const u8 gText_CheckCompleted[];
extern const u8 gText_SaveCompleteGameCannotContinue[];
extern const u8 gText_SaveCompletePressA[];
extern const u8 gText_GamePlayCannotBeContinued[];

extern u8 gTestResult;

// sClockInfo enum
enum
{
    CLOCK_RUNNING,
    DEBUG_TIMER
};

// sWindowIds enum
enum
{
    TEXT_WIN_ID,
    CLOCK_WIN_ID
};

static EWRAM_DATA u16 sSaveFailedType = {0};
static EWRAM_DATA u16 sClockInfo[2] = {0};
static EWRAM_DATA u8 sWindowIds[2] = {0};

static const struct OamData sClockOamData =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct BgTemplate sBgTemplates[3] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 15,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sDummyWindowTemplate[] = { DUMMY_WIN_TEMPLATE };

static const struct WindowTemplate sWindowTemplate_Text[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 13,
        .width = 28,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 1,
    }
};

static const struct WindowTemplate sWindowTemplate_Clock[] =
{
    {
        .bg = 0,
        .tilemapLeft = 14,
        .tilemapTop = 9,
        .width = 2,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 169,
    }
};

static const u8 sClockFrames[8][3] =
{
    { 1, 0, 0 },
    { 5, 0, 0 },
    { 9, 0, 0 },
    { 5, 0, 1 },
    { 1, 0, 1 },
    { 5, 1, 1 },
    { 9, 1, 0 },
    { 5, 1, 0 },
};

static const u8 sSaveFailedClockPal[] = INCBIN_U8("graphics/misc/clock_small.gbapal");
static const u32 sSaveFailedClockGfx[] = INCBIN_U32("graphics/misc/clock_small.4bpp.smol");

static void CB2_SaveFailedScreen(void);
static void CB2_WipeSave(void);
static void CB2_FadeAndReturnToTitleScreen(void);
static void CB2_ReturnToTitleScreen(void);
static void VBlankCB_UpdateClockGraphics(void);

// Although this is a general text printer, it's only used in this file.
static void SaveFailedScreenTextPrint(const u8 *text, u8 x, u8 y)
{
    u8 color[3];

    color[0] = TEXT_COLOR_TRANSPARENT;
    color[1] = TEXT_DYNAMIC_COLOR_6;
    color[2] = TEXT_COLOR_LIGHT_GRAY;
    AddTextPrinterParameterized4(sWindowIds[TEXT_WIN_ID], FONT_NORMAL, x * 8, y * 8 + 1, 0, 0, color, 0, text);
}

void DoSaveFailedScreen(u8 saveType)
{
    SetMainCallback2(CB2_SaveFailedScreen);
    sSaveFailedType = saveType;
    sClockInfo[CLOCK_RUNNING] = FALSE;
    sClockInfo[DEBUG_TIMER] = 0;
    sWindowIds[TEXT_WIN_ID] = 0;
    sWindowIds[CLOCK_WIN_ID] = 0;
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

struct SaveFailedBuffers
{
    ALIGNED(4) u8 tilemapBuffer[BG_SCREEN_SIZE];
    ALIGNED(4) u8 window1TileData[0x200];
    ALIGNED(4) u8 window2TileData[0x200];
};

static EWRAM_DATA struct SaveFailedBuffers *sSaveFailedBuffers = NULL;

static void CB2_SaveFailedScreen(void)
{
    switch (gMain.state)
    {
    case 0:
    default:
        SetVBlankCallback(NULL);
        sSaveFailedBuffers = Alloc(sizeof(*sSaveFailedBuffers));
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetGpuReg(REG_OFFSET_BG3CNT, 0);
        SetGpuReg(REG_OFFSET_BG2CNT, 0);
        SetGpuReg(REG_OFFSET_BG1CNT, 0);
        SetGpuReg(REG_OFFSET_BG0CNT, 0);
        SetGpuReg(REG_OFFSET_BG3HOFS, 0);
        SetGpuReg(REG_OFFSET_BG3VOFS, 0);
        SetGpuReg(REG_OFFSET_BG2HOFS, 0);
        SetGpuReg(REG_OFFSET_BG2VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        DmaFill16(3, 0, VRAM, VRAM_SIZE);
        DmaFill32(3, 0, OAM, OAM_SIZE);
        DmaFill16(3, 0, PLTT, PLTT_SIZE);
        DecompressDataWithHeaderVram(gBirchBagGrass_Gfx, (void *)VRAM);
        DecompressDataWithHeaderVram(gBirchBagTilemap, (void *)(BG_SCREEN_ADDR(14)));
        DecompressDataWithHeaderVram(gBirchGrassTilemap, (void *)(BG_SCREEN_ADDR(15)));
        DecompressDataWithHeaderVram(sSaveFailedClockGfx, (void *)(OBJ_VRAM0 + 0x20));
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
        SetBgTilemapBuffer(0, sSaveFailedBuffers->tilemapBuffer);
        CpuFill32(0, sSaveFailedBuffers->tilemapBuffer, BG_SCREEN_SIZE);
        LoadBgTiles(0, gTextWindowFrame1_Gfx, 0x120, 0x214);
        InitWindows(sDummyWindowTemplate);
        sWindowIds[TEXT_WIN_ID] = AddWindowWithoutTileMap(sWindowTemplate_Text);
        SetWindowAttribute(sWindowIds[TEXT_WIN_ID], 7, (u32)&sSaveFailedBuffers->window1TileData);
        sWindowIds[CLOCK_WIN_ID] = AddWindowWithoutTileMap(sWindowTemplate_Clock);
        SetWindowAttribute(sWindowIds[CLOCK_WIN_ID], 7, (u32)&sSaveFailedBuffers->window2TileData);
        DeactivateAllTextPrinters();
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        LoadPalette(gBirchBagGrass_Pal, BG_PLTT_ID(0), 2 * PLTT_SIZE_4BPP);
        LoadPalette(sSaveFailedClockPal, OBJ_PLTT_ID(0), PLTT_SIZE_4BPP);
        LoadPalette(gTextWindowFrame1_Pal, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
        LoadPalette(gStandardMenuPalette, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        DrawStdFrameWithCustomTileAndPalette(sWindowIds[TEXT_WIN_ID], FALSE, 0x214, 0xE);
        DrawStdFrameWithCustomTileAndPalette(sWindowIds[CLOCK_WIN_ID], FALSE, 0x214, 0xE);
        FillWindowPixelBuffer(sWindowIds[CLOCK_WIN_ID], PIXEL_FILL(1)); // backwards?
        FillWindowPixelBuffer(sWindowIds[TEXT_WIN_ID], PIXEL_FILL(1));
        CopyWindowToVram(sWindowIds[CLOCK_WIN_ID], COPYWIN_GFX); // again?
        CopyWindowToVram(sWindowIds[TEXT_WIN_ID], COPYWIN_MAP);
        SaveFailedScreenTextPrint(gText_SaveFailedCheckingBackup, 1, 0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        EnableInterrupts(1);
        SetVBlankCallback(VBlankCB);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(2);
        ShowBg(3);
        gMain.state++;
        break;
    case 1:
        if (!UpdatePaletteFade())
        {
            SetMainCallback2(CB2_WipeSave);
            SetVBlankCallback(VBlankCB_UpdateClockGraphics);
        }
        break;
    }
}

static void CB2_WipeSave(void)
{
    sClockInfo[CLOCK_RUNNING] = TRUE;
    SetMainCallback2(CB2_FadeAndReturnToTitleScreen);
}

static void CB2_FadeAndReturnToTitleScreen(void)
{
    sClockInfo[CLOCK_RUNNING] = FALSE;

    if (JOY_NEW(A_BUTTON))
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(CB2_ReturnToTitleScreen);
    }
}

static void CB2_ReturnToTitleScreen(void)
{
    if (!UpdatePaletteFade())
    {
        TRY_FREE_AND_SET_NULL(sSaveFailedBuffers);
        if (gGameContinueCallback == NULL) // no callback exists, so do a soft reset.
        {
            DoSoftReset();
        }
        else
        {
            SetMainCallback2((MainCallback)gGameContinueCallback);
            gGameContinueCallback = NULL;
        }
    }
}

static void VBlankCB_UpdateClockGraphics(void)
{
    u32 n = (gMain.vblankCounter2 >> 3) & 7;

    gMain.oamBuffer[0] = sClockOamData;
    gMain.oamBuffer[0].x = 112;
    gMain.oamBuffer[0].y = (CLOCK_WIN_TOP + 1) * 8;

    if (sClockInfo[CLOCK_RUNNING])
    {
        gMain.oamBuffer[0].tileNum = sClockFrames[n][0];
        gMain.oamBuffer[0].matrixNum = (sClockFrames[n][2] << 4) | (sClockFrames[n][1] << 3);
    }
    else
    {
        gMain.oamBuffer[0].tileNum = 1;
    }

    CpuFastCopy(gMain.oamBuffer, (void *)OAM, 4);

    if (sClockInfo[DEBUG_TIMER])
        sClockInfo[DEBUG_TIMER]--;
}

void CB2_FlashNotDetectedScreen(void)
{
    static const struct WindowTemplate textWin[] =
    {
        {
            .bg = 0,
            .tilemapLeft = 3,
            .tilemapTop = 2,
            .width = 24,
            .height = 16,
            .paletteNum = 15,
            .baseBlock = 1,
        }
    };

    if (gMain.state)
        return;

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    LoadBgTiles(0, gTextWindowFrame1_Gfx, 0x120, 0x214);
    DeactivateAllTextPrinters();
    ResetTasks();
    ResetPaletteFade();
    LoadPalette(gTextWindowFrame1_Pal, 0xE0, 0x20);
    LoadPalette(gStandardMenuPalette, 0xF0, 0x20);
    InitWindows(textWin);
    DrawStdFrameWithCustomTileAndPalette(0, TRUE, 0x214, 0xE);
    static const u8 saveFailedMessage[] =_(
        "{COLOR RED}ERROR! {COLOR DARK_GRAY}Flash memory not detected!\n"
        "\n"
        "If playing on an emulator, set your\n"
        "save type setting to\n"
        "Flash 1Mb/128K and reload the ROM.\n"
        "\n"
        "If playing on hardware, your cart\n"
        "does not have a working flash chip.");
    SaveFailedScreenTextPrint(saveFailedMessage, 1, 0);
    TransferPlttBuffer();
    *(u16*)PLTT = RGB(17, 18, 31);
    ShowBg(0);
    gMain.state++;
}

static const u8 gDeviceAccuracy0[] = _("{COLOR RED}Device Accuracy: 0/5  {LEFT_ARROW_2} Awful {EMOJI_ANGRY}");
static const u8 gDeviceAccuracy1[] = _("{COLOR RED}Device Accuracy: 1/5  {LEFT_ARROW_2} Awful {EMOJI_ANGRY}");
static const u8 gDeviceAccuracy2[] = _("{COLOR RED}Device Accuracy: 2/5  {LEFT_ARROW_2} Bad {EMOJI_ANGRY}");
static const u8 gDeviceAccuracy3[] = _("{COLOR RED}Device Accuracy: 3/5  {LEFT_ARROW_2} Bad {EMOJI_ANGRY}");
static const u8 gDeviceAccuracy4[] = _("{COLOR LIGHT_RED}Device Accuracy: 4/5  {LEFT_ARROW_2}  It's OK  {EMOJI_NEUTRAL}");
static const u8 gDeviceAccuracy5[] = _("{COLOR BLUE}Device Accuracy: 5/5  {LEFT_ARROW_2}  Perfect {EMOJI_BIGSMILE}");

static const u8* const deviceAccuracyStrings[6] = {
    gDeviceAccuracy0,
    gDeviceAccuracy1,
    gDeviceAccuracy2,
    gDeviceAccuracy3,
    gDeviceAccuracy4,
    gDeviceAccuracy5
};

void CB2_TestResultCallback(void)
{
    static const struct WindowTemplate textWin[] =
    {
        {
            .bg = 0,
            .tilemapLeft = 3,
            .tilemapTop = 2,
            .width = 24,
            .height = 16,
            .paletteNum = 15,
            .baseBlock = 1,
        }
    };

    if (gMain.newKeys & START_BUTTON)
        DoSoftReset();

    if (gMain.state)
        return;

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    LoadBgTiles(0, gTextWindowFrame3_Gfx, 0x120, 0x214);
    DeactivateAllTextPrinters();
    ResetTasks();
    ResetPaletteFade();
    LoadPalette(gTextWindowFrame3_Pal, 0xE0, 0x20);
    LoadPalette(gStandardMenuPalette, 0xF0, 0x20);
    InitWindows(textWin);
    DrawStdFrameWithCustomTileAndPalette(0, TRUE, 0x214, 0xE);

    static const u8 disclaimer[] =_("{COLOR DARK_GRAY}This mini-hack was for a jam themed on\n'Myths'. It should NOT be distributed for\nprofit or as a ROM file!");
    SaveFailedScreenTextPrint(disclaimer, 1, 1);

    // If it wrote random data to the test ram addr then just mark it as bad <--- based on a true story
    if (gTestResult > ACC_TEST_COUNT)
    {
        gTestResult = 0;
    }

    SaveFailedScreenTextPrint(deviceAccuracyStrings[gTestResult], 1, 9);


    static const u8 continueMessage[] =_("{COLOR DARK_GRAY}Press {START_BUTTON} to continue...");
    SaveFailedScreenTextPrint(continueMessage, 1, 13);

    TransferPlttBuffer();
    *(u16*)PLTT = RGB(17, 18, 31);
    ShowBg(0);
    gMain.state++;

}

void CB2_EndGameCallback(void)
{
    static const struct WindowTemplate textWin[] =
    {
        {
            .bg = 0,
            .tilemapLeft = 3,
            .tilemapTop = 2,
            .width = 24,
            .height = 16,
            .paletteNum = 15,
            .baseBlock = 1,
        }
    };

    if (gMain.newKeys & START_BUTTON)
        DoSoftReset();

    if (gMain.state)
        return;

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    LoadBgTiles(0, gTextWindowFrame3_Gfx, 0x120, 0x214);
    DeactivateAllTextPrinters();
    ResetTasks();
    ResetPaletteFade();
    LoadPalette(gTextWindowFrame3_Pal, 0xE0, 0x20);
    LoadPalette(gStandardMenuPalette, 0xF0, 0x20);
    InitWindows(textWin);
    DrawStdFrameWithCustomTileAndPalette(0, TRUE, 0x214, 0xE);

    static const u8 disclaimer[] =_("{COLOR DARK_GRAY}The End!\n\n'Developed' By KittyPBoxx.\nWith Battle Puzzles By Sabata & Kitty\n\nThanks For Playing!");
    SaveFailedScreenTextPrint(disclaimer, 1, 1);

    TransferPlttBuffer();
    *(u16*)PLTT = RGB(17, 18, 31);
    ShowBg(0);
    gMain.state++;

}
    

