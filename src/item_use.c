#include "global.h"
#include "item_use.h"
#include "battle.h"
#include "battle_anim.h"
#include "berry.h"
#include "data.h"
#include "event_data.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "fldeff.h"
#include "item.h"
#include "item_menu.h"
#include "item_use.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokemon.h"
#include "script.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "constants/event_bg.h"
#include "constants/event_objects.h"
#include "constants/item_effects.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/map_types.h"

static void FieldCB_UseItemOnField(void);
static void Task_CallItemUseOnFieldCallback(u8);
static void Task_UseItemfinder(u8);
static void Task_CloseItemfinderMessage(u8);
static void Task_HiddenItemNearby(u8);
static void Task_StandingOnHiddenItem(u8);
static bool8 ItemfinderCheckForHiddenItems(const struct MapEvents *, u8);
static u8 GetDirectionToHiddenItem(s16, s16);
static void PlayerFaceHiddenItem(u8);
static void CheckForHiddenItemsInMapConnection(u8);
static void ItemUseOnFieldCB_Rod(u8);
static void ItemUseOnFieldCB_Itemfinder(u8);
static void Task_CloseCantUseKeyItemMessage(u8);
static void SetDistanceOfClosestHiddenItem(u8, s16, s16);

static const u8 sText_CantDismountBike[] = _("You can't dismount your BIKE here.{PAUSE_UNTIL_PRESS}");
static const u8 sText_ItemFinderNearby[] = _("Huh?\nThe ITEMFINDER's responding!\pThere's an item buried around here!{PAUSE_UNTIL_PRESS}");
static const u8 sText_ItemFinderOnTop[] = _("Oh!\nThe ITEMFINDER's shaking wildly!{PAUSE_UNTIL_PRESS}");
static const u8 sText_ItemFinderNothing[] = _("… … … …Nope!\nThere's no response.{PAUSE_UNTIL_PRESS}");
static const u8 sText_CoinCase[] = _("Your COINS:\n{STR_VAR_1}{PAUSE_UNTIL_PRESS}");
static const u8 sText_PowderQty[] = _("POWDER QTY: {STR_VAR_1}{PAUSE_UNTIL_PRESS}");
static const u8 sText_BootedUpTM[] = _("Booted up a TM.");
static const u8 sText_BootedUpHM[] = _("Booted up an HM.");
static const u8 sText_TMHMContainedVar1[] = _("It contained\n{STR_VAR_1}.\pTeach {STR_VAR_1}\nto a POKéMON?");
static const u8 sText_UsedVar2WildLured[] = _("{PLAYER} used the\n{STR_VAR_2}.\pWild POKéMON will be lured.{PAUSE_UNTIL_PRESS}");
static const u8 sText_UsedVar2WildRepelled[] = _("{PLAYER} used the\n{STR_VAR_2}.\pWild POKéMON will be repelled.{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayedPokeFluteCatchy[] = _("Played the POKé FLUTE.\pNow, that's a catchy tune!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayedPokeFlute[] = _("Played the POKé FLUTE.");
static const u8 sText_PokeFluteAwakenedMon[] = _("The POKé FLUTE awakened sleeping\nPOKéMON.{PAUSE_UNTIL_PRESS}");

// EWRAM variables
EWRAM_DATA static void(*sItemUseOnFieldCB)(u8 taskId) = NULL;

// Below is set TRUE by UseRegisteredKeyItemOnField
#define tUsingRegisteredKeyItem  data[3]

// UB here if an item with type ITEM_USE_MAIL or ITEM_USE_BAG_MENU uses SetUpItemUseCallback
// Never occurs in vanilla, but can occur with improperly created items
static const MainCallback sItemUseCallbacks[] =
{
    [ITEM_USE_PARTY_MENU - 1]       = CB2_ShowPartyMenuForItemUse,
    [ITEM_USE_FIELD - 1]            = CB2_ReturnToField,
    [ITEM_USE_PBLOCK_CASE - 1]      = NULL,
    [ITEM_USE_PARTY_MENU_MOVES - 1] = CB2_ShowPartyMenuForItemUse,
};

static const u8 sClockwiseDirections[] = {DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST};

#define tEnigmaBerryType data[4]

static void SetUpItemUseOnFieldCallback(u8 taskId)
{
    if (gTasks[taskId].tUsingRegisteredKeyItem != TRUE)
    {
        gFieldCallback = FieldCB_UseItemOnField;
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
    else
    {
        sItemUseOnFieldCB(taskId);
    }
}

static void FieldCB_UseItemOnField(void)
{
    FadeInFromBlack();
    CreateTask(Task_CallItemUseOnFieldCallback, 8);
}

static void Task_CallItemUseOnFieldCallback(u8 taskId)
{
    if (IsWeatherNotFadingIn() == 1)
        sItemUseOnFieldCB(taskId);
}

static void DisplayCannotUseItemMessage(u8 taskId, bool8 isUsingRegisteredKeyItemOnField, const u8 *str)
{
    DisplayItemMessageOnField(taskId, gStringVar4, Task_CloseCantUseKeyItemMessage);
}

void DisplayDadsAdviceCannotUseItemMessage(u8 taskId, bool8 isUsingRegisteredKeyItemOnField)
{
    DisplayCannotUseItemMessage(taskId, isUsingRegisteredKeyItemOnField, gText_DadsAdvice);
}

static void Task_CloseCantUseKeyItemMessage(u8 taskId)
{
    ClearDialogWindowAndFrame(0, TRUE);
    DestroyTask(taskId);
    ScriptUnfreezeObjectEvents();
    UnlockPlayerFieldControls();
}

u8 CheckIfItemIsTMHMOrEvolutionStone(u16 itemId)
{
    if (GetItemFieldFunc(itemId) == ItemUseOutOfBattle_TMHM)
        return 1;
    else if (GetItemFieldFunc(itemId) == ItemUseOutOfBattle_EvolutionStone)
        return 2;
    else
        return 0;
}

STATIC_ASSERT(I_EXP_SHARE_ITEM < GEN_6 || I_EXP_SHARE_FLAG > TEMP_FLAGS_END, YouNeedToSetAFlagToUseGen6ExpShare);

void ItemUseOutOfBattle_ExpShare(u8 taskId)
{
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_Bike(u8 taskId)
{
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

static bool32 CanFish(void)
{
    s16 x, y;
    u16 tileBehavior;

    GetXYCoordsOneStepInFrontOfPlayerNonDiagonal(&x, &y);
    tileBehavior = MapGridGetMetatileBehaviorAt(x, y);

    if (MetatileBehavior_IsWaterfall(tileBehavior))
        return FALSE;

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_UNDERWATER))
        return FALSE;

    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
    {
        if (IsPlayerFacingSurfableFishableWater())
            return TRUE;
    }
    else
    {
        if (MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehavior) && MapGridGetCollisionAt(x, y) == 0)
            return TRUE;
        if (MetatileBehavior_IsBridgeOverWaterNoEdge(tileBehavior) == TRUE)
            return TRUE;
    }

    return FALSE;
}

void ItemUseOutOfBattle_Rod(u8 taskId)
{
    if (CanFish() == TRUE)
    {
        sItemUseOnFieldCB = ItemUseOnFieldCB_Rod;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
    {
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
}

static void ItemUseOnFieldCB_Rod(u8 taskId)
{
    StartFishing(GetItemSecondaryId(gSpecialVar_ItemId));
    DestroyTask(taskId);
}

void ItemUseOutOfBattle_Itemfinder(u8 var)
{
    sItemUseOnFieldCB = ItemUseOnFieldCB_Itemfinder;
    SetUpItemUseOnFieldCallback(var);
}

static void ItemUseOnFieldCB_Itemfinder(u8 taskId)
{
    if (ItemfinderCheckForHiddenItems(gMapHeader.events, taskId) == TRUE)
        gTasks[taskId].func = Task_UseItemfinder;
    else
        DisplayItemMessageOnField(taskId, sText_ItemFinderNothing, Task_CloseItemfinderMessage);
}

// Define itemfinder task data
#define tItemDistanceX    data[0]
#define tItemDistanceY    data[1]
#define tItemFound        data[2]
#define tCounter          data[3] // Used to count delay between beeps and rotations during player spin
#define tItemfinderBeeps  data[4]
#define tFacingDir        data[5]

static void Task_UseItemfinder(u8 taskId)
{
    u8 playerDir;
    u8 playerDirToItem;
    u8 i;
    s16 *data = gTasks[taskId].data;
    if (tCounter == 0)
    {
        if (tItemfinderBeeps == 4)
        {
            playerDirToItem = GetDirectionToHiddenItem(tItemDistanceX, tItemDistanceY);
            if (playerDirToItem != DIR_NONE)
            {
                PlayerFaceHiddenItem(sClockwiseDirections[playerDirToItem - 1]);
                gTasks[taskId].func = Task_HiddenItemNearby;
            }
            else
            {
                // Player is standing on hidden item
                playerDir = GetPlayerFacingDirection();
                for (i = 0; i < ARRAY_COUNT(sClockwiseDirections); i++)
                {
                    if (playerDir == sClockwiseDirections[i])
                        tFacingDir = (i + 1) & 3;
                }
                gTasks[taskId].func = Task_StandingOnHiddenItem;
                tCounter = 0;
                tItemFound = 0;
            }
            return;
        }
        PlaySE(SE_ITEMFINDER);
        tItemfinderBeeps++;
    }
    tCounter = (tCounter + 1) & 0x1F;
}

static void Task_CloseItemfinderMessage(u8 taskId)
{
    ClearDialogWindowAndFrame(0, TRUE);
    ScriptUnfreezeObjectEvents();
    UnlockPlayerFieldControls();
    DestroyTask(taskId);
}

static bool8 ItemfinderCheckForHiddenItems(const struct MapEvents *events, u8 taskId)
{
    int itemX, itemY;
    s16 playerX, playerY, i, distanceX, distanceY;
    PlayerGetDestCoords(&playerX, &playerY);
    gTasks[taskId].tItemFound = FALSE;

    for (i = 0; i < events->bgEventCount; i++)
    {
        // Check if there are any hidden items on the current map that haven't been picked up
        if (events->bgEvents[i].kind == BG_EVENT_HIDDEN_ITEM && !FlagGet(events->bgEvents[i].bgUnion.hiddenItem.hiddenItemId + FLAG_HIDDEN_ITEMS_START))
        {
            itemX = (u16)events->bgEvents[i].x + MAP_OFFSET;
            distanceX = itemX - playerX;
            itemY = (u16)events->bgEvents[i].y + MAP_OFFSET;
            distanceY = itemY - playerY;

            // Player can see 7 metatiles on either side horizontally
            // and 5 metatiles on either side vertically
            if (distanceX >= -7 && distanceX <= 7 && distanceY >= -5 && distanceY <= 5)
                SetDistanceOfClosestHiddenItem(taskId, distanceX, distanceY);
        }
    }

    CheckForHiddenItemsInMapConnection(taskId);
    if (gTasks[taskId].tItemFound == TRUE)
        return TRUE;
    else
        return FALSE;
}

static bool8 IsHiddenItemPresentAtCoords(const struct MapEvents *events, s16 x, s16 y)
{
    u8 bgEventCount = events->bgEventCount;
    const struct BgEvent *bgEvent = events->bgEvents;
    int i;

    for (i = 0; i < bgEventCount; i++)
    {
        if (bgEvent[i].kind == BG_EVENT_HIDDEN_ITEM && x == (u16)bgEvent[i].x && y == (u16)bgEvent[i].y) // hidden item and coordinates matches x and y passed?
        {
            if (!FlagGet(bgEvent[i].bgUnion.hiddenItem.hiddenItemId + FLAG_HIDDEN_ITEMS_START))
                return TRUE;
            else
                return FALSE;
        }
    }
    return FALSE;
}

static bool8 IsHiddenItemPresentInConnection(const struct MapConnection *connection, int x, int y)
{
    s16 connectionX, connectionY;
    struct MapHeader const *const connectionHeader = GetMapHeaderFromConnection(connection);

// To convert our x/y into coordinates that are relative to the connected map, we must:
//  - Subtract the virtual offset used for the border buffer (MAP_OFFSET).
//  - Subtract the horizontal offset between North/South connections, or the vertical offset for East/West
//  - Account for map size. (0,0) is in the NW corner of our map, so when looking North/West we have to add the height/width of the connected map,
//     and when looking South/East we have to subtract the height/width of our current map.
#define localX (x - MAP_OFFSET)
#define localY (y - MAP_OFFSET)
    switch (connection->direction)
    {
    case CONNECTION_NORTH:
        connectionX = localX - connection->offset;
        connectionY = connectionHeader->mapLayout->height + localY;
        break;
    case CONNECTION_SOUTH:
        connectionX = localX - connection->offset;
        connectionY = localY - gMapHeader.mapLayout->height;
        break;
    case CONNECTION_WEST:
        connectionX = connectionHeader->mapLayout->width + localX;
        connectionY = localY - connection->offset;
        break;
    case CONNECTION_EAST:
        connectionX = localX - gMapHeader.mapLayout->width;
        connectionY = localY - connection->offset;
        break;
    default:
        return FALSE;
    }
    return IsHiddenItemPresentAtCoords(connectionHeader->events, connectionX, connectionY);
}

#undef localX
#undef localY

static void CheckForHiddenItemsInMapConnection(u8 taskId)
{
    s16 playerX, playerY;
    s16 x, y;
    s16 width = gMapHeader.mapLayout->width + MAP_OFFSET;
    s16 height = gMapHeader.mapLayout->height + MAP_OFFSET;

    s16 var1 = MAP_OFFSET;
    s16 var2 = MAP_OFFSET;

    PlayerGetDestCoords(&playerX, &playerY);

    // Player can see 7 metatiles on either side horizontally
    // and 5 metatiles on either side vertically
    for (x = playerX - 7; x <= playerX + 7; x++)
    {
        for (y = playerY - 5; y <= playerY + 5; y++)
        {
            if (var1 > x
             || x >= width
             || var2 > y
             || y >= height)
            {
                const struct MapConnection *conn = GetMapConnectionAtPos(x, y);
                if (conn && IsHiddenItemPresentInConnection(conn, x, y) == TRUE)
                    SetDistanceOfClosestHiddenItem(taskId, x - playerX, y - playerY);
            }
        }
    }
}

static void SetDistanceOfClosestHiddenItem(u8 taskId, s16 itemDistanceX, s16 itemDistanceY)
{
    s16 *data = gTasks[taskId].data;
    s16 oldItemAbsX, oldItemAbsY, newItemAbsX, newItemAbsY;

    if (tItemFound == FALSE)
    {
        // No other items found yet, set this one
        tItemDistanceX = itemDistanceX;
        tItemDistanceY = itemDistanceY;
        tItemFound = TRUE;
    }
    else
    {
        // Other items have been found, check if this one is closer

        // Get absolute x distance of the already-found item
        if (tItemDistanceX < 0)
            oldItemAbsX = tItemDistanceX * -1; // WEST
        else
            oldItemAbsX = tItemDistanceX;      // EAST

        // Get absolute y distance of the already-found item
        if (tItemDistanceY < 0)
            oldItemAbsY = tItemDistanceY * -1; // NORTH
        else
            oldItemAbsY = tItemDistanceY;      // SOUTH

        // Get absolute x distance of the newly-found item
        if (itemDistanceX < 0)
            newItemAbsX = itemDistanceX * -1;
        else
            newItemAbsX = itemDistanceX;

        // Get absolute y distance of the newly-found item
        if (itemDistanceY < 0)
            newItemAbsY = itemDistanceY * -1;
        else
            newItemAbsY = itemDistanceY;


        if (oldItemAbsX + oldItemAbsY > newItemAbsX + newItemAbsY)
        {
            // New item is closer
            tItemDistanceX = itemDistanceX;
            tItemDistanceY = itemDistanceY;
        }
        else
        {
            if (oldItemAbsX + oldItemAbsY == newItemAbsX + newItemAbsY
            && (oldItemAbsY > newItemAbsY || (oldItemAbsY == newItemAbsY && tItemDistanceY < itemDistanceY)))
            {
                // If items are equal distance, use whichever is closer on the Y axis or further south
                tItemDistanceX = itemDistanceX;
                tItemDistanceY = itemDistanceY;
            }
        }
    }
}

static u8 GetDirectionToHiddenItem(s16 itemDistanceX, s16 itemDistanceY)
{
    s16 absX, absY;

    if (itemDistanceX == 0 && itemDistanceY == 0)
        return DIR_NONE; // player is standing on the item.

    // Get absolute X distance.
    if (itemDistanceX < 0)
        absX = itemDistanceX * -1;
    else
        absX = itemDistanceX;

    // Get absolute Y distance.
    if (itemDistanceY < 0)
        absY = itemDistanceY * -1;
    else
        absY = itemDistanceY;

    if (absX > absY)
    {
        if (itemDistanceX < 0)
            return DIR_EAST;
        else
            return DIR_NORTH;
    }
    else
    {
        if (absX < absY)
        {
            if (itemDistanceY < 0)
                return DIR_SOUTH;
            else
                return DIR_WEST;
        }
        if (absX == absY)
        {
            if (itemDistanceY < 0)
                return DIR_SOUTH;
            else
                return DIR_WEST;
        }
        return DIR_NONE; // Unreachable
    }
}

static void PlayerFaceHiddenItem(u8 direction)
{
    ObjectEventClearHeldMovementIfFinished(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(LOCALID_PLAYER, 0, 0)]);
    ObjectEventClearHeldMovement(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(LOCALID_PLAYER, 0, 0)]);
    UnfreezeObjectEvent(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(LOCALID_PLAYER, 0, 0)]);
    PlayerTurnInPlace(direction);
}

