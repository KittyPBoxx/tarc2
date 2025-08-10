#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_message.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "scanline_effect.h"
#include "palette.h"
#include "party_menu.h"
#include "pokemon_icon.h"
#include "sprite.h"
#include "item.h"
#include "task.h"
#include "bg.h"
#include "gpu_regs.h"
#include "window.h"
#include "text.h"
#include "text_window.h"
#include "international_string_util.h"
#include "strings.h"
#include "battle_ai_main.h"
#include "battle_ai_util.h"
#include "list_menu.h"
#include "decompress.h"
#include "trainer_pokemon_sprites.h"
#include "malloc.h"
#include "string_util.h"
#include "util.h"
#include "data.h"
#include "reset_rtc_screen.h"
#include "reshow_battle_screen.h"
#include "constants/abilities.h"
#include "constants/party_menu.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/rgb.h"
#include "constants/hold_effects.h"

struct MonInfoStruct
{
    u8 battlerId:2;
    u8 aiBattlerId:2;

    u8 battlerWindowId;

    u8 mainListWindowId;
    u8 mainListTaskId;
    u8 currentSecondaryListItemId;
    u8 secondaryListItemCount;

    u8 modifyWindowId;

    u8 activeWindow;
    const struct BitfieldInfo *bitfield;
    bool8 battlerWasChanged[MAX_BATTLERS_COUNT];

    u8 aiViewState;

    u8 aiMonSpriteId;
    u8 aiMovesWindowId;

    union
    {
        u8 aiIconSpriteIds[MAX_BATTLERS_COUNT];
        u8 aiPartyIcons[PARTY_SIZE];
    } spriteIds;
};

struct __attribute__((__packed__)) BitfieldInfo
{
    u8 bitsCount;
    u8 currBit;
};

static const u8 sText_Moves[] = _("{COLOR BLUE}Moves");
static const u8 sText_Ability[] = _("{COLOR BLUE}Ability");
static const u8 sText_HeldItem[] = _("{COLOR BLUE}Held Item");

static const u16 sBgColor[] = {RGB_WHITE, RGB_GRAY};

enum
{
    ACTIVE_WIN_MAIN
};

static const struct BgTemplate sBgTemplates[] =
{
   {
       .bg = 0,
       .charBaseIndex = 0,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
   },
   {
       .bg = 1,
       .charBaseIndex = 2,
       .mapBaseIndex = 20,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
   }
};

static const struct WindowTemplate sBattlerWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 30,
    .height = 2,
    .paletteNum = 0xF,
    .baseBlock = 0x1B5
};

static const struct WindowTemplate sMainListWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 2,
    .width = 30,
    .height = 14,
    .paletteNum = 0xF,
    .baseBlock = 0x1
};

static void Task_MonInfoFadeIn(u8 taskId);
static void Task_MonInfoProcessInput(u8 taskId);
static void Task_MonInfoFadeOut(u8 taskId);
static void PrintOnBattlerWindow(u8 windowId, u8 battlerId);
static void PrintOnSummaryWindow(u8 windowId, u8 battlerId);
static void CreateMonSprite(u8 taskId, u16 species);
static void ClearMonSprite(u8 taskId);

static struct MonInfoStruct *GetStructPtr(u8 taskId)
{
    u8 *taskDataPtr = (u8 *)(&gTasks[taskId].data[0]);

    return (struct MonInfoStruct*)(T1_READ_PTR(taskDataPtr));
}

static void SetStructPtr(u8 taskId, void *ptr)
{
    u32 structPtr = (u32)(ptr);
    u8 *taskDataPtr = (u8 *)(&gTasks[taskId].data[0]);

    taskDataPtr[0] = structPtr >> 0;
    taskDataPtr[1] = structPtr >> 8;
    taskDataPtr[2] = structPtr >> 16;
    taskDataPtr[3] = structPtr >> 24;
}

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

#define sConditionSpriteId data[1]

