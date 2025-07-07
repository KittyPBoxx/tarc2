#include "global.h"
#include "battle.h"
#include "task.h"
#include "text.h"
#include "main.h"
#include "bg.h"
#include "palette.h"
#include "gpu_regs.h"
#include "malloc.h"
#include "menu.h"
#include "window.h"
#include "text_window.h"
#include "scanline_effect.h"
#include "overworld.h"
#include "strings.h"
#include "party_menu.h"
#include "battle_setup.h"
#include "union_room_battle.h"
#include "constants/rgb.h"
#include "constants/trainers.h"

struct UnionRoomBattle
{
    s16 textState;
};

static const struct BgTemplate sBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 3,
        .mapBaseIndex = 31
    }
};

static const struct WindowTemplate sWindowTemplates[] = {
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 15,
        .width = 24,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x014
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sTextColors[] = { TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY };

void CB2_UnionRoomBattle(void)
{
    // TODO: remove
}
