#include "global.h"
#include "battle_setup.h"
#include "coord_event_weather.h"
#include "debug.h"
#include "faraway_island.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_control_avatar.h"
#include "field_message_box.h"
#include "field_player_avatar.h"
#include "field_poison.h"
#include "field_screen_effect.h"
#include "field_specials.h"
#include "fldeff_misc.h"
#include "item_menu.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "pokemon.h"
#include "script.h"
#include "sound.h"
#include "start_menu.h"
#include "trainer_see.h"
#include "wild_encounter.h"
#include "constants/event_bg.h"
#include "constants/event_objects.h"
#include "constants/field_poison.h"
#include "constants/map_types.h"
#include "constants/metatile_behaviors.h"
#include "constants/songs.h"
#include "constants/game_stat.h"
#include "map_preview_screen.h"

static EWRAM_DATA u8 sWildEncounterImmunitySteps = 0;
static EWRAM_DATA u16 sPrevMetatileBehavior = 0;
static EWRAM_DATA u8 sCurrentDirection = 0;
static EWRAM_DATA u8 sPreviousDirection = 0;

COMMON_DATA u8 gSelectedObjectEvent = 0;

static void GetPlayerPosition(struct MapPosition *);
static void GetInFrontOfPlayerPosition(struct MapPosition *);
static u16 GetPlayerCurMetatileBehavior(int);
static bool8 TryStartInteractionScript(struct MapPosition *, u16, u8);
static const u8 *GetInteractionScript(struct MapPosition *, u8, u8);
static const u8 *GetInteractedObjectEventScript(struct MapPosition *, u8, u8);
static const u8 *GetInteractedBackgroundEventScript(struct MapPosition *, u8, u8);
static const u8 *GetInteractedMetatileScript(struct MapPosition *, u8, u8);
static const u8 *GetInteractedWaterScript(struct MapPosition *, u8, u8);
static bool8 TryStartStepBasedScript(struct MapPosition *, u16, u16);
static bool8 CheckStandardWildEncounter(u16);
static bool8 TryArrowWarp(struct MapPosition *, u16, u8);
static bool8 IsWarpMetatileBehavior(u16);
static bool8 IsArrowWarpMetatileBehavior(u16, u8);
static s8 GetWarpEventAtMapPosition(struct MapHeader *, struct MapPosition *);
static void SetupWarp(struct MapHeader *, s8, struct MapPosition *);
static bool8 TryDoorWarp(struct MapPosition *, u16, u8);
static s8 GetWarpEventAtPosition(struct MapHeader *, u16, u16, u8);
static const u8 *GetCoordEventScriptAtPosition(struct MapHeader *, u16, u16, u8);
static const struct BgEvent *GetBackgroundEventAtPosition(struct MapHeader *, u16, u16, u8);
static bool8 TryStartCoordEventScript(struct MapPosition *);
static bool8 TryStartWarpEventScript(struct MapPosition *, u16);
static bool8 TryStartMiscWalkingScripts(u16);
static bool8 TryStartStepCountScript(u16);
static void UpdateFriendshipStepCounter(void);
static void UpdateFollowerStepCounter(void);
#if OW_POISON_DAMAGE < GEN_5
static bool8 UpdatePoisonStepCounter(void);
#endif // OW_POISON_DAMAGE
static bool32 TrySetUpWalkIntoSignpostScript(struct MapPosition * position, u32 metatileBehavior, u32 playerDirection);
static void SetMsgSignPostAndVarFacing(u32 playerDirection);
static void SetUpWalkIntoSignScript(const u8 *script, u32 playerDirection);
static u32 GetFacingSignpostType(u16 metatileBehvaior, u32 direction);
static const u8 *GetSignpostScriptAtMapPosition(struct MapPosition * position);

static u8 GetDirectionFromBitfield(u8 bitfield)
{
    u8 direction = 0;
    while (bitfield >>= 1) direction++;
    return direction;
}

void FieldClearPlayerInput(struct FieldInput *input)
{
    input->pressedAButton = FALSE;
    input->checkStandardWildEncounter = FALSE;
    input->pressedStartButton = FALSE;
    input->pressedSelectButton = FALSE;
    input->heldDirection = FALSE;
    input->heldDirection2 = FALSE;
    input->tookStep = FALSE;
    input->pressedBButton = FALSE;
    input->pressedRButton = FALSE;
    input->input_field_1_1 = FALSE;
    input->input_field_1_2 = FALSE;
    input->input_field_1_3 = FALSE;
    input->dpadDirection = 0;
}

