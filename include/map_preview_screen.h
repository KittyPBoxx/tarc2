#ifndef GUARD_MAP_PREVIEW_SCREEN_H
#define GUARD_MAP_PREVIEW_SCREEN_H

enum MapPreviewScreenId
{
    MPS_FOREST,
    MPS_BRIDGE,
    MPS_MANOR,
    MPS_CAVE,
    MPS_SUMMIT,
    MPS_COUNT
};

enum PreviewImageId
{
    IMG_FOREST,
    IMG_BRIDGE,
    IMG_MANOR,
    IMG_CAVE,
    IMG_SUMMIT,
    IMG_COUNT
};

// Map Preview Configs
#define MPS_DURATION_LONG       120 // This is the number of frames the map preview will last when the player enters the map for the first time.

#define MPS_DURATION_SHORT      40  // This is the number of frames the map preview will last if the player has visited the map before.

#define MPS_DURATION_NO_FLAG    40  // This is the number of frames the map preview will last if the map's flagId is MPS_FLAG_NULL.

#define MPS_DURATION_ALWAYS     0   // If this number is not 0, MPS_DURATION_ALWAYS will override all other duration values.
                                    // For example, if you set MPS_DURATION_ALWAYS to 100, the map preview will always
                                    // have a duration of 100 frames every time, regardless of the flag state.

#define MPS_DURATION_SCRIPT     120 // This is the number of frames the map preview will last when it is called
                                    // with the mappreview scripting macro.
                                    // It is unaffected by MPS_DURATION_ALWAYS.

#define MPS_BASIC_FADE_SPEED    1   // This sets the speed that the map preview fades out when MPS_TYPE_BASIC is used.
                                    // Numbers less than 1 will have a shorter fade time (negative numbers can be used).
                                    // Numbers greater than 1 will have a longer fade time.


// Constants
#define MPS_TYPE_BASIC      0
#define MPS_TYPE_FADE_IN    1
#define MPS_TYPE_CAVE       2
#define MPS_TYPE_ANY        3

#define MPS_FLAG_NULL       0

struct MapPreviewScreen
{
    u8 mapnum;
    u8 type;
    u16 flagId;
    u8 image;
};

struct ImageData
{
    const void *tilesptr;
    const void *tilemapptr;
    const void *palptr;
};

u16 MapPreview_CreateMapNameWindow(u8 id);
u16 MapPreview_GetDuration(u8 id);
bool8 MapHasPreviewScreen(u8 mapnum, u8 type);
bool32 ForestMapPreviewScreenIsRunning(void);
const struct MapPreviewScreen * GetDungeonMapPreviewScreenInfo(u8 mapnum);
bool32 MapHasPreviewScreen_HandleQLState2(u8 mapnum, u8 type);
void MapPreview_InitBgs(void);
void MapPreview_LoadGfx(u8 mapnum);
bool32 MapPreview_IsGfxLoadFinished(void);
void MapPreview_Unload(s32 windowId);
void MapPreview_StartForestTransition(u8 mapnum);

#endif //GUARD_MAP_PREVIEW_SCREEN_H
