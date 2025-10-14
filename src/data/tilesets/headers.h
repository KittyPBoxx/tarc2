#include "fieldmap.h"

// Whether a palette has a night version, located at ((x + 9) % 16).pal
#define SWAP_PAL(x) ((x) < NUM_PALS_IN_PRIMARY ? 1 << (x) : 1 << ((x) - NUM_PALS_IN_PRIMARY))

const struct Tileset gTileset_General =
{
    .isCompressed = TRUE,
    .isSecondary = FALSE,
    .tiles = gTilesetTiles_General,
    .palettes = gTilesetPalettes_General,
    .metatiles = gMetatiles_General,
    .metatileAttributes = gMetatileAttributes_General,
    .callback = InitTilesetAnim_General,
};

const struct Tileset gTileset_Petalburg =
{
    .isCompressed = TRUE,
    .isSecondary = TRUE,
    .tiles = gTilesetTiles_Petalburg,
    .palettes = gTilesetPalettes_Petalburg,
    .metatiles = gMetatiles_Petalburg,
    .metatileAttributes = gMetatileAttributes_Petalburg,
    .callback = InitTilesetAnim_Petalburg,
};
const struct Tileset gTileset_woodPrimary1 =
{
    .isCompressed = TRUE,
    .isSecondary = FALSE,
    .tiles = gTilesetTiles_woodPrimary1,
    .palettes = gTilesetPalettes_woodPrimary1,
    .metatiles = gMetatiles_woodPrimary1,
    .metatileAttributes = gMetatileAttributes_woodPrimary1,
    .callback = NULL,
};

const struct Tileset gTileset_manorPrimary1 =
{
    .isCompressed = TRUE,
    .isSecondary = FALSE,
    .tiles = gTilesetTiles_manorPrimary1,
    .palettes = gTilesetPalettes_manorPrimary1,
    .metatiles = gMetatiles_manorPrimary1,
    .metatileAttributes = gMetatileAttributes_manorPrimary1,
    .callback = InitTilesetAnim_ManorPrimary1,
};

const struct Tileset gTileset_forestPrimary1 =
{
    .isCompressed = TRUE,
    .isSecondary = FALSE,
    .tiles = gTilesetTiles_forestPrimary1,
    .palettes = gTilesetPalettes_forestPrimary1,
    .metatiles = gMetatiles_forestPrimary1,
    .metatileAttributes = gMetatileAttributes_forestPrimary1,
    .callback = InitTilesetAnim_ForestPrimary1,
};

const struct Tileset gTileset_summitPrimary1 =
{
    .isCompressed = TRUE,
    .isSecondary = FALSE,
    .tiles = gTilesetTiles_summitPrimary1,
    .palettes = gTilesetPalettes_summitPrimary1,
    .metatiles = gMetatiles_summitPrimary1,
    .metatileAttributes = gMetatileAttributes_summitPrimary1,
    .callback = NULL,
};

const struct Tileset gTileset_cavePrimary1 =
{
    .isCompressed = TRUE,
    .isSecondary = FALSE,
    .tiles = gTilesetTiles_cavePrimary1,
    .palettes = gTilesetPalettes_cavePrimary1,
    .metatiles = gMetatiles_cavePrimary1,
    .metatileAttributes = gMetatileAttributes_cavePrimary1,
    .callback = NULL,
};