static void SetDirectionFromHeldKeys(u16 heldKeys)
{
    u8 dpadDirections = 0;

    if (heldKeys & DPAD_UP)
        dpadDirections |= (1 << DIR_NORTH);
    if (heldKeys & DPAD_DOWN)
        dpadDirections |= (1 << DIR_SOUTH);
    if (heldKeys & DPAD_LEFT)
        dpadDirections |= (1 << DIR_WEST);
    if (heldKeys & DPAD_RIGHT)
        dpadDirections |= (1 << DIR_EAST);

    if (dpadDirections == 0) // no dir is pushed
    {
        sCurrentDirection = DIR_NONE;
        sPreviousDirection = DIR_NONE;
        return;
    }

    if ((dpadDirections & (dpadDirections - 1)) == 0) // only 1 dir is pushed
    {
        // simply set currDir to that dir
        sCurrentDirection = GetDirectionFromBitfield(dpadDirections);
        sPreviousDirection = DIR_NONE;
        return;
    }

    // Check for diagonal movement
    if (dpadDirections & (1 << DIR_NORTH) && dpadDirections & (1 << DIR_EAST))
    {
        sCurrentDirection = DIR_NORTHEAST;
    }
    else if (dpadDirections & (1 << DIR_NORTH) && dpadDirections & (1 << DIR_WEST))
    {
        sCurrentDirection = DIR_NORTHWEST;
    }
    else if (dpadDirections & (1 << DIR_SOUTH) && dpadDirections & (1 << DIR_EAST))
    {
        sCurrentDirection = DIR_SOUTHEAST;
    }
    else if (dpadDirections & (1 << DIR_SOUTH) && dpadDirections & (1 << DIR_WEST))
    {
        sCurrentDirection = DIR_SOUTHWEST;
    }
    else if (((dpadDirections >> sCurrentDirection) & 1) == 0) // none of the multiple dirs pushed is currDir
    {
        sCurrentDirection = DIR_NONE;
        sPreviousDirection = DIR_NONE;
    }
    else if ((sPreviousDirection == DIR_NONE) || (((dpadDirections >> sPreviousDirection) & 1) == 0))
    {
        // turn
        sCurrentDirection = GetDirectionFromBitfield(dpadDirections & ~(1 << sCurrentDirection));
        sPreviousDirection = sCurrentDirection;
    }
    // else, currDir and prevDir are the dirs pushed
    // do nothing (keep the same currDir and prevDir)
}

void FieldGetPlayerInput(struct FieldInput *input, u16 newKeys, u16 heldKeys)
{
    u8 tileTransitionState = gPlayerAvatar.tileTransitionState;
    u8 runningState = gPlayerAvatar.runningState;
    bool8 forcedMove = MetatileBehavior_IsForcedMovementTile(GetPlayerCurMetatileBehavior(runningState));

    if ((tileTransitionState == T_TILE_CENTER && forcedMove == FALSE) || tileTransitionState == T_NOT_MOVING)
    {
        if (GetPlayerSpeed() != PLAYER_SPEED_FASTEST)
        {
            if (newKeys & START_BUTTON)
                input->pressedStartButton = TRUE;
            if (newKeys & SELECT_BUTTON)
                input->pressedSelectButton = TRUE;
            if (newKeys & A_BUTTON)
                input->pressedAButton = TRUE;
            if (newKeys & B_BUTTON)
                input->pressedBButton = TRUE;
            if (newKeys & R_BUTTON)
                input->pressedRButton = TRUE;
        }

        if (heldKeys & (DPAD_UP | DPAD_DOWN | DPAD_LEFT | DPAD_RIGHT))
        {
            input->heldDirection = TRUE;
            input->heldDirection2 = TRUE;
        }
    }

    if (forcedMove == FALSE)
    {
        if (tileTransitionState == T_TILE_CENTER && runningState == MOVING)
            input->tookStep = TRUE;
        if (forcedMove == FALSE && tileTransitionState == T_TILE_CENTER)
            input->checkStandardWildEncounter = TRUE;
    }

    SetDirectionFromHeldKeys(heldKeys);
    input->dpadDirection = sCurrentDirection;

    if(DEBUG_OVERWORLD_MENU && !DEBUG_OVERWORLD_IN_MENU)
    {
        if ((heldKeys & DEBUG_OVERWORLD_HELD_KEYS) && input->DEBUG_OVERWORLD_TRIGGER_EVENT)
        {
            input->input_field_1_2 = TRUE;
            input->DEBUG_OVERWORLD_TRIGGER_EVENT = FALSE;
        }
    }
}

