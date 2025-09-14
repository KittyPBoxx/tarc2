#include "global.h"
#include "bg.h"
#include "debug.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_object_lock.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "field_screen_effect.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "load_save.h"
#include "main.h"
#include "menu.h"
#include "new_game.h"
#include "option_menu.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "start_menu.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "wild_encounter.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/maps.h"
#include "palette_effects.h"

enum 
{
    MENU_TYPE_START,
    MENU_TYPE_WARP,
    MENU_TYPE_SAVE,
    MENU_TYPE_LOAD
};

// Menu actions
enum
{
    // Main actions
    MENU_ACTION_SAVE,
    MENU_ACTION_LOAD,
    MENU_ACTION_OPTION,

    // Save Actions
    MENU_ACTION_SAVE_1,
    MENU_ACTION_SAVE_2,
    MENU_ACTION_SAVE_3,
    MENU_ACTION_SAVE_4,

    // Load Actions
    MENU_ACTION_LOAD_1,
    MENU_ACTION_LOAD_2,
    MENU_ACTION_LOAD_3,
    MENU_ACTION_LOAD_4,
    MENU_ACTION_LOAD_AUTO,
    
    // Warp actions
    MENU_ACTION_BRIDGE,
    MENU_ACTION_MANOR,
    MENU_ACTION_FOREST,
    MENU_ACTION_CAVE_F1,
    MENU_ACTION_CAVE_F2,
    MENU_ACTION_SUMMIT,
    MENU_ACTION_PASS,

    // Exit
    MENU_ACTION_EXIT
};

// Save status
enum
{
    SAVE_IN_PROGRESS,
    SAVE_SUCCESS,
    SAVE_CANCELED,
    SAVE_ERROR
};

// IWRAM common
COMMON_DATA bool8 (*gMenuCallback)(void) = NULL;

// EWRAM
EWRAM_DATA static u8 sStartMenuCursorPos = 0;
EWRAM_DATA static u8 sNumStartMenuActions = 0;
EWRAM_DATA static u8 sCurrentStartMenuActions[9] = {0};
EWRAM_DATA static s8 sInitStartMenuData[3] = {0};

EWRAM_DATA static u8 (*sSaveDialogCallback)(void) = NULL;
EWRAM_DATA static u8 sSaveDialogTimer = 0;
EWRAM_DATA static bool8 sSavingComplete = FALSE;
EWRAM_DATA static u8 sSaveInfoWindowId = 0;
EWRAM_DATA static u8 sChosenSaveSlot = 0;
EWRAM_DATA static u8 sLastLoadCursorPos = 0;

// Menu action callbacks
static bool8 StartMenuSaveCallback(void);
static bool8 StartMenuLoadCallback(void);
static bool8 StartMenuOptionCallback(void);
static bool8 StartMenuExitCallback(void);

// Menu callbacks
static bool8 SaveStartCallback(void);
static bool8 SaveCallback(void);
static bool8 HandleStartMenuInput(void);
static bool8 LoadStartCallback(void);
static bool8 LoadCallback(void);

// Save callbacks
static bool8 SaveMenuGenericSaveCallback(u8 slot);
static bool8 SaveMenuSave1Callback(void);
static bool8 SaveMenuSave2Callback(void);
static bool8 SaveMenuSave3Callback(void);
static bool8 SaveMenuSave4Callback(void);

// Load callbacks
static bool8 LoadMenuGenericLoadCallback(u8 slot);
static bool8 LoadMenuLoad1Callback(void);
static bool8 LoadMenuLoad2Callback(void);
static bool8 LoadMenuLoad3Callback(void);
static bool8 LoadMenuLoad4Callback(void);
static bool8 LoadMenuLoadAutoCallback(void);

// Warp callbacks
static bool8 WarpMenuGenericCallback(u8 map);
static bool8 WarpMenuBridgeCallback(void);
static bool8 WarpMenuManorCallback(void);
static bool8 WarpMenuForestCallback(void);
static bool8 WarpMenuCaveF1Callback(void);
static bool8 WarpMenuCaveF2Callback(void);
static bool8 WarpMenuSummitCallback(void);
static bool8 WarpMenuPassCallback(void);

// Save dialog callbacks
static u8 SaveConfirmSaveCallback(void);
static u8 SaveYesNoCallback(void);
static u8 SaveConfirmInputCallback(void);
static u8 SaveDoSaveCallback(void);
static u8 SaveSuccessCallback(void);
static u8 SaveReturnSuccessCallback(void);
static u8 SaveErrorCallback(void);
static u8 SaveReturnErrorCallback(void);
static u8 LoadDoLoadCallback(void);
static u8 LoadConfirmLoadCallback(void);
static u8 LoadYesNoCallback(void);
static u8 LoadConfirmInputCallback(void);

