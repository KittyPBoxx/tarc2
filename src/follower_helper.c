#include "global.h"
#include "data.h"
#include "event_scripts.h"
#include "follower_helper.h"
#include "overworld.h"
#include "rtc.h"
#include "constants/battle.h"
#include "constants/followers.h"
#include "constants/metatile_behaviors.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/weather.h"

#define TYPE_NOT_TYPE1 NUMBER_OF_MON_TYPES

// difficult conditional messages follow
static const u8 sCondMsg00[] = _("{STR_VAR_1} danced happily.");
static const u8 sCondMsg01[] = _("{STR_VAR_1} danced beautifully.");
static const u8* const sCelebiTexts[] = {sCondMsg00, sCondMsg01, NULL};
static const u8 sCondMsg02[] = _("{STR_VAR_1} emitted fire and shouted.");
static const u8 sCondMsg03[] = _("{STR_VAR_1} is vigorously producing\nfire!");
static const u8 sCondMsg04[] = _("{STR_VAR_1} spat fire!");
static const u8 sCondMsg05[] = _("{STR_VAR_1} is vigorously breathing\nfire!");
static const u8* const sFireTexts[] = {sCondMsg02, sCondMsg03, sCondMsg04, sCondMsg05, NULL};
static const u8 sCondMsg06[] = _("{STR_VAR_1} is staring straight at\nthe POKéMON LEAGUE.");
static const u8 sCondMsg07[] = _("Your POKéMON is staring intently at\nthe mountain peak.");
static const u8 sCondMsg08[] = _("Sniff sniff, something smells good!");
static const u8 sCondMsg09[] = _("Your POKéMON is surveying the\nshelves restlessly.");
static const u8 sCondMsg10[] = _("{STR_VAR_1} is staring intently at\nthe shelves.");
static const u8* const sShopTexts[] = {sCondMsg09, sCondMsg10, NULL};
static const u8 sCondMsg11[] = _("{STR_VAR_1} focused with a sharp\ngaze!");
static const u8 sCondMsg12[] = _("{STR_VAR_1} seems very interested in\nthe bicycles.");
static const u8 sCondMsg13[] = _("{STR_VAR_1} seems to want to touch\nthe machines!");
static const u8 sCondMsg14[] = _("{STR_VAR_1} is swaying with the\nboat!");
static const u8 sCondMsg15[] = _("{STR_VAR_1} is dancing along with\nthe rolling of the ship.");
static const u8 sCondMsg16[] = _("{STR_VAR_1} doesn't want to get off\nthe boat yet!");
static const u8* const sBoatTexts[] = {sCondMsg14, sCondMsg15, sCondMsg16, NULL};
static const u8 sCondMsg17[] = _("{STR_VAR_1} is listening to the\nsound of the machines.");
static const u8* const sMachineTexts[] = {sCondMsg13, sCondMsg17, NULL};
static const u8 sCondMsg18[] = _("Waah! your POKéMON suddenly splashed\nwater!");
static const u8 sCondMsg19[] = _("Your POKéMON is blowing sand in the\nair!");
static const u8 sCondMsg20[] = _("{STR_VAR_1} is playing around,\nplucking bits of grass.");
static const u8 sCondMsg21[] = _("Your POKéMON is happily looking at\nyour footprints!");
static const u8 sCondMsg22[] = _("{STR_VAR_1} is feeling nervous and a\ntouch claustrophobic.");
static const u8 sCondMsg23[] = _("{STR_VAR_1} is cautious about the\nconfined area!");
static const u8* const sElevatorTexts[] = {sCondMsg22, sCondMsg23, NULL};
static const u8 sCondMsg24[] = _("A cold wind suddenly blew by!");
static const u8 sCondMsg25[] = _("Your POKéMON almost slipped and fell\nover!");
static const u8 sCondMsg26[] = _("Your POKéMON seems surprised to\ntouch ice.");
static const u8* const sColdTexts[] = {sCondMsg24, sCondMsg25, sCondMsg26, NULL};
static const u8 sCondMsg27[] = _("Your POKéMON has a flower petal on\nits face!");
static const u8 sCondMsg28[] = _("{STR_VAR_1} is growling softly.");
static const u8 sCondMsg29[] = _("{STR_VAR_1} is trembling with fear.");
static const u8 sCondMsg30[] = _("{STR_VAR_1} seems somehow sad…");
static const u8* const sFearTexts[] = {sCondMsg29, sCondMsg30, NULL};
static const u8 sCondMsg31[] = _("{STR_VAR_1} is taking shelter in the\ngrass from the rain.");
static const u8 sCondMsg32[] = _("{STR_VAR_1} seems very cold.");
static const u8 sCondMsg33[] = _("{STR_VAR_1} is staring at the sea.");
static const u8 sCondMsg34[] = _("Your POKéMON is staring intently at\nthe sea!");
static const u8 sCondMsg35[] = _("{STR_VAR_1} is looking at the\nsurging sea.");
static const u8* const sSeaTexts[] = {sCondMsg33, sCondMsg34, sCondMsg35, NULL};
static const u8 sCondMsg36[] = _("{STR_VAR_1} is listening to the\nsound of the waterfall.");
static const u8 sCondMsg37[] = _("{STR_VAR_1} seems to be happy about\nthe rain!");
static const u8 sCondMsg38[] = _("{STR_VAR_1} is staring at its\nreflection in the water.");
static const u8 sCondMsg39[] = _("{STR_VAR_1} seems to relax as it\nhears the sound of rustling leaves…");
static const u8 sCondMsg40[] = _("{STR_VAR_1} is gnawing at the ice.");
static const u8 sCondMsg41[] = _("{STR_VAR_1} is touching the ice.");
static const u8* const sIceTexts[] = {sCondMsg26, sCondMsg40, sCondMsg41, NULL};
static const u8 sCondMsg42[] = _("{STR_VAR_1}'s burn looks painful!");
static const u8 sCondMsg43[] = _("{STR_VAR_1} is happy to see what's\noutdoors!");
static const u8 sCondMsg44[] = _("{STR_VAR_1} is looking up at the\nsky.");
static const u8* const sDayTexts[] = {sCondMsg43, sCondMsg44, NULL};
static const u8 sCondMsg45[] = _("Your POKéMON is staring spellbound\nat the night sky!");
static const u8 sCondMsg46[] = _("Your POKéMON is happily gazing at\nthe beautiful, starry sky!");
static const u8* const sNightTexts[] = {sCondMsg45, sCondMsg46, NULL};


