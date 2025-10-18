#include "palette_effects.h"
#include "global.h"
#include "constants/map_groups.h"
#include "constants/maps.h"
#include "gpu_regs.h"
#include "main.h"

// void VblankPaletteEffect_Example(void)
// {
//     // implement palette changes during VBlank
// }


void ResumeMapPaletteEffects(void)
{
    switch(gSaveBlock1Ptr->location.mapNum)
    {
        case MAP_NUM(MAP_BRIDGE):
            gMain.hblankPaletteEffect = PALETTE_EFFECT_GRASS;
            EnableInterrupts(INTR_FLAG_HBLANK | INTR_FLAG_VBLANK);
            break;
        case MAP_NUM(MAP_FOREST):
            gMain.vblankPaletteEffect = PALETTE_EFFECT_WATER;
            EnableInterrupts(INTR_FLAG_VBLANK);
            break;
        case MAP_NUM(MAP_MANOR):
        case MAP_NUM(MAP_CAVE_BOTTOM):
        case MAP_NUM(MAP_CAVE_TOP):
        case MAP_NUM(MAP_SUMMIT):
        case MAP_NUM(MAP_RETURN_PASS):
        default:

    }
}

void PauseMapPaletteEffects(void)
{
    gMain.hblankPaletteEffect = PALETTE_EFFECT_NONE;
    gMain.vblankPaletteEffect = PALETTE_EFFECT_NONE;
}