static void Task_HiddenItemNearby(u8 taskId)
{
    if (ObjectEventCheckHeldMovementStatus(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(LOCALID_PLAYER, 0, 0)]) == TRUE)
        DisplayItemMessageOnField(taskId, sText_ItemFinderNearby, Task_CloseItemfinderMessage);
}

static void Task_StandingOnHiddenItem(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (ObjectEventCheckHeldMovementStatus(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(LOCALID_PLAYER, 0, 0)]) == TRUE
    || tItemFound == FALSE)
    {
        // Spin player around on item
        PlayerFaceHiddenItem(sClockwiseDirections[tFacingDir]);
        tItemFound = TRUE;
        tFacingDir = (tFacingDir + 1) & 3;
        tCounter++;

        if (tCounter == 4)
            DisplayItemMessageOnField(taskId, sText_ItemFinderOnTop, Task_CloseItemfinderMessage);
    }
}

// Undefine itemfinder task data
#undef tItemDistanceX
#undef tItemDistanceY
#undef tItemFound
#undef tCounter
#undef tItemfinderBeeps
#undef tFacingDir

void ItemUseOutOfBattle_CoinCase(u8 taskId)
{
    DisplayItemMessageOnField(taskId, gStringVar4, Task_CloseCantUseKeyItemMessage);
}

