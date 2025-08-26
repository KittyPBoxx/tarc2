#include "global.h"
#include "event_data.h"
#include "heal_location.h"
#include "constants/heal_locations.h"
#include "constants/maps.h"

#include "data/heal_locations.h"
#include "data/heal_locations_pkm_center.h"

u32 GetHealLocationIndexByMap(u16 mapGroup, u16 mapNum)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sHealLocations); i++)
    {
        if (sHealLocations[i].mapGroup == mapGroup && sHealLocations[i].mapNum == mapNum)
            return i + 1;
    }
    return HEAL_LOCATION_NONE;
}

const struct HealLocation *GetHealLocationByMap(u16 mapGroup, u16 mapNum)
{
    u32 index = GetHealLocationIndexByMap(mapGroup, mapNum);

    if (index == HEAL_LOCATION_NONE)
        return NULL;
    else
        return &sHealLocations[index - 1];
}

u32 GetHealLocationIndexByWarpData(struct WarpData *warp)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sHealLocations); i++)
    {
        if (sHealLocations[i].mapGroup == warp->mapGroup
        && sHealLocations[i].mapNum == warp->mapNum
        && sHealLocations[i].x == warp->x
        && sHealLocations[i].y == warp->y)
            return i + 1;
    }
    return HEAL_LOCATION_NONE;
}

const struct HealLocation *GetHealLocation(u32 index)
{
    if (index == HEAL_LOCATION_NONE)
        return NULL;
    else if (index > ARRAY_COUNT(sHealLocations))
        return NULL;
    else
        return &sHealLocations[index - 1];
}

u32 GetHealNpcLocalId(u32 healLocationId)
{
    if (healLocationId == HEAL_LOCATION_NONE || healLocationId >= NUM_HEAL_LOCATIONS)
        return 0;

    return sHealNpcLocalId[healLocationId - 1];
}

void SetWhiteoutRespawnWarpAndHealerNPC(struct WarpData *warp)
{
    u32 healLocationId = GetHealLocationIndexByWarpData(&gSaveBlock1Ptr->continueGameWarp);
    u32 healNpcLocalId = GetHealNpcLocalId(healLocationId);
    struct HealLocation pkmCenterHealLocation;

    pkmCenterHealLocation = sHealLocationsPokemonCenter[healLocationId - 1];
    warp->mapGroup = pkmCenterHealLocation.mapGroup;
    warp->mapNum = pkmCenterHealLocation.mapNum;
    warp->warpId = WARP_ID_NONE;
    warp->x = pkmCenterHealLocation.x;
    warp->y = pkmCenterHealLocation.y;
    gSpecialVar_LastTalked = healNpcLocalId;
}
