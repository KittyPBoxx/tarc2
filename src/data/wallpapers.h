enum {
    WALLPAPER_FOREST,
    WALLPAPER_CITY,
    WALLPAPER_DESERT,
    WALLPAPER_SAVANNA,
    WALLPAPER_CRAG,
    WALLPAPER_VOLCANO,
    WALLPAPER_SNOW,
    WALLPAPER_CAVE,
    WALLPAPER_BEACH,
    WALLPAPER_SEAFLOOR,
    WALLPAPER_RIVER,
    WALLPAPER_SKY,
    WALLPAPER_POLKADOT,
    WALLPAPER_POKECENTER,
    WALLPAPER_MACHINE,
    WALLPAPER_PLAIN,
    WALLPAPER_FRIENDS, // The one received as a gift from Walda's parents.
    WALLPAPER_COUNT
};
#define MAX_DEFAULT_WALLPAPER WALLPAPER_SAVANNA

static const u16 sWallpaperPalettes_Forest[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/forest/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/forest/bg.gbapal"),
};
static const u32 sWallpaperTiles_Forest[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/forest/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Forest[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/forest/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_City[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/city/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/city/bg.gbapal"),
};
static const u32 sWallpaperTiles_City[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/city/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_City[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/city/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Desert[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/desert/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/desert/bg.gbapal"),
};
static const u32 sWallpaperTiles_Desert[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/desert/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Desert[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/desert/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Savanna[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/savanna/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/savanna/bg.gbapal"),
};
static const u32 sWallpaperTiles_Savanna[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/savanna/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Savanna[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/savanna/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Crag[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/crag/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/crag/bg.gbapal"),
};
static const u32 sWallpaperTiles_Crag[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/crag/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Crag[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/crag/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Volcano[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/volcano/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/volcano/bg.gbapal"),
};
static const u32 sWallpaperTiles_Volcano[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/volcano/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Volcano[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/volcano/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Snow[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/snow/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/snow/bg.gbapal"),
};
static const u32 sWallpaperTiles_Snow[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/snow/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Snow[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/snow/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Cave[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/cave/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/cave/bg.gbapal"),
};
static const u32 sWallpaperTiles_Cave[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/cave/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Cave[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/cave/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Beach[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/beach/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/beach/bg.gbapal"),
};
static const u32 sWallpaperTiles_Beach[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/beach/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Beach[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/beach/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Seafloor[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/seafloor/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/seafloor/bg.gbapal"),
};
static const u32 sWallpaperTiles_Seafloor[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/seafloor/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Seafloor[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/seafloor/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_River[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/river/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/river/bg.gbapal"),
};
static const u32 sWallpaperTiles_River[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/river/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_River[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/river/tilemap.bin.smolTM");
static const u16 sWallpaperPalettes_Sky[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/sky/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/sky/bg.gbapal"),
};
static const u32 sWallpaperTiles_Sky[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/sky/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Sky[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/sky/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_PolkaDot[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/polkadot/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/polkadot/bg.gbapal"),
};
static const u32 sWallpaperTiles_PolkaDot[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/polkadot/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_PolkaDot[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/polkadot/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Pokecenter[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/pokecenter/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/pokecenter/bg.gbapal"),
};
static const u32 sWallpaperTiles_Pokecenter[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/pokecenter/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Pokecenter[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/pokecenter/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Machine[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/machine/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/machine/bg.gbapal"),
};
static const u32 sWallpaperTiles_Machine[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/machine/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Machine[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/machine/tilemap.bin.smolTM");

static const u16 sWallpaperPalettes_Plain[][16] =
{
    INCBIN_U16("graphics/pokemon_storage/wallpapers/plain/frame.gbapal"),
    INCBIN_U16("graphics/pokemon_storage/wallpapers/plain/bg.gbapal"),
};
static const u32 sWallpaperTiles_Plain[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/plain/tiles.4bpp.smol");
static const u32 sWallpaperTilemap_Plain[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/plain/tilemap.bin.smolTM");

// 12x18 tilemap
static const u32 sWallpaperTilemap_Unused[] = INCBIN_U32("graphics/pokemon_storage/wallpapers/unused.bin");

// Shadow color, text color
static const u16 sBoxTitleColors[WALLPAPER_COUNT][2] =
{
    [WALLPAPER_FOREST]     = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_CITY]       = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_DESERT]     = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_SAVANNA]    = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_CRAG]       = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_VOLCANO]    = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_SNOW]       = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_CAVE]       = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_BEACH]      = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_SEAFLOOR]   = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_RIVER]      = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_SKY]        = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_POLKADOT]   = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_POKECENTER] = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_MACHINE]    = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_PLAIN]      = {RGB(7, 7, 7), RGB_WHITE},
    [WALLPAPER_FRIENDS]    = {RGB(7, 7, 7), RGB_WHITE}
};

#define WALLPAPER_ENTRY(name) {sWallpaperTiles_##name, sWallpaperTilemap_##name, sWallpaperPalettes_##name[0]}
// A few wallpapers are not defined in this file
#define WALLPAPER_ENTRY_G(name) {gWallpaperTiles_##name, gWallpaperTilemap_##name, gWallpaperPalettes_##name[0]}

static const struct Wallpaper sWallpapers[WALLPAPER_COUNT - 1] =
{
    [WALLPAPER_FOREST]     = WALLPAPER_ENTRY(Forest),
    [WALLPAPER_CITY]       = WALLPAPER_ENTRY(City),
    [WALLPAPER_DESERT]     = WALLPAPER_ENTRY(Desert),
    [WALLPAPER_SAVANNA]    = WALLPAPER_ENTRY(Savanna),
    [WALLPAPER_CRAG]       = WALLPAPER_ENTRY(Crag),
    [WALLPAPER_VOLCANO]    = WALLPAPER_ENTRY(Volcano),
    [WALLPAPER_SNOW]       = WALLPAPER_ENTRY(Snow),
    [WALLPAPER_CAVE]       = WALLPAPER_ENTRY(Cave),
    [WALLPAPER_BEACH]      = WALLPAPER_ENTRY(Beach),
    [WALLPAPER_SEAFLOOR]   = WALLPAPER_ENTRY(Seafloor),
    [WALLPAPER_RIVER]      = WALLPAPER_ENTRY(River),
    [WALLPAPER_SKY]        = WALLPAPER_ENTRY(Sky),
    [WALLPAPER_POLKADOT]   = WALLPAPER_ENTRY(PolkaDot),
    [WALLPAPER_POKECENTER] = WALLPAPER_ENTRY(Pokecenter),
    [WALLPAPER_MACHINE]    = WALLPAPER_ENTRY(Machine),
    [WALLPAPER_PLAIN]      = WALLPAPER_ENTRY(Plain),
};

static const u8 sArrow_Gfx[] = INCBIN_U8("graphics/pokemon_storage/arrow.4bpp");