int ProcessPlayerFieldInput(struct FieldInput *input)
{
    struct MapPosition position;
    u8 playerDirection, nonDiagonalPlayerDirection;
    u16 metatileBehavior;

    gSpecialVar_LastTalked = LOCALID_NONE;
    gSelectedObjectEvent = 0;

    gMsgIsSignPost = FALSE;
    playerDirection = GetPlayerFacingDirection();
    nonDiagonalPlayerDirection = GetNonDiagonalDirection(playerDirection);
    GetPlayerPosition(&position);
    metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);

    if (CheckForTrainersWantingBattle() == TRUE)
        return TRUE;

    if (TryRunOnFrameMapScript() == TRUE)
        return TRUE;

    if (input->tookStep)
    {
        IncrementGameStat(GAME_STAT_STEPS);
        IncrementBirthIslandRockStepCount();
        if (TryStartStepBasedScript(&position, metatileBehavior, playerDirection) == TRUE)
            return TRUE;
    }

    if ((input->checkStandardWildEncounter) && ((input->dpadDirection == 0) || input->dpadDirection == playerDirection))
    {
        GetInFrontOfPlayerPosition(&position);
        metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);
        if (TrySetUpWalkIntoSignpostScript(&position, metatileBehavior, playerDirection) == TRUE)
            return TRUE;
        GetPlayerPosition(&position);
        metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);
    }

    if (input->checkStandardWildEncounter && CheckStandardWildEncounter(metatileBehavior) == TRUE)
        return TRUE;
    if (input->heldDirection && input->dpadDirection == playerDirection)
    {
        if (TryArrowWarp(&position, metatileBehavior, nonDiagonalPlayerDirection) == TRUE)
            return TRUE;
    }

    GetInFrontOfPlayerPosition(&position);
    metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);

    if (input->heldDirection && (input->dpadDirection == playerDirection) && (TrySetUpWalkIntoSignpostScript(&position, metatileBehavior, nonDiagonalPlayerDirection) == TRUE))
        return TRUE;

    if (input->pressedAButton && TryStartInteractionScript(&position, metatileBehavior, playerDirection) == TRUE)
        return TRUE;

    if (input->heldDirection2 && input->dpadDirection == playerDirection)
    {
        if (TryDoorWarp(&position, metatileBehavior, nonDiagonalPlayerDirection) == TRUE)
            return TRUE;
    }

    // TODO: TARC start should close the preview
    if (input->pressedStartButton && !ForestMapPreviewScreenIsRunning())
    {
        PlaySE(SE_WIN_OPEN);
        ShowStartMenu();
        return TRUE;
    }

    if (input->pressedSelectButton && UseRegisteredKeyItemOnField() == TRUE)
        return TRUE;


    if(input->input_field_1_2 && DEBUG_OVERWORLD_MENU && !DEBUG_OVERWORLD_IN_MENU)
    {
        PlaySE(SE_WIN_OPEN);
        FreezeObjectEvents();
        Debug_ShowMainMenu();
        return TRUE;
    }

    if (CanTriggerSpinEvolution())
    {
        ResetSpinTimer();
        TrySpecialOverworldEvo(); // Special vars set in CanTriggerSpinEvolution.
        return TRUE;
    }

    return FALSE;
}

static void GetPlayerPosition(struct MapPosition *position)
{
    PlayerGetDestCoords(&position->x, &position->y);
    position->elevation = PlayerGetElevation();
}

static void GetInFrontOfPlayerPosition(struct MapPosition *position)
{
    s16 x, y;

    GetXYCoordsOneStepInFrontOfPlayer(&position->x, &position->y);
    PlayerGetDestCoords(&x, &y);
    if (MapGridGetElevationAt(x, y) != 0)
        position->elevation = PlayerGetElevation();
    else
        position->elevation = 0;
}

static u16 GetPlayerCurMetatileBehavior(int runningState)
{
    s16 x, y;

    PlayerGetDestCoords(&x, &y);
    return MapGridGetMetatileBehaviorAt(x, y);
}

static bool8 TryStartInteractionScript(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    const u8 *script = GetInteractionScript(position, metatileBehavior, direction);

    if (script == EventScript_Script_WildEncounter)
    {
        if (gSelectedObjectEvent != OBJECT_EVENTS_COUNT && gObjectEvents[gSelectedObjectEvent].graphicsId > OBJ_EVENT_GFX_SPECIES(NONE) && gObjectEvents[gSelectedObjectEvent].graphicsId < OBJ_EVENT_GFX_SPECIES(EGG))
        {
            PlayCry_Script(gObjectEvents[gSelectedObjectEvent].graphicsId - OBJ_EVENT_MON, 2);
            CreateWildMon(gObjectEvents[gSelectedObjectEvent].graphicsId - OBJ_EVENT_MON, min(100, max(1, gObjectEvents[gSelectedObjectEvent].trainerRange_berryTreeId)));  
            LockPlayerFieldControls();
            FreezeObjectEvents();
            BattleSetup_StartWildBattle();
            return FALSE;
        }
    }

    if (script == NULL || Script_HasNoEffect(script) || script == EventScript_Script_WildEncounter)
        return FALSE;

    PlaySE(SE_SELECT);

    ScriptContext_SetupScript(script);
    return TRUE;
}

static const u8 sVisionDirections[][4] = 
{
    [DIR_NORTH]      = { DIR_NORTHEAST, DIR_NORTHWEST, DIR_EAST, DIR_WEST },
    [DIR_SOUTH]      = { DIR_SOUTHEAST, DIR_SOUTHWEST, DIR_EAST, DIR_WEST },
    [DIR_EAST]       = { DIR_NORTHEAST, DIR_SOUTHEAST, DIR_NORTH, DIR_SOUTH },
    [DIR_WEST]       = { DIR_NORTHWEST, DIR_SOUTHWEST, DIR_NORTH, DIR_SOUTH },
    [DIR_NORTHEAST]  = { DIR_NORTH, DIR_EAST, DIR_NORTHWEST, DIR_SOUTHEAST },
    [DIR_NORTHWEST]  = { DIR_NORTH, DIR_WEST, DIR_NORTHEAST, DIR_SOUTHWEST },
    [DIR_SOUTHEAST]  = { DIR_SOUTH, DIR_EAST, DIR_SOUTHWEST, DIR_NORTHEAST },
    [DIR_SOUTHWEST]  = { DIR_SOUTH, DIR_WEST, DIR_SOUTHEAST, DIR_NORTHWEST },
};

