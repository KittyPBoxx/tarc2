#include "global.h"
#include "debug.h"
#include "malloc.h"
#include "battle.h"
#include "data.h"
#include "diploma.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_message_box.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_specials.h"
#include "field_weather.h"
#include "graphics.h"
#include "international_string_util.h"
#include "item_icon.h"
#include "load_save.h"
#include "list_menu.h"
#include "main.h"
#include "menu.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon.h"
#include "random.h"
#include "rtc.h"
#include "script.h"
#include "script_menu.h"
#include "sound.h"
#include "starter_choose.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "tilesets.h"
#include "wallclock.h"
#include "window.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/field_effects.h"
#include "constants/field_specials.h"
#include "constants/items.h"
#include "constants/heal_locations.h"
#include "constants/map_types.h"
#include "constants/songs.h"
#include "constants/moves.h"
#include "constants/party_menu.h"
#include "constants/weather.h"
#include "constants/metatile_labels.h"
#include "constants/rgb.h"
#include "palette.h"
#include "battle_util.h"
#include "naming_screen.h"

#define TAG_ITEM_ICON 5500

#define GFXTAG_MULTICHOICE_SCROLL_ARROWS 2000
#define PALTAG_MULTICHOICE_SCROLL_ARROWS 100

#define ELEVATOR_WINDOW_WIDTH  3
#define ELEVATOR_WINDOW_HEIGHT 3
#define ELEVATOR_LIGHT_STAGES  3

static EWRAM_DATA u8 sSlidingDoorNextFrameCounter = 0;
static EWRAM_DATA u8 sSlidingDoorFrame = 0;
static EWRAM_DATA u8 sTutorMoveAndElevatorWindowId = 0;
static EWRAM_DATA u16 sLilycoveDeptStore_DefaultFloorChoice = 0;
static EWRAM_DATA struct ListMenuItem *sScrollableMultichoice_ListMenuItem = NULL;

static EWRAM_DATA u16 sFrontierExchangeCorner_NeverRead = 0;
static EWRAM_DATA u8 sScrollableMultichoice_ItemSpriteId = 0;
static EWRAM_DATA u8 sBattlePointsWindowId = 0;
static EWRAM_DATA u8 sFrontierExchangeCorner_ItemIconWindowId = 0;
static EWRAM_DATA u8 sPCBoxToSendMon = 0;
COMMON_DATA struct ListMenuTemplate gScrollableMultichoice_ListMenuTemplate = {0};
EWRAM_DATA u16 gScrollableMultichoice_ScrollOffset = 0;

void TryLoseFansFromPlayTime(void);
void SetPlayerGotFirstFans(void);
u16 GetNumFansOfPlayerInTrainerFanClub(void);

static void LoadLinkPartnerObjectEventSpritePalette(u16, u8, u8);
static void Task_PetalburgGymSlideOpenRoomDoors(u8);
static void PetalburgGymSetDoorMetatiles(u8, u16);
static void Task_PCTurnOnEffect(u8);
static void PCTurnOnEffect(struct Task *);
static void PCTurnOnEffect_SetMetatile(s16, s8, s8);
static void PCTurnOffEffect(void);
static void Task_LotteryCornerComputerEffect(u8);
static void LotteryCornerComputerEffect(struct Task *);
static void Task_ShakeCamera(u8);
static void StopCameraShake(u8);
static void Task_MoveElevator(u8);
static void MoveElevatorWindowLights(u16, bool8);
static void Task_MoveElevatorWindowLights(u8);
static void Task_ShowScrollableMultichoice(u8);
static void FillFrontierExchangeCornerWindowAndItemIcon(u16, u16);
static void ShowBattleFrontierTutorWindow(u8, u16);
static void InitScrollableMultichoice(void);
static void ScrollableMultichoice_ProcessInput(u8);
static void ScrollableMultichoice_UpdateScrollArrows(u8);
static void ScrollableMultichoice_MoveCursor(s32, bool8, struct ListMenu *);
static void HideFrontierExchangeCornerItemIcon(u16, u16);
static void ShowBattleFrontierTutorMoveDescription(u8, u16);
static void CloseScrollableMultichoice(u8);
static void ScrollableMultichoice_RemoveScrollArrows(u8);
static void Task_ScrollableMultichoice_WaitReturnToList(u8);
static void Task_ScrollableMultichoice_ReturnToList(u8);
static void Task_DeoxysRockInteraction(u8);
static void ChangeDeoxysRockLevel(u8);
static void Task_LinkRetireStatusWithBattleTowerPartner(u8);
static void Task_LoopWingFlapSE(u8);
static void Task_CloseBattlePikeCurtain(u8);
static u8 DidPlayerGetFirstFans(void);
static void SetInitialFansOfPlayer(void);
static u16 PlayerGainRandomTrainerFan(void);
#if FREE_LINK_BATTLE_RECORDS == FALSE
static void BufferFanClubTrainerName_(struct LinkBattleRecords *, u8, u8);
#else
static void BufferFanClubTrainerName_(u8 whichLinkTrainer, u8 whichNPCTrainer);
#endif //FREE_LINK_BATTLE_RECORDS

static const u8 sText_BigGuy[] = _("Big guy");
static const u8 sText_BigGirl[] = _("Big girl");
static const u8 sText_Son[] = _("son");
static const u8 sText_Daughter[] = _("daughter");
static const u8 sText_99TimesPlus[] = _("99 times +");
static const u8 sText_1MinutePlus[] = _("1 minute +");
static const u8 sText_SpaceSeconds[] = _(" seconds");
static const u8 sText_SpaceTimes[] = _(" time(s)");

size_t CountDigits2(int value)
{
    u32 count = 0;

    while (value > 0)
    {
        value /= 10;
        count++;
    }
    return count;
}

void Special_ShowDiploma(void)
{
    SetMainCallback2(CB2_ShowDiploma);
    LockPlayerFieldControls();
}

void Special_ViewWallClock(void)
{
    gMain.savedCallback = CB2_ReturnToField;
    SetMainCallback2(CB2_ViewWallClock);
    LockPlayerFieldControls();
}

void ResetCyclingRoadChallengeData(void)
{
}

void Special_BeginCyclingRoadChallenge(void)
{
}

u16 GetPlayerAvatarBike(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE))
        return 1;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE))
        return 2;
    return 0;
}

