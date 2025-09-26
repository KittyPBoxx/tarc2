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
                MOVE_HEAVY_SLAM,
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
#line 125
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 124
            .lvl = 25,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 126
                MOVE_FLING,
                MOVE_COVET,
                MOVE_MINIMIZE,
                MOVE_LOW_KICK,
            },
            },
            {
#line 131
            .species = SPECIES_AMPHAROS_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 134
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 133
            .ability = ABILITY_MOLD_BREAKER,
#line 132
            .lvl = 25,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 135
                MOVE_ZAP_CANNON,
                MOVE_FOCUS_BLAST,
                MOVE_TRAILBLAZE,
                MOVE_NONE,
            },
            },
        },
    },
#line 142
[TRAINER_EXAMINER_RUNE] =
    {
#line 143
        .trainerName = _("Rune"),
#line 144
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 145
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 147
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 148
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 149
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 151
            .species = SPECIES_HAWLUCHA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 155
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 154
            .ability = ABILITY_MOLD_BREAKER,
#line 152
            .lvl = 30,
#line 153
            .nature = NATURE_LONELY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 156
                MOVE_SKY_DROP,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 161
            .species = SPECIES_MR_MIME,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 165
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 164
            .ability = ABILITY_SOUNDPROOF,
#line 162
            .lvl = 30,
#line 163
            .nature = NATURE_MILD,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 166
                MOVE_DREAM_EATER,
                MOVE_YAWN,
                MOVE_MAGIC_POWDER,
                MOVE_NONE,
            },
            },
        },
    },
#line 173
[TRAINER_EXAMINER_DANTE] =
    {
#line 174
        .trainerName = _("Dante"),
#line 175
        .trainerClass = TRAINER_CLASS_TEAM_AQUA,
#line 176
        .trainerPic = TRAINER_PIC_AQUA_GRUNT_M,
        .encounterMusic_gender =
#line 178
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 179
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 180
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN | AI_FLAG_PREFER_STATUS_MOVES | AI_FLAG_DOUBLE_BATTLE | AI_FLAG_HP_AWARE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 182
            .species = SPECIES_STARMIE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 182
            .heldItem = ITEM_TERRAIN_EXTENDER,
#line 186
            .iv = TRAINER_PARTY_IVS(31, 31, 0, 31, 31, 31),
#line 185
            .ability = ABILITY_ANALYTIC,
#line 183
            .lvl = 0,
#line 184
            .nature = NATURE_HASTY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 187
                MOVE_PSYCHIC_TERRAIN,
                MOVE_REFRESH,
                MOVE_HEAL_PULSE,
                MOVE_TOXIC,
            },
            },
            {
#line 192
            .species = SPECIES_ILLUMISE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 195
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 194
            .ability = ABILITY_PRANKSTER,
#line 193
            .lvl = 0,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 196
                MOVE_WILL_O_WISP,
                MOVE_COUNTER,
                MOVE_LIGHT_SCREEN,
                MOVE_NATURE_POWER,
            },
            },
            {
#line 201
            .species = SPECIES_HAUNTER,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 201
            .heldItem = ITEM_TOXIC_ORB,
#line 204
            .iv = TRAINER_PARTY_IVS(31, 0, 31, 0, 31, 31),
#line 202
            .lvl = 1,
#line 203
            .nature = NATURE_SASSY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 205
                MOVE_FLING,
                MOVE_CURSE,
                MOVE_NONE,
            },
            },
        },
    },
#line 212
[TRAINER_DEITY_TENJIN] =
    {
#line 213
        .trainerName = _("GRUNT"),
#line 214
        .trainerClass = TRAINER_CLASS_EXPERT,
#line 215
        .trainerPic = TRAINER_PIC_EXPERT_M,
        .encounterMusic_gender =
#line 217
            TRAINER_ENCOUNTER_MUSIC_AQUA,
#line 218
        .battleType = TRAINER_BATTLE_TYPE_DOUBLES,
#line 219
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY | AI_FLAG_FORCE_SETUP_FIRST_TURN,
        .partySize = 2,
        .party = (const struct TrainerMon[])
        {
            {
#line 221
            .species = SPECIES_SLOWPOKE,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 223
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 0, 31, 31),
#line 222
            .lvl = 20,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 224
                MOVE_PERISH_SONG,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
            {
#line 229
            .species = SPECIES_ZERAORA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 232
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 231
            .ability = ABILITY_VOLT_ABSORB,
#line 230
            .lvl = 50,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 233
                MOVE_PLASMA_FISTS,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
#line 241
[TRAINER_BERUS] =
    {
#line 242
        .trainerName = _("Berus"),
#line 243
        .trainerClass = TRAINER_CLASS_GENTLEMAN,
#line 244
        .trainerPic = TRAINER_PIC_GENTLEMAN,
        .encounterMusic_gender =
#line 246
            TRAINER_ENCOUNTER_MUSIC_INTENSE,
#line 247
        .battleType = TRAINER_BATTLE_TYPE_SINGLES,
#line 248
        .aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY,
        .partySize = 1,
        .party = (const struct TrainerMon[])
        {
            {
#line 250
            .species = SPECIES_HOUNDOOM_MEGA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 250
            .heldItem = ITEM_BRIGHT_POWDER,
#line 252
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 251
            .lvl = 100,
            .nature = NATURE_HARDY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 253
                MOVE_METRONOME,
                MOVE_NONE,
                MOVE_NONE,
                MOVE_NONE,
            },
            },
        },
    },
