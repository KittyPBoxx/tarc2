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
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 92
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 93
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN | AI_FLAG_PREFER_STATUS_MOVES,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 95
            .species = SPECIES_CHESPIN,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 99
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 0, 31, 31),
#line 98
            .ability = ABILITY_BULLETPROOF,
#line 96
            .lvl = 30,
#line 97
            .nature = NATURE_RELAXED,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 100
                MOVE_WORRY_SEED,
                MOVE_TAKE_DOWN,
                MOVE_NONE,
            },
            },
            {
#line 104
            .species = SPECIES_EXEGGUTOR,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 107
            .iv = TRAINER_PARTY_IVS(0, 31, 0, 31, 0, 31),
#line 105
            .lvl = 45,
#line 106
            .nature = NATURE_ADAMANT,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 108
                MOVE_STORED_POWER,
                MOVE_CALM_MIND,
                MOVE_ABSORB,
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
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN | AI_FLAG_PREFER_STATUS_MOVES,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 123
            .species = SPECIES_CHANSEY,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 126
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 124
            .lvl = 25,
#line 125
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 127
                MOVE_FLING,
                MOVE_COVET,
                MOVE_MINIMIZE,
                MOVE_LOW_KICK,
            },
            },
            {
#line 132
            .species = SPECIES_AMPHAROS_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 136
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 135
            .ability = ABILITY_MOLD_BREAKER,
#line 133
            .lvl = 35,
#line 134
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 137
                MOVE_ZAP_CANNON,
                MOVE_FOCUS_BLAST,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
#line 144
[TRAINER_EXAMINER_RUNE] =
    {
#line 145
        .trainerName = _("Rune"),
#line 146
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 147
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 149
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 150
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 151
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 153
            .species = SPECIES_HAWLUCHA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 157
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 156
            .ability = ABILITY_MOLD_BREAKER,
#line 154
            .lvl = 30,
#line 155
            .nature = NATURE_LONELY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 158
                MOVE_SKY_DROP,
                MOVE_POISON_JAB,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 163
            .species = SPECIES_MR_MIME,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 163
            .heldItem = ITEM_LAGGING_TAIL,
#line 167
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 166
            .ability = ABILITY_SOUNDPROOF,
#line 164
            .lvl = 30,
#line 165
            .nature = NATURE_MILD,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 168
                MOVE_DREAM_EATER,
                MOVE_YAWN,
                MOVE_MAGIC_POWDER,
                MOVE_ROLE_PLAY,
            },
            },
        },
    },
#line 175
[TRAINER_EXAMINER_DANTE] =
    {
#line 176
        .trainerName = _("Dante"),
#line 177
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 178
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 180
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 181
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 182
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN | AI_FLAG_DOUBLE_BATTLE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 184
            .species = SPECIES_STARMIE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 184
            .heldItem = ITEM_TERRAIN_EXTENDER,
#line 188
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 0, 31, 31),
#line 186
            .ability = ABILITY_ANALYTIC,
#line 185
            .lvl = 50,
#line 187
            .nature = NATURE_SASSY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 189
                MOVE_PSYCHIC_TERRAIN,
                MOVE_REFRESH,
                MOVE_TOXIC,
                MOVE_FORESIGHT,
            },
            },
            {
#line 194
            .species = SPECIES_ILLUMISE,
#line 194
            .gender = TRAINER_MON_FEMALE,
#line 198
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 196
            .ability = ABILITY_PRANKSTER,
#line 195
            .lvl = 50,
#line 197
            .nature = NATURE_TIMID,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 199
                MOVE_WILL_O_WISP,
                MOVE_TAILWIND,
                MOVE_LAST_RESORT,
                MOVE_ROLE_PLAY,
            },
            },
            {
#line 204
            .species = SPECIES_HAUNTER,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 204
            .heldItem = ITEM_TOXIC_ORB,
#line 208
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 206
            .ability = ABILITY_LEVITATE,
#line 205
            .lvl = 50,
#line 207
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 209
                MOVE_CURSE,
                MOVE_DESTINY_BOND,
                MOVE_HAZE,
            },
            },
        },
    },
#line 217
[TRAINER_DEITY_TENJIN] =
    {
#line 218
        .trainerName = _("TENJIN"),
#line 219
        .trainerClass = TRAINER_CLASS_EXPERT,
#line 220
        .trainerPic = TRAINER_PIC_EXPERT_M,
        .encounterMusic_gender =
#line 222
            TRAINER_ENCOUNTER_MUSIC_MAGMA,
#line 223
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 224
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 226
            .species = SPECIES_NINJASK,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 226
            .heldItem = ITEM_KINGS_ROCK,
#line 229
            .iv = TRAINER_PARTY_IVS(31, 0, 31, 31, 31, 31),
#line 227
            .lvl = 30,
#line 228
            .nature = NATURE_TIMID,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 230
                MOVE_POISON_STING,
                MOVE_BULLET_PUNCH,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 235
            .species = SPECIES_SLOWPOKE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 235
            .heldItem = ITEM_ROCKY_HELMET,
#line 238
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 0, 31, 31),
#line 236
            .lvl = 20,
#line 237
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 239
                MOVE_PERISH_SONG,
                MOVE_MISTY_TERRAIN,
                MOVE_TELEKINESIS,
                MOVE_CONFUSION,
            },
            },
            {
#line 244
            .species = SPECIES_ZERAORA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 248
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 247
            .ability = ABILITY_VOLT_ABSORB,
#line 245
            .lvl = 90,
#line 246
            .nature = NATURE_NAUGHTY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 249
                MOVE_PLASMA_FISTS,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
#line 257
[TRAINER_BERUS] =
    {
#line 258
        .trainerName = _("Berus"),
#line 259
        .trainerClass = TRAINER_CLASS_GENTLEMAN,
#line 260
        .trainerPic = TRAINER_PIC_GENTLEMAN,
        .encounterMusic_gender =
#line 262
            TRAINER_ENCOUNTER_MUSIC_INTENSE,
#line 263
        .items = { ITEM_FULL_RESTORE },
#line 264
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 265
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 1,
        .party = (const struct TrainerMon[])
        {
            {
#line 267
            .species = SPECIES_HOUNDOOM_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 267
            .heldItem = ITEM_BRIGHT_POWDER,
#line 271
            .ev = TRAINER_PARTY_EVS(30, 0, 0, 0, 0, 0),
#line 270
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 268
            .lvl = 100,
#line 269
            .nature = NATURE_BOLD,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 272
                MOVE_SHEER_COLD,
                MOVE_FISSURE,
                MOVE_GUILLOTINE,
            },
            },
        },
    },