static void DetermineCyclingRoadResults(u32 numFrames, u8 numBikeCollisions)
{
    u8 result;

    if (numBikeCollisions < 100)
    {
        ConvertIntToDecimalStringN(gStringVar1, numBikeCollisions, STR_CONV_MODE_LEFT_ALIGN, 2);
        StringAppend(gStringVar1, sText_SpaceTimes);
    }
    else
    {
        StringCopy(gStringVar1, sText_99TimesPlus);
    }

    if (numFrames < 3600)
    {
        ConvertIntToDecimalStringN(gStringVar2, numFrames / 60, STR_CONV_MODE_RIGHT_ALIGN, 2);
        gStringVar2[2] = CHAR_DEC_SEPARATOR;
        ConvertIntToDecimalStringN(&gStringVar2[3], ((numFrames % 60) * 100) / 60, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringAppend(gStringVar2, sText_SpaceSeconds);
    }
    else
    {
        StringCopy(gStringVar2, sText_1MinutePlus);
    }

    result = 0;
    if (numBikeCollisions == 0)
        result = 5;
    else if (numBikeCollisions < 4)
        result = 4;
    else if (numBikeCollisions < 10)
        result = 3;
    else if (numBikeCollisions < 20)
        result = 2;
    else if (numBikeCollisions < 100)
        result = 1;

    if (numFrames / 60 <= 10)
        result += 5;
    else if (numFrames / 60 <= 15)
        result += 4;
    else if (numFrames / 60 <= 20)
        result += 3;
    else if (numFrames / 60 <= 40)
        result += 2;
    else if (numFrames / 60 < 60)
        result += 1;

    gSpecialVar_Result = result;
}

void FinishCyclingRoadChallenge(void)
{
}

u16 GetRecordedCyclingRoadResults(void)
{
    u16 low = VarGet(VAR_CYCLING_ROAD_RECORD_TIME_L);
    u16 high = VarGet(VAR_CYCLING_ROAD_RECORD_TIME_H);
    u32 framesRecord = low + (high << 16);

    if (framesRecord == 0)
        return FALSE;

    DetermineCyclingRoadResults(framesRecord, VarGet(VAR_CYCLING_ROAD_RECORD_COLLISIONS));
    return TRUE;
}

void UpdateCyclingRoadState(void)
{
    // TODO: remove
}

void SetSSTidalFlag(void)
{
    FlagSet(FLAG_SYS_CRUISE_MODE);
    *GetVarPointer(VAR_CRUISE_STEP_COUNT) = 0;
}

void ResetSSTidalFlag(void)
{
    FlagClear(FLAG_SYS_CRUISE_MODE);
}

// Returns TRUE if the Cruise is over
bool32 CountSSTidalStep(u16 delta)
{
    if (!FlagGet(FLAG_SYS_CRUISE_MODE) || (*GetVarPointer(VAR_CRUISE_STEP_COUNT) += delta) < SS_TIDAL_MAX_STEPS)
        return FALSE;

    return TRUE;
}

u8 GetSSTidalLocation(s8 *mapGroup, s8 *mapNum, s16 *x, s16 *y)
{
    // TODO: remove
    return SS_TIDAL_LOCATION_CURRENTS;
}

bool32 ShouldDoWallyCall(void)
{
    if (FlagGet(FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL))
    {
        switch (gMapHeader.mapType)
        {
        case MAP_TYPE_TOWN:
        case MAP_TYPE_CITY:
        case MAP_TYPE_ROUTE:
        case MAP_TYPE_OCEAN_ROUTE:
            if (++(*GetVarPointer(VAR_WALLY_CALL_STEP_COUNTER)) < 250)
                return FALSE;
            break;
        default:
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

bool32 ShouldDoScottFortreeCall(void)
{
    if (FlagGet(FLAG_SCOTT_CALL_FORTREE_GYM))
    {
        switch (gMapHeader.mapType)
        {
        case MAP_TYPE_TOWN:
        case MAP_TYPE_CITY:
        case MAP_TYPE_ROUTE:
        case MAP_TYPE_OCEAN_ROUTE:
            if (++(*GetVarPointer(VAR_SCOTT_FORTREE_CALL_STEP_COUNTER)) < 10)
                return FALSE;
            break;
        default:
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

bool32 ShouldDoScottBattleFrontierCall(void)
{
    if (FlagGet(FLAG_SCOTT_CALL_BATTLE_FRONTIER))
    {
        switch (gMapHeader.mapType)
        {
        case MAP_TYPE_TOWN:
        case MAP_TYPE_CITY:
        case MAP_TYPE_ROUTE:
        case MAP_TYPE_OCEAN_ROUTE:
            if (++(*GetVarPointer(VAR_SCOTT_BF_CALL_STEP_COUNTER)) < 10)
                return FALSE;
            break;
        default:
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

bool32 ShouldDoRoxanneCall(void)
{
    if (FlagGet(FLAG_ENABLE_ROXANNE_FIRST_CALL))
    {
        switch (gMapHeader.mapType)
        {
        case MAP_TYPE_TOWN:
        case MAP_TYPE_CITY:
        case MAP_TYPE_ROUTE:
        case MAP_TYPE_OCEAN_ROUTE:
            if (++(*GetVarPointer(VAR_ROXANNE_CALL_STEP_COUNTER)) < 250)
                return FALSE;
            break;
        default:
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

bool32 ShouldDoRivalRayquazaCall(void)
{
    if (FlagGet(FLAG_DEFEATED_MAGMA_SPACE_CENTER))
    {
        switch (gMapHeader.mapType)
        {
        case MAP_TYPE_TOWN:
        case MAP_TYPE_CITY:
        case MAP_TYPE_ROUTE:
        case MAP_TYPE_OCEAN_ROUTE:
            if (++(*GetVarPointer(VAR_RIVAL_RAYQUAZA_CALL_STEP_COUNTER)) < 250)
                return FALSE;
            break;
        default:
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

u8 GetLinkPartnerNames(void)
{
    return 0;
}

void SpawnLinkPartnerObjectEvent(void)
{
    u8 j = 0;
    s16 x = 0;
    s16 y = 0;
    u8 movementTypes[] = {
        MOVEMENT_TYPE_FACE_UP,
        MOVEMENT_TYPE_FACE_LEFT,
        MOVEMENT_TYPE_FACE_DOWN,
        MOVEMENT_TYPE_FACE_RIGHT
    };
    s8 coordOffsets[][2] = {
        { 0,  1},
        { 1,  0},
        { 0, -1},
        {-1,  0}
    };
    u8 myLinkPlayerNumber;
    u8 playerFacingDirection;
    u8 linkSpriteId;
    u8 i;

    myLinkPlayerNumber = 0;
    playerFacingDirection = GetPlayerFacingDirection();
    switch (playerFacingDirection)
    {
    case DIR_WEST:
        j = 2;
        x = gSaveBlock1Ptr->pos.x - 1;
        y = gSaveBlock1Ptr->pos.y;
        break;
    case DIR_NORTH:
        j = 1;
        x = gSaveBlock1Ptr->pos.x;
        y = gSaveBlock1Ptr->pos.y - 1;
        break;
    case DIR_EAST:
        x = gSaveBlock1Ptr->pos.x + 1;
        y = gSaveBlock1Ptr->pos.y;
        break;
    case DIR_SOUTH:
        j = 3;
        x = gSaveBlock1Ptr->pos.x;
        y = gSaveBlock1Ptr->pos.y + 1;
    }
    for (i = 0; i < gSpecialVar_0x8004; i++)
    {
        if (myLinkPlayerNumber != i)
        {
            linkSpriteId = OBJ_EVENT_GFX_RIVAL_MAY_NORMAL;
            SpawnSpecialObjectEventParameterized(linkSpriteId, movementTypes[j], LOCALID_BERRY_BLENDER_PLAYER_END - i, coordOffsets[j][0] + x + MAP_OFFSET, coordOffsets[j][1] + y + MAP_OFFSET, 0);
            LoadLinkPartnerObjectEventSpritePalette(linkSpriteId, LOCALID_BERRY_BLENDER_PLAYER_END - i, i);
            j++;
        }
    }
}

static void LoadLinkPartnerObjectEventSpritePalette(u16 graphicsId, u8 localEventId, u8 paletteNum)
{
    u8 adjustedPaletteNum;
    // Note: This temp var is necessary; paletteNum += 6 doesn't match.
    adjustedPaletteNum = paletteNum + 6;
    if (graphicsId == OBJ_EVENT_GFX_LINK_RS_BRENDAN ||
        graphicsId == OBJ_EVENT_GFX_LINK_RS_MAY ||
        graphicsId == OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL ||
        graphicsId == OBJ_EVENT_GFX_RIVAL_MAY_NORMAL)
    {
        u8 obj = GetObjectEventIdByLocalIdAndMap(localEventId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
        if (obj != OBJECT_EVENTS_COUNT)
        {
            u8 spriteId = gObjectEvents[obj].spriteId;
            struct Sprite *sprite = &gSprites[spriteId];
            sprite->oam.paletteNum = adjustedPaletteNum;

            switch (graphicsId)
            {
            case OBJ_EVENT_GFX_LINK_RS_BRENDAN:
                LoadPalette(gObjectEventPal_RubySapphireBrendan, OBJ_PLTT_ID(adjustedPaletteNum), PLTT_SIZE_4BPP);
                break;
            case OBJ_EVENT_GFX_LINK_RS_MAY:
                LoadPalette(gObjectEventPal_RubySapphireMay, OBJ_PLTT_ID(adjustedPaletteNum), PLTT_SIZE_4BPP);
                break;
            case OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL:
                LoadPalette(gObjectEventPal_Brendan, OBJ_PLTT_ID(adjustedPaletteNum), PLTT_SIZE_4BPP);
                break;
            case OBJ_EVENT_GFX_RIVAL_MAY_NORMAL:
                LoadPalette(gObjectEventPal_May, OBJ_PLTT_ID(adjustedPaletteNum), PLTT_SIZE_4BPP);
                break;
            }
        }
    }
}

static const struct UCoords8 sMauvilleGymSwitchCoords[] =
{
    { 0 + MAP_OFFSET, 15 + MAP_OFFSET},
    { 4 + MAP_OFFSET, 12 + MAP_OFFSET},
    { 3 + MAP_OFFSET,  9 + MAP_OFFSET},
    { 8 + MAP_OFFSET,  9 + MAP_OFFSET}
};

// Presses the stepped-on switch and raises the rest
void MauvilleGymPressSwitch(void)
{
    u8 i;
    for (i = 0; i < ARRAY_COUNT(sMauvilleGymSwitchCoords); i++)
    {
        if (i == gSpecialVar_0x8004)
            MapGridSetMetatileIdAt(sMauvilleGymSwitchCoords[i].x, sMauvilleGymSwitchCoords[i].y, METATILE_MauvilleGym_PressedSwitch);
        else
            MapGridSetMetatileIdAt(sMauvilleGymSwitchCoords[i].x, sMauvilleGymSwitchCoords[i].y, METATILE_MauvilleGym_RaisedSwitch);
    }
}

// Sets the gym barriers back to the default state; their alt state is handled by MauvilleCity_Gym_EventScript_SetAltBarriers
void MauvilleGymSetDefaultBarriers(void)
{
    int x, y;
    // All switches/barriers are within these coord ranges
    for (y = 5 + MAP_OFFSET; y < 17 + MAP_OFFSET; y++)
    {
        for (x = 0 + MAP_OFFSET; x < 9 + MAP_OFFSET; x++)
        {
            switch (MapGridGetMetatileIdAt(x, y))
            {
            case METATILE_MauvilleGym_GreenBeamH1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH1_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamH2_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH2_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamH3_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH3_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamH4_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH4_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamH1_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH1_On);
                break;
            case METATILE_MauvilleGym_GreenBeamH2_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH2_On);
                break;
            case METATILE_MauvilleGym_GreenBeamH3_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH3_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_GreenBeamH4_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH4_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_RedBeamH1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH1_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH2_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH2_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH3_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH3_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH4_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH4_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH1_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH1_On);
                break;
            case METATILE_MauvilleGym_RedBeamH2_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH2_On);
                break;
            case METATILE_MauvilleGym_RedBeamH3_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH3_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_RedBeamH4_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH4_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_GreenBeamV1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_PoleBottom_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_GreenBeamV2_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_FloorTile);
                break;
            case METATILE_MauvilleGym_RedBeamV1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_PoleBottom_Off | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_RedBeamV2_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_FloorTile);
                break;
            case METATILE_MauvilleGym_PoleBottom_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamV1_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_FloorTile:
                if (MapGridGetMetatileIdAt(x, y - 1) == METATILE_MauvilleGym_GreenBeamV1_On)
                    MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamV2_On | MAPGRID_COLLISION_MASK);
                else
                    MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamV2_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_PoleBottom_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamV1_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_PoleTop_Off:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_PoleTop_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_PoleTop_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_PoleTop_Off);
                break;
            }
        }
    }
}

// Presses all switches and deactivates all beams.
void MauvilleGymDeactivatePuzzle(void)
{
    int i, x, y;
    const struct UCoords8 *switchCoords = sMauvilleGymSwitchCoords;
    for (i = ARRAY_COUNT(sMauvilleGymSwitchCoords) - 1; i >= 0; i--)
    {
        MapGridSetMetatileIdAt(switchCoords->x, switchCoords->y, METATILE_MauvilleGym_PressedSwitch);
        switchCoords++;
    }
    for (y = 5 + MAP_OFFSET; y < 17 + MAP_OFFSET; y++)
    {
        for (x = 0 + MAP_OFFSET; x < 9 + MAP_OFFSET; x++)
        {
            switch (MapGridGetMetatileIdAt(x, y))
            {
            case METATILE_MauvilleGym_GreenBeamH1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH1_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamH2_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH2_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamH3_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH3_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamH4_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_GreenBeamH4_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH1_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH2_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH2_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH3_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH3_Off);
                break;
            case METATILE_MauvilleGym_RedBeamH4_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_RedBeamH4_Off);
                break;
            case METATILE_MauvilleGym_GreenBeamV1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_PoleBottom_On | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_RedBeamV1_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_PoleBottom_Off | MAPGRID_COLLISION_MASK);
                break;
            case METATILE_MauvilleGym_GreenBeamV2_On:
            case METATILE_MauvilleGym_RedBeamV2_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_FloorTile);
                break;
            case METATILE_MauvilleGym_PoleTop_On:
                MapGridSetMetatileIdAt(x, y, METATILE_MauvilleGym_PoleTop_Off);
                break;
            }
        }
    }
}

static const bool8 sSlidingDoorNextFrameDelay[] = {0, 1, 1, 1, 1};

static const u16 sPetalburgGymSlidingDoorMetatiles[] = {
    METATILE_PetalburgGym_SlidingDoor_Frame0,
    METATILE_PetalburgGym_SlidingDoor_Frame1,
    METATILE_PetalburgGym_SlidingDoor_Frame2,
    METATILE_PetalburgGym_SlidingDoor_Frame3,
    METATILE_PetalburgGym_SlidingDoor_Frame4,
};

void PetalburgGymSlideOpenRoomDoors(void)
{
    sSlidingDoorNextFrameCounter = 0;
    sSlidingDoorFrame = 0;
    PlaySE(SE_UNLOCK);
    CreateTask(Task_PetalburgGymSlideOpenRoomDoors, 8);
}

static void Task_PetalburgGymSlideOpenRoomDoors(u8 taskId)
{
    if (sSlidingDoorNextFrameDelay[sSlidingDoorFrame] == sSlidingDoorNextFrameCounter)
    {
        PetalburgGymSetDoorMetatiles(gSpecialVar_0x8004, sPetalburgGymSlidingDoorMetatiles[sSlidingDoorFrame]);
        sSlidingDoorNextFrameCounter = 0;
        if ((++sSlidingDoorFrame) == ARRAY_COUNT(sPetalburgGymSlidingDoorMetatiles))
        {
            DestroyTask(taskId);
            ScriptContext_Enable();
        }
    }
    else
    {
        sSlidingDoorNextFrameCounter++;
    }
}

static void PetalburgGymSetDoorMetatiles(u8 roomNumber, u16 metatileId)
{
    u16 doorCoordsX[4];
    u16 doorCoordsY[4];
    u8 i;
    u8 nDoors = 0;
    switch (roomNumber)
    {
    case 1:
        nDoors = 2;
        doorCoordsX[0] = 1;
        doorCoordsX[1] = 7;
        doorCoordsY[0] = 104;
        doorCoordsY[1] = 104;
        break;
    case 2:
        nDoors = 2;
        doorCoordsX[0] = 1;
        doorCoordsX[1] = 7;
        doorCoordsY[0] = 78;
        doorCoordsY[1] = 78;
        break;
    case 3:
        nDoors = 2;
        doorCoordsX[0] = 1;
        doorCoordsX[1] = 7;
        doorCoordsY[0] = 91;
        doorCoordsY[1] = 91;
        break;
    case 4:
        nDoors = 1;
        doorCoordsX[0] = 7;
        doorCoordsY[0] = 39;
        break;
    case 5:
        nDoors = 2;
        doorCoordsX[0] = 1;
        doorCoordsX[1] = 7;
        doorCoordsY[0] = 52;
        doorCoordsY[1] = 52;
        break;
    case 6:
        nDoors = 1;
        doorCoordsX[0] = 1;
        doorCoordsY[0] = 65;
        break;
    case 7:
        nDoors = 1;
        doorCoordsX[0] = 7;
        doorCoordsY[0] = 13;
        break;
    case 8:
        nDoors = 1;
        doorCoordsX[0] = 1;
        doorCoordsY[0] = 26;
        break;
    }
    for (i = 0; i < nDoors; i++)
    {
        MapGridSetMetatileIdAt(doorCoordsX[i] + MAP_OFFSET, doorCoordsY[i] + MAP_OFFSET, metatileId | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(doorCoordsX[i] + MAP_OFFSET, doorCoordsY[i] + MAP_OFFSET + 1, (metatileId + METATILE_ROW_WIDTH) | MAPGRID_COLLISION_MASK);
    }
    DrawWholeMapView();
}

void PetalburgGymUnlockRoomDoors(void)
{
    PetalburgGymSetDoorMetatiles(gSpecialVar_0x8004, sPetalburgGymSlidingDoorMetatiles[4]);
}

void ShowFieldMessageStringVar4(void)
{
    ShowFieldMessage(gStringVar4);
}

void StorePlayerCoordsInVars(void)
{
    gSpecialVar_0x8004 = gSaveBlock1Ptr->pos.x;
    gSpecialVar_0x8005 = gSaveBlock1Ptr->pos.y;
}

u8 GetPlayerTrainerIdOnesDigit(void)
{
    return (u16)((gSaveBlock2Ptr->playerTrainerId[1] << 8) | gSaveBlock2Ptr->playerTrainerId[0]) % 10;
}

void GetPlayerBigGuyGirlString(void)
{
    if (gSaveBlock2Ptr->playerGender == MALE)
        StringCopy(gStringVar1, sText_BigGuy);
    else
        StringCopy(gStringVar1, sText_BigGirl);
}

void GetRivalSonDaughterString(void)
{
    if (gSaveBlock2Ptr->playerGender == MALE)
        StringCopy(gStringVar1, sText_Daughter);
    else
        StringCopy(gStringVar1, sText_Son);
}

u8 GetBattleOutcome(void)
{
    return gBattleOutcome;
}

void CableCarWarp(void)
{
    // TODO: remove
}

void SetHiddenItemFlag(void)
{
    FlagSet(gSpecialVar_0x8004);
}

u16 GetWeekCount(void)
{
    u16 weekCount = gLocalTime.days / 7;
    if (weekCount > 9999)
        weekCount = 9999;

    return weekCount;
}

u8 GetLeadMonFriendshipScore(void)
{
    return GetMonFriendshipScore(&gPlayerParty[GetLeadMonIndex()]);
}

static void CB2_FieldShowRegionMap(void)
{
}

void FieldShowRegionMap(void)
{
    SetMainCallback2(CB2_FieldShowRegionMap);
}

static bool32 IsBuildingPCTile(u32 tileId)
{
    return gMapHeader.mapLayout->primaryTileset == &gTileset_Building && (tileId == METATILE_Building_PC_On || tileId == METATILE_Building_PC_Off);
}

static bool32 IsPlayerHousePCTile(u32 tileId)
{
    return gMapHeader.mapLayout->secondaryTileset == &gTileset_BrendansMaysHouse
        && (tileId == METATILE_BrendansMaysHouse_BrendanPC_On
            || tileId == METATILE_BrendansMaysHouse_BrendanPC_Off
            || tileId == METATILE_BrendansMaysHouse_MayPC_On
            || tileId == METATILE_BrendansMaysHouse_MayPC_Off);
}

static bool8 IsPlayerInFrontOfPC(void)
{
    s16 x, y;
    u32 tileInFront;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    tileInFront = MapGridGetMetatileIdAt(x, y);

    return IsBuildingPCTile(tileInFront) || IsPlayerHousePCTile(tileInFront);
}

// Task data for Task_PCTurnOnEffect and Task_LotteryCornerComputerEffect
#define tPaused       data[0] // Never set
#define tTaskId       data[1]
#define tFlickerCount data[2]
#define tTimer        data[3]
#define tIsScreenOn   data[4]

// For this special, gSpecialVar_0x8004 is expected to be some PC_LOCATION_* value.
void DoPCTurnOnEffect(void)
{
    if (FuncIsActiveTask(Task_PCTurnOnEffect) != TRUE && IsPlayerInFrontOfPC() == TRUE)
    {
        u8 taskId = CreateTask(Task_PCTurnOnEffect, 8);
        gTasks[taskId].tPaused = FALSE;
        gTasks[taskId].tTaskId = taskId;
        gTasks[taskId].tFlickerCount = 0;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tIsScreenOn = FALSE;
    }
}

static void Task_PCTurnOnEffect(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (!task->tPaused)
        PCTurnOnEffect(task);
}

static void PCTurnOnEffect(struct Task *task)
{
    u8 playerDirection;
    s8 dx = 0;
    s8 dy = 0;
    if (task->tTimer == 6)
    {
        task->tTimer = 0;

        // Get where the PC should be, depending on where the player is looking.
        playerDirection = GetPlayerFacingDirection();
        switch (playerDirection)
        {
        case DIR_NORTH:
            dx = 0;
            dy = -1;
            break;
        case DIR_WEST:
            dx = -1;
            dy = -1;
            break;
        case DIR_EAST:
            dx = 1;
            dy = -1;
            break;
        }

        // Update map
        PCTurnOnEffect_SetMetatile(task->tIsScreenOn, dx, dy);
        DrawWholeMapView();

        // Screen flickers 5 times. Odd number and starting with the
        // screen off means the animation ends with the screen on.
        task->tIsScreenOn ^= 1;
        if (++task->tFlickerCount == 5)
            DestroyTask(task->tTaskId);
    }
    task->tTimer++;
}

static void PCTurnOnEffect_SetMetatile(s16 isScreenOn, s8 dx, s8 dy)
{
    u16 metatileId = 0;
    if (isScreenOn)
    {
        // Screen is on, set it off
        if (gSpecialVar_0x8004 == PC_LOCATION_OTHER)
            metatileId = METATILE_Building_PC_Off;
        else if (gSpecialVar_0x8004 == PC_LOCATION_BRENDANS_HOUSE)
            metatileId = METATILE_BrendansMaysHouse_BrendanPC_Off;
        else if (gSpecialVar_0x8004 == PC_LOCATION_MAYS_HOUSE)
            metatileId = METATILE_BrendansMaysHouse_MayPC_Off;
    }
    else
    {
        // Screen is off, set it on
        if (gSpecialVar_0x8004 == PC_LOCATION_OTHER)
            metatileId = METATILE_Building_PC_On;
        else if (gSpecialVar_0x8004 == PC_LOCATION_BRENDANS_HOUSE)
            metatileId = METATILE_BrendansMaysHouse_BrendanPC_On;
        else if (gSpecialVar_0x8004 == PC_LOCATION_MAYS_HOUSE)
            metatileId = METATILE_BrendansMaysHouse_MayPC_On;
    }
    MapGridSetMetatileIdAt(gSaveBlock1Ptr->pos.x + dx + MAP_OFFSET, gSaveBlock1Ptr->pos.y + dy + MAP_OFFSET, metatileId | MAPGRID_COLLISION_MASK);
}

// For this special, gSpecialVar_0x8004 is expected to be some PC_LOCATION_* value.
void DoPCTurnOffEffect(void)
{
    PCTurnOffEffect();
}

static void PCTurnOffEffect(void)
{
    s8 dx = 0;
    s8 dy = 0;
    u16 metatileId = 0;

    // Get where the PC should be, depending on where the player is looking.
    u8 playerDirection = GetPlayerFacingDirection();

    if (IsPlayerInFrontOfPC() == FALSE)
        return;
    switch (playerDirection)
    {
    case DIR_NORTH:
        dx = 0;
        dy = -1;
        break;
    case DIR_WEST:
        dx = -1;
        dy = -1;
        break;
    case DIR_EAST:
        dx = 1;
        dy = -1;
        break;
    }

    if (gSpecialVar_0x8004 == PC_LOCATION_OTHER)
        metatileId = METATILE_Building_PC_Off;
    else if (gSpecialVar_0x8004 == PC_LOCATION_BRENDANS_HOUSE)
        metatileId = METATILE_BrendansMaysHouse_BrendanPC_Off;
    else if (gSpecialVar_0x8004 == PC_LOCATION_MAYS_HOUSE)
        metatileId = METATILE_BrendansMaysHouse_MayPC_Off;

    MapGridSetMetatileIdAt(gSaveBlock1Ptr->pos.x + dx + MAP_OFFSET, gSaveBlock1Ptr->pos.y + dy + MAP_OFFSET, metatileId | MAPGRID_COLLISION_MASK);
    DrawWholeMapView();
}

void DoLotteryCornerComputerEffect(void)
{
    if (FuncIsActiveTask(Task_LotteryCornerComputerEffect) != TRUE)
    {
        u8 taskId = CreateTask(Task_LotteryCornerComputerEffect, 8);
        gTasks[taskId].tPaused = FALSE;
        gTasks[taskId].tTaskId = taskId;
        gTasks[taskId].tFlickerCount = 0;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tIsScreenOn = FALSE;
    }
}

static void Task_LotteryCornerComputerEffect(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (!task->tPaused)
        LotteryCornerComputerEffect(task);
}

static void LotteryCornerComputerEffect(struct Task *task)
{
    if (task->tTimer == 6)
    {
        task->tTimer = 0;
        if (task->tIsScreenOn)
        {
            // Screen is on, set it off
            MapGridSetMetatileIdAt(11 + MAP_OFFSET, 1 + MAP_OFFSET, METATILE_Shop_Laptop1_Normal | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(11 + MAP_OFFSET, 2 + MAP_OFFSET, METATILE_Shop_Laptop2_Normal | MAPGRID_COLLISION_MASK);
        }
        else
        {
            // Screen is off, set it on
            MapGridSetMetatileIdAt(11 + MAP_OFFSET, 1 + MAP_OFFSET, METATILE_Shop_Laptop1_Flash | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(11 + MAP_OFFSET, 2 + MAP_OFFSET, METATILE_Shop_Laptop2_Flash | MAPGRID_COLLISION_MASK);
        }
        DrawWholeMapView();

        // Screen flickers 5 times. Odd number and starting with the
        // screen off means the animation ends with the screen on.
        task->tIsScreenOn ^= 1;
        if (++task->tFlickerCount == 5)
            DestroyTask(task->tTaskId);
    }
    task->tTimer++;
}

void EndLotteryCornerComputerEffect(void)
{
    MapGridSetMetatileIdAt(11 + MAP_OFFSET, 1 + MAP_OFFSET, METATILE_Shop_Laptop1_Normal | MAPGRID_COLLISION_MASK);
    MapGridSetMetatileIdAt(11 + MAP_OFFSET, 2 + MAP_OFFSET, METATILE_Shop_Laptop2_Normal | MAPGRID_COLLISION_MASK);
    DrawWholeMapView();
}

#undef tPaused
#undef tTaskId
#undef tFlickerCount
#undef tTimer
#undef tIsScreenOn

void SetTrickHouseNuggetFlag(void)
{
    u16 *specVar = &gSpecialVar_0x8004;
    u16 flag = FLAG_HIDDEN_ITEM_TRICK_HOUSE_NUGGET;
    *specVar = flag;
    FlagSet(flag);
}

void ResetTrickHouseNuggetFlag(void)
{
    u16 *specVar = &gSpecialVar_0x8004;
    u16 flag = FLAG_HIDDEN_ITEM_TRICK_HOUSE_NUGGET;
    *specVar = flag;
    FlagClear(flag);
}

bool8 CheckLeadMonCool(void)
{
    if (GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_COOL) < 200)
        return FALSE;

    return TRUE;
}

bool8 CheckLeadMonBeauty(void)
{
    if (GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_BEAUTY) < 200)
        return FALSE;

    return TRUE;
}

bool8 CheckLeadMonCute(void)
{
    if (GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_CUTE) < 200)
        return FALSE;

    return TRUE;
}

bool8 CheckLeadMonSmart(void)
{
    if (GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_SMART) < 200)
        return FALSE;

    return TRUE;
}

bool8 CheckLeadMonTough(void)
{
    if (GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_TOUGH) < 200)
        return FALSE;

    return TRUE;
}

void IsGrassTypeInParty(void)
{
    u8 i;
    u16 species;
    struct Pokemon *pokemon;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        pokemon = &gPlayerParty[i];
        if (GetMonData(pokemon, MON_DATA_SANITY_HAS_SPECIES) && !GetMonData(pokemon, MON_DATA_IS_EGG))
        {
            species = GetMonData(pokemon, MON_DATA_SPECIES);
            if (gSpeciesInfo[species].types[0] == TYPE_GRASS || gSpeciesInfo[species].types[1] == TYPE_GRASS)
            {
                gSpecialVar_Result = TRUE;
                return;
            }
        }
    }
    gSpecialVar_Result = FALSE;
}

void SpawnCameraObject(void)
{
    u8 obj = SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_BOY_1,
                                                  MOVEMENT_TYPE_FACE_DOWN,
                                                  LOCALID_CAMERA,
                                                  gSaveBlock1Ptr->pos.x + MAP_OFFSET,
                                                  gSaveBlock1Ptr->pos.y + MAP_OFFSET,
                                                  3); // elevation
    gObjectEvents[obj].invisible = TRUE;
    CameraObjectSetFollowedSpriteId(gObjectEvents[obj].spriteId);
}

void RemoveCameraObject(void)
{
    CameraObjectSetFollowedSpriteId(GetPlayerAvatarSpriteId());
    RemoveObjectEventByLocalIdAndMap(LOCALID_CAMERA, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
}

u16 GetSlotMachineId(void)
{
    return 0;
}

bool8 FoundAbandonedShipRoom1Key(void)
{
    u16 *specVar = &gSpecialVar_0x8004;
    u16 flag = FLAG_HIDDEN_ITEM_ABANDONED_SHIP_RM_1_KEY;
    *specVar = flag;
    if (!FlagGet(flag))
        return FALSE;

    return TRUE;
}

bool8 FoundAbandonedShipRoom2Key(void)
{
    u16 *specVar = &gSpecialVar_0x8004;
    u16 flag = FLAG_HIDDEN_ITEM_ABANDONED_SHIP_RM_2_KEY;
    *specVar = flag;
    if (!FlagGet(flag))
        return FALSE;

    return TRUE;
}

bool8 FoundAbandonedShipRoom4Key(void)
{
    u16 *specVar = &gSpecialVar_0x8004;
    u16 flag = FLAG_HIDDEN_ITEM_ABANDONED_SHIP_RM_4_KEY;
    *specVar = flag;
    if (!FlagGet(flag))
        return FALSE;

    return TRUE;
}

bool8 FoundAbandonedShipRoom6Key(void)
{
    u16 *specVar = &gSpecialVar_0x8004;
    u16 flag = FLAG_HIDDEN_ITEM_ABANDONED_SHIP_RM_6_KEY;
    *specVar = flag;
    if (!FlagGet(flag))
        return FALSE;

    return TRUE;
}

bool8 LeadMonHasEffortRibbon(void)
{
    return GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_EFFORT_RIBBON, NULL);
}

void GiveLeadMonEffortRibbon(void)
{
    bool8 ribbonSet;
    struct Pokemon *leadMon;
    FlagSet(FLAG_SYS_RIBBON_GET);
    ribbonSet = TRUE;
    leadMon = &gPlayerParty[GetLeadMonIndex()];
    SetMonData(leadMon, MON_DATA_EFFORT_RIBBON, &ribbonSet);
}

bool8 Special_AreLeadMonEVsMaxedOut(void)
{
    if (GetMonEVCount(&gPlayerParty[GetLeadMonIndex()]) >= MAX_TOTAL_EVS)
        return TRUE;

    return FALSE;
}

u8 TryUpdateRusturfTunnelState(void)
{
    // TODO: remove
    return FALSE;
}

void SetShoalItemFlag(u16 unused)
{
    FlagSet(FLAG_SYS_SHOAL_ITEM);
}

void LoadWallyZigzagoon(void)
{
    u16 monData;
    CreateMon(&gPlayerParty[0], SPECIES_ZIGZAGOON, 7, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
    monData = TRUE;
    SetMonData(&gPlayerParty[0], MON_DATA_ABILITY_NUM, &monData);
    monData = MOVE_TACKLE;
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &monData);
    monData = MOVE_NONE;
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE2, &monData);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE3, &monData);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE4, &monData);
}