void CB2_BattleInfoMenu(void)
{
    u8 taskId;
    struct MonInfoStruct *data;

    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        ResetVramOamAndBgCntRegs();
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
        ResetAllBgsCoordinates();
        FreeAllWindowBuffers();
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadPalette(sBgColor, 0, PLTT_SIZEOF(2));
        LoadPalette(GetOverworldTextboxPalettePtr(), 0xf0, 16);
        gMain.state++;
        break;
    case 4:
        taskId = CreateTask(Task_MonInfoFadeIn, 0);
        data = AllocZeroed(sizeof(struct MonInfoStruct));
        SetStructPtr(taskId, data);

        data->battlerId = gBattleStruct->debugBattler;
        data->battlerWindowId = AddWindow(&sBattlerWindowTemplate);
        PutWindowTilemap(data->battlerWindowId);
        PrintOnBattlerWindow(data->battlerWindowId, data->battlerId);
        CreateMonSprite(taskId, gBattleMons[data->battlerId].species);
        data->mainListWindowId = AddWindow(&sMainListWindowTemplate);
        PutWindowTilemap(data->mainListWindowId);
        PrintOnSummaryWindow(data->mainListWindowId, data->battlerId);
        data->activeWindow = ACTIVE_WIN_MAIN;
        gMain.state++;
        break;
    case 5:
        BeginNormalPaletteFade(-1, 0, 0x10, 0, 0);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_MonInfoFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_MonInfoProcessInput;
}

static void Task_MonInfoFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        struct MonInfoStruct *data = GetStructPtr(taskId);
        DestroyListMenuTask(data->mainListTaskId, 0, 0);
        FreeAllWindowBuffers();
        gBattleStruct->debugBattler = data->battlerId;
        Free(data);
        DestroyTask(taskId);
        SetMainCallback2(ReshowBattleScreenAfterMenu);
    }
}

static void ClearMonSprite(u8 taskId)
{
    struct MonInfoStruct *data = GetStructPtr(taskId);

    FreeMonIconPalettes();
    if (data->spriteIds.aiPartyIcons[0] != 0xFF)
    {
        DestroySpriteAndFreeResources(&gSprites[gSprites[data->spriteIds.aiPartyIcons[0]].sConditionSpriteId]);
        FreeAndDestroyMonIconSprite(&gSprites[data->spriteIds.aiPartyIcons[0]]);
    }
}

static void CreateMonSprite(u8 taskId, u16 species)
{
    struct MonInfoStruct *data = GetStructPtr(taskId);

    ClearMonSprite(taskId);
    LoadMonIconPalettes();
    data->spriteIds.aiPartyIcons[0] = CreateMonIcon(species, SpriteCallbackDummy, (5 * 41), 120, 1, 0);
    gSprites[data->spriteIds.aiPartyIcons[0]].sConditionSpriteId = CreateSprite(&gSpriteTemplate_StatusIcons, (5 * 41), 120, 0);
    gSprites[gSprites[data->spriteIds.aiPartyIcons[0]].sConditionSpriteId].oam.priority = 0;
}

static void Task_MonInfoProcessInput(u8 taskId)
{
    struct MonInfoStruct *data = GetStructPtr(taskId);

    // Exit the menu.
    if (JOY_NEW(B_BUTTON))
    {
        ClearMonSprite(taskId);
        BeginNormalPaletteFade(-1, 0, 0, 0x10, 0);
        gTasks[taskId].func = Task_MonInfoFadeOut;
        return;
    }

    // Try changing active battler.
    if (JOY_NEW(R_BUTTON))
    {
        if (data->battlerId++ == gBattlersCount - 1)
            data->battlerId = 0;
        PrintOnBattlerWindow(data->battlerWindowId, data->battlerId);
        PrintOnSummaryWindow(data->mainListWindowId, data->battlerId);
        CreateMonSprite(taskId, gBattleMons[data->battlerId].species);
    }
    else if (JOY_NEW(L_BUTTON))
    {
        if (data->battlerId-- == 0)
            data->battlerId = gBattlersCount - 1;
        PrintOnBattlerWindow(data->battlerWindowId, data->battlerId);
        PrintOnSummaryWindow(data->mainListWindowId, data->battlerId);
        CreateMonSprite(taskId, gBattleMons[data->battlerId].species);
    }
}

#undef sConditionSpriteId

static const u8 sText_Your[] = _("Your");
static const u8 sText_Opponent[] = _("Foe");

