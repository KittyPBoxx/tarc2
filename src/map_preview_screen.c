#include "global.h"
#include "string_util.h"
#include "malloc.h"
#include "gpu_regs.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "script.h"
#include "overworld.h"
#include "event_data.h"
#include "map_preview_screen.h"
#include "constants/region_map_sections.h"
#include "constants/rgb.h"
#include "bg.h"
#include "menu.h"

static EWRAM_DATA bool8 sAllocedBg0TilemapBuffer = FALSE;

static void Task_RunMapPreviewScreenForest(u8 taskId);
static void Task_RunMapPreview_Script(u8 taskId);
static void CB2_MapPreviewScript(void);
static void VblankCB_MapPreviewScript(void);

static const u8 sBerryForestMapPreviewPalette[] = INCBIN_U8("graphics/map_preview/berry_forest/tiles.gbapal");
static const u8 sBerryForestMapPreviewTiles[] = INCBIN_U8("graphics/map_preview/berry_forest/tiles.4bpp.lz");
static const u8 sBerryForestMapPreviewTilemap[] = INCBIN_U8("graphics/map_preview/berry_forest/tilemap.bin.lz");

// If you set flagId to MPS_FLAG_NULL, it will not set a flag when visiting the map for the first time
// and the duration will default to MPS_DURATION_NO_FLAG.
static const struct MapPreviewScreen sMapPreviewScreenData[MPS_COUNT] = {
    [MPS_BERRY_FOREST] = {
        .mapsec = MAPSEC_BERRY_FOREST,
        .type = MPS_TYPE_FADE_IN,
        .flagId = MPS_FLAG_NULL,
        .image = IMG_BERRY_FOREST
    }
};

static const struct ImageData sMapPreviewImageData[IMG_COUNT] = {
    [IMG_BERRY_FOREST] = {
        .tilesptr = sBerryForestMapPreviewTiles,
        .tilemapptr = sBerryForestMapPreviewTilemap,
        .palptr = sBerryForestMapPreviewPalette
    }
};

static const struct WindowTemplate sMapNameWindow = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 13,
    .height = 2,
    .paletteNum = 14,
    .baseBlock = 0x259
};

static const struct WindowTemplate sMapNameWindowLarge = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 22,
    .height = 2,
    .paletteNum = 14,
    .baseBlock = 0x259
};

static const struct BgTemplate sMapPreviewBgTemplate[1] = {
    {
        .mapBaseIndex = 31
    }
};

static u8 GetMapPreviewScreenIdx(u8 mapsec)
{
    s32 i;

    for (i = 0; i < MPS_COUNT; i++)
    {
        if (sMapPreviewScreenData[i].mapsec == mapsec)
        {
            return i;
        }
    }
    return MPS_COUNT;
}