bool8 IsStarterInParty(void)
{
    u8 i;
    u16 starter = GetStarterPokemon(VarGet(VAR_STARTER_MON));
    u8 partyCount = CalculatePlayerPartyCount();
    for (i = 0; i < partyCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG, NULL) == starter)
            return TRUE;
    }
    return FALSE;
}

bool8 IsPokerusInParty(void)
{
    if (!CheckPartyPokerus(gPlayerParty, (1 << PARTY_SIZE) - 1))
        return FALSE;

    return TRUE;
}

// Task data for Task_ShakeCamera
#define tHorizontalPan  data[0]
#define tDelayCounter   data[1]
#define tNumShakes      data[2]
#define tDelay          data[3]
#define tVerticalPan    data[4]

void ShakeCamera(void)
{
    u8 taskId = CreateTask(Task_ShakeCamera, 9);
    gTasks[taskId].tHorizontalPan = gSpecialVar_0x8005;
    gTasks[taskId].tDelayCounter = 0;
    gTasks[taskId].tNumShakes = gSpecialVar_0x8006;
    gTasks[taskId].tDelay = gSpecialVar_0x8007;
    gTasks[taskId].tVerticalPan = gSpecialVar_0x8004;
    SetCameraPanningCallback(NULL);
    PlaySE(SE_M_STRENGTH);
}

static void Task_ShakeCamera(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    tDelayCounter++;
    if (tDelayCounter % tDelay == 0)
    {
        tDelayCounter = 0;
        tNumShakes--;
        tHorizontalPan = -tHorizontalPan;
        tVerticalPan = -tVerticalPan;
        SetCameraPanning(tHorizontalPan, tVerticalPan);
        if (tNumShakes == 0)
        {
            StopCameraShake(taskId);
            InstallCameraPanAheadCallback();
        }
    }
}

