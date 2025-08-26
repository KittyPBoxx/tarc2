//
// DO NOT MODIFY THIS FILE! It is auto-generated from src/data/trainers.party
//
// If you want to modify this file set COMPETITIVE_PARTY_SYNTAX to FALSE
// in include/config/general.h and remove this notice.
// Use sed -i '/^#line/d' 'src/data/trainers.h' to remove #line markers.
//

#line 1 "src/data/trainers.party"

#line 76
[TRAINER_NONE] =
    {
#line 78
        .trainerClass = TRAINER_CLASS_PKMN_TRAINER_1,
#line 79
        .trainerPic = TRAINER_PIC_HIKER,
        .encounterMusic_gender =
#line 81
            TRAINER_ENCOUNTER_MUSIC_MALE,
#line 82
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
        .partySize = 0,
        .party = (const struct TrainerMon[])
        {
        },
    },
#line 86
[TRAINER_EXAMINER_FIONN] =
    {
#line 87
        .trainerName = _("Fionn"),
#line 88
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 89
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 91
            TRAINER_ENCOUNTER_MUSIC_HIKER,
#line 92
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 93
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 95
            .species = SPECIES_CHESPIN,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 98
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 97
            .ability = ABILITY_BULLETPROOF,
#line 96
            .lvl = 21,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 99
                MOVE_WORRY_SEED,
                MOVE_IMPRISON,
                MOVE_STOMP,
                MOVE_NONE,
            },
            },
            {
#line 104
            .species = SPECIES_CHANSEY,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 106
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 105
            .lvl = 21,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 107
                MOVE_FLING,
                MOVE_COVET,
                MOVE_MINIMIZE,
                MOVE_LOW_KICK,
            },
            },
        },
    },
#line 114
[TRAINER_EXAMINER_SORA] =
    {
#line 115
        .trainerName = _("Sora"),
#line 116
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 117
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 119
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 120
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 121
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 123
            .species = SPECIES_MR_MIME,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 126
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 125
            .ability = ABILITY_SOUNDPROOF,
#line 124
            .lvl = 32,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 127
                MOVE_DREAM_EATER,
                MOVE_YAWN,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 132
            .species = SPECIES_HAWLUCHA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 135
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 134
            .ability = ABILITY_MOLD_BREAKER,
#line 133
            .lvl = 32,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 136
                MOVE_SKY_DROP,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
#line 143
[TRAINER_EXAMINER_RUNE] =
    {
#line 144
        .trainerName = _("Rune"),
#line 145
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 146
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 148
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 149
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 150
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 152
            .species = SPECIES_SLOWBRO,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 152
            .heldItem = ITEM_KINGS_ROCK,
#line 154
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 153
            .lvl = 31,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 155
                MOVE_AQUA_JET,
                MOVE_CURSE,
                MOVE_SOAK,
                MOVE_AVALANCHE,
            },
            },
            {
#line 161
            .species = SPECIES_AMPHAROS_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 163
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 162
            .lvl = 31,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 164
                MOVE_ZAP_CANNON,
                MOVE_FOCUS_BLAST,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
#line 171
[TRAINER_EXAMINER_DANTE] =
    {
#line 172
        .trainerName = _("Dante"),
#line 173
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 174
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 176
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 177
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 178
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 180
            .species = SPECIES_STARMIE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 182
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 181
            .lvl = 32,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 183
                MOVE_PSYCHIC_TERRAIN,
                MOVE_SAFEGUARD,
                MOVE_HEAL_PULSE,
                MOVE_TOXIC,
            },
            },
            {
#line 188
            .species = SPECIES_ILLUMISE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 190
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 189
            .lvl = 32,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 191
                MOVE_WILL_O_WISP,
                MOVE_COUNTER,
                MOVE_LIGHT_SCREEN,
                MOVE_NONE,
            },
            },
            {
#line 196
            .species = SPECIES_HAUNTER,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 196
            .heldItem = ITEM_TOXIC_ORB,
#line 198
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 197
            .lvl = 32,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 199
                MOVE_FLING,
                MOVE_CURSE,
                MOVE_NONE,
            },
            },
        },
    },
#line 205
[TRAINER_DEITY_TENJIN] =
    {
#line 206
        .trainerName = _("GRUNT"),
#line 207
        .trainerClass = TRAINER_CLASS_EXPERT,
#line 208
        .trainerPic = TRAINER_PIC_EXPERT_M,
        .encounterMusic_gender =
#line 210
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 211
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 212
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 214
            .species = SPECIES_KADABRA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 216
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 215
            .lvl = 32,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 217
                MOVE_TELEKINESIS,
                MOVE_QUICK_ATTACK,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 222
            .species = SPECIES_ZERAORA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 224
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 223
            .lvl = 32,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 225
                MOVE_PLASMA_FISTS,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
#line 233
[TRAINER_BERUS] =
    {
#line 234
        .trainerName = _("Berus"),
#line 235
        .trainerClass = TRAINER_CLASS_GENTLEMAN,
#line 236
        .trainerPic = TRAINER_PIC_GENTLEMAN,
        .encounterMusic_gender =
#line 238
            TRAINER_ENCOUNTER_MUSIC_INTENSE,
#line 239
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 240
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 1,
        .party = (const struct TrainerMon[])
        {
            {
#line 242
            .species = SPECIES_HOUNDOOM_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 242
            .heldItem = ITEM_BRIGHT_POWDER,
#line 244
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 243
            .lvl = 100,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 245
                MOVE_METRONOME,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
