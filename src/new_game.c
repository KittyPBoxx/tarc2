#include "global.h"
#include "new_game.h"
#include "random.h"
#include "pokemon.h"
#include "script.h"
#include "play_time.h"
#include "load_save.h"
#include "berry.h"
#include "rtc.h"
#include "event_data.h"
#include "text.h"
#include "overworld.h"
#include "item.h"
#include "save.h"
#include "main.h"
#include "item_menu.h"
#include "field_specials.h"
#include "constants/map_groups.h"
#include "constants/items.h"
#include "script_pokemon_util.h"
#include "constants/moves.h"
#include "field_effect.h"

extern const u8 EventScript_ResetAllMapFlags[];

static void InitialWarp(void);

EWRAM_DATA bool8 gDifferentSaveFile = FALSE;
EWRAM_DATA bool8 gEnableContestDebugging = FALSE;

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

u32 GetTrainerId(u8 *trainerId)
{
    return (trainerId[3] << 24) | (trainerId[2] << 16) | (trainerId[1] << 8) | (trainerId[0]);
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = (Random() << 16) | GetGeneratedTrainerIdLower();
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
}

static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_FAST;
    gSaveBlock2Ptr->optionsWindowFrameType = 0;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_STEREO;
    gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    gSaveBlock2Ptr->optionsButtonMode = OPTIONS_BUTTON_MODE_L_EQUALS_A;
}

static void InitialWarp(void)
{
    SetWarpDestination(MAP_GROUP(MAP_BRIDGE), MAP_NUM(MAP_BRIDGE), WARP_ID_NONE, -1, -1);
    WarpIntoMap();
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
}

void NewGameInitData(void)
{
    if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_CORRUPT)
        RtcReset();

    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ClearSav1();
    ClearSav3();
    gSaveBlock2Ptr->specialSaveWarpFlags = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    InitEventData();
    ResetGameStats();
    gPlayerPartyCount = 1;
    ZeroPlayerPartyMons();
    //ScriptGiveMon(SPECIES_DITTO, 5, ITEM_BOTTLE_CAP);

    u16 monData;
    CreateMon(&gPlayerParty[0], SPECIES_DITTO, 25, 0, FALSE, 0, OT_ID_PLAYER_ID, 0);
    monData = TRUE;
    SetMonData(&gPlayerParty[0], MON_DATA_ABILITY_NUM, &monData);
    monData = MOVE_SPLASH;
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &monData);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE2, &monData);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE3, &monData);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE4, &monData);
    monData = ITEM_BOTTLE_CAP;
    SetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM, &monData);

    ClearBag();
    ResetFanClub();
    InitialWarp();

    AddBagItem(ITEM_TERA_ORB, 1);

    RunScriptImmediately(EventScript_ResetAllMapFlags);
    gFieldCallback = FieldCB_FallWarpExit;
    RtcInitLocalTimeOffset(0, 0);
}