// Task callbacks
static void StartMenuTask(u8 taskId);
static void SaveGameTask(u8 taskId);
static bool8 FieldCB_ReturnToFieldStartMenu(void);

static const u8 sText_MenuLoad[] = _("LOAD");

static const u8 sText_MenuSave1[] = _("SLOT 1");
static const u8 sText_MenuSave2[] = _("SLOT 2");
static const u8 sText_MenuSave3[] = _("SLOT 3");
static const u8 sText_MenuSave4[] = _("SLOT 4");
static const u8 sText_MenuSaveAuto[] = _("AUTO");

static const u8 sText_SlotEmpty[] = _("{COLOR LIGHT_GRAY} SLOT EMPTY");

static const u8 sText_WarpBridge[] = _("Bridge");
static const u8 sText_WarpManor[] = _("Manor");
static const u8 sText_WarpForest[] = _("Forest");
static const u8 sText_WarpCaveF1[] = _("Cave F1");
static const u8 sText_WarpCaveF2[] = _("Cave F2");
static const u8 sText_WarpSummit[] = _("Summit");
static const u8 sText_WarpPass[] = _("Pass");

static const struct MenuAction sStartMenuItems[] =
{
    // Menu Actions
    [MENU_ACTION_SAVE]            = {gText_MenuSave,    {.u8_void = StartMenuSaveCallback}},
    [MENU_ACTION_LOAD]            = {sText_MenuLoad,    {.u8_void = StartMenuLoadCallback}},
    [MENU_ACTION_OPTION]          = {gText_MenuOption,  {.u8_void = StartMenuOptionCallback}},

    // Save Actions
    [MENU_ACTION_SAVE_1]          = {sText_MenuSave1,  {.u8_void = SaveMenuSave1Callback}},
    [MENU_ACTION_SAVE_2]          = {sText_MenuSave2,  {.u8_void = SaveMenuSave2Callback}},
    [MENU_ACTION_SAVE_3]          = {sText_MenuSave3,  {.u8_void = SaveMenuSave3Callback}},
    [MENU_ACTION_SAVE_4]          = {sText_MenuSave4,  {.u8_void = SaveMenuSave4Callback}},

    // Load Actions
    [MENU_ACTION_LOAD_1]          = {sText_MenuSave1,     {.u8_void = LoadMenuLoad1Callback}},
    [MENU_ACTION_LOAD_2]          = {sText_MenuSave2,     {.u8_void = LoadMenuLoad2Callback}},
    [MENU_ACTION_LOAD_3]          = {sText_MenuSave3,     {.u8_void = LoadMenuLoad3Callback}},
    [MENU_ACTION_LOAD_4]          = {sText_MenuSave4,     {.u8_void = LoadMenuLoad4Callback}},
    [MENU_ACTION_LOAD_AUTO]       = {sText_MenuSaveAuto,  {.u8_void = LoadMenuLoadAutoCallback}},
    
    // Warp Actions
    [MENU_ACTION_BRIDGE]          = {sText_WarpBridge,  {.u8_void = WarpMenuBridgeCallback}},
    [MENU_ACTION_MANOR]           = {sText_WarpManor,   {.u8_void = WarpMenuManorCallback}},
    [MENU_ACTION_FOREST]          = {sText_WarpForest,  {.u8_void = WarpMenuForestCallback}},
    [MENU_ACTION_CAVE_F1]         = {sText_WarpCaveF1,  {.u8_void = WarpMenuCaveF1Callback}},
    [MENU_ACTION_CAVE_F2]         = {sText_WarpCaveF2,  {.u8_void = WarpMenuCaveF2Callback}},
    [MENU_ACTION_SUMMIT]          = {sText_WarpSummit,  {.u8_void = WarpMenuSummitCallback}},
    [MENU_ACTION_PASS]            = {sText_WarpPass,    {.u8_void = WarpMenuPassCallback}},
    
    // Exit
    [MENU_ACTION_EXIT]            = {gText_MenuExit,    {.u8_void = StartMenuExitCallback}},
};

static const struct WindowTemplate sSaveInfoWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 14,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 8
};

// Local functions
static void BuildStartMenuActions(void);
static void AddStartMenuAction(u8 action);
static void BuildNormalStartMenu(void);
static void BuildWarpMenuActions(void);
static void BuildWarpMenu(void);
static void BuildSaveMenuActions(void);
static void BuildSaveMenu(void);
static void BuildLoadMenuActions(void);
static void BuildLoadMenu(void);