static const u8 *GetInteractionScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    struct MapPosition playerPos;
    struct MapPosition tryInteractPos;
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    // Try the tile directly in front (original behavior)
    const u8 *script = GetInteractedObjectEventScript(position, metatileBehavior, direction);
    if (script != NULL)
    {
        ObjectEventTurn(playerObjEvent, direction);
        return script;
    }

    script = GetInteractedBackgroundEventScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedMetatileScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedWaterScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    GetPlayerPosition(&playerPos);

    for (int i = 0; i < 4; i++)
    {
        u8 testDir = sVisionDirections[direction][i];
        tryInteractPos = playerPos;
        MoveCoords(testDir, &tryInteractPos.x, &tryInteractPos.y);

        u8 behavior = MapGridGetMetatileBehaviorAt(tryInteractPos.x, tryInteractPos.y);

        script = GetInteractedObjectEventScript(&tryInteractPos, behavior, testDir);
        if (script != NULL)
        {
            ObjectEventTurn(playerObjEvent, testDir);
            return script;
        }

        script = GetInteractedBackgroundEventScript(&tryInteractPos, behavior, testDir);
        if (script != NULL)
        {
            ObjectEventTurn(playerObjEvent, testDir);
            return script;
        }

        script = GetInteractedMetatileScript(&tryInteractPos, behavior, testDir);
        if (script != NULL)
        {
            ObjectEventTurn(playerObjEvent, testDir);
            return script;
        }

        script = GetInteractedWaterScript(&tryInteractPos, behavior, testDir);
        if (script != NULL)
        {
            ObjectEventTurn(playerObjEvent, testDir);
            return script;
        }
    }

    return NULL;
}


static const u8 *GetInteractedObjectEventScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    u8 objectEventId;
    const u8 *script;
    s16 currX = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x;
    s16 currY = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y;
    u8 currBehavior = MapGridGetMetatileBehaviorAt(currX, currY);

    switch (direction)
    {
    case DIR_EAST:
        if (MetatileBehavior_IsSidewaysStairsLeftSideAny(metatileBehavior))
            // sideways stairs left-side to your right -> check northeast
            objectEventId = GetObjectEventIdByPosition(currX + 1, currY - 1, position->elevation);
        else if (MetatileBehavior_IsSidewaysStairsRightSideAny(currBehavior))
            // on top of right-side stairs -> check southeast
            objectEventId = GetObjectEventIdByPosition(currX + 1, currY + 1, position->elevation);
        else
            // check in front of player
            objectEventId = GetObjectEventIdByPosition(position->x, position->y, position->elevation);
        break;
    case DIR_WEST:
        if (MetatileBehavior_IsSidewaysStairsRightSideAny(metatileBehavior))
            // facing sideways stairs right side -> check northwest
            objectEventId = GetObjectEventIdByPosition(currX - 1, currY - 1, position->elevation);
        else if (MetatileBehavior_IsSidewaysStairsLeftSideAny(currBehavior))
            // on top of left-side stairs -> check southwest
            objectEventId = GetObjectEventIdByPosition(currX - 1, currY + 1, position->elevation);
        else
            // check in front of player
            objectEventId = GetObjectEventIdByPosition(position->x, position->y, position->elevation);
        break;
    default:
        objectEventId = GetObjectEventIdByPosition(position->x, position->y, position->elevation);
        break;
    }

    if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == LOCALID_PLAYER)
    {
        if (MetatileBehavior_IsCounter(metatileBehavior) != TRUE)
            return NULL;

        // Look for an object event on the other side of the counter.
        objectEventId = GetObjectEventIdByPosition(position->x + gDirectionToVectors[direction].x, position->y + gDirectionToVectors[direction].y, position->elevation);
        if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == LOCALID_PLAYER)
            return NULL;
    }

    gSelectedObjectEvent = objectEventId;
    gSpecialVar_LastTalked = gObjectEvents[objectEventId].localId;
    gSpecialVar_Facing = direction;

    script = GetObjectEventScriptPointerByObjectEventId(objectEventId);

    return script;
}

static const u8 *GetInteractedBackgroundEventScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    // TODO: remove
    return NULL;
}

static const u8 *GetInteractedMetatileScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    if (MetatileBehavior_IsPC(metatileBehavior) == TRUE)
        return EventScript_PC;
    if (MetatileBehavior_IsPokeMartSign(metatileBehavior) == TRUE)
    {
        if(direction != DIR_NORTH)
            return NULL;
        SetMsgSignPostAndVarFacing(direction);
        return Common_EventScript_ShowPokemartSign;
    }
    if (MetatileBehavior_IsPokemonCenterSign(metatileBehavior) == TRUE)
    {
        if(direction != DIR_NORTH)
            return NULL;
        SetMsgSignPostAndVarFacing(direction);
        return Common_EventScript_ShowPokemonCenterSign;
    }

    return NULL;
}