static void StopCameraShake(u8 taskId)
{
    DestroyTask(taskId);
    ScriptContext_Enable();
}

#undef tHorizontalPan
#undef tDelayCounter
#undef tNumShakes
#undef tDelay
#undef tVerticalPan

bool8 FoundBlackGlasses(void)
{
    return FlagGet(FLAG_HIDDEN_ITEM_ROUTE_116_BLACK_GLASSES);
}

void SetRoute119Weather(void)
{
    if (IsMapTypeOutdoors(GetLastUsedWarpMapType()) != TRUE)
        SetSavedWeather(WEATHER_ROUTE119_CYCLE);
}

void SetRoute123Weather(void)
{
    if (IsMapTypeOutdoors(GetLastUsedWarpMapType()) != TRUE)
        SetSavedWeather(WEATHER_ROUTE123_CYCLE);
}

u8 GetLeadMonIndex(void)
{
    u8 i;
    u8 partyCount = CalculatePlayerPartyCount();
    for (i = 0; i < partyCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG, NULL) != SPECIES_EGG
         && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG, NULL) != SPECIES_NONE)
            return i;
    }
    return 0;
}

u16 ScriptGetPartyMonSpecies(void)
{
    return GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES_OR_EGG, NULL);
}

// Removed for Emerald
void TryInitBattleTowerAwardManObjectEvent(void)
{
    //TryInitLocalObjectEvent(6);
}

u16 GetDaysUntilPacifidlogTMAvailable(void)
{
    u16 tmReceivedDay = VarGet(VAR_PACIFIDLOG_TM_RECEIVED_DAY);
    if (gLocalTime.days - tmReceivedDay >= 7)
        return 0;
    else if (gLocalTime.days < 0)
        return 8;

    return 7 - (gLocalTime.days - tmReceivedDay);
}

u16 SetPacifidlogTMReceivedDay(void)
{
    VarSet(VAR_PACIFIDLOG_TM_RECEIVED_DAY, gLocalTime.days);
    return gLocalTime.days;
}

bool8 MonOTNameNotPlayer(void)
{
    if (GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_LANGUAGE) != GAME_LANGUAGE)
        return TRUE;

    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_OT_NAME, gStringVar1);

    if (!StringCompare(gSaveBlock2Ptr->playerName, gStringVar1))
        return FALSE;

    return TRUE;
}

void BufferLottoTicketNumber(void)
{
    if (gSpecialVar_Result >= 1000)
    {
        gStringVar1[0] = CHAR_0;
        ConvertIntToDecimalStringN(gStringVar1 + 1, gSpecialVar_Result, STR_CONV_MODE_LEFT_ALIGN, CountDigits2(gSpecialVar_Result));
    }
    else if (gSpecialVar_Result >= 100)
    {
        gStringVar1[0] = CHAR_0;
        gStringVar1[1] = CHAR_0;
        ConvertIntToDecimalStringN(gStringVar1 + 2, gSpecialVar_Result, STR_CONV_MODE_LEFT_ALIGN, CountDigits2(gSpecialVar_Result));
    }
    else if (gSpecialVar_Result >= 10)
    {
        gStringVar1[0] = CHAR_0;
        gStringVar1[1] = CHAR_0;
        gStringVar1[2] = CHAR_0;
        ConvertIntToDecimalStringN(gStringVar1 + 3, gSpecialVar_Result, STR_CONV_MODE_LEFT_ALIGN, CountDigits2(gSpecialVar_Result));
    }
    else
    {
        gStringVar1[0] = CHAR_0;
        gStringVar1[1] = CHAR_0;
        gStringVar1[2] = CHAR_0;
        gStringVar1[3] = CHAR_0;
        ConvertIntToDecimalStringN(gStringVar1 + 4, gSpecialVar_Result, STR_CONV_MODE_LEFT_ALIGN, CountDigits2(gSpecialVar_Result));
    }
}

u16 GetMysteryGiftCardStat(void)
{
    return 0;
}

bool8 BufferTMHMMoveName(void)
{
    if (gSpecialVar_0x8004 >= ITEM_TM01 && gSpecialVar_0x8004 <= ITEM_HM08)
    {
        StringCopy(gStringVar2, GetMoveName(ItemIdToBattleMoveId(gSpecialVar_0x8004)));
        return TRUE;
    }

    return FALSE;
}

bool8 IsBadEggInParty(void)
{
    u8 partyCount = CalculatePlayerPartyCount();
    u8 i;

    for (i = 0; i < partyCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SANITY_IS_BAD_EGG) == TRUE)
            return TRUE;
    }

    return FALSE;
}

bool8 InMultiPartnerRoom(void)
{
    // TODO: remove
    return FALSE;
}

void OffsetCameraForBattle(void)
{
    SetCameraPanningCallback(NULL);
    SetCameraPanning(8, 0);
}

static const struct WindowTemplate sWindowTemplate_ElevatorFloor =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 1,
    .width = 8,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 8,
};

static const u8 *const sDeptStoreFloorNames[] =
{
    [DEPT_STORE_FLOORNUM_B4F] = gText_B4F,
    [DEPT_STORE_FLOORNUM_B3F] = gText_B3F,
    [DEPT_STORE_FLOORNUM_B2F] = gText_B2F,
    [DEPT_STORE_FLOORNUM_B1F] = gText_B1F,
    [DEPT_STORE_FLOORNUM_1F] = gText_1F,
    [DEPT_STORE_FLOORNUM_2F] = gText_2F,
    [DEPT_STORE_FLOORNUM_3F] = gText_3F,
    [DEPT_STORE_FLOORNUM_4F] = gText_4F,
    [DEPT_STORE_FLOORNUM_5F] = gText_5F,
    [DEPT_STORE_FLOORNUM_6F] = gText_6F,
    [DEPT_STORE_FLOORNUM_7F] = gText_7F,
    [DEPT_STORE_FLOORNUM_8F] = gText_8F,
    [DEPT_STORE_FLOORNUM_9F] = gText_9F,
    [DEPT_STORE_FLOORNUM_10F] = gText_10F,
    [DEPT_STORE_FLOORNUM_11F] = gText_11F,
    [DEPT_STORE_FLOORNUM_ROOFTOP] = gText_Rooftop
};

static const u16 sElevatorWindowTiles_Ascending[ELEVATOR_WINDOW_HEIGHT][ELEVATOR_LIGHT_STAGES] =
{
    {
        METATILE_BattleFrontier_Elevator_Top0,
        METATILE_BattleFrontier_Elevator_Top1,
        METATILE_BattleFrontier_Elevator_Top2
    },
    {
        METATILE_BattleFrontier_Elevator_Mid0,
        METATILE_BattleFrontier_Elevator_Mid1,
        METATILE_BattleFrontier_Elevator_Mid2
    },
    {
        METATILE_BattleFrontier_Elevator_Bottom0,
        METATILE_BattleFrontier_Elevator_Bottom1,
        METATILE_BattleFrontier_Elevator_Bottom2
    },
};

static const u16 sElevatorWindowTiles_Descending[ELEVATOR_WINDOW_HEIGHT][ELEVATOR_LIGHT_STAGES] =
{
    {
        METATILE_BattleFrontier_Elevator_Top0,
        METATILE_BattleFrontier_Elevator_Top2,
        METATILE_BattleFrontier_Elevator_Top1
    },
    {
        METATILE_BattleFrontier_Elevator_Mid0,
        METATILE_BattleFrontier_Elevator_Mid2,
        METATILE_BattleFrontier_Elevator_Mid1
    },
    {
        METATILE_BattleFrontier_Elevator_Bottom0,
        METATILE_BattleFrontier_Elevator_Bottom2,
        METATILE_BattleFrontier_Elevator_Bottom1
    },
};

void SetDeptStoreFloor(void)
{
    // TODO: remove
}

u16 GetDeptStoreDefaultFloorChoice(void)
{
    // TODO: remove
    return sLilycoveDeptStore_DefaultFloorChoice;
}

// Task data for Task_MoveElevator
#define tTimer       data[1]
#define tMoveCounter data[2]
#define tVerticalPan data[4]
#define tTotalMoves  data[5]
#define tDescending  data[6]

// The maximum considered difference between floors.
// Elevator trips with a larger difference are treated the same
// (i.e. traveling 9 floors and 200 floors would take the same amount of time).
#define MAX_ELEVATOR_TRIP 9

// gSpecialVar_0x8005 here is expected to be the current floor number, and
// gSpecialVar_0x8006 is expected to be the destination floor number.
void MoveElevator(void)
{
    static const u8 sElevatorTripLength[MAX_ELEVATOR_TRIP] = { 8, 16, 24, 32, 38, 46, 52, 56, 57 };

    s16 *data = gTasks[CreateTask(Task_MoveElevator, 9)].data;
    u16 floorDelta;

    tTimer = 0;
    tMoveCounter = 0;
    tVerticalPan = 1;

    if (gSpecialVar_0x8005 > gSpecialVar_0x8006)
    {
        floorDelta = gSpecialVar_0x8005 - gSpecialVar_0x8006;
        tDescending = TRUE;
    }
    else
    {
        floorDelta = gSpecialVar_0x8006 - gSpecialVar_0x8005;
        tDescending = FALSE;
    }

    if (floorDelta > MAX_ELEVATOR_TRIP - 1)
        floorDelta = MAX_ELEVATOR_TRIP - 1;

    tTotalMoves = sElevatorTripLength[floorDelta];

    SetCameraPanningCallback(NULL);
    MoveElevatorWindowLights(floorDelta, tDescending);
    PlaySE(SE_ELEVATOR);
}

static void Task_MoveElevator(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    tTimer++;
    if (tTimer % 3 == 0)
    {
        tTimer = 0;
        tMoveCounter++;
        tVerticalPan = -tVerticalPan;
        SetCameraPanning(0, tVerticalPan);

        if (tMoveCounter == tTotalMoves)
        {
            // Arrived at floor
            PlaySE(SE_DING_DONG);
            DestroyTask(taskId);
            ScriptContext_Enable();
            InstallCameraPanAheadCallback();
        }
    }
}

#undef tTimer
#undef tMoveCounter
#undef tVerticalPan
#undef tTotalMoves
#undef tDescending

void ShowDeptStoreElevatorFloorSelect(void)
{
    int xPos;

    sTutorMoveAndElevatorWindowId = AddWindow(&sWindowTemplate_ElevatorFloor);
    SetStandardWindowBorderStyle(sTutorMoveAndElevatorWindowId, FALSE);

    xPos = GetStringCenterAlignXOffset(FONT_NORMAL, gText_ElevatorNowOn, 64);
    AddTextPrinterParameterized(sTutorMoveAndElevatorWindowId, FONT_NORMAL, gText_ElevatorNowOn, xPos, 1, TEXT_SKIP_DRAW, NULL);

    xPos = GetStringCenterAlignXOffset(FONT_NORMAL, sDeptStoreFloorNames[gSpecialVar_0x8005], 64);
    AddTextPrinterParameterized(sTutorMoveAndElevatorWindowId, FONT_NORMAL, sDeptStoreFloorNames[gSpecialVar_0x8005], xPos, 17, TEXT_SKIP_DRAW, NULL);

    PutWindowTilemap(sTutorMoveAndElevatorWindowId);
    CopyWindowToVram(sTutorMoveAndElevatorWindowId, COPYWIN_FULL);
}

void CloseDeptStoreElevatorWindow(void)
{
    ClearStdWindowAndFrameToTransparent(sTutorMoveAndElevatorWindowId, TRUE);
    RemoveWindow(sTutorMoveAndElevatorWindowId);
}

// Task data for Task_MoveElevatorWindowLights
#define tMoveCounter data[0]
#define tTimer       data[1]
#define tDescending  data[2]
#define tTotalMoves  data[3]

static void MoveElevatorWindowLights(u16 floorDelta, bool8 descending)
{
    static const u8 sElevatorLightCycles[MAX_ELEVATOR_TRIP] = { 3, 6, 9, 12, 15, 18, 21, 24, 27 };

    if (FuncIsActiveTask(Task_MoveElevatorWindowLights) != TRUE)
    {
        u8 taskId = CreateTask(Task_MoveElevatorWindowLights, 8);
        gTasks[taskId].tMoveCounter = 0;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tDescending = descending;
        gTasks[taskId].tTotalMoves = sElevatorLightCycles[floorDelta];
    }
}

static void Task_MoveElevatorWindowLights(u8 taskId)
{
    u8 x, y;
    s16 *data = gTasks[taskId].data;

    if (tTimer == 6)
    {
        tMoveCounter++;

        if (!tDescending)
        {
            // Ascending
            for (y = 0; y < ELEVATOR_WINDOW_HEIGHT; y++)
            {
                for (x = 0; x < ELEVATOR_WINDOW_WIDTH; x++)
                    MapGridSetMetatileIdAt(x + MAP_OFFSET + 1, y + MAP_OFFSET, sElevatorWindowTiles_Ascending[y][tMoveCounter % ELEVATOR_LIGHT_STAGES] | MAPGRID_COLLISION_MASK);
            }
        }
        else
        {
            // Descending
            for (y = 0; y < ELEVATOR_WINDOW_HEIGHT; y++)
            {
                for (x = 0; x < ELEVATOR_WINDOW_WIDTH; x++)
                    MapGridSetMetatileIdAt(x + MAP_OFFSET + 1, y + MAP_OFFSET, sElevatorWindowTiles_Descending[y][tMoveCounter % ELEVATOR_LIGHT_STAGES] | MAPGRID_COLLISION_MASK);
            }
        }
        DrawWholeMapView();
        tTimer = 0;
        if (tMoveCounter == tTotalMoves)
            DestroyTask(taskId);
    }
    tTimer++;
}

#undef tMoveCounter
#undef tTimer
#undef tDescending
#undef tTotalMoves