static bool32 PrintStartMenuActions(s8 *pIndex, u32 count);
static bool32 InitStartMenuStep(void);
static void InitStartMenu(void);
static void CreateStartMenuTask(TaskFunc followupFunc, u8 menuType);
static void InitSave(void);
static void InitLoad(void);
static u8 RunSaveCallback(void);
static void ShowSaveMessage(const u8 *message, u8 (*saveCallback)(void));
static void HideSaveMessageWindow(void);
static void HideSaveInfoWindow(void);
static void SaveStartTimer(void);
static bool8 SaveErrorTimer(void);
static void ShowSaveInfoWindow(void);
static void ShowLoadInfoWindow(u8 slot);
static void RemoveSaveInfoWindow(void);
static void HideStartMenuWindow(void);

static void BuildStartMenuActions(void)
{
    sNumStartMenuActions = 0;
    BuildNormalStartMenu();
}

static void BuildWarpMenuActions(void)
{
    sNumStartMenuActions = 0;
    BuildWarpMenu();
}

static void BuildSaveMenuActions(void)
{
    sNumStartMenuActions = 0;
    BuildSaveMenu();
}

static void BuildLoadMenuActions(void)
{
    sNumStartMenuActions = 0;
    BuildLoadMenu();
}


static void AddStartMenuAction(u8 action)
{
    AppendToList(sCurrentStartMenuActions, &sNumStartMenuActions, action);
}

static void BuildNormalStartMenu(void)
{
    AddStartMenuAction(MENU_ACTION_SAVE);
    AddStartMenuAction(MENU_ACTION_LOAD);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}

static void BuildWarpMenu(void)
{
    AddStartMenuAction(MENU_ACTION_EXIT);
    AddStartMenuAction(MENU_ACTION_BRIDGE);
    AddStartMenuAction(MENU_ACTION_MANOR);
    AddStartMenuAction(MENU_ACTION_FOREST);
    AddStartMenuAction(MENU_ACTION_CAVE_F1);
    AddStartMenuAction(MENU_ACTION_CAVE_F2);
    AddStartMenuAction(MENU_ACTION_SUMMIT);
    AddStartMenuAction(MENU_ACTION_PASS);
}

static void BuildSaveMenu(void)
{
    AddStartMenuAction(MENU_ACTION_SAVE_1);
    AddStartMenuAction(MENU_ACTION_SAVE_2);
    AddStartMenuAction(MENU_ACTION_SAVE_3);
    AddStartMenuAction(MENU_ACTION_SAVE_4);   
}

static void BuildLoadMenu(void)
{
    AddStartMenuAction(MENU_ACTION_LOAD_AUTO);
    AddStartMenuAction(MENU_ACTION_LOAD_1);
    AddStartMenuAction(MENU_ACTION_LOAD_2);
    AddStartMenuAction(MENU_ACTION_LOAD_3);
    AddStartMenuAction(MENU_ACTION_LOAD_4);
}

static bool32 PrintStartMenuActions(s8 *pIndex, u32 count)
{
    s8 index = *pIndex;

    do
    {
        StringExpandPlaceholders(gStringVar4, sStartMenuItems[sCurrentStartMenuActions[index]].text);
        AddTextPrinterParameterized(GetStartMenuWindowId(), FONT_NORMAL, gStringVar4, 8, (index << 4) + 9, TEXT_SKIP_DRAW, NULL);

        index++;
        if (index >= sNumStartMenuActions)
        {
            *pIndex = index;
            return TRUE;
        }

        count--;
    }
    while (count != 0);

    *pIndex = index;
    return FALSE;
}

static bool32 InitStartMenuStep(void)
{
    s8 state = sInitStartMenuData[0];

    switch (state)
    {
    case 0:
        sInitStartMenuData[0]++;
        break;
    case 1:
        if (sInitStartMenuData[2] == MENU_TYPE_WARP)
        {
            BuildWarpMenuActions();
        }
        else if (sInitStartMenuData[2] == MENU_TYPE_SAVE)
        {
            BuildSaveMenuActions();
        }
        else if (sInitStartMenuData[2] == MENU_TYPE_LOAD)
        {
            BuildLoadMenuActions();
        }
        else
        {
            BuildStartMenuActions();
        }
        sInitStartMenuData[0]++;
        break;
    case 2:
        LoadMessageBoxAndBorderGfx();
        DrawStdWindowFrame(AddStartMenuWindow(sNumStartMenuActions), FALSE);
        sInitStartMenuData[1] = 0;
        sInitStartMenuData[0]++;
        break;
    case 3:
        sInitStartMenuData[0]++;
        break;
    case 4:
        if (PrintStartMenuActions(&sInitStartMenuData[1], 2))
            sInitStartMenuData[0]++;
        break;
    case 5:
        sStartMenuCursorPos = InitMenuWithSpriteCursor(GetStartMenuWindowId(), FONT_NORMAL, 0, 9, 16, sNumStartMenuActions, sStartMenuCursorPos);
        CopyWindowToVram(GetStartMenuWindowId(), COPYWIN_MAP);
        return TRUE;
    }

    return FALSE;
}

