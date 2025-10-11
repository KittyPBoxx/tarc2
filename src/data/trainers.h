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
#line 104
            .heldItem = ITEM_COBA_BERRY,
#line 107
            .iv = TRAINER_PARTY_IVS(0, 31, 0, 31, 0, 31),
#line 105
            .lvl = 38,
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
        .partySize = 3,
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
            .species = SPECIES_HAWLUCHA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 136
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 135
            .ability = ABILITY_UNBURDEN,
#line 133
            .lvl = 35,
#line 134
            .nature = NATURE_LONELY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 137
                MOVE_SKY_DROP,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 142
            .species = SPECIES_COTTONEE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 146
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 145
            .ability = ABILITY_PRANKSTER,
#line 143
            .lvl = 30,
#line 144
            .nature = NATURE_IMPISH,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 147
                MOVE_STUN_SPORE,
                MOVE_ROUND,
            },
            },
        },
    },
#line 153
[TRAINER_EXAMINER_RUNE] =
    {
#line 154
        .trainerName = _("Rune"),
#line 155
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 156
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 158
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 159
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 160
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_ACE_POKEMON,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 163
            .species = SPECIES_AMPHAROS_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 167
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 166
            .ability = ABILITY_MOLD_BREAKER,
#line 164
            .lvl = 35,
#line 165
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 168
                MOVE_ZAP_CANNON,
                MOVE_FOCUS_BLAST,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 173
            .species = SPECIES_MR_MIME,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 173
            .heldItem = ITEM_LAGGING_TAIL,
#line 177
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 176
            .ability = ABILITY_SOUNDPROOF,
#line 174
            .lvl = 30,
#line 175
            .nature = NATURE_MILD,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 178
                MOVE_DREAM_EATER,
                MOVE_YAWN,
                MOVE_MAGIC_POWDER,
                MOVE_ROLE_PLAY,
            },
            },
        },
    },
#line 186
[TRAINER_EXAMINER_DANTE] =
    {
#line 187
        .trainerName = _("Dante"),
#line 188
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 189
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 191
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 192
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 193
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN | AI_FLAG_DOUBLE_BATTLE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 195
            .species = SPECIES_STARMIE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 195
            .heldItem = ITEM_TERRAIN_EXTENDER,
#line 199
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 0, 31, 31),
#line 197
            .ability = ABILITY_ANALYTIC,
#line 196
            .lvl = 50,
#line 198
            .nature = NATURE_SASSY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 200
                MOVE_PSYCHIC_TERRAIN,
                MOVE_REFRESH,
                MOVE_TOXIC,
                MOVE_FORESIGHT,
            },
            },
            {
#line 205
            .species = SPECIES_ILLUMISE,
#line 205
            .gender = TRAINER_MON_FEMALE,
#line 209
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 207
            .ability = ABILITY_PRANKSTER,
#line 206
            .lvl = 50,
#line 208
            .nature = NATURE_TIMID,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 210
                MOVE_WILL_O_WISP,
                MOVE_TAILWIND,
                MOVE_LAST_RESORT,
                MOVE_ROLE_PLAY,
            },
            },
            {
#line 215
            .species = SPECIES_HAUNTER,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 215
            .heldItem = ITEM_TOXIC_ORB,
#line 219
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 217
            .ability = ABILITY_LEVITATE,
#line 216
            .lvl = 50,
#line 218
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 220
                MOVE_CURSE,
                MOVE_DESTINY_BOND,
                MOVE_HAZE,
            },
            },
        },
    },
#line 228
[TRAINER_DEITY_TENJIN] =
    {
#line 229
        .trainerName = _("TENJIN"),
#line 230
        .trainerClass = TRAINER_CLASS_EXPERT,
#line 231
        .trainerPic = TRAINER_PIC_EXPERT_M,
        .encounterMusic_gender =
#line 233
            TRAINER_ENCOUNTER_MUSIC_MAGMA,
#line 234
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 235
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 237
            .species = SPECIES_NINJASK,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 237
            .heldItem = ITEM_KINGS_ROCK,
#line 240
            .iv = TRAINER_PARTY_IVS(31, 0, 31, 31, 31, 31),
#line 238
            .lvl = 30,
#line 239
            .nature = NATURE_TIMID,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 241
                MOVE_POISON_STING,
                MOVE_BULLET_PUNCH,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 246
            .species = SPECIES_SLOWPOKE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 246
            .heldItem = ITEM_ROCKY_HELMET,
#line 249
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 0, 31, 31),
#line 247
            .lvl = 20,
#line 248
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 250
                MOVE_PERISH_SONG,
                MOVE_MISTY_TERRAIN,
                MOVE_TELEKINESIS,
                MOVE_CONFUSION,
            },
            },
            {
#line 255
            .species = SPECIES_ZERAORA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 259
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 258
            .ability = ABILITY_VOLT_ABSORB,
#line 256
            .lvl = 90,
#line 257
            .nature = NATURE_NAUGHTY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 260
                MOVE_PLASMA_FISTS,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
#line 268
[TRAINER_BERUS] =
    {
#line 269
        .trainerName = _("Berus"),
#line 270
        .trainerClass = TRAINER_CLASS_GENTLEMAN,
#line 271
        .trainerPic = TRAINER_PIC_GENTLEMAN,
        .encounterMusic_gender =
#line 273
            TRAINER_ENCOUNTER_MUSIC_INTENSE,
#line 274
        .items = { ITEM_FULL_RESTORE },
#line 275
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 276
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 1,
        .party = (const struct TrainerMon[])
        {
            {
#line 278
            .species = SPECIES_HOUNDOOM_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 278
            .heldItem = ITEM_BRIGHT_POWDER,
#line 282
            .ev = TRAINER_PARTY_EVS(30, 0, 0, 0, 0, 0),
#line 281
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 279
            .lvl = 100,
#line 280
            .nature = NATURE_BOLD,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 283
                MOVE_SHEER_COLD,
                MOVE_FISSURE,
                MOVE_GUILLOTINE,
            },
            },
        },
    },