void BufferVarsForIVRater(void)
{
    u8 i;
    u32 ivStorage[NUM_STATS];

    ivStorage[STAT_HP] = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HP_IV);
    ivStorage[STAT_ATK] = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_ATK_IV);
    ivStorage[STAT_DEF] = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_DEF_IV);
    ivStorage[STAT_SPEED] = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPEED_IV);
    ivStorage[STAT_SPATK] = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPATK_IV);
    ivStorage[STAT_SPDEF] = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPDEF_IV);

    gSpecialVar_0x8005 = 0;

    for (i = 0; i < NUM_STATS; i++)
        gSpecialVar_0x8005 += ivStorage[i];

    gSpecialVar_0x8006 = 0;
    gSpecialVar_0x8007 = ivStorage[STAT_HP];

    for (i = 1; i < NUM_STATS; i++)
    {
        if (ivStorage[gSpecialVar_0x8006] < ivStorage[i])
        {
            gSpecialVar_0x8006 = i;
            gSpecialVar_0x8007 = ivStorage[i];
        }
        else if (ivStorage[gSpecialVar_0x8006] == ivStorage[i])
        {
            u16 randomNumber = Random();
            if (randomNumber & 1)
            {
                gSpecialVar_0x8006 = i;
                gSpecialVar_0x8007 = ivStorage[i];
            }
        }
    }
}

bool8 UsedPokemonCenterWarp(void)
{
    static const u16 sPokemonCenters[] =
    {
        MAP_UNDEFINED
    };

    int i;
    u16 map = (gLastUsedWarp.mapGroup << 8) + gLastUsedWarp.mapNum;

    for (i = 0; sPokemonCenters[i] != MAP_UNDEFINED; i++)
    {
        if (sPokemonCenters[i] == map)
            return TRUE;
    }

    return FALSE;
}

bool32 PlayerNotAtTrainerHillEntrance(void)
{
    // TODO: remove
    return TRUE;
}

void UpdateFrontierManiac(u16 daysSince)
{
}

void ShowFrontierManiacMessage(void)
{
    // TODO: remove
}

// gSpecialVar_0x8005 and 0x8006 here are used by MoveElevator
void BufferBattleTowerElevatorFloors(void)
{
}

// Scrollable Multichoice task data defines
#define tMaxItemsOnScreen    data[0]
#define tNumItems            data[1]
#define tLeft                data[2]
#define tTop                 data[3]
#define tWidth               data[4]
#define tHeight              data[5]
#define tKeepOpenAfterSelect data[6]
#define tScrollOffset        data[7]
#define tSelectedRow         data[8]
#define tScrollMultiId       data[11]
#define tScrollArrowId       data[12]
#define tWindowId            data[13]
#define tListTaskId          data[14]
#define tTaskId              data[15]
// data[9] and [10] unused

void ShowScrollableMultichoice(void)
{
    u8 taskId = CreateTask(Task_ShowScrollableMultichoice, 8);
    struct Task *task = &gTasks[taskId];
    task->tScrollMultiId = gSpecialVar_0x8004;

    switch (gSpecialVar_0x8004)
    {
    case SCROLL_MULTI_NONE:
        task->tMaxItemsOnScreen = 1;
        task->tNumItems = 1;
        task->tLeft = 1;
        task->tTop = 1;
        task->tWidth = 1;
        task->tHeight = 1;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_GLASS_WORKSHOP_VENDOR:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN - 1;
        task->tNumItems = 8;
        task->tLeft = 1;
        task->tTop = 1;
        task->tWidth = 9;
        task->tHeight = 10;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_POKEMON_FAN_CLUB_RATER:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 12;
        task->tLeft = 1;
        task->tTop = 1;
        task->tWidth = 7;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_1:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 11;
        task->tLeft = 14;
        task->tTop = 1;
        task->tWidth = 15;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_2:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 6;
        task->tLeft = 14;
        task->tTop = 1;
        task->tWidth = 15;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_BF_EXCHANGE_CORNER_VITAMIN_VENDOR:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 7;
        task->tLeft = 14;
        task->tTop = 1;
        task->tWidth = 15;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_BF_EXCHANGE_CORNER_HOLD_ITEM_VENDOR:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 10;
        task->tLeft = 14;
        task->tTop = 1;
        task->tWidth = 15;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_BF_RECEPTIONIST:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 10;
        task->tLeft = 17;
        task->tTop = 1;
        task->tWidth = 11;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_BF_MOVE_TUTOR_1:
    case SCROLL_MULTI_BF_MOVE_TUTOR_2:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 11;
        task->tLeft = 15;
        task->tTop = 1;
        task->tWidth = 14;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_SS_TIDAL_DESTINATION:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 7;
        task->tLeft = 19;
        task->tTop = 1;
        task->tWidth = 10;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    case SCROLL_MULTI_BATTLE_TENT_RULES:
        task->tMaxItemsOnScreen = MAX_SCROLL_MULTI_ON_SCREEN;
        task->tNumItems = 7;
        task->tLeft = 17;
        task->tTop = 1;
        task->tWidth = 12;
        task->tHeight = 12;
        task->tKeepOpenAfterSelect = FALSE;
        task->tTaskId = taskId;
        break;
    default:
        gSpecialVar_Result = MULTI_B_PRESSED;
        DestroyTask(taskId);
        break;
    }
}