static const u8 *GetInteractedWaterScript(struct MapPosition *unused1, u8 metatileBehavior, u8 direction)
{
    if (FlagGet(FLAG_BADGE05_GET) == TRUE && PartyHasMonWithSurf() == TRUE && IsPlayerFacingSurfableFishableWater() == TRUE)
        return EventScript_UseSurf;

    if (MetatileBehavior_IsWaterfall(metatileBehavior) == TRUE)
    {
        if (FlagGet(FLAG_BADGE08_GET) == TRUE && IsPlayerSurfingNorth() == TRUE)
            return EventScript_UseWaterfall;
        else
            return EventScript_CannotUseWaterfall;
    }
    return NULL;
}

static bool8 TryStartStepBasedScript(struct MapPosition *position, u16 metatileBehavior, u16 direction)
{
    if (TryStartCoordEventScript(position) == TRUE)
        return TRUE;
    if (TryStartWarpEventScript(position, metatileBehavior) == TRUE)
        return TRUE;
    if (TryStartMiscWalkingScripts(metatileBehavior) == TRUE)
        return TRUE;
    if (TryStartStepCountScript(metatileBehavior) == TRUE)
        return TRUE;
    if (UpdateRepelCounter() == TRUE)
        return TRUE;
    return FALSE;
}

static bool8 TryStartCoordEventScript(struct MapPosition *position)
{
    const u8 *script = GetCoordEventScriptAtPosition(&gMapHeader, position->x - MAP_OFFSET, position->y - MAP_OFFSET, position->elevation);

    if (script == NULL)
        return FALSE;

    struct ScriptContext ctx;
    if (!RunScriptImmediatelyUntilEffect(SCREFF_V1 | SCREFF_HARDWARE, script, &ctx))
        return FALSE;

    ScriptContext_ContinueScript(&ctx);
    return TRUE;
}

static bool8 TryStartMiscWalkingScripts(u16 metatileBehavior)
{
    s16 x, y;

    if (MetatileBehavior_IsCrackedFloorHole(metatileBehavior))
    {
        ScriptContext_SetupScript(EventScript_FallDownHole);
        return TRUE;
    }
    else if (MetatileBehavior_IsSecretBaseGlitterMat(metatileBehavior) == TRUE)
    {
        DoSecretBaseGlitterMatSparkle();
        return FALSE;
    }
    else if (MetatileBehavior_IsSecretBaseSoundMat(metatileBehavior) == TRUE)
    {
        PlayerGetDestCoords(&x, &y);
        PlaySecretBaseMusicNoteMatSound(MapGridGetMetatileIdAt(x, y));
        return FALSE;
    }
    return FALSE;
}

static bool8 TryStartStepCountScript(u16 metatileBehavior)
{
    IncrementRematchStepCounter();
    UpdateFriendshipStepCounter();
    UpdateFarawayIslandStepCounter();
    UpdateFollowerStepCounter();

    if (!(gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_FORCED_MOVE) && !MetatileBehavior_IsForcedMovementTile(metatileBehavior))
    {
    #if OW_POISON_DAMAGE < GEN_5
        if (UpdatePoisonStepCounter() == TRUE)
        {
            ScriptContext_SetupScript(EventScript_FieldPoison);
            return TRUE;
        }
    #endif
    }

    return FALSE;
}

static void UpdateFriendshipStepCounter(void)
{
    u16 *ptr = GetVarPointer(VAR_FRIENDSHIP_STEP_COUNTER);
    int i;

    (*ptr)++;
    (*ptr) %= 128;
    if (*ptr == 0)
    {
        struct Pokemon *mon = gPlayerParty;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            AdjustFriendship(mon, FRIENDSHIP_EVENT_WALKING);
            mon++;
        }
    }
}

static void UpdateFollowerStepCounter(void)
{
    if (gPlayerPartyCount > 0 && gFollowerSteps < (u16)-1)
        gFollowerSteps++;
}

void ClearPoisonStepCounter(void)
{
    VarSet(VAR_POISON_STEP_COUNTER, 0);
}

#if OW_POISON_DAMAGE < GEN_5
static bool8 UpdatePoisonStepCounter(void)
{
    u16 *ptr;

    if (gMapHeader.mapType != MAP_TYPE_SECRET_BASE)
    {
        ptr = GetVarPointer(VAR_POISON_STEP_COUNTER);
        (*ptr)++;
        (*ptr) %= 4;
        if (*ptr == 0)
        {
            switch (DoPoisonFieldEffect())
            {
            case FLDPSN_NONE:
                return FALSE;
            case FLDPSN_PSN:
                return FALSE;
            case FLDPSN_FNT:
                return TRUE;
            }
        }
    }
    return FALSE;
}
#endif // OW_POISON_DAMAGE

void RestartWildEncounterImmunitySteps(void)
{
    // Starts at 0 and counts up to 4 steps.
    sWildEncounterImmunitySteps = 0;
}

static bool8 CheckStandardWildEncounter(u16 metatileBehavior)
{
    if (FlagGet(OW_FLAG_NO_ENCOUNTER))
        return FALSE;

    if (sWildEncounterImmunitySteps < 4)
    {
        sWildEncounterImmunitySteps++;
        sPrevMetatileBehavior = metatileBehavior;
        return FALSE;
    }

    if (StandardWildEncounter(metatileBehavior, sPrevMetatileBehavior) == TRUE)
    {
        sWildEncounterImmunitySteps = 0;
        sPrevMetatileBehavior = metatileBehavior;
        return TRUE;
    }

    sPrevMetatileBehavior = metatileBehavior;
    return FALSE;
}