static const u8 sText_ButtonL[] = _("{L_BUTTON}");
static const u8 sText_ButtonR[] = _("{R_BUTTON}");
static const u8 sText_Close[] = _("{B_BUTTON} Close");

static void PrintOnBattlerWindow(u8 windowId, u8 battlerId)
{
    u8 text[POKEMON_NAME_LENGTH + 16];

    memset(text, CHAR_SPACE, sizeof(text));

    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        StringCopy(&text[0], sText_Your);
    else
        StringCopy(&text[0], sText_Opponent);

    u8 pos = StringLength(text);
    text[pos++] = CHAR_SPACE;
    text[pos++] = CHAR_HYPHEN;
    text[pos++] = CHAR_SPACE;

    StringCopy(&text[pos], gBattleMons[battlerId].nickname);

    u8 color[3];
    color[0] = TEXT_COLOR_DARK_GRAY;
    color[1] = TEXT_COLOR_WHITE;
    color[2] = TEXT_COLOR_DARK_GRAY;

    FillWindowPixelBuffer(windowId, PIXEL_FILL(2));
    AddTextPrinterParameterized4(windowId, FONT_SMALL, 8 * 4, 0, 0, 0, color, 0, text);

    AddTextPrinterParameterized(windowId, FONT_SMALL, sText_ButtonL, 8 * 1, 0, 0, NULL);
    AddTextPrinterParameterized(windowId, FONT_SMALL, sText_ButtonR, 8 * 14, 0, 0, NULL);

    AddTextPrinterParameterized4(windowId, FONT_SMALL, 8 * 24, 0, 0, 0, color, 0, sText_Close);

    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void PrintOnSummaryWindow(u8 windowId, u8 battlerId)
{
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));

    // Moves
    u8 move1[MOVE_NAME_LENGTH + 1];
    u8 move2[MOVE_NAME_LENGTH + 1];
    u8 move3[MOVE_NAME_LENGTH + 1];
    u8 move4[MOVE_NAME_LENGTH + 1];

    memset(move1, EOS, sizeof(move1));
    memset(move2, EOS, sizeof(move2));
    memset(move3, EOS, sizeof(move3));
    memset(move4, EOS, sizeof(move4));

    StringCopy(move1, GetMoveName(gBattleMons[battlerId].moves[0]));
    StringCopy(move2, GetMoveName(gBattleMons[battlerId].moves[1]));
    StringCopy(move3, GetMoveName(gBattleMons[battlerId].moves[2]));
    StringCopy(move4, GetMoveName(gBattleMons[battlerId].moves[3]));

    AddTextPrinterParameterized(windowId, FONT_NORMAL, sText_Moves, 0, 8*1, 0  , NULL);
    AddTextPrinterParameterized(windowId, FONT_SMALL, move1       , 0, 8*3, 0  , NULL);
    AddTextPrinterParameterized(windowId, FONT_SMALL, move2       , 8 * 13, 8*3, 0, NULL);
    AddTextPrinterParameterized(windowId, FONT_SMALL, move3       , 0     , 8*5, 0, NULL);
    AddTextPrinterParameterized(windowId, FONT_SMALL, move4       , 8 * 13, 8*5, 0, NULL);

    // Abilities
    u8 ability[ITEM_NAME_LENGTH + 1];
    memset(ability, EOS, sizeof(ability));
    StringCopy(ability, gAbilitiesInfo[gBattleMons[battlerId].ability].name);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, sText_Ability, 0, 8*8, 0, NULL);
    AddTextPrinterParameterized(windowId, FONT_SMALL, ability, 0, 8*10, 0, NULL);

    // Items
    u8 item[ITEM_NAME_LENGTH + 1];
    memset(item, EOS, sizeof(item));
    StringCopy(item, GetItemName(gBattleMons[battlerId].item));
    AddTextPrinterParameterized(windowId, FONT_NORMAL, sText_HeldItem, 8 * 13, 8*8, 0, NULL);
    AddTextPrinterParameterized(windowId, FONT_SMALL, item, 8 * 13, 8*10, 0, NULL);


    CopyWindowToVram(windowId, COPYWIN_FULL);
}