static const u8 *const sScrollableMultichoiceOptions[][MAX_SCROLL_MULTI_LENGTH] =
{
    [SCROLL_MULTI_NONE] =
    {
        gText_Exit
    },
    [SCROLL_MULTI_GLASS_WORKSHOP_VENDOR] =
    {
        COMPOUND_STRING("BLUE FLUTE"),
        COMPOUND_STRING("YELLOW FLUTE"),
        COMPOUND_STRING("RED FLUTE"),
        COMPOUND_STRING("WHITE FLUTE"),
        COMPOUND_STRING("BLACK FLUTE"),
        COMPOUND_STRING("PRETTY CHAIR"),
        COMPOUND_STRING("PRETTY DESK"),
        gText_Exit
    },
    [SCROLL_MULTI_POKEMON_FAN_CLUB_RATER] =
    {
        COMPOUND_STRING("0 pts"),
        COMPOUND_STRING("10 pts"),
        COMPOUND_STRING("20 pts"),
        COMPOUND_STRING("30 pts"),
        COMPOUND_STRING("40 pts"),
        COMPOUND_STRING("50 pts"),
        COMPOUND_STRING("60 pts"),
        COMPOUND_STRING("70 pts"),
        COMPOUND_STRING("80 pts"),
        COMPOUND_STRING("90 pts"),
        COMPOUND_STRING("100 pts"),
        COMPOUND_STRING("?")
    },
    [SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_1] =
    {
        COMPOUND_STRING("KISS POSTER{CLEAR_TO 0x5E}16BP"),
        COMPOUND_STRING("KISS CUSHION{CLEAR_TO 0x5E}32BP"),
        COMPOUND_STRING("SMOOCHUM DOLL{CLEAR_TO 0x5E}32BP"),
        COMPOUND_STRING("TOGEPI DOLL{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("MEOWTH DOLL{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("CLEFAIRY DOLL{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("DITTO DOLL{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("CYNDAQUIL DOLL{CLEAR_TO 0x5E}80BP"),
        COMPOUND_STRING("CHIKORITA DOLL{CLEAR_TO 0x5E}80BP"),
        COMPOUND_STRING("TOTODILE DOLL{CLEAR_TO 0x5E}80BP"),
        gText_Exit
    },
    [SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_2] =
    {
        COMPOUND_STRING("LAPRAS DOLL{CLEAR_TO 0x58}128BP"),
        COMPOUND_STRING("SNORLAX DOLL{CLEAR_TO 0x58}128BP"),
        COMPOUND_STRING("VENUSAUR DOLL{CLEAR_TO 0x58}256BP"),
        COMPOUND_STRING("CHARIZARD DOLL{CLEAR_TO 0x58}256BP"),
        COMPOUND_STRING("BLASTOISE DOLL{CLEAR_TO 0x58}256BP"),
        gText_Exit
    },
    [SCROLL_MULTI_BF_EXCHANGE_CORNER_VITAMIN_VENDOR] =
    {
        COMPOUND_STRING("PROTEIN{CLEAR_TO 0x64}1BP"),
        COMPOUND_STRING("CALCIUM{CLEAR_TO 0x64}1BP"),
        COMPOUND_STRING("IRON{CLEAR_TO 0x64}1BP"),
        COMPOUND_STRING("ZINC{CLEAR_TO 0x64}1BP"),
        COMPOUND_STRING("CARBOS{CLEAR_TO 0x64}1BP"),
        COMPOUND_STRING("HP UP{CLEAR_TO 0x64}1BP"),
        gText_Exit
    },
    [SCROLL_MULTI_BF_EXCHANGE_CORNER_HOLD_ITEM_VENDOR] =
    {
        COMPOUND_STRING("LEFTOVERS{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("WHITE HERB{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("QUICK CLAW{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("MENTAL HERB{CLEAR_TO 0x5E}48BP"),
        COMPOUND_STRING("BRIGHTPOWDER{CLEAR_TO 0x5E}64BP"),
        COMPOUND_STRING("CHOICE BAND{CLEAR_TO 0x5E}64BP"),
        COMPOUND_STRING("KING'S ROCK{CLEAR_TO 0x5E}64BP"),
        COMPOUND_STRING("FOCUS BAND{CLEAR_TO 0x5E}64BP"),
        COMPOUND_STRING("SCOPE LENS{CLEAR_TO 0x5E}64BP"),
        gText_Exit
    },
    [SCROLL_MULTI_BF_RECEPTIONIST] =
    {
        gText_BattleTower2,
        gText_BattleDome,
        gText_BattlePalace,
        gText_BattleArena,
        gText_BattleFactory,
        gText_BattlePike,
        gText_BattlePyramid,
        gText_RankingHall,
        gText_ExchangeService,
        gText_Exit
    },
    [SCROLL_MULTI_BF_MOVE_TUTOR_1] =
    {
        COMPOUND_STRING("SOFTBOILED{CLEAR_TO 0x4E}16BP"),
        COMPOUND_STRING("SEISMIC TOSS{CLEAR_TO 0x4E}24BP"),
        COMPOUND_STRING("DREAM EATER{CLEAR_TO 0x4E}24BP"),
        COMPOUND_STRING("MEGA PUNCH{CLEAR_TO 0x4E}24BP"),
        COMPOUND_STRING("MEGA KICK{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("BODY SLAM{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("ROCK SLIDE{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("COUNTER{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("THUNDER WAVE{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("SWORDS DANCE{CLEAR_TO 0x4E}48BP"),
        gText_Exit
    },
    [SCROLL_MULTI_BF_MOVE_TUTOR_2] =
    {
        COMPOUND_STRING("DEFENSE CURL{CLEAR_TO 0x4E}16BP"),
        COMPOUND_STRING("SNORE{CLEAR_TO 0x4E}24BP"),
        COMPOUND_STRING("MUD-SLAP{CLEAR_TO 0x4E}24BP"),
        COMPOUND_STRING("SWIFT{CLEAR_TO 0x4E}24BP"),
        COMPOUND_STRING("ICY WIND{CLEAR_TO 0x4E}24BP"),
        COMPOUND_STRING("ENDURE{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("PSYCH UP{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("ICE PUNCH{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("THUNDERPUNCH{CLEAR_TO 0x4E}48BP"),
        COMPOUND_STRING("FIRE PUNCH{CLEAR_TO 0x4E}48BP"),
        gText_Exit
    },
    [SCROLL_MULTI_SS_TIDAL_DESTINATION] =
    {
        gText_SlateportCity,
        gText_BattleFrontier,
        gText_SouthernIsland,
        gText_NavelRock,
        gText_BirthIsland,
        gText_FarawayIsland,
        gText_Exit
    },
    [SCROLL_MULTI_BATTLE_TENT_RULES] =
    {
        gText_BattleTrainers,
        gText_BattleBasics,
        gText_PokemonNature,
        gText_PokemonMoves,
        gText_Underpowered,
        gText_WhenInDanger,
        gText_Exit
    }
};

static void Task_ShowScrollableMultichoice(u8 taskId)
{
    u32 width;
    u8 i, windowId;
    struct WindowTemplate template;
    struct Task *task = &gTasks[taskId];

    LockPlayerFieldControls();
    gScrollableMultichoice_ScrollOffset = 0;
    sScrollableMultichoice_ItemSpriteId = MAX_SPRITES;
    FillFrontierExchangeCornerWindowAndItemIcon(task->tScrollMultiId, 0);
    ShowBattleFrontierTutorWindow(task->tScrollMultiId, 0);
    sScrollableMultichoice_ListMenuItem = AllocZeroed(task->tNumItems * 8);
    sFrontierExchangeCorner_NeverRead = 0;
    InitScrollableMultichoice();

    for (width = 0, i = 0; i < task->tNumItems; i++)
    {
        const u8 *text = sScrollableMultichoiceOptions[gSpecialVar_0x8004][i];
        sScrollableMultichoice_ListMenuItem[i].name = text;
        sScrollableMultichoice_ListMenuItem[i].id = i;
        width = DisplayTextAndGetWidth(text, width);
    }

    task->tWidth = ConvertPixelWidthToTileWidth(width);

    if (task->tLeft + task->tWidth > MAX_MULTICHOICE_WIDTH + 1)
    {
        int adjustedLeft = MAX_MULTICHOICE_WIDTH + 1 - task->tWidth;
        if (adjustedLeft < 0)
            task->tLeft = 0;
        else
            task->tLeft = adjustedLeft;
    }

    template = CreateWindowTemplate(0, task->tLeft, task->tTop, task->tWidth, task->tHeight, 0xF, 0x64);
    windowId = AddWindow(&template);
    task->tWindowId = windowId;
    SetStandardWindowBorderStyle(windowId, FALSE);

    gScrollableMultichoice_ListMenuTemplate.totalItems = task->tNumItems;
    gScrollableMultichoice_ListMenuTemplate.maxShowed = task->tMaxItemsOnScreen;
    gScrollableMultichoice_ListMenuTemplate.windowId = task->tWindowId;

    ScrollableMultichoice_UpdateScrollArrows(taskId);
    task->tListTaskId = ListMenuInit(&gScrollableMultichoice_ListMenuTemplate, task->tScrollOffset, task->tSelectedRow);
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].func = ScrollableMultichoice_ProcessInput;
}

static void InitScrollableMultichoice(void)
{
    gScrollableMultichoice_ListMenuTemplate.items = sScrollableMultichoice_ListMenuItem;
    gScrollableMultichoice_ListMenuTemplate.moveCursorFunc = ScrollableMultichoice_MoveCursor;
    gScrollableMultichoice_ListMenuTemplate.itemPrintFunc = NULL;
    gScrollableMultichoice_ListMenuTemplate.totalItems = 1;
    gScrollableMultichoice_ListMenuTemplate.maxShowed = 1;
    gScrollableMultichoice_ListMenuTemplate.windowId = 0;
    gScrollableMultichoice_ListMenuTemplate.header_X = 0;
    gScrollableMultichoice_ListMenuTemplate.item_X = 8;
    gScrollableMultichoice_ListMenuTemplate.cursor_X = 0;
    gScrollableMultichoice_ListMenuTemplate.upText_Y = 1;
    gScrollableMultichoice_ListMenuTemplate.cursorPal = 2;
    gScrollableMultichoice_ListMenuTemplate.fillValue = 1;
    gScrollableMultichoice_ListMenuTemplate.cursorShadowPal = 3;
    gScrollableMultichoice_ListMenuTemplate.lettersSpacing = 0;
    gScrollableMultichoice_ListMenuTemplate.itemVerticalPadding = 0;
    gScrollableMultichoice_ListMenuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    gScrollableMultichoice_ListMenuTemplate.fontId = FONT_NORMAL;
    gScrollableMultichoice_ListMenuTemplate.cursorKind = CURSOR_BLACK_ARROW;
}

static void ScrollableMultichoice_MoveCursor(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    u8 taskId;
    PlaySE(SE_SELECT);
    taskId = FindTaskIdByFunc(ScrollableMultichoice_ProcessInput);
    if (taskId != TASK_NONE)
    {
        u16 selection;
        struct Task *task = &gTasks[taskId];
        ListMenuGetScrollAndRow(task->tListTaskId, &selection, NULL);
        gScrollableMultichoice_ScrollOffset = selection;
        ListMenuGetCurrentItemArrayId(task->tListTaskId, &selection);
        HideFrontierExchangeCornerItemIcon(task->tScrollMultiId, sFrontierExchangeCorner_NeverRead);
        FillFrontierExchangeCornerWindowAndItemIcon(task->tScrollMultiId, selection);
        ShowBattleFrontierTutorMoveDescription(task->tScrollMultiId, selection);
        sFrontierExchangeCorner_NeverRead = selection;
    }
}

static void ScrollableMultichoice_ProcessInput(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    s32 input = ListMenu_ProcessInput(task->tListTaskId);

    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        gSpecialVar_Result = MULTI_B_PRESSED;
        PlaySE(SE_SELECT);
        CloseScrollableMultichoice(taskId);
        break;
    default:
        gSpecialVar_Result = input;
        PlaySE(SE_SELECT);
        if (!task->tKeepOpenAfterSelect)
        {
            CloseScrollableMultichoice(taskId);
        }
        else if (input == task->tNumItems - 1)
        {
            // Selected option was the last one (Exit)
            CloseScrollableMultichoice(taskId);
        }
        else
        {
            // Handle selection while keeping the menu open
            ScrollableMultichoice_RemoveScrollArrows(taskId);
            task->func = Task_ScrollableMultichoice_WaitReturnToList;
            ScriptContext_Enable();
        }
        break;
    }
}

static void CloseScrollableMultichoice(u8 taskId)
{
    u16 selection;
    struct Task *task = &gTasks[taskId];
    ListMenuGetCurrentItemArrayId(task->tListTaskId, &selection);
    HideFrontierExchangeCornerItemIcon(task->tScrollMultiId, selection);
    ScrollableMultichoice_RemoveScrollArrows(taskId);
    DestroyListMenuTask(task->tListTaskId, NULL, NULL);
    Free(sScrollableMultichoice_ListMenuItem);
    ClearStdWindowAndFrameToTransparent(task->tWindowId, TRUE);
    FillWindowPixelBuffer(task->tWindowId, PIXEL_FILL(0));
    CopyWindowToVram(task->tWindowId, COPYWIN_GFX);
    RemoveWindow(task->tWindowId);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

// Never run, tKeepOpenAfterSelect is FALSE for all scrollable multichoices.
static void Task_ScrollableMultichoice_WaitReturnToList(u8 taskId)
{
    switch (gTasks[taskId].tKeepOpenAfterSelect)
    {
    case 1:
    default:
        break;
    case 2:
        gTasks[taskId].tKeepOpenAfterSelect = 1;
        gTasks[taskId].func = Task_ScrollableMultichoice_ReturnToList;
        break;
    }
}

// Never called
void ScrollableMultichoice_TryReturnToList(void)
{
    u8 taskId = FindTaskIdByFunc(Task_ScrollableMultichoice_WaitReturnToList);
    if (taskId == TASK_NONE)
        ScriptContext_Enable();
    else
        gTasks[taskId].tKeepOpenAfterSelect++; // Return to list
}

static void Task_ScrollableMultichoice_ReturnToList(u8 taskId)
{
    LockPlayerFieldControls();
    ScrollableMultichoice_UpdateScrollArrows(taskId);
    gTasks[taskId].func = ScrollableMultichoice_ProcessInput;
}

static void ScrollableMultichoice_UpdateScrollArrows(u8 taskId)
{
    static const struct ScrollArrowsTemplate sScrollableMultichoice_ScrollArrowsTemplate = {
        .firstArrowType = SCROLL_ARROW_UP,
        .firstX = 0,
        .firstY = 0,
        .secondArrowType = SCROLL_ARROW_DOWN,
        .secondX = 0,
        .secondY = 0,
        .fullyUpThreshold = 0,
        .fullyDownThreshold = 0,
        .tileTag = GFXTAG_MULTICHOICE_SCROLL_ARROWS,
        .palTag = PALTAG_MULTICHOICE_SCROLL_ARROWS,
        .palNum = 0
    };

    struct Task *task = &gTasks[taskId];
    struct ScrollArrowsTemplate template = sScrollableMultichoice_ScrollArrowsTemplate;
    if (task->tMaxItemsOnScreen != task->tNumItems)
    {
        u32 y0 = (8 * (task->tTop - 1));

        template.firstX = (task->tWidth / 2) * 8 + 12 + (task->tLeft - 1) * 8;
        template.firstY = 8 + y0;
        template.secondX = (task->tWidth / 2) * 8 + 12 + (task->tLeft - 1) * 8;
        template.secondY = task->tHeight * 8 + 10 + y0;
        template.fullyUpThreshold = 0;
        template.fullyDownThreshold = task->tNumItems - task->tMaxItemsOnScreen;
        task->tScrollArrowId = AddScrollIndicatorArrowPair(&template, &gScrollableMultichoice_ScrollOffset);
    }
}

static void ScrollableMultichoice_RemoveScrollArrows(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (task->tMaxItemsOnScreen != task->tNumItems)
        RemoveScrollIndicatorArrowPair(task->tScrollArrowId);
}

// Removed for Emerald (replaced by ShowScrollableMultichoice)
void ShowGlassWorkshopMenu(void)
{

}

void SetBattleTowerLinkPlayerGfx(void)
{
    u8 i;
    for (i = 0; i < 2; i++)
    {
        VarSet(VAR_OBJ_GFX_ID_F - i, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL);
    }
}

void ShowNatureGirlMessage(void)
{
    u8 nature;

    if (gSpecialVar_0x8004 >= PARTY_SIZE)
        gSpecialVar_0x8004 = 0;

    nature = GetNature(&gPlayerParty[gSpecialVar_0x8004]);
    ShowFieldMessage(gNaturesInfo[nature].natureGirlMessage);
}

void UpdateFrontierGambler(u16 daysSince)
{
}

void ShowFrontierGamblerLookingMessage(void)
{
    // TODO: remove
}

void ShowFrontierGamblerGoMessage(void)
{
    // TODO: remove
}

void FrontierGamblerSetWonOrLost(bool8 won)
{
    // TODO: remove
}

void UpdateBattlePointsWindow(void)
{
}

void ShowBattlePointsWindow(void)
{
    static const struct WindowTemplate sBattlePoints_WindowTemplate =
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 6,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 8,
    };

    sBattlePointsWindowId = AddWindow(&sBattlePoints_WindowTemplate);
    SetStandardWindowBorderStyle(sBattlePointsWindowId, FALSE);
    UpdateBattlePointsWindow();
    CopyWindowToVram(sBattlePointsWindowId, COPYWIN_GFX);
}

void CloseBattlePointsWindow(void)
{
    ClearStdWindowAndFrameToTransparent(sBattlePointsWindowId, TRUE);
    RemoveWindow(sBattlePointsWindowId);
}

void TakeFrontierBattlePoints(void)
{
}

void GiveFrontierBattlePoints(void)
{
}

u16 GetFrontierBattlePoints(void)
{
    return 0;
}

void CloseFrontierExchangeCornerItemIconWindow(void)
{
    ClearStdWindowAndFrameToTransparent(sFrontierExchangeCorner_ItemIconWindowId, TRUE);
    RemoveWindow(sFrontierExchangeCorner_ItemIconWindowId);
}

static void FillFrontierExchangeCornerWindowAndItemIcon(u16 menu, u16 selection)
{
}

static void HideFrontierExchangeCornerItemIcon(u16 menu, u16 unused)
{
    if (sScrollableMultichoice_ItemSpriteId != MAX_SPRITES)
    {
        switch (menu)
        {
        case SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_1:
        case SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_2:
        case SCROLL_MULTI_BF_EXCHANGE_CORNER_VITAMIN_VENDOR:
        case SCROLL_MULTI_BF_EXCHANGE_CORNER_HOLD_ITEM_VENDOR:
            // This makes sure deleting the icon will not clear palettes in use by object events
            FieldEffectFreeGraphicsResources(&gSprites[sScrollableMultichoice_ItemSpriteId]);
            break;
        }
        sScrollableMultichoice_ItemSpriteId = MAX_SPRITES;
    }
}

void BufferBattleFrontierTutorMoveName(void)
{
    StringCopy(gStringVar1, GetMoveName(gSpecialVar_0x8005));
}

static void ShowBattleFrontierTutorWindow(u8 menu, u16 selection)
{
    static const struct WindowTemplate sBattleFrontierTutor_WindowTemplate =
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 7,
        .width = 12,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 28,
    };

    if (menu == SCROLL_MULTI_BF_MOVE_TUTOR_1 || menu == SCROLL_MULTI_BF_MOVE_TUTOR_2)
    {
        if (gSpecialVar_0x8006 == 0)
        {
            sTutorMoveAndElevatorWindowId = AddWindow(&sBattleFrontierTutor_WindowTemplate);
            SetStandardWindowBorderStyle(sTutorMoveAndElevatorWindowId, FALSE);
        }
        ShowBattleFrontierTutorMoveDescription(menu, selection);
    }
}

static void ShowBattleFrontierTutorMoveDescription(u8 menu, u16 selection)
{
    static const u8 *const sBattleFrontier_TutorMoveDescriptions1[] =
    {
        gText_Exit,
    };

    static const u8 *const sBattleFrontier_TutorMoveDescriptions2[] =
    {
        gText_Exit,
    };

    if (menu == SCROLL_MULTI_BF_MOVE_TUTOR_1 || menu == SCROLL_MULTI_BF_MOVE_TUTOR_2)
    {
        FillWindowPixelRect(sTutorMoveAndElevatorWindowId, PIXEL_FILL(1), 0, 0, 96, 48);
        if (menu == SCROLL_MULTI_BF_MOVE_TUTOR_2)
            AddTextPrinterParameterized(sTutorMoveAndElevatorWindowId, FONT_NORMAL, sBattleFrontier_TutorMoveDescriptions2[selection], 0, 1, 0, NULL);
        else
            AddTextPrinterParameterized(sTutorMoveAndElevatorWindowId, FONT_NORMAL, sBattleFrontier_TutorMoveDescriptions1[selection], 0, 1, 0, NULL);
    }
}

void CloseBattleFrontierTutorWindow(void)
{
    ClearStdWindowAndFrameToTransparent(sTutorMoveAndElevatorWindowId, TRUE);
    RemoveWindow(sTutorMoveAndElevatorWindowId);
}

// Never called
void ScrollableMultichoice_RedrawPersistentMenu(void)
{
    u16 scrollOffset, selectedRow;
    u8 i;
    u8 taskId = FindTaskIdByFunc(Task_ScrollableMultichoice_WaitReturnToList);
    if (taskId != TASK_NONE)
    {
        struct Task *task = &gTasks[taskId];
        ListMenuGetScrollAndRow(task->tListTaskId, &scrollOffset, &selectedRow);
        SetStandardWindowBorderStyle(task->tWindowId, FALSE);

        for (i = 0; i < MAX_SCROLL_MULTI_ON_SCREEN; i++)
            AddTextPrinterParameterized5(task->tWindowId, FONT_NORMAL, sScrollableMultichoiceOptions[gSpecialVar_0x8004][scrollOffset + i], 10, i * 16, TEXT_SKIP_DRAW, NULL, 0, 0);

        AddTextPrinterParameterized(task->tWindowId, FONT_NORMAL, gText_SelectorArrow, 0, selectedRow * 16, TEXT_SKIP_DRAW, NULL);
        PutWindowTilemap(task->tWindowId);
        CopyWindowToVram(task->tWindowId, COPYWIN_FULL);
    }
}

// Never called
// Close a scrollable multichoice that stays open after selection
void ScrollableMultichoice_ClosePersistentMenu(void)
{
    u8 taskId = FindTaskIdByFunc(Task_ScrollableMultichoice_WaitReturnToList);
    if (taskId != TASK_NONE)
    {
        struct Task *task = &gTasks[taskId];
        DestroyListMenuTask(task->tListTaskId, NULL, NULL);
        Free(sScrollableMultichoice_ListMenuItem);
        ClearStdWindowAndFrameToTransparent(task->tWindowId, TRUE);
        FillWindowPixelBuffer(task->tWindowId, PIXEL_FILL(0));
        ClearWindowTilemap(task->tWindowId);
        CopyWindowToVram(task->tWindowId, COPYWIN_GFX);
        RemoveWindow(task->tWindowId);
        DestroyTask(taskId);
    }
}

// Undefine Scrollable Multichoice task data macros
#undef tMaxItemsOnScreen
#undef tNumItems
#undef tLeft
#undef tTop
#undef tWidth
#undef tHeight
#undef tKeepOpenAfterSelect
#undef tScrollOffset
#undef tSelectedRow
#undef tScrollMultiId
#undef tScrollArrowId
#undef tWindowId
#undef tListTaskId
#undef tTaskId

#define DEOXYS_ROCK_LEVELS 11

void DoDeoxysRockInteraction(void)
{
    CreateTask(Task_DeoxysRockInteraction, 8);
}

static const u16 sDeoxysRockPalettes[DEOXYS_ROCK_LEVELS][16] = {
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_1.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_2.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_3.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_4.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_5.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_6.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_7.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_8.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_9.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_10.gbapal"),
    INCBIN_U16("graphics/field_effects/palettes/deoxys_rock_11.gbapal"),
};

static const u8 sDeoxysRockCoords[DEOXYS_ROCK_LEVELS][2] = {
    { 15, 12 },
    { 11, 14 },
    { 15,  8 },
    { 19, 14 },
    { 12, 11 },
    { 18, 11 },
    { 15, 14 },
    { 11, 14 },
    { 19, 14 },
    { 15, 15 },
    { 15, 10 },
};

static void Task_DeoxysRockInteraction(u8 taskId)
{
    static const u8 sStoneMaxStepCounts[DEOXYS_ROCK_LEVELS - 1] = { 4, 8, 8, 8, 4, 4, 4, 6, 3, 3 };

    if (FlagGet(FLAG_DEOXYS_ROCK_COMPLETE) == TRUE)
    {
        gSpecialVar_Result = DEOXYS_ROCK_COMPLETE;
        ScriptContext_Enable();
        DestroyTask(taskId);
    }
    else
    {
        u16 rockLevel = VarGet(VAR_DEOXYS_ROCK_LEVEL);
        u16 stepCount = VarGet(VAR_DEOXYS_ROCK_STEP_COUNT);

        VarSet(VAR_DEOXYS_ROCK_STEP_COUNT, 0);
        if (rockLevel != 0 && sStoneMaxStepCounts[rockLevel - 1] < stepCount)
        {
            // Player failed to take the shortest path to the stone, so it resets.
            ChangeDeoxysRockLevel(0);
            VarSet(VAR_DEOXYS_ROCK_LEVEL, 0);
            gSpecialVar_Result = DEOXYS_ROCK_FAILED;
            DestroyTask(taskId);
        }
        else if (rockLevel == DEOXYS_ROCK_LEVELS - 1)
        {
            FlagSet(FLAG_DEOXYS_ROCK_COMPLETE);
            gSpecialVar_Result = DEOXYS_ROCK_SOLVED;
            ScriptContext_Enable();
            DestroyTask(taskId);
        }
        else
        {
            rockLevel++;
            ChangeDeoxysRockLevel(rockLevel);
            VarSet(VAR_DEOXYS_ROCK_LEVEL, rockLevel);
            gSpecialVar_Result = DEOXYS_ROCK_PROGRESSED;
            DestroyTask(taskId);
        }
    }
}

static void ChangeDeoxysRockLevel(u8 rockLevel)
{
    // TODO: remove
}

void IncrementBirthIslandRockStepCount(void)
{
    // TODO: remove
}

// called before fade-in
void SetDeoxysRockPalette(void)
{
    u32 paletteNum = IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_BIRTH_ISLAND_STONE);
    LoadPalette(&sDeoxysRockPalettes[(u8)VarGet(VAR_DEOXYS_ROCK_LEVEL)], OBJ_PLTT_ID(paletteNum), PLTT_SIZEOF(4));
    // Set faded to all black, weather blending handled during fade-in
    CpuFill16(RGB_BLACK, &gPlttBufferFaded[OBJ_PLTT_ID(paletteNum)], PLTT_SIZE_4BPP);
}

void SetPCBoxToSendMon(u8 boxId)
{
    sPCBoxToSendMon = boxId;
}

u16 GetPCBoxToSendMon(void)
{
    return sPCBoxToSendMon;
}

bool8 ShouldShowBoxWasFullMessage(void)
{
    FlagSet(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
    return TRUE;
}

bool8 IsDestinationBoxFull(void)
{
    return TRUE;
}

void CreateAbnormalWeatherEvent(void)
{
    u16 randomValue = Random();
    VarSet(VAR_ABNORMAL_WEATHER_STEP_COUNTER, 0);

    if (FlagGet(FLAG_DEFEATED_KYOGRE) == TRUE)
    {
        VarSet(VAR_ABNORMAL_WEATHER_LOCATION, (randomValue % TERRA_CAVE_LOCATIONS) + TERRA_CAVE_LOCATIONS_START);
    }
    else if (FlagGet(FLAG_DEFEATED_GROUDON) == TRUE)
    {
        VarSet(VAR_ABNORMAL_WEATHER_LOCATION, (randomValue % MARINE_CAVE_LOCATIONS) + MARINE_CAVE_LOCATIONS_START);
    }
    else if ((randomValue & 1) == 0)
    {
        randomValue = Random();
        VarSet(VAR_ABNORMAL_WEATHER_LOCATION, (randomValue % TERRA_CAVE_LOCATIONS) + TERRA_CAVE_LOCATIONS_START);
    }
    else
    {
        randomValue = Random();
        VarSet(VAR_ABNORMAL_WEATHER_LOCATION, (randomValue % MARINE_CAVE_LOCATIONS) + MARINE_CAVE_LOCATIONS_START);
    }
}

// Saves the map name for the current abnormal weather location in gStringVar1, then
// returns TRUE if the weather is for Kyogre, and FALSE if it's for Groudon.
bool32 GetAbnormalWeatherMapNameAndType(void)
{
    // TODO: remove
    return FALSE;
}

bool8 AbnormalWeatherHasExpired(void)
{
    // TODO: remove
    return FALSE;
}

void Unused_SetWeatherSunny(void)
{
    SetCurrentAndNextWeather(WEATHER_SUNNY);
}

// All mart employees have a local id of 1, so function always returns 1
u32 GetMartEmployeeObjectEventId(void)
{
    // TODO: remove
    return 1;
}

bool32 IsTrainerRegistered(void)
{
    return FALSE;
}

// Always returns FALSE
bool32 ShouldDistributeEonTicket(void)
{
    if (!VarGet(VAR_DISTRIBUTE_EON_TICKET))
        return FALSE;

    return TRUE;
}

#define tState data[0]

void BattleTowerReconnectLink(void)
{
}

void LinkRetireStatusWithBattleTowerPartner(void)
{
    CreateTask(Task_LinkRetireStatusWithBattleTowerPartner, 5);
}

// Communicate with a Battle Tower link partner to tell them
// whether or not the player chose to continue or retire,
// and determine what the partner chose to do
// gSpecialVar_0x8004: Player's choice
// gSpecialVar_0x8005: Partner's choice (read from gBlockRecvBuffer[1][0])
static void Task_LinkRetireStatusWithBattleTowerPartner(u8 taskId)
{
    // TODO: remove
}

#undef tState

#define playCount data[0]
#define delay     data[1]

void LoopWingFlapSE(void)
{
    CreateTask(Task_LoopWingFlapSE, 8);
    PlaySE(SE_M_WING_ATTACK);
}

static void Task_LoopWingFlapSE(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    delay++;
    if (delay == gSpecialVar_0x8005)
    {
        playCount++;
        delay = 0;
        PlaySE(SE_M_WING_ATTACK);
    }

    if (playCount == gSpecialVar_0x8004 - 1)
        DestroyTask(taskId);
}

#undef playCount
#undef delay

#define CURTAIN_HEIGHT 4
#define CURTAIN_WIDTH 3
#define tFrameTimer   data
#define tCurrentFrame data[3]

void CloseBattlePikeCurtain(void)
{
    u8 taskId = CreateTask(Task_CloseBattlePikeCurtain, 8);
    gTasks[taskId].tFrameTimer[0] = 4;
    gTasks[taskId].tFrameTimer[1] = 4;
    gTasks[taskId].tFrameTimer[2] = 4;
    gTasks[taskId].tCurrentFrame = 0;
}

static void Task_CloseBattlePikeCurtain(u8 taskId)
{
    u8 x, y;
    s16 *data = gTasks[taskId].data;

    tFrameTimer[tCurrentFrame]--;
    if (tFrameTimer[tCurrentFrame] == 0)
    {
        for (y = 0; y < CURTAIN_HEIGHT; y++)
        {
            for (x = 0; x < CURTAIN_WIDTH; x++)
            {
                MapGridSetMetatileIdAt(gSaveBlock1Ptr->pos.x + x + MAP_OFFSET - 1,
                                       gSaveBlock1Ptr->pos.y + y + MAP_OFFSET - 3,
                                       (x + METATILE_BattlePike_CurtainFrames_Start) + (y * METATILE_ROW_WIDTH) + (tCurrentFrame * CURTAIN_HEIGHT * METATILE_ROW_WIDTH));
            }
        }
        DrawWholeMapView();
        tCurrentFrame++;
        if (tCurrentFrame == 3)
        {
            DestroyTask(taskId);
            ScriptContext_Enable();
        }
    }
}

#undef CURTAIN_HEIGHT
#undef CURTAIN_WIDTH
#undef tFrameTimer
#undef tCurrentFrame

void GetBattlePyramidHint(void)
{
}

bool8 InPokemonCenter(void)
{
    static const u16 sPokemonCenters[] =
    {
        MAP_UNDEFINED
    };

    int i;
    u16 map = (gSaveBlock1Ptr->location.mapGroup << 8) + gSaveBlock1Ptr->location.mapNum;

    for (i = 0; sPokemonCenters[i] != MAP_UNDEFINED; i++)
    {
        if (sPokemonCenters[i] == map)
            return TRUE;
    }
    return FALSE;
}

/*  Summary of the Lilycove Trainer Fan Club, because it's a little messy

    ## The Fan Club room itself
    There are initially 4 members of the Fan Club (+ an interviewer), none of whom are fans of the player
    After becoming the champion there will be 8 members of the Fan Club, 3 of whom are automatically fans of the player
    After this point, if a club member is a fan of the player they will sit at the front table and comment on the player
    If they are not fans of the player, they will sit at the far table and can make comments about a different trainer (see BufferFanClubTrainerName)

    ## Gaining/losing fans
    After every link battle the player will gain a fan if they won, or lose a fan if they lost
    If the player has at least 3 fans, this is the only way to gain fans
    If the player has fewer than 3 fans, they may also gain fans by completing certain tasks enough times (see TryGainNewFanFromCounter)
    If the player has at least 5 fans, they can lose a fan every 12 real-time hours, or more often if the timer variable is reset (see TryLoseFansFromPlayTime)
    If the player has only 1 fan left it cannot be lost

    ## Variables
    VAR_FANCLUB_FAN_COUNTER, a bitfield for tracking the fans
      Bits  1-7: Counter for when to add new fans
      Bit     8: Flag set after receiving the initial 3 fans
      Bits 9-16: Flags for each of the 8 club members, set to 1 when theyre a fan of the player and 0 when theyre not

    VAR_FANCLUB_LOSE_FAN_TIMER, a timer for when to lose fans
      Compared against playTimeHours. When theyre equal, a fan is ready to be lost
      For every fan thats lost this way 12 hours are added to the timer

    VAR_LILYCOVE_FAN_CLUB_STATE
      0: Player is not the champion yet
      1: Player is the champion, ready to meet their initial fans
      2: Player has met their initial fans
*/

#define FANCLUB_BITFIELD (gSaveBlock1Ptr->vars[VAR_FANCLUB_FAN_COUNTER - VARS_START])
#define FANCLUB_COUNTER    0x007F

#define GET_TRAINER_FAN_CLUB_FLAG(flag)  (FANCLUB_BITFIELD >> (flag) & 1)
#define SET_TRAINER_FAN_CLUB_FLAG(flag)  (FANCLUB_BITFIELD |= 1 << (flag))
#define FLIP_TRAINER_FAN_CLUB_FLAG(flag) (FANCLUB_BITFIELD ^= 1 << (flag))

#define GET_TRAINER_FAN_CLUB_COUNTER         (FANCLUB_BITFIELD & FANCLUB_COUNTER)
#define SET_TRAINER_FAN_CLUB_COUNTER(count)  (FANCLUB_BITFIELD = (FANCLUB_BITFIELD & ~FANCLUB_COUNTER) | (count))
#define INCR_TRAINER_FAN_CLUB_COUNTER(count) (FANCLUB_BITFIELD += (count))
#define CLEAR_TRAINER_FAN_CLUB_COUNTER       (FANCLUB_BITFIELD &= ~FANCLUB_COUNTER)

void ResetFanClub(void)
{
    gSaveBlock1Ptr->vars[VAR_FANCLUB_FAN_COUNTER - VARS_START] = 0;
    gSaveBlock1Ptr->vars[VAR_FANCLUB_LOSE_FAN_TIMER - VARS_START] = 0;
}

void TryLoseFansFromPlayTimeAfterLinkBattle(void)
{
    if (DidPlayerGetFirstFans())
    {
        TryLoseFansFromPlayTime();
        gSaveBlock1Ptr->vars[VAR_FANCLUB_LOSE_FAN_TIMER - VARS_START] = gSaveBlock2Ptr->playTimeHours;
    }
}

void UpdateTrainerFanClubGameClear(void)
{
    if (!GET_TRAINER_FAN_CLUB_FLAG(FANCLUB_GOT_FIRST_FANS))
    {
        SetPlayerGotFirstFans();
        SetInitialFansOfPlayer();
        gSaveBlock1Ptr->vars[VAR_FANCLUB_LOSE_FAN_TIMER - VARS_START] = gSaveBlock2Ptr->playTimeHours;
        FlagClear(FLAG_HIDE_FANCLUB_OLD_LADY);
        FlagClear(FLAG_HIDE_FANCLUB_BOY);
        FlagClear(FLAG_HIDE_FANCLUB_LITTLE_BOY);
        FlagClear(FLAG_HIDE_FANCLUB_LADY);
        FlagClear(FLAG_HIDE_LILYCOVE_FAN_CLUB_INTERVIEWER);
        VarSet(VAR_LILYCOVE_FAN_CLUB_STATE, 1);
    }
}

// If the player has < 3 fans, gain a new fan whenever the counter reaches 20+
// Defeating Drake or participating in a Contest increments the counter by 2
// Participating at Battle Tower or in a Secret Base battle increments the counter by 1
u8 TryGainNewFanFromCounter(u8 incrementId)
{
    static const u8 sCounterIncrements[] =
    {
        [FANCOUNTER_DEFEATED_DRAKE]    = 2,
        [FANCOUNTER_BATTLED_AT_BASE]   = 1,
        [FANCOUNTER_FINISHED_CONTEST]  = 2,
        [FANCOUNTER_USED_BATTLE_TOWER] = 1
    };

    if (VarGet(VAR_LILYCOVE_FAN_CLUB_STATE) == 2)
    {
        if (GET_TRAINER_FAN_CLUB_COUNTER + sCounterIncrements[incrementId] > 19)
        {
            if (GetNumFansOfPlayerInTrainerFanClub() < 3)
            {
                PlayerGainRandomTrainerFan();
                CLEAR_TRAINER_FAN_CLUB_COUNTER;
            }
            else
            {
                SET_TRAINER_FAN_CLUB_COUNTER(20);
            }
        }
        else
        {
            INCR_TRAINER_FAN_CLUB_COUNTER(sCounterIncrements[incrementId]);
        }
    }

    return GET_TRAINER_FAN_CLUB_COUNTER;
}


// Loop through the fan club members, and if theyre not a fan of the player there is a 50% chance for them to become a fan
// Stops when a fan is gained
// If no new fan was gained while looping, the last non-fan in the list becomes a fan
// If all the members are already fans of the player then this redundantly sets the first fan in the list to be a fan
static u16 PlayerGainRandomTrainerFan(void)
{
    static const u8 sFanClubMemberIds[NUM_TRAINER_FAN_CLUB_MEMBERS] =
    {
        FANCLUB_MEMBER1,
        FANCLUB_MEMBER2,
        FANCLUB_MEMBER3,
        FANCLUB_MEMBER4,
        FANCLUB_MEMBER5,
        FANCLUB_MEMBER6,
        FANCLUB_MEMBER7,
        FANCLUB_MEMBER8
    };

    u8 i;
    u8 idx = 0;

    for (i = 0; i < ARRAY_COUNT(sFanClubMemberIds); i++)
    {
        if (!GET_TRAINER_FAN_CLUB_FLAG(sFanClubMemberIds[i]))
        {
            idx = i;
            if (Random() & 1)
            {
                SET_TRAINER_FAN_CLUB_FLAG(sFanClubMemberIds[idx]);
                return idx;
            }
        }
    }
    SET_TRAINER_FAN_CLUB_FLAG(sFanClubMemberIds[idx]);
    return idx;
}

// Loops through the fan club members, and if theyre a fan of the player there is a 50% chance for them to stop being a fan
// Stops if a fan is removed, or if the player has only one fan left
// If no fan was lost while looping, the last current fan in the list will stop being a fan
static u16 PlayerLoseRandomTrainerFan(void)
{
    static const u8 sFanClubMemberIds[NUM_TRAINER_FAN_CLUB_MEMBERS] =
    {
        FANCLUB_MEMBER1,
        FANCLUB_MEMBER6,
        FANCLUB_MEMBER7,
        FANCLUB_MEMBER4,
        FANCLUB_MEMBER3,
        FANCLUB_MEMBER5,
        FANCLUB_MEMBER8,
        FANCLUB_MEMBER2
    };

    u8 i;
    u8 idx = 0;

    if (GetNumFansOfPlayerInTrainerFanClub() == 1)
        return 0;

    for (i = 0; i < ARRAY_COUNT(sFanClubMemberIds); i++)
    {
        if (GET_TRAINER_FAN_CLUB_FLAG(sFanClubMemberIds[i]))
        {
            idx = i;
            if (Random() & 1)
            {
                FLIP_TRAINER_FAN_CLUB_FLAG(sFanClubMemberIds[idx]);
                return idx;
            }
        }
    }

    if (GET_TRAINER_FAN_CLUB_FLAG(sFanClubMemberIds[idx]))
        FLIP_TRAINER_FAN_CLUB_FLAG(sFanClubMemberIds[idx]);

    return idx;
}

u16 GetNumFansOfPlayerInTrainerFanClub(void)
{
    u8 i;
    u8 numFans = 0;

    for (i = 0; i < NUM_TRAINER_FAN_CLUB_MEMBERS; i++)
    {
        if (GET_TRAINER_FAN_CLUB_FLAG(i + FANCLUB_MEMBER1))
            numFans++;
    }

    return numFans;
}

// If the player has > 5 fans in the Trainer Fan Club, then lose 1 fan for every 12 hours since the last fan loss / timer reset
void TryLoseFansFromPlayTime(void)
{
    u8 i = 0;
    if (gSaveBlock2Ptr->playTimeHours < 999)
    {
        while (TRUE)
        {
            if (GetNumFansOfPlayerInTrainerFanClub() < 5)
            {
                gSaveBlock1Ptr->vars[VAR_FANCLUB_LOSE_FAN_TIMER - VARS_START] = gSaveBlock2Ptr->playTimeHours;
                break;
            }
            else if (i == NUM_TRAINER_FAN_CLUB_MEMBERS)
            {
                break;
            }
            else if (gSaveBlock2Ptr->playTimeHours - gSaveBlock1Ptr->vars[VAR_FANCLUB_LOSE_FAN_TIMER - VARS_START] < 12)
            {
                return;
            }
            PlayerLoseRandomTrainerFan();
            gSaveBlock1Ptr->vars[VAR_FANCLUB_LOSE_FAN_TIMER - VARS_START] += 12;
            i++;
        }
    }
}

bool8 IsFanClubMemberFanOfPlayer(void)
{
    return GET_TRAINER_FAN_CLUB_FLAG(gSpecialVar_0x8004);
}

static void SetInitialFansOfPlayer(void)
{
    SET_TRAINER_FAN_CLUB_FLAG(FANCLUB_MEMBER6);
    SET_TRAINER_FAN_CLUB_FLAG(FANCLUB_MEMBER1);
    SET_TRAINER_FAN_CLUB_FLAG(FANCLUB_MEMBER3);
}

void BufferFanClubTrainerName(void)
{
    u8 whichLinkTrainer = 0;
    u8 whichNPCTrainer = 0;
    switch (gSpecialVar_0x8004)
    {
    case FANCLUB_MEMBER1:
        break;
    case FANCLUB_MEMBER2:
        break;
    case FANCLUB_MEMBER3:
        whichLinkTrainer = 0;
        whichNPCTrainer = 3;
        break;
    case FANCLUB_MEMBER4:
        whichLinkTrainer = 0;
        whichNPCTrainer = 1;
        break;
    case FANCLUB_MEMBER5:
        whichLinkTrainer = 1;
        whichNPCTrainer = 0;
        break;
    case FANCLUB_MEMBER6:
        whichLinkTrainer = 0;
        whichNPCTrainer = 4;
        break;
    case FANCLUB_MEMBER7:
        whichLinkTrainer = 1;
        whichNPCTrainer = 5;
        break;
    case FANCLUB_MEMBER8:
        break;
    }
#if FREE_LINK_BATTLE_RECORDS == FALSE
    BufferFanClubTrainerName_(&gSaveBlock1Ptr->linkBattleRecords, whichLinkTrainer, whichNPCTrainer);
#else
    BufferFanClubTrainerName_(whichLinkTrainer, whichNPCTrainer);
#endif //FREE_LINK_BATTLE_RECORDS
}

#if FREE_LINK_BATTLE_RECORDS == FALSE
static void BufferFanClubTrainerName_(struct LinkBattleRecords *linkRecords, u8 whichLinkTrainer, u8 whichNPCTrainer)
{
    struct LinkBattleRecord *record = &linkRecords->entries[whichLinkTrainer];
    if (record->name[0] == EOS)
    {
        switch (whichNPCTrainer)
        {
        case 0:
            StringCopy(gStringVar1, gText_Wallace);
            break;
        case 1:
            StringCopy(gStringVar1, gText_Steven);
            break;
        case 2:
            StringCopy(gStringVar1, gText_Brawly);
            break;
        case 3:
            StringCopy(gStringVar1, gText_Winona);
            break;
        case 4:
            StringCopy(gStringVar1, gText_Phoebe);
            break;
        case 5:
            StringCopy(gStringVar1, gText_Glacia);
            break;
        default:
            StringCopy(gStringVar1, gText_Wallace);
            break;
        }
    }
    else
    {
        StringCopyN(gStringVar1, record->name, PLAYER_NAME_LENGTH);
        gStringVar1[PLAYER_NAME_LENGTH] = EOS;
        ConvertInternationalString(gStringVar1, linkRecords->languages[whichLinkTrainer]);
    }
}
#else
static void BufferFanClubTrainerName_(u8 whichLinkTrainer, u8 whichNPCTrainer)
{
    switch (whichNPCTrainer)
    {
        case 0:
            StringCopy(gStringVar1, gText_Wallace);
            break;
        case 1:
            StringCopy(gStringVar1, gText_Steven);
            break;
        case 2:
            StringCopy(gStringVar1, gText_Brawly);
            break;
        case 3:
            StringCopy(gStringVar1, gText_Winona);
            break;
        case 4:
            StringCopy(gStringVar1, gText_Phoebe);
            break;
        case 5:
            StringCopy(gStringVar1, gText_Glacia);
            break;
        default:
            StringCopy(gStringVar1, gText_Wallace);
            break;
    }
}
#endif //FREE_LINK_BATTLE_RECORDS

void UpdateTrainerFansAfterLinkBattle(void)
{
    if (VarGet(VAR_LILYCOVE_FAN_CLUB_STATE) == 2)
    {
        TryLoseFansFromPlayTimeAfterLinkBattle();
        if (gBattleOutcome == B_OUTCOME_WON)
            PlayerGainRandomTrainerFan();
        else
            PlayerLoseRandomTrainerFan();
    }
}

static bool8 DidPlayerGetFirstFans(void)
{
    return GET_TRAINER_FAN_CLUB_FLAG(FANCLUB_GOT_FIRST_FANS);
}

void SetPlayerGotFirstFans(void)
{
    SET_TRAINER_FAN_CLUB_FLAG(FANCLUB_GOT_FIRST_FANS);
}

// return value is always ignored
u8 Script_TryGainNewFanFromCounter(void)
{
    return TryGainNewFanFromCounter(gSpecialVar_0x8004);
}

void GetObjectPosition(u16* xPointer, u16* yPointer, u32 localId, u32 useTemplate)
{
    u32 objectId;
    struct ObjectEvent* objEvent;

    if (useTemplate)
    {
        const struct ObjectEventTemplate *objTemplate = FindObjectEventTemplateByLocalId(localId, gSaveBlock1Ptr->objectEventTemplates, gMapHeader.events->objectEventCount);
        *xPointer = objTemplate->x;
        *yPointer = objTemplate->y;
        return;
    }

    objectId = GetObjectEventIdByLocalId(localId);
    objEvent = &gObjectEvents[objectId];
    *xPointer = objEvent->currentCoords.x - 7;
    *yPointer = objEvent->currentCoords.y - 7;
}

bool32 CheckObjectAtXY(u32 x, u32 y)
{
    u32 i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (!gObjectEvents[i].active)
            continue;

        if (gObjectEvents[i].currentCoords.x != x)
            continue;

        if (gObjectEvents[i].currentCoords.y != y)
            continue;
        return TRUE;
    }
    return FALSE;
}

bool32 CheckPartyHasSpecies(u32 givenSpecies)
{
    u32 partyIndex;

    for (partyIndex = 0; partyIndex < CalculatePlayerPartyCount(); partyIndex++)
        if (GetMonData(&gPlayerParty[partyIndex], MON_DATA_SPECIES) == givenSpecies)
            return TRUE;

    return FALSE;
}

void UseBlankMessageToCancelPokemonPic(void)
{
    u8 t = EOS;
    AddTextPrinterParameterized(0, FONT_NORMAL, &t, 0, 1, 0, NULL);
    ScriptMenu_HidePokemonPic();
}

void EnterCode(void)
{
    DoNamingScreen(NAMING_SCREEN_CODE, gStringVar2, 0, 0, 0, CB2_ReturnToFieldContinueScript);
}

void GetCodeFeedback(void)
{
    static const u8 sText_SampleCode[] = _("SampleCode");
    if (!StringCompare(gStringVar2, sText_SampleCode))
        gSpecialVar_Result = 1;
    else
        gSpecialVar_Result = 0;
}