static void InitStartMenu(void)
{
    sInitStartMenuData[0] = 0;
    sInitStartMenuData[1] = 0;
    while (!InitStartMenuStep())
        ;
}

static void StartMenuTask(u8 taskId)
{
    if (InitStartMenuStep() == TRUE)
        SwitchTaskToFollowupFunc(taskId);
}

static void CreateStartMenuTask(TaskFunc followupFunc, u8 menuType)
{
    u8 taskId;

    sInitStartMenuData[0] = 0;
    sInitStartMenuData[1] = 0;
    sInitStartMenuData[2] = menuType;
    taskId = CreateTask(StartMenuTask, 0x50);
    SetTaskFuncWithFollowupFunc(taskId, StartMenuTask, followupFunc);
}

static bool8 FieldCB_ReturnToFieldStartMenu(void)
{
    if (InitStartMenuStep() == FALSE)
    {
        return FALSE;
    }

    ReturnToFieldOpenStartMenu();
    return TRUE;
}

void ShowReturnToFieldStartMenu(void)
{
    sInitStartMenuData[0] = 0;
    sInitStartMenuData[1] = 0;
    gFieldCallback2 = FieldCB_ReturnToFieldStartMenu;
}

void Task_ShowStartMenu(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch(task->data[0])
    {
    case 0:
        gMenuCallback = HandleStartMenuInput;
        task->data[0]++;
        break;
    case 1:
        if (gMenuCallback() == TRUE)
        {
            FillBgTilemapBufferRect(0, 0, 0, 0, 32, 32, 17); // TODO: TARC This is a hack to clear the window, Why is it not actually getting removed?
            DestroyTask(taskId);
        }
        break;
    }
}

void ShowStartMenu(void)
{
    FreezeObjectEvents();
    PlayerFreeze();
    StopPlayerAvatar();
    CreateStartMenuTask(Task_ShowStartMenu, MENU_TYPE_START);
    LockPlayerFieldControls();
}

void ShowWarpMenu(void)
{
    FreezeObjectEvents();
    PlayerFreeze();
    StopPlayerAvatar();
    CreateStartMenuTask(Task_ShowStartMenu, MENU_TYPE_WARP);
    LockPlayerFieldControls();
}

