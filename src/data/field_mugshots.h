static const u32 sFieldMugshotGfx_TestBerus[] = INCBIN_U32("graphics/field_mugshots/test/berus.4bpp.lz");
static const u32 sFieldMugshotGfx_TestRae[] = INCBIN_U32("graphics/field_mugshots/test/rae.4bpp.lz");
static const u32 sFieldMugshotGfx_TestTenjin[] = INCBIN_U32("graphics/field_mugshots/test/tenjin.4bpp.lz");
static const u32 sFieldMugshotGfx_TestExaminer[] = INCBIN_U32("graphics/field_mugshots/test/examiner.4bpp.lz");

static const u16 sFieldMugshotPal_TestBerus[] = INCBIN_U16("graphics/field_mugshots/test/berus.gbapal");
static const u16 sFieldMugshotPal_TestRae[] = INCBIN_U16("graphics/field_mugshots/test/rae.gbapal");
static const u16 sFieldMugshotPal_TestTenjin[] = INCBIN_U16("graphics/field_mugshots/test/tenjin.gbapal");
static const u16 sFieldMugshotPal_TestExaminer[] = INCBIN_U16("graphics/field_mugshots/test/examiner.gbapal");


struct MugshotGfx
{
    const u32 *gfx;
    const u16 *pal;
};

static const struct MugshotGfx sFieldMugshots[MUGSHOT_COUNT][EMOTE_COUNT] =
{
    [MUGSHOT_TEST] =
    {
        [EMOTE_BERUS] =
        {
            .gfx = sFieldMugshotGfx_TestBerus,
            .pal = sFieldMugshotPal_TestBerus,
        },

        [EMOTE_RAE] =
        {
            .gfx = sFieldMugshotGfx_TestRae,
            .pal = sFieldMugshotPal_TestRae,
        },

        [EMOTE_TENJIN] =
        {
            .gfx = sFieldMugshotGfx_TestTenjin,
            .pal = sFieldMugshotPal_TestTenjin,
        },
        [EMOTE_EXAMINER] =
        {
            .gfx = sFieldMugshotGfx_TestExaminer,
            .pal = sFieldMugshotPal_TestExaminer,
        },
    },
};