// Pool of "unconditional" follower messages
const struct FollowerMessagePool gFollowerBasicMessages[FOLLOWER_EMOTION_LENGTH] =
{
    [FOLLOWER_EMOTION_HAPPY]    = {gFollowerHappyMessages,    EventScript_FollowerGeneric, FOLLOWER_HAPPY_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_NEUTRAL]  = {gFollowerNeutralMessages,  EventScript_FollowerGeneric, FOLLOWER_NEUTRAL_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_SAD]      = {gFollowerSadMessages,      EventScript_FollowerGeneric, FOLLOWER_SAD_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_UPSET]    = {gFollowerUpsetMessages,    EventScript_FollowerGeneric, FOLLOWER_UPSET_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_ANGRY]    = {gFollowerAngryMessages,    EventScript_FollowerGeneric, FOLLOWER_ANGRY_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_PENSIVE]  = {gFollowerPensiveMessages,  EventScript_FollowerGeneric, FOLLOWER_PENSIVE_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_LOVE]     = {gFollowerLoveMessages,     EventScript_FollowerGeneric, FOLLOWER_LOVE_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_SURPRISE] = {gFollowerSurpriseMessages, EventScript_FollowerGeneric, FOLLOWER_SURPRISE_MESSAGE_COUNT},\
    [FOLLOWER_EMOTION_CURIOUS]  = {gFollowerCuriousMessages,  EventScript_FollowerGeneric, FOLLOWER_CURIOUS_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_MUSIC]    = {gFollowerMusicMessages,    EventScript_FollowerGeneric, FOLLOWER_MUSIC_MESSAGE_COUNT},
    [FOLLOWER_EMOTION_POISONED] = {gFollowerPoisonedMessages, EventScript_FollowerGeneric, FOLLOWER_POISONED_MESSAGE_COUNT},
};