bool8 MapHasPreviewScreen(u8 mapsec, u8 type)
{
    u8 idx;

    idx = GetMapPreviewScreenIdx(mapsec);
    if (idx != MPS_COUNT)
    {
        if (type == MPS_TYPE_ANY)
        {
            return TRUE;
        }
        else
        {
            return sMapPreviewScreenData[idx].type == type ? TRUE : FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

bool32 MapHasPreviewScreen_HandleQLState2(u8 mapsec, u8 type)
{
    return MapHasPreviewScreen(mapsec, type);
}

void MapPreview_InitBgs(void)
{
    InitBgsFromTemplates(0, sMapPreviewBgTemplate, NELEMS(sMapPreviewBgTemplate));
    ShowBg(0);
}

void MapPreview_LoadGfx(u8 mapsec)
{
    u8 idx;

    idx = GetMapPreviewScreenIdx(mapsec);
    if (idx != MPS_COUNT)
    {
       ResetTempTileDataBuffers();
       if (MapHasPreviewScreen_HandleQLState2(gMapHeader.regionMapSectionId, MPS_TYPE_FADE_IN) == TRUE)
            LoadPalette(sMapPreviewImageData[sMapPreviewScreenData[idx].image].palptr, BG_PLTT_ID(13), 3 * PLTT_SIZE_4BPP);
        else
            LoadPalette(sMapPreviewImageData[sMapPreviewScreenData[idx].image].palptr, BG_PLTT_ID(0), 16 * PLTT_SIZE_4BPP);
            
       DecompressAndCopyTileDataToVram(0, sMapPreviewImageData[sMapPreviewScreenData[idx].image].tilesptr, 0, 0, 0);
       if (GetBgTilemapBuffer(0) == NULL)
       {
           SetBgTilemapBuffer(0, Alloc(BG_SCREEN_SIZE));
           sAllocedBg0TilemapBuffer = TRUE;
       }
       else
       {
           sAllocedBg0TilemapBuffer = FALSE;
       }
       CopyToBgTilemapBuffer(0, sMapPreviewImageData[sMapPreviewScreenData[idx].image].tilemapptr, 0, 0x000);
       CopyBgTilemapBufferToVram(0);
    }
}

void MapPreview_Unload(s32 windowId)
{
    RemoveWindow(windowId);
    if (sAllocedBg0TilemapBuffer)
    {
        Free(GetBgTilemapBuffer(0));
    }
}

bool32 MapPreview_IsGfxLoadFinished(void)
{
    return FreeTempTileDataBuffersIfPossible();
}

void MapPreview_StartForestTransition(u8 mapsec)
{
    u8 taskId;

    taskId = CreateTask(Task_RunMapPreviewScreenForest, 0);
    gTasks[taskId].data[2] = GetBgAttribute(0, BG_ATTR_PRIORITY);
    gTasks[taskId].data[4] = GetGpuReg(REG_OFFSET_BLDCNT);
    gTasks[taskId].data[5] = GetGpuReg(REG_OFFSET_BLDALPHA);
    gTasks[taskId].data[3] = GetGpuReg(REG_OFFSET_DISPCNT);
    gTasks[taskId].data[6] = GetGpuReg(REG_OFFSET_WININ);
    gTasks[taskId].data[7] = GetGpuReg(REG_OFFSET_WINOUT);
    gTasks[taskId].data[10] = MapPreview_GetDuration(mapsec);
    gTasks[taskId].data[8] = 16;
    gTasks[taskId].data[9] = 0;
    SetBgAttribute(0, BG_ATTR_PRIORITY, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
    SetGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_CLR | WININ_WIN1_CLR);
    SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WIN01_CLR);
    gTasks[taskId].data[11] = MapPreview_CreateMapNameWindow(mapsec);
    LockPlayerFieldControls();
}

u16 MapPreview_CreateMapNameWindow(u8 mapsec)
{
    u16 windowId;
    u32 xctr;
    #ifdef BUGFIX
    // Fixes access violations indicated below.
    u8 color[3];
    #else
    u8 color[0];
    #endif

    GetMapName(gStringVar4, mapsec, 0);
    if (GetStringWidth(FONT_NORMAL, gStringVar4, 0) > 104)
    {
        windowId = AddWindow(&sMapNameWindowLarge);
        xctr = 177 - GetStringWidth(FONT_NORMAL, gStringVar4, 0);
    }
    else
    {
        xctr = 104 - GetStringWidth(FONT_NORMAL, gStringVar4, 0);
        windowId = AddWindow(&sMapNameWindow);
    }
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    color[0] = TEXT_COLOR_WHITE; // Access violation
    color[1] = TEXT_COLOR_RED; // Access violation
    color[2] = TEXT_COLOR_LIGHT_GRAY; // Access violation
    AddTextPrinterParameterized4(windowId, FONT_NORMAL, xctr / 2, 2, 0, 0, color/* Access violation */, -1, gStringVar4);
    return windowId;
}

bool32 ForestMapPreviewScreenIsRunning(void)
{
    if (FuncIsActiveTask(Task_RunMapPreviewScreenForest) == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void Task_RunMapPreviewScreenForest(u8 taskId)
{
    s16 * data;

    data = gTasks[taskId].data;
    switch (data[0])
    {
    case 0:
        if (!MapPreview_IsGfxLoadFinished() && !IsDma3ManagerBusyWithBgCopy())
        {
            CopyWindowToVram(data[11], COPYWIN_FULL);
            data[0]++;
        }
        break;
    case 1:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            FadeInFromBlack();
            data[0]++;
        }
        break;
    case 2:
        if (IsWeatherNotFadingIn())
        {
            Overworld_PlaySpecialMapMusic();
            data[0]++;
        }
        break;
    case 3:
        data[1]++;
        if (data[1] > data[10] || JOY_HELD(B_BUTTON))
        {
            data[1] = 0;
            data[0]++;
        }
        break;
    case 4:
        switch (data[1])
        {
        case 0:
            data[9]++;
            if (data[9] > 16)
            {
                data[9] = 16;
            }
            break;
        case 1:
            data[8]--;
            if (data[8] < 0)
            {
                data[8] = 0;
            }
            break;
        }
        data[1] = (data[1] + 1) % 3;
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(data[8], data[9]));
        if (data[8] == 0 && data[9] == 16)
        {
            FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
            CopyBgTilemapBufferToVram(0);
            data[0]++;
        }
        break;
    case 5:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            MapPreview_Unload(data[11]);
            SetBgAttribute(0, BG_ATTR_PRIORITY, data[2]);
            SetGpuReg(REG_OFFSET_DISPCNT, data[3]);
            SetGpuReg(REG_OFFSET_BLDCNT, data[4]);
            SetGpuReg(REG_OFFSET_BLDALPHA, data[5]);
            SetGpuReg(REG_OFFSET_WININ, data[6]);
            SetGpuReg(REG_OFFSET_WINOUT, data[7]);
            DestroyTask(taskId);
        }
        break;
    }
}

const struct MapPreviewScreen * GetDungeonMapPreviewScreenInfo(u8 mapsec)
{
    u8 idx;

    idx = GetMapPreviewScreenIdx(mapsec);
    if (idx == MPS_COUNT)
    {
        return NULL;
    }
    else
    {
        return &sMapPreviewScreenData[idx];
    }
}

u16 MapPreview_GetDuration(u8 mapsec)
{
    u8 idx;
    u16 flagId;

    idx = GetMapPreviewScreenIdx(mapsec);

    if (idx == MPS_COUNT)
    {
        return 0;
    }

    flagId = sMapPreviewScreenData[idx].flagId;

    if (flagId == MPS_FLAG_NULL) {
        if (MPS_DURATION_ALWAYS != 0) {
            return MPS_DURATION_ALWAYS;
        }
        else
        {
            return MPS_DURATION_NO_FLAG;
        }
    }
    else if (MPS_DURATION_ALWAYS != 0) {
        if (!FlagGet(flagId))
        {
            FlagSet(flagId);
        }
        return MPS_DURATION_ALWAYS;
    }
    else {
        if (!FlagGet(flagId))
        {
            FlagSet(flagId);
            return MPS_DURATION_LONG;
        }
        else
        {
            return MPS_DURATION_SHORT;
        }
    }
}

static void VblankCB_MapPreviewScript(void)
{
    TransferPlttBuffer();
}

#define taskStep        data[0]
#define frameCounter    data[1]
#define MPWindowId      data[2]

void Script_MapPreview(void)
{
    SetVBlankCallback(NULL);
    gMain.savedCallback = CB2_ReturnToFieldContinueScript;
    MapPreview_LoadGfx(gMapHeader.regionMapSectionId);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
    SetVBlankCallback(VblankCB_MapPreviewScript);
    SetMainCallback2(CB2_MapPreviewScript);
    CreateTask(Task_RunMapPreview_Script, 0);
}

static void CB2_MapPreviewScript(void)
{
    RunTasks();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void Task_RunMapPreview_Script(u8 taskId)
{
    s16 * data;

    data = gTasks[taskId].data;
    switch (taskStep)
    {
    case 0:
        if (!MapPreview_IsGfxLoadFinished() && !IsDma3ManagerBusyWithBgCopy())
        {
            MPWindowId = MapPreview_CreateMapNameWindow(gMapHeader.regionMapSectionId);
            CopyWindowToVram(MPWindowId, COPYWIN_FULL);
            taskStep++;
        }
        break;
    case 1:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            FadeInFromBlack();
            taskStep++;
        }
        break;
    case 2:
        frameCounter++;
        if (frameCounter > MPS_DURATION_SCRIPT || JOY_HELD(B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, MPS_BASIC_FADE_SPEED, 0, 16, RGB_BLACK);
            frameCounter = 0;
            taskStep++;
        }
        break;
    case 3:
        if (!UpdatePaletteFade())
        {
            MapPreview_Unload(MPWindowId);
            DestroyTask(taskId);
            SetMainCallback2(gMain.savedCallback);
        }
        break;
    }
}

#undef taskStep
#undef frameCounter
#undef MPWindowId