static bool8 HandleStartMenuInput(void)
{
    if (JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        sStartMenuCursorPos = Menu_MoveCursor(-1);
    }

    if (JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        sStartMenuCursorPos = Menu_MoveCursor(1);
    }

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);

        gMenuCallback = sStartMenuItems[sCurrentStartMenuActions[sStartMenuCursorPos]].func.u8_void;

        if (gMenuCallback != StartMenuSaveCallback && gMenuCallback != StartMenuExitCallback && gMenuCallback != StartMenuLoadCallback && gMenuCallback != StartMenuLoadCallback)
        {
           FadeScreen(FADE_TO_BLACK, 0);
        }

        return FALSE;
    }

    if (JOY_NEW(START_BUTTON | B_BUTTON | SELECT_BUTTON))
    {
        HideStartMenu();
        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuSaveCallback(void)
{
    gMenuCallback = SaveStartCallback; // Display save menu
    return FALSE;
}

static bool8 StartMenuLoadCallback(void)
{
    gMenuCallback = LoadStartCallback; // Display save menu
    return FALSE;
}

static bool8 StartMenuOptionCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        CleanupOverworldWindowsAndTilemaps();
        HideStartMenuWindow();
        SetMainCallback2(CB2_InitOptionMenu); // Display option menu
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuExitCallback(void)
{
    HideStartMenu(); // Hide start menu
    return TRUE;
}

static bool8 SaveStartCallback(void)
{
    InitSave();
    gMenuCallback = SaveCallback;
    return FALSE;
}

static bool8 LoadStartCallback(void)
{
    InitLoad();
    gMenuCallback = LoadCallback;
    return FALSE;
}

static bool8 SaveCallback(void)
{
    switch (RunSaveCallback())
    {
    case SAVE_IN_PROGRESS:
        return FALSE;
    case SAVE_CANCELED: // Back to start menu
        ClearDialogWindowAndFrameToTransparent(0, FALSE);
        InitStartMenu();
        gMenuCallback = HandleStartMenuInput;
        return FALSE;
    case SAVE_SUCCESS:
    case SAVE_ERROR:    // Close start menu
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ScriptUnfreezeObjectEvents();
        UnlockPlayerFieldControls();
        return TRUE;
    }

    return FALSE;
}

static bool8 LoadCallback(void)
{
    switch (RunSaveCallback())
    {
    case SAVE_IN_PROGRESS:
        return FALSE;
    case SAVE_CANCELED: // Back to start menu
        ClearDialogWindowAndFrameToTransparent(0, FALSE);
        InitStartMenu();
        gMenuCallback = HandleStartMenuInput;
        return FALSE;
    }

    return FALSE;
}

// static void DebugLogWindows(void)
// {
//     for (int i = 0; i < WINDOWS_MAX; i++)
//     {
//         struct Window *w = &gWindows[i];
//         if (w->tileData != NULL) // only log active windows
//         {
//             DebugPrintfLevel(MGBA_LOG_ERROR, "Win %x: bg=%x pos=(%x,%x) size=%xx%x baseBlock=0x%x tileData=%x\n",
//                         i, w->window.bg, w->window.tilemapLeft, w->window.tilemapTop,
//                         w->window.width, w->window.height,
//                         w->window.baseBlock, w->tileData);
//         }
//     }
// }


static bool8 WarpMenuGenericCallback(u8 map)
{
    switch(map)
    {
        case MAP_BRIDGE:
            SetWarpDestination(MAP_GROUP(MAP_BRIDGE),MAP_NUM(MAP_BRIDGE),0,10,9);
            break;
        case MAP_MANOR:
            SetWarpDestination(MAP_GROUP(MAP_MANOR),MAP_NUM(MAP_MANOR),0,10,9);
            break;
        case MAP_FOREST:
            SetWarpDestination(MAP_GROUP(MAP_FOREST),MAP_NUM(MAP_FOREST),0,10,9);
            break;
        case MAP_CAVE_BOTTOM:
            SetWarpDestination(MAP_GROUP(MAP_CAVE_BOTTOM),MAP_NUM(MAP_CAVE_BOTTOM),0,10,9);
            break;
        case MAP_CAVE_TOP:
            SetWarpDestination(MAP_GROUP(MAP_CAVE_TOP),MAP_NUM(MAP_CAVE_TOP),0,10,9);
            break;
        case MAP_SUMMIT:
            SetWarpDestination(MAP_GROUP(MAP_SUMMIT),MAP_NUM(MAP_SUMMIT),0,10,9);
            break;
        case MAP_RETURN_PASS:
            SetWarpDestination(MAP_GROUP(MAP_RETURN_PASS),MAP_NUM(MAP_RETURN_PASS),0,10,9);
            break;
        default:
            SetWarpDestination(MAP_GROUP(MAP_BRIDGE),MAP_NUM(MAP_BRIDGE),0,10,9);
    }

    //DebugLogWindows();
    FreeAllWindowBuffers();
    CleanupOverworldWindowsAndTilemaps();
    PauseMapPaletteEffects();
    DisableInterrupts(INTR_FLAG_HBLANK);
    HideStartMenuWindow();
    DoTeleportTileWarp();

    return TRUE;
}

static bool8 WarpMenuBridgeCallback(void) 
{
    return WarpMenuGenericCallback(MAP_BRIDGE);
}

static bool8 WarpMenuManorCallback(void) 
{
    return WarpMenuGenericCallback(MAP_MANOR);
}

static bool8 WarpMenuForestCallback(void) 
{
    return WarpMenuGenericCallback(MAP_FOREST);
}

static bool8 WarpMenuCaveF1Callback(void) 
{
    return WarpMenuGenericCallback(MAP_CAVE_BOTTOM);
}

static bool8 WarpMenuCaveF2Callback(void) 
{
    return WarpMenuGenericCallback(MAP_CAVE_TOP);
}

static bool8 WarpMenuSummitCallback(void) 
{
    return WarpMenuGenericCallback(MAP_SUMMIT);
}

static bool8 WarpMenuPassCallback(void) 
{
    return WarpMenuGenericCallback(MAP_RETURN_PASS);
}

static bool8 SaveMenuGenericSaveCallback(u8 slot)
{
    u8 saveStatus;

    saveStatus = TrySavingData(SAVE_NORMAL, SAVE_MANUAL_1);

    if (saveStatus == SAVE_STATUS_OK)
        ShowSaveMessage(gText_PlayerSavedGame, SaveSuccessCallback);
    else
        ShowSaveMessage(gText_SaveError, SaveErrorCallback);

    SaveStartTimer();
    return SAVE_IN_PROGRESS;
}

static bool8 SaveMenuSave1Callback(void) 
{
    return SaveMenuGenericSaveCallback(SAVE_MANUAL_1);
}

static bool8 SaveMenuSave2Callback(void) 
{
    return SaveMenuGenericSaveCallback(SAVE_MANUAL_2);
}

static bool8 SaveMenuSave3Callback(void) 
{
    return SaveMenuGenericSaveCallback(SAVE_MANUAL_3);
}

static bool8 SaveMenuSave4Callback(void) 
{
    return SaveMenuGenericSaveCallback(SAVE_MANUAL_4);
}

static bool8 LoadMenuGenericLoadCallback(u8 slot)
{
    ReloadSlot(slot);
    return FALSE;
}

static bool8 LoadMenuLoad1Callback(void)
{
    return LoadMenuGenericLoadCallback(SAVE_MANUAL_1);
}

static bool8 LoadMenuLoad2Callback(void)
{
    return LoadMenuGenericLoadCallback(SAVE_MANUAL_2);
}

static bool8 LoadMenuLoad3Callback(void)
{
    return LoadMenuGenericLoadCallback(SAVE_MANUAL_3);
}

static bool8 LoadMenuLoad4Callback(void)
{
    return LoadMenuGenericLoadCallback(SAVE_MANUAL_4);
}

static bool8 LoadMenuLoadAutoCallback(void)
{
    return LoadMenuGenericLoadCallback(SAVE_AUTO_1);
}

static void InitSave(void)
{
    sSaveDialogCallback = SaveConfirmSaveCallback;
    sSavingComplete = FALSE;
}

static void InitLoad(void)
{
    sSaveDialogCallback = LoadConfirmLoadCallback;
    sSavingComplete = FALSE;
}

static u8 RunSaveCallback(void)
{
    // True if text is still printing
    if (RunTextPrintersAndIsPrinter0Active() == TRUE)
    {
        return SAVE_IN_PROGRESS;
    }

    sSavingComplete = FALSE;
    return sSaveDialogCallback();
}

void SaveGame(void)
{
    InitSave();
    CreateTask(SaveGameTask, 0x50);
}

static void ShowSaveMessage(const u8 *message, u8 (*saveCallback)(void))
{
    StringExpandPlaceholders(gStringVar4, message);

    // Clear just the top rows (reserved prompt area)
    FillWindowPixelRect(sSaveInfoWindowId,
                        PIXEL_FILL(1),
                        0, 0,
                        14 * 8, 16);  // width = window width, height = 2 lines

    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL,
                                gStringVar4, 0, 0, 0, NULL);

    CopyWindowToVram(sSaveInfoWindowId, COPYWIN_MAP);

    sSavingComplete = TRUE;
    sSaveDialogCallback = saveCallback;
}