void ItemUseOutOfBattle_PowderJar(u8 taskId)
{
}

void ItemUseOutOfBattle_WailmerPail(u8 taskId)
{
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_Medicine(u8 taskId)
{
    gItemUseCB = ItemUseCB_Medicine;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_AbilityCapsule(u8 taskId)
{
    gItemUseCB = ItemUseCB_AbilityCapsule;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_AbilityPatch(u8 taskId)
{
    gItemUseCB = ItemUseCB_AbilityPatch;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_Mint(u8 taskId)
{
    gItemUseCB = ItemUseCB_Mint;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_ResetEVs(u8 taskId)
{
    gItemUseCB = ItemUseCB_ResetEVs;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_ReduceEV(u8 taskId)
{
    gItemUseCB = ItemUseCB_ReduceEV;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_SacredAsh(u8 taskId)
{
    gItemUseCB = ItemUseCB_SacredAsh;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_PPRecovery(u8 taskId)
{
    gItemUseCB = ItemUseCB_PPRecovery;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_PPUp(u8 taskId)
{
    gItemUseCB = ItemUseCB_PPUp;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_RareCandy(u8 taskId)
{
    gItemUseCB = ItemUseCB_RareCandy;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_DynamaxCandy(u8 taskId)
{
    gItemUseCB = ItemUseCB_DynamaxCandy;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_TMHM(u8 taskId)
{
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void ItemUseOutOfBattle_Repel(u8 taskId)
{
}

void HandleUseExpiredRepel(struct ScriptContext *ctx)
{
#if VAR_LAST_REPEL_LURE_USED != 0
    VarSet(VAR_REPEL_STEP_COUNT, GetItemHoldEffectParam(VarGet(VAR_LAST_REPEL_LURE_USED)));
#endif
}

void ItemUseOutOfBattle_Lure(u8 taskId)
{
}

void HandleUseExpiredLure(struct ScriptContext *ctx)
{
#if VAR_LAST_REPEL_LURE_USED != 0
    VarSet(VAR_REPEL_STEP_COUNT, GetItemHoldEffectParam(VarGet(VAR_LAST_REPEL_LURE_USED)) | REPEL_LURE_MASK);
#endif
}

void Task_UseDigEscapeRopeOnField(u8 taskId)
{
    ResetInitialPlayerAvatarState();
    StartEscapeRopeFieldEffect();
    DestroyTask(taskId);
}

static void ItemUseOnFieldCB_EscapeRope(u8 taskId)
{
    Overworld_ResetStateAfterDigEscRope();
    if (I_KEY_ESCAPE_ROPE < GEN_8)
        RemoveBagItem(gSpecialVar_ItemId, 1);

    CopyItemName(gSpecialVar_ItemId, gStringVar2);
    StringExpandPlaceholders(gStringVar4, gText_PlayerUsedVar2);
    gTasks[taskId].data[0] = 0;
    DisplayItemMessageOnField(taskId, gStringVar4, Task_UseDigEscapeRopeOnField);
}

bool8 CanUseDigOrEscapeRopeOnCurMap(void)
{
    if (gMapHeader.allowEscaping)
        return TRUE;
    else
        return FALSE;
}

void ItemUseOutOfBattle_EscapeRope(u8 taskId)
{
    if (CanUseDigOrEscapeRopeOnCurMap() == TRUE)
    {
        sItemUseOnFieldCB = ItemUseOnFieldCB_EscapeRope;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
    {
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
}

void ItemUseOutOfBattle_EvolutionStone(u8 taskId)
{
    gItemUseCB = ItemUseCB_EvolutionStone;
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

static u32 GetBallThrowableState(void)
{
    return BALL_THROW_UNABLE_NO_ROOM;
}

bool32 CanThrowBall(void)
{
    return (GetBallThrowableState() == BALL_THROW_ABLE);
}

static const u8 sText_CantThrowPokeBall_TwoMons[] = _("Cannot throw a ball!\nThere are two Pokémon out there!\p");
static const u8 sText_CantThrowPokeBall_SemiInvulnerable[] = _("Cannot throw a ball!\nThere's no Pokémon in sight!\p");
static const u8 sText_CantThrowPokeBall_Disabled[] = _("POKé BALLS cannot be used\nright now!\p");
void ItemUseInBattle_PokeBall(u8 taskId)
{
}

void ItemUseInBattle_PartyMenu(u8 taskId)
{
    gItemUseCB = ItemUseCB_BattleScript;
}

void ItemUseInBattle_PartyMenuChooseMove(u8 taskId)
{
    gItemUseCB = ItemUseCB_BattleChooseMove;
}

static bool32 SelectedMonHasStatus2(u16 itemId)
{
    if (gPartyMenu.slotId == 0)
        return gBattleMons[0].status2 & GetItemStatus2Mask(itemId);
    else if (gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI) && gPartyMenu.slotId == 1)
        return gBattleMons[2].status2 & GetItemStatus2Mask(itemId);
    return FALSE;
}

// Returns whether an item can be used in battle and sets the fail text.
bool32 CannotUseItemsInBattle(u16 itemId, struct Pokemon *mon)
{
    u16 battleUsage = GetItemBattleUsage(itemId);
    bool8 cannotUse = FALSE;
    const u8* failStr = NULL;
    u32 i;
    u16 hp = GetMonData(mon, MON_DATA_HP);

    // Embargo Check
    if ((gPartyMenu.slotId == 0 && gStatuses3[B_POSITION_PLAYER_LEFT] & STATUS3_EMBARGO)
        || (gPartyMenu.slotId == 1 && gStatuses3[B_POSITION_PLAYER_RIGHT] & STATUS3_EMBARGO))
    {
        return TRUE;
    }

    // battleUsage checks
    switch (battleUsage)
    {
    case EFFECT_ITEM_INCREASE_STAT:
        if (gBattleMons[gBattlerInMenuId].statStages[GetItemEffect(itemId)[1]] == MAX_STAT_STAGE)
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_SET_FOCUS_ENERGY:
        if (gBattleMons[gBattlerInMenuId].status2 & STATUS2_FOCUS_ENERGY_ANY)
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_SET_MIST:
        if (gSideStatuses[GetBattlerSide(gBattlerInMenuId)] & SIDE_STATUS_MIST)
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_ESCAPE:
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_THROW_BALL:
        switch (GetBallThrowableState())
        {
        case BALL_THROW_UNABLE_TWO_MONS:
            failStr = sText_CantThrowPokeBall_TwoMons;
            cannotUse = TRUE;
            break;
        case BALL_THROW_UNABLE_NO_ROOM:
            failStr = gText_BoxFull;
            cannotUse = TRUE;
            break;
        case BALL_THROW_UNABLE_SEMI_INVULNERABLE:
            failStr = sText_CantThrowPokeBall_SemiInvulnerable;
            cannotUse = TRUE;
            break;
        case BALL_THROW_UNABLE_DISABLED_FLAG:
            failStr = sText_CantThrowPokeBall_Disabled;
            cannotUse = TRUE;
            break;
        }
        break;
    case EFFECT_ITEM_INCREASE_ALL_STATS:
        for (i = STAT_ATK; i < NUM_STATS; i++)
        {
            if (CompareStat(gBattlerInMenuId, i, MAX_STAT_STAGE, CMP_EQUAL))
            {
                cannotUse = TRUE;
                break;
            }
        }
        break;
    case EFFECT_ITEM_RESTORE_HP:
        if (hp == 0 || hp == GetMonData(mon, MON_DATA_MAX_HP))
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_CURE_STATUS:
        if (!((GetMonData(mon, MON_DATA_STATUS) & GetItemStatus1Mask(itemId))
            || SelectedMonHasStatus2(itemId)))
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_HEAL_AND_CURE_STATUS:
        if ((hp == 0 || hp == GetMonData(mon, MON_DATA_MAX_HP))
            && !((GetMonData(mon, MON_DATA_STATUS) & GetItemStatus1Mask(itemId))
            || SelectedMonHasStatus2(itemId)))
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_REVIVE:
        if (hp != 0)
            cannotUse = TRUE;
        break;
    case EFFECT_ITEM_RESTORE_PP:
        if (GetItemEffect(itemId)[4] == ITEM4_HEAL_PP)
        {
            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                if (GetMonData(mon, MON_DATA_PP1 + i) < CalculatePPWithBonus(GetMonData(mon, MON_DATA_MOVE1 + i), GetMonData(mon, MON_DATA_PP_BONUSES), i))
                    break;
            }
            if (i == MAX_MON_MOVES)
                cannotUse = TRUE;
        }
        else if (GetMonData(mon, MON_DATA_PP1 + gPartyMenu.data1) == CalculatePPWithBonus(GetMonData(mon, MON_DATA_MOVE1 + gPartyMenu.data1), GetMonData(mon, MON_DATA_PP_BONUSES), gPartyMenu.data1))
        {
            cannotUse = TRUE;
        }
        break;
    }

    if (failStr != NULL)
        StringExpandPlaceholders(gStringVar4, failStr);
    else
        StringExpandPlaceholders(gStringVar4, gText_WontHaveEffect);

    return cannotUse;
}

void ItemUseInBattle_BagMenu(u8 taskId)
{
}

void ItemUseOutOfBattle_EnigmaBerry(u8 taskId)
{
    switch (GetItemEffectType(gSpecialVar_ItemId))
    {
    case ITEM_EFFECT_HEAL_HP:
    case ITEM_EFFECT_CURE_POISON:
    case ITEM_EFFECT_CURE_SLEEP:
    case ITEM_EFFECT_CURE_BURN:
    case ITEM_EFFECT_CURE_FREEZE_FROSTBITE:
    case ITEM_EFFECT_CURE_PARALYSIS:
    case ITEM_EFFECT_CURE_ALL_STATUS:
    case ITEM_EFFECT_ATK_EV:
    case ITEM_EFFECT_HP_EV:
    case ITEM_EFFECT_SPATK_EV:
    case ITEM_EFFECT_SPDEF_EV:
    case ITEM_EFFECT_SPEED_EV:
    case ITEM_EFFECT_DEF_EV:
        gTasks[taskId].tEnigmaBerryType = ITEM_USE_PARTY_MENU;
        ItemUseOutOfBattle_Medicine(taskId);
        break;
    case ITEM_EFFECT_SACRED_ASH:
        gTasks[taskId].tEnigmaBerryType = ITEM_USE_PARTY_MENU;
        ItemUseOutOfBattle_SacredAsh(taskId);
        break;
    case ITEM_EFFECT_RAISE_LEVEL:
        gTasks[taskId].tEnigmaBerryType = ITEM_USE_PARTY_MENU;
        ItemUseOutOfBattle_RareCandy(taskId);
        break;
    case ITEM_EFFECT_PP_UP:
    case ITEM_EFFECT_PP_MAX:
        gTasks[taskId].tEnigmaBerryType = ITEM_USE_PARTY_MENU;
        ItemUseOutOfBattle_PPUp(taskId);
        break;
    case ITEM_EFFECT_HEAL_PP:
        gTasks[taskId].tEnigmaBerryType = ITEM_USE_PARTY_MENU;
        ItemUseOutOfBattle_PPRecovery(taskId);
        break;
    default:
        gTasks[taskId].tEnigmaBerryType = ITEM_USE_BAG_MENU;
        ItemUseOutOfBattle_CannotUse(taskId);
        break;
    }
}

void ItemUseOutOfBattle_FormChange(u8 taskId)
{
    if (!gTasks[taskId].tUsingRegisteredKeyItem)
    {
        gItemUseCB = ItemUseCB_FormChange;
        gTasks[taskId].data[0] = FALSE;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
    {
        // TODO: handle key items with callbacks to menus allow to be used by registering them.
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
}

void ItemUseOutOfBattle_FormChange_ConsumedOnUse(u8 taskId)
{
    if (!gTasks[taskId].tUsingRegisteredKeyItem)
    {
        gItemUseCB = ItemUseCB_FormChange_ConsumedOnUse;
        gTasks[taskId].data[0] = TRUE;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
    {
        // TODO: handle key items with callbacks to menus allow to be used by registering them.
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
}

void ItemUseOutOfBattle_RotomCatalog(u8 taskId)
{
    if (!gTasks[taskId].tUsingRegisteredKeyItem)
    {
        gItemUseCB = ItemUseCB_RotomCatalog;
        gTasks[taskId].data[0] = TRUE;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
    {
        // TODO: handle key items with callbacks to menus allow to be used by registering them.
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
}

void ItemUseOutOfBattle_ZygardeCube(u8 taskId)
{
    if (!gTasks[taskId].tUsingRegisteredKeyItem)
    {
        gItemUseCB = ItemUseCB_ZygardeCube;
        gTasks[taskId].data[0] = TRUE;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
    {
        // TODO: handle key items with callbacks to menus allow to be used by registering them.
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
}

void ItemUseOutOfBattle_Fusion(u8 taskId)
{
    if (!gTasks[taskId].tUsingRegisteredKeyItem)
    {
        gItemUseCB = ItemUseCB_Fusion;
        gTasks[taskId].data[0] = FALSE;
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
    else
    {
        // TODO: handle key items with callbacks to menus allow to be used by registering them.
        DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
    }
}

void Task_UseHoneyOnField(u8 taskId)
{
    //ResetInitialPlayerAvatarState();
    StartSweetScentFieldEffect();
    DestroyTask(taskId);
}

void ItemUseOutOfBattle_CannotUse(u8 taskId)
{
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void FieldUseFunc_VsSeeker(u8 taskId)
{
    DisplayDadsAdviceCannotUseItemMessage(taskId, gTasks[taskId].tUsingRegisteredKeyItem);
}

void Task_ItemUse_CloseMessageBoxAndReturnToField_VsSeeker(u8 taskId)
{
    Task_CloseCantUseKeyItemMessage(taskId);
}


void ItemUseOutOfBattle_PokeFlute(u8 taskId)
{
}

#undef tUsingRegisteredKeyItem