static void StorePlayerStateAndSetupWarp(struct MapPosition *position, s32 warpEventId)
{
    StoreInitialPlayerAvatarState();
    SetupWarp(&gMapHeader, warpEventId, position);
}

static bool8 TryArrowWarp(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    s32 warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);
    u32 delay;

    if (warpEventId == WARP_ID_NONE)
        return FALSE;

    if (IsArrowWarpMetatileBehavior(metatileBehavior, direction) == TRUE)
    {
        StorePlayerStateAndSetupWarp(position, warpEventId);
        DoWarp();
        return TRUE;
    }
    else if (IsDirectionalStairWarpMetatileBehavior(metatileBehavior, direction) == TRUE)
    {
        delay = 0;
        if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_BIKE)
        {
            SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
            delay = 12;
        }

        StorePlayerStateAndSetupWarp(position, warpEventId);
        DoStairWarp(metatileBehavior, delay);
        return TRUE;
    }
    return FALSE;
}

static bool8 TryStartWarpEventScript(struct MapPosition *position, u16 metatileBehavior)
{
    s8 warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);

    if (warpEventId != WARP_ID_NONE && IsWarpMetatileBehavior(metatileBehavior) == TRUE)
    {
        StoreInitialPlayerAvatarState();
        SetupWarp(&gMapHeader, warpEventId, position);
        if (MetatileBehavior_IsEscalator(metatileBehavior) == TRUE)
        {
            DoEscalatorWarp(metatileBehavior);
            return TRUE;
        }
        if (MetatileBehavior_IsLavaridgeB1FWarp(metatileBehavior) == TRUE)
        {
            DoLavaridgeGymB1FWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsLavaridge1FWarp(metatileBehavior) == TRUE)
        {
            DoLavaridgeGym1FWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsAquaHideoutWarp(metatileBehavior) == TRUE)
        {
            DoTeleportTileWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsUnionRoomWarp(metatileBehavior) == TRUE)
        {
            DoSpinExitWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsMtPyreHole(metatileBehavior) == TRUE)
        {
            ScriptContext_SetupScript(EventScript_FallDownHoleMtPyre);
            return TRUE;
        }
        if (MetatileBehavior_IsMossdeepGymWarp(metatileBehavior) == TRUE)
        {
            DoMossdeepGymWarp();
            return TRUE;
        }
        DoWarp();
        return TRUE;
    }
    return FALSE;
}

static bool8 IsWarpMetatileBehavior(u16 metatileBehavior)
{
    if (MetatileBehavior_IsWarpDoor(metatileBehavior) != TRUE
     && MetatileBehavior_IsLadder(metatileBehavior) != TRUE
     && MetatileBehavior_IsEscalator(metatileBehavior) != TRUE
     && MetatileBehavior_IsNonAnimDoor(metatileBehavior) != TRUE
     && MetatileBehavior_IsLavaridgeB1FWarp(metatileBehavior) != TRUE
     && MetatileBehavior_IsLavaridge1FWarp(metatileBehavior) != TRUE
     && MetatileBehavior_IsAquaHideoutWarp(metatileBehavior) != TRUE
     && MetatileBehavior_IsMtPyreHole(metatileBehavior) != TRUE
     && MetatileBehavior_IsMossdeepGymWarp(metatileBehavior) != TRUE
     && MetatileBehavior_IsUnionRoomWarp(metatileBehavior) != TRUE)
        return FALSE;
    return TRUE;
}

static bool8 IsArrowWarpMetatileBehavior(u16 metatileBehavior, u8 direction)
{
    switch (direction)
    {
    case DIR_NORTH:
        return MetatileBehavior_IsNorthArrowWarp(metatileBehavior);
    case DIR_SOUTH:
        return MetatileBehavior_IsSouthArrowWarp(metatileBehavior);
    case DIR_WEST:
        return MetatileBehavior_IsWestArrowWarp(metatileBehavior);
    case DIR_EAST:
        return MetatileBehavior_IsEastArrowWarp(metatileBehavior);
    case DIR_NORTHWEST:
        return MetatileBehavior_IsNorthwestArrowWarp(metatileBehavior);
    case DIR_NORTHEAST:
        return MetatileBehavior_IsNortheastArrowWarp(metatileBehavior);
    case DIR_SOUTHWEST:
        return MetatileBehavior_IsSouthwestArrowWarp(metatileBehavior);
    case DIR_SOUTHEAST:
        return MetatileBehavior_IsSoutheastArrowWarp(metatileBehavior);
    }
    return FALSE;
}

static s8 GetWarpEventAtMapPosition(struct MapHeader *mapHeader, struct MapPosition *position)
{
    return GetWarpEventAtPosition(mapHeader, position->x - MAP_OFFSET, position->y - MAP_OFFSET, position->elevation);
}