static void SaveGameTask(u8 taskId)
{
    u8 status = RunSaveCallback();

    switch (status)
    {
    case SAVE_CANCELED:
    case SAVE_ERROR:
        gSpecialVar_Result = 0;
        break;
    case SAVE_SUCCESS:
        gSpecialVar_Result = status;
        break;
    case SAVE_IN_PROGRESS:
        return;
    }

    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void HideSaveMessageWindow(void)
{
    ClearDialogWindowAndFrame(0, TRUE);
}

static void HideSaveInfoWindow(void)
{
    RemoveSaveInfoWindow();
}

static void SaveStartTimer(void)
{
    sSaveDialogTimer = 10;
}


static bool8 SaveErrorTimer(void)
{
    if (sSaveDialogTimer != 0)
    {
        sSaveDialogTimer--;
    }
    else if (JOY_HELD(A_BUTTON))
    {
        return TRUE;
    }

    return FALSE;
}

static u8 SaveConfirmSaveCallback(void)
{
    RemoveStartMenuWindow();
    sInitStartMenuData[2] = MENU_TYPE_SAVE;
    ClearDialogWindowAndFrameToTransparent(0, FALSE);
    InitStartMenu();

    ShowSaveInfoWindow();
    sInitStartMenuData[2] = MENU_TYPE_START;
    ShowSaveMessage(gText_ConfirmSave, SaveYesNoCallback);

    return SAVE_IN_PROGRESS;
}

static u8 LoadConfirmLoadCallback(void)
{
    RemoveStartMenuWindow();
    sInitStartMenuData[2] = MENU_TYPE_LOAD;
    ClearDialogWindowAndFrameToTransparent(0, FALSE);
    InitStartMenu();
    sSaveInfoWindowId = 0;
    ShowLoadInfoWindow(SAVE_MANUAL_1);
    sInitStartMenuData[2] = MENU_TYPE_START;
    ShowSaveMessage(gText_ConfirmLoad, LoadYesNoCallback);

    return SAVE_IN_PROGRESS;
}

static u8 SaveYesNoCallback(void)
{
    //DisplayYesNoMenuDefaultYes(); // Show Yes/No menu
    sSaveDialogCallback = SaveConfirmInputCallback;
    return SAVE_IN_PROGRESS;
}

static u8 LoadYesNoCallback(void)
{
    //DisplayYesNoMenuDefaultYes(); // Show Yes/No menu
    sSaveDialogCallback = LoadConfirmInputCallback;
    return SAVE_IN_PROGRESS;
}

static u8 SaveConfirmInputCallback(void)
{
    s8 result = Menu_ProcessInputNoWrapClearOnChoose();

    switch (result)
    {
    case 0: // slot
        sChosenSaveSlot = SAVE_MANUAL_1;
        sSaveDialogCallback = SaveDoSaveCallback;
        StringCopy(gStringVar3, sText_MenuSave1);
        return SAVE_IN_PROGRESS;
    case 1: 
        sChosenSaveSlot = SAVE_MANUAL_2;
        sSaveDialogCallback = SaveDoSaveCallback;
        StringCopy(gStringVar3, sText_MenuSave2);
        return SAVE_IN_PROGRESS;
    case 2:    
        sChosenSaveSlot = SAVE_MANUAL_3;
        sSaveDialogCallback = SaveDoSaveCallback;
        StringCopy(gStringVar3, sText_MenuSave3);
        return SAVE_IN_PROGRESS;
    case 3:    
        sChosenSaveSlot = SAVE_MANUAL_4;
        sSaveDialogCallback = SaveDoSaveCallback;
        StringCopy(gStringVar3, sText_MenuSave4);
        return SAVE_IN_PROGRESS;
    case MENU_B_PRESSED:
        sSaveDialogCallback = NULL;
        RemoveStartMenuWindow();
        HideSaveInfoWindow();
        HideSaveMessageWindow();
        return SAVE_CANCELED;
    }

    return SAVE_IN_PROGRESS;
}

static u8 LoadConfirmInputCallback(void)
{
    s8 result = Menu_ProcessInputNoWrapClearOnChoose();

    u8 cursorPos = Menu_GetCursorPos();

    if (sLastLoadCursorPos != cursorPos)
    {
        switch (cursorPos)
        {
            case 0: 
                ShowLoadInfoWindow(SAVE_AUTO_1);
                break;
            case 1:
                ShowLoadInfoWindow(SAVE_MANUAL_1);
                break;
            case 2:
                ShowLoadInfoWindow(SAVE_MANUAL_2);
                break;
            case 3:
                ShowLoadInfoWindow(SAVE_MANUAL_3);
                break;
            case 4:
                ShowLoadInfoWindow(SAVE_MANUAL_4);
                break;
        }
    }

    sLastLoadCursorPos = cursorPos;

    switch (result)
    {
    case 0: 
        sChosenSaveSlot = SAVE_AUTO_1;
        if (SlotIsValid(sChosenSaveSlot))
        {
            sSaveDialogCallback = LoadDoLoadCallback;
        }
        return SAVE_IN_PROGRESS;
    case 1: // slot
        sChosenSaveSlot = SAVE_MANUAL_1;
        if (SlotIsValid(sChosenSaveSlot))
        {
            sSaveDialogCallback = LoadDoLoadCallback;
        }
        return SAVE_IN_PROGRESS;
    case 2:
        sChosenSaveSlot = SAVE_MANUAL_2;
        if (SlotIsValid(sChosenSaveSlot))
        {
            sSaveDialogCallback = LoadDoLoadCallback;
        }
        return SAVE_IN_PROGRESS;
    case 3:
        sChosenSaveSlot = SAVE_MANUAL_3;
        if (SlotIsValid(sChosenSaveSlot))    
        {
            sSaveDialogCallback = LoadDoLoadCallback;
        }
        return SAVE_IN_PROGRESS;
    case 4:
        sChosenSaveSlot = SAVE_MANUAL_4;
        if (SlotIsValid(sChosenSaveSlot))
        {
            sSaveDialogCallback = LoadDoLoadCallback;
        }
        return SAVE_IN_PROGRESS;
    case MENU_B_PRESSED:
        sSaveDialogCallback = NULL;
        RemoveStartMenuWindow();
        HideSaveInfoWindow();
        HideSaveMessageWindow();
        FillBgTilemapBufferRect(0, 0, 0, 0, 32, 32, 17); // TODO: TARC This is a hack to clear the window, Why is it not actually getting removed?
        return SAVE_CANCELED;
    }

    return SAVE_IN_PROGRESS;
}


static u8 SaveDoSaveCallback(void)
{
    u8 saveStatus;

    if (gDifferentSaveFile == TRUE)
    {
        saveStatus = TrySavingData(SAVE_OVERWRITE_DIFFERENT_FILE, sChosenSaveSlot);
        gDifferentSaveFile = FALSE;
    }
    else
    {
        saveStatus = TrySavingData(SAVE_NORMAL, sChosenSaveSlot);
    }

    if (saveStatus == SAVE_STATUS_OK)
    {
        ShowSaveMessage(gText_PlayerSavedGame, SaveSuccessCallback);
    }
    else
        ShowSaveMessage(gText_SaveError, SaveErrorCallback);

    SaveStartTimer();
    return SAVE_IN_PROGRESS;
}

static u8 LoadDoLoadCallback(void)
{
    ReloadSlot(sChosenSaveSlot);
    return SAVE_IN_PROGRESS;
}

static u8 SaveSuccessCallback(void)
{
    if (!IsTextPrinterActive(0))
    {
        PlaySE(SE_SAVE);
        sSaveDialogCallback = SaveReturnSuccessCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveReturnSuccessCallback(void)
{
    if (!IsSEPlaying())
    {
        sSaveDialogCallback = NULL;

        // Clear and remove save windows
        ClearStdWindowAndFrame(sSaveInfoWindowId, TRUE);
        RemoveWindow(sSaveInfoWindowId);
        HideStartMenuWindow();
        FillBgTilemapBufferRect(0, 0, 0, 0, 32, 32, 17); // TODO: TARC This is a hack to clear the window, Why is it not actually getting removed?
        return SAVE_SUCCESS;
    }
    else
    {
        return SAVE_IN_PROGRESS;
    }
}

static u8 SaveErrorCallback(void)
{
    if (!IsTextPrinterActive(0))
    {
        PlaySE(SE_BOO);
        sSaveDialogCallback = SaveReturnErrorCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveReturnErrorCallback(void)
{
    if (!SaveErrorTimer())
    {
        return SAVE_IN_PROGRESS;
    }
    else
    {
        HideSaveInfoWindow();
        return SAVE_ERROR;
    }
}

static void ShowSaveInfoWindow(void)
{
    struct WindowTemplate saveInfoWindow = sSaveInfoWindowTemplate;
    u8 color;
    u32 xOffset;
    u32 yOffset;

    saveInfoWindow.height -= 2;
    sSaveInfoWindowId = AddWindow(&saveInfoWindow);
    DrawStdWindowFrame(sSaveInfoWindowId, FALSE);

    color = TEXT_COLOR_RED;

    yOffset = 1;
    
    // Leave a blank space to draw the 'Which save slot' message. Why not render the text here....? That my friend is a good question 
    yOffset += 16;

    // Print player name
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingPlayer, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_NAME, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    PrintPlayerNameOnWindow(sSaveInfoWindowId, gStringVar4, xOffset, yOffset);

    // Print Completion
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingBadges, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_BADGES, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);

    // Print play time
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingTime, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_PLAY_TIME, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);

    CopyWindowToVram(sSaveInfoWindowId, COPYWIN_GFX);
}

static void ShowLoadInfoWindow(u8 slot)
{
    struct WindowTemplate saveInfoWindow = sSaveInfoWindowTemplate;
    u8 color;
    u32 xOffset;
    u32 yOffset;

    saveInfoWindow.height -= 2;

    if (!sSaveInfoWindowId)
    {
        sSaveInfoWindowId = AddWindow(&saveInfoWindow);
    }

    DrawStdWindowFrame(sSaveInfoWindowId, FALSE);

    color = TEXT_COLOR_RED;

   
    yOffset = 0;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_ConfirmLoad, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    yOffset += 17;

    DebugPrintfLevel(MGBA_LOG_ERROR, "ShowSaveInfoWindow %x", slot);

    if (!SlotIsValid(slot))
    {
         AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, sText_SlotEmpty, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    }
    else
    {

        // Print player name
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingPlayer, 0, yOffset, TEXT_SKIP_DRAW, NULL);
        BufferLoadMenuText(slot, SAVE_MENU_NAME, gStringVar4, color);
        xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
        PrintPlayerNameOnWindow(sSaveInfoWindowId, gStringVar4, xOffset, yOffset);

        // Print Completion
        yOffset += 16;
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingBadges, 0, yOffset, TEXT_SKIP_DRAW, NULL);
        BufferLoadMenuText(slot, SAVE_MENU_BADGES, gStringVar4, color);
        xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);

        // Print play time
        yOffset += 16;
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingTime, 0, yOffset, TEXT_SKIP_DRAW, NULL);
        BufferLoadMenuText(slot, SAVE_MENU_PLAY_TIME, gStringVar4, color);
        xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);
    }

    CopyWindowToVram(sSaveInfoWindowId, COPYWIN_GFX);
}

static void RemoveSaveInfoWindow(void)
{
    ClearStdWindowAndFrame(sSaveInfoWindowId, FALSE);
    RemoveWindow(sSaveInfoWindowId);
}

static void HideStartMenuWindow(void)
{
    ClearStdWindowAndFrame(GetStartMenuWindowId(), TRUE);
    RemoveStartMenuWindow();
    ScriptUnfreezeObjectEvents();
    UnlockPlayerFieldControls();
}

void HideStartMenu(void)
{
    PlaySE(SE_SELECT);
    HideStartMenuWindow();
}

void AppendToList(u8 *list, u8 *pos, u8 newEntry)
{
    list[*pos] = newEntry;
    (*pos)++;
}

void Script_ForceSaveGame(struct ScriptContext *ctx)
{
    SaveGame();
    ShowSaveInfoWindow();
    gMenuCallback = SaveCallback;
    sSaveDialogCallback = SaveDoSaveCallback;
}
