#include "global.h"
#include "main.h"
#include "credits.h"
#include "event_data.h"
#include "load_save.h"
#include "overworld.h"
#include "script_pokemon_util.h"
#include "constants/heal_locations.h"


extern void CB2_EndGameCallback(void);

int GameClear(void)
{
    int i;

    HealPlayerParty();

    if (FlagGet(FLAG_SYS_GAME_CLEAR) == TRUE)
    {
        gHasHallOfFameRecords = TRUE;
    }
    else
    {
        gHasHallOfFameRecords = FALSE;
        FlagSet(FLAG_SYS_GAME_CLEAR);
    }

    SetContinueGameWarpStatus();

    SetContinueGameWarpToHealLocation(HEAL_LOCATION_LITTLEROOT_TOWN);

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];

        if (GetMonData(mon, MON_DATA_SANITY_HAS_SPECIES)
         && !GetMonData(mon, MON_DATA_SANITY_IS_EGG)
         && !GetMonData(mon, MON_DATA_CHAMPION_RIBBON))
        {
            u8 val[1] = {TRUE};
            SetMonData(mon, MON_DATA_CHAMPION_RIBBON, val);
        }
    }

    SetMainCallback2(CB2_EndGameCallback); // TODO: this will probably need updating
    return 0;
}

bool8 SetCB2WhiteOut(void)
{
    SetMainCallback2(CB2_WhiteOut);
    return FALSE;
}