static void SetupWarp(struct MapHeader *unused, s8 warpEventId, struct MapPosition *position)
{
    const struct WarpEvent *warpEvent;

    warpEvent = &gMapHeader.events->warps[warpEventId];

    if (warpEvent->mapNum == MAP_NUM(MAP_DYNAMIC))
    {
        SetWarpDestinationToDynamicWarp(warpEvent->warpId);
    }
    else
    {
        const struct MapHeader *mapHeader;

        SetWarpDestinationToMapWarp(warpEvent->mapGroup, warpEvent->mapNum, warpEvent->warpId);
        UpdateEscapeWarp(position->x, position->y);
        mapHeader = Overworld_GetMapHeaderByGroupAndId(warpEvent->mapGroup, warpEvent->mapNum);
        if (mapHeader->events->warps[warpEvent->warpId].mapNum == MAP_NUM(MAP_DYNAMIC))
            SetDynamicWarp(mapHeader->events->warps[warpEventId].warpId, gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, warpEventId);
    }
}

static bool8 TryDoorWarp(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    s8 warpEventId;

    if (direction == DIR_NORTH)
    {

        if (MetatileBehavior_IsWarpDoor(metatileBehavior) == TRUE)
        {
            warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);
            if (warpEventId != WARP_ID_NONE && IsWarpMetatileBehavior(metatileBehavior) == TRUE)
            {
                StoreInitialPlayerAvatarState();
                SetupWarp(&gMapHeader, warpEventId, position);
                DoDoorWarp();
                return TRUE;
            }
        }
    }
    return FALSE;
}

static s8 GetWarpEventAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    s32 i;
    const struct WarpEvent *warpEvent = mapHeader->events->warps;
    u8 warpCount = mapHeader->events->warpCount;

    for (i = 0; i < warpCount; i++, warpEvent++)
    {
        if ((u16)warpEvent->x == x && (u16)warpEvent->y == y)
        {
            if (warpEvent->elevation == elevation || warpEvent->elevation == 0)
                return i;
        }
    }
    return WARP_ID_NONE;
}

static bool32 ShouldTriggerScriptRun(const struct CoordEvent *coordEvent)
{
    u16 *varPtr = GetVarPointer(coordEvent->trigger);
    // Treat non Vars as flags
    if (varPtr == NULL)
        return (FlagGet(coordEvent->trigger) == coordEvent->index);
    else
        return (*varPtr == coordEvent->index);
}

static const u8 *TryRunCoordEventScript(const struct CoordEvent *coordEvent)
{
    if (coordEvent != NULL)
    {
        if (coordEvent->script == NULL)
        {
            DoCoordEventWeather(coordEvent->trigger);
            return NULL;
        }
        if (coordEvent->trigger == TRIGGER_RUN_IMMEDIATELY)
        {
            RunScriptImmediately(coordEvent->script);
            return NULL;
        }
        if (ShouldTriggerScriptRun(coordEvent))
            return coordEvent->script;
    }
    return NULL;
}

static const u8 *GetCoordEventScriptAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    s32 i;
    const struct CoordEvent *coordEvents = mapHeader->events->coordEvents;
    u8 coordEventCount = mapHeader->events->coordEventCount;

    for (i = 0; i < coordEventCount; i++)
    {
        if ((u16)coordEvents[i].x == x && (u16)coordEvents[i].y == y)
        {
            if (coordEvents[i].elevation == elevation || coordEvents[i].elevation == 0)
            {
                const u8 *script = TryRunCoordEventScript(&coordEvents[i]);
                if (script != NULL)
                    return script;
            }
        }
    }
    return NULL;
}

const u8 *GetCoordEventScriptAtMapPosition(struct MapPosition *position)
{
    return GetCoordEventScriptAtPosition(&gMapHeader, position->x - MAP_OFFSET, position->y - MAP_OFFSET, position->elevation);
}

static const struct BgEvent *GetBackgroundEventAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    u8 i;
    const struct BgEvent *bgEvents = mapHeader->events->bgEvents;
    u8 bgEventCount = mapHeader->events->bgEventCount;

    for (i = 0; i < bgEventCount; i++)
    {
        if ((u16)bgEvents[i].x == x && (u16)bgEvents[i].y == y)
        {
            if (bgEvents[i].elevation == elevation || bgEvents[i].elevation == 0)
                return &bgEvents[i];
        }
    }
    return NULL;
}

bool8 TryDoDiveWarp(struct MapPosition *position, u16 metatileBehavior)
{
    if (gMapHeader.mapType == MAP_TYPE_UNDERWATER && !MetatileBehavior_IsUnableToEmerge(metatileBehavior))
    {
        if (SetDiveWarpEmerge(position->x - MAP_OFFSET, position->y - MAP_OFFSET))
        {
            StoreInitialPlayerAvatarState();
            DoDiveWarp();
            PlaySE(SE_M_DIVE);
            return TRUE;
        }
    }
    else if (MetatileBehavior_IsDiveable(metatileBehavior) == TRUE)
    {
        if (SetDiveWarpDive(position->x - MAP_OFFSET, position->y - MAP_OFFSET))
        {
            StoreInitialPlayerAvatarState();
            DoDiveWarp();
            PlaySE(SE_M_DIVE);
            return TRUE;
        }
    }
    return FALSE;
}

const u8 *GetObjectEventScriptPointerPlayerFacing(void)
{
    u8 direction;
    struct MapPosition position;

    direction = GetPlayerMovementDirection();
    GetInFrontOfPlayerPosition(&position);
    return GetInteractedObjectEventScript(&position, MapGridGetMetatileBehaviorAt(position.x, position.y), direction);
}

int SetCableClubWarp(void)
{
    struct MapPosition position;

    GetPlayerPosition(&position);
    SetupWarp(&gMapHeader, GetWarpEventAtMapPosition(&gMapHeader, &position), &position);
    return 0;
}

static bool32 TrySetUpWalkIntoSignpostScript(struct MapPosition *position, u32 metatileBehavior, u32 playerDirection)
{
    const u8 *script;

    if ((JOY_HELD(DPAD_LEFT | DPAD_RIGHT)) || (playerDirection != DIR_NORTH))
        return FALSE;

    switch (GetFacingSignpostType(metatileBehavior, playerDirection))
    {
    case MB_POKEMON_CENTER_SIGN:
        SetUpWalkIntoSignScript(Common_EventScript_ShowPokemonCenterSign, playerDirection);
        return TRUE;
    case MB_POKEMART_SIGN:
        SetUpWalkIntoSignScript(Common_EventScript_ShowPokemartSign, playerDirection);
        return TRUE;
    case MB_SIGNPOST:
        script = GetSignpostScriptAtMapPosition(position);
        if (script == NULL)
            return FALSE;
        SetUpWalkIntoSignScript(script, playerDirection);
        return TRUE;
    default:
        return FALSE;
    }
}

static u32 GetFacingSignpostType(u16 metatileBehavior, u32 playerDirection)
{
    if (MetatileBehavior_IsPokemonCenterSign(metatileBehavior) == TRUE)
        return MB_POKEMON_CENTER_SIGN;
    if (MetatileBehavior_IsPokeMartSign(metatileBehavior) == TRUE)
        return MB_POKEMART_SIGN;
    if (MetatileBehavior_IsSignpost(metatileBehavior) == TRUE)
        return MB_SIGNPOST;

    return NOT_SIGNPOST;
}

static void SetMsgSignPostAndVarFacing(u32 playerDirection)
{
    gWalkAwayFromSignpostTimer = WALK_AWAY_SIGNPOST_FRAMES;
    gMsgBoxIsCancelable = TRUE;
    gMsgIsSignPost = TRUE;
    gSpecialVar_Facing = playerDirection;
}

static void SetUpWalkIntoSignScript(const u8 *script, u32 playerDirection)
{
    ScriptContext_SetupScript(script);
    SetMsgSignPostAndVarFacing(playerDirection);
}

static const u8 *GetSignpostScriptAtMapPosition(struct MapPosition *position)
{
    const struct BgEvent *event = GetBackgroundEventAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->elevation);
    if (event == NULL)
        return NULL;
    if (event->bgUnion.script != NULL)
        return event->bgUnion.script;
    return EventScript_TestSignpostMsg;
}

static void Task_OpenStartMenu(u8 taskId)
{
    if (ArePlayerFieldControlsLocked())
        return;

    PlaySE(SE_WIN_OPEN);
    ShowStartMenu();
    DestroyTask(taskId);
}

bool32 IsDpadPushedToTurnOrMovePlayer(struct FieldInput *input)
{
    return (input->dpadDirection != 0 && GetPlayerFacingDirection() != input->dpadDirection);
}

void CancelSignPostMessageBox(struct FieldInput *input)
{
    if (!ScriptContext_IsEnabled())
        return;

    if (gWalkAwayFromSignpostTimer)
    {
        gWalkAwayFromSignpostTimer--;
        return;
    }

    if (!gMsgBoxIsCancelable)
        return;

    if (IsDpadPushedToTurnOrMovePlayer(input))
    {
        ScriptContext_SetupScript(EventScript_CancelMessageBox);
        LockPlayerFieldControls();
        return;
    }

    if (!input->pressedStartButton)
        return;

    ScriptContext_SetupScript(EventScript_CancelMessageBox);
    LockPlayerFieldControls();

    if (FuncIsActiveTask(Task_OpenStartMenu))
        return;

    CreateTask(Task_OpenStartMenu, 8);
}

s16 GetPlayerSpeed(void)
{
    if (gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_SURFING | PLAYER_AVATAR_FLAG_DASH))
        return PLAYER_SPEED_FAST;
    else
        return PLAYER_SPEED_NORMAL;
}

static bool8 IsRunningDisallowedByMetatile(u8 tile)
{
    if (MetatileBehavior_IsRunningDisallowed(tile))
        return TRUE;
    if (MetatileBehavior_IsFortreeBridge(tile) && (PlayerGetElevation() & 1) == 0)
        return TRUE;
    return FALSE;
}

bool32 IsRunningDisallowed(u8 metatile)
{
    if ((OW_RUNNING_INDOORS == GEN_3 && !gMapHeader.allowRunning) || IsRunningDisallowedByMetatile(metatile) == TRUE)
        return TRUE;

    return FALSE;
}