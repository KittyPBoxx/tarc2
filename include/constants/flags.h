#ifndef GUARD_CONSTANTS_FLAGS_H
#define GUARD_CONSTANTS_FLAGS_H

#include "constants/opponents.h"
#include "constants/rematches.h"

// Temporary Flags
// These temporary flags are are cleared every time a map is loaded. They are used
// for things like shortening an NPCs introduction text if the player already spoke
// to them once.
#define TEMP_FLAGS_START 0x0
#define FLAG_TEMP_1      (TEMP_FLAGS_START + 0x1)
#define FLAG_TEMP_2      (TEMP_FLAGS_START + 0x2)
#define FLAG_TEMP_3      (TEMP_FLAGS_START + 0x3)
#define FLAG_TEMP_4      (TEMP_FLAGS_START + 0x4)
#define FLAG_TEMP_5      (TEMP_FLAGS_START + 0x5)  // Unused Flag
#define FLAG_TEMP_6      (TEMP_FLAGS_START + 0x6)  // Unused Flag
#define FLAG_TEMP_7      (TEMP_FLAGS_START + 0x7)  // Unused Flag
#define FLAG_TEMP_8      (TEMP_FLAGS_START + 0x8)  // Unused Flag
#define FLAG_TEMP_9      (TEMP_FLAGS_START + 0x9)  // Unused Flag
#define FLAG_TEMP_A      (TEMP_FLAGS_START + 0xA)  // Unused Flag
#define FLAG_TEMP_B      (TEMP_FLAGS_START + 0xB)  // Unused Flag
#define FLAG_TEMP_C      (TEMP_FLAGS_START + 0xC)  // Unused Flag
#define FLAG_TEMP_D      (TEMP_FLAGS_START + 0xD)  // Unused Flag
#define FLAG_TEMP_E      (TEMP_FLAGS_START + 0xE)  // When set, follower pokemon won't be spawned
#define FLAG_TEMP_F      (TEMP_FLAGS_START + 0xF)  // Unused Flag
#define FLAG_TEMP_10     (TEMP_FLAGS_START + 0x10) // Unused Flag
#define FLAG_TEMP_11     (TEMP_FLAGS_START + 0x11)
#define FLAG_TEMP_12     (TEMP_FLAGS_START + 0x12)
#define FLAG_TEMP_13     (TEMP_FLAGS_START + 0x13)
#define FLAG_TEMP_14     (TEMP_FLAGS_START + 0x14)
#define FLAG_TEMP_15     (TEMP_FLAGS_START + 0x15)
#define FLAG_TEMP_16     (TEMP_FLAGS_START + 0x16)
#define FLAG_TEMP_17     (TEMP_FLAGS_START + 0x17)
#define FLAG_TEMP_18     (TEMP_FLAGS_START + 0x18)
#define FLAG_TEMP_19     (TEMP_FLAGS_START + 0x19)
#define FLAG_TEMP_1A     (TEMP_FLAGS_START + 0x1A)
#define FLAG_TEMP_1B     (TEMP_FLAGS_START + 0x1B)
#define FLAG_TEMP_1C     (TEMP_FLAGS_START + 0x1C)
#define FLAG_TEMP_1D     (TEMP_FLAGS_START + 0x1D)
#define FLAG_TEMP_1E     (TEMP_FLAGS_START + 0x1E)
#define FLAG_TEMP_1F     (TEMP_FLAGS_START + 0x1F)
#define TEMP_FLAGS_END   FLAG_TEMP_1F
#define NUM_TEMP_FLAGS   (TEMP_FLAGS_END - TEMP_FLAGS_START + 1)

// Temp flag aliases
#define FLAG_TEMP_HIDE_FOLLOWER                 FLAG_TEMP_E

// Flags
#define FLAG_CHOSE_STARTER           0x57
#define FLAG_FIRST_WILD_BATTLE_DONE  0x58
#define FLAG_TREE_CUT                0x59
#define FLAG_FOREST_PUZZLE_SOLVED    0x5A
#define FLAG_COMPLETED               0x5B
#define FLAG_SUMMIT_INTRO_DONE       0x5C
#define FLAG_TILE_PUZZLE_ROCK_BROKEN 0x5D
#define FLAG_UNUSED_0x5E             0x5E
#define FLAG_GAME_CLEAR              0x5F
#define FLAG_BRIDGE_INTRO_DONE       0x60
#define FLAG_UNUSED_0x61 0x61
#define FLAG_UNUSED_0x62 0x62
#define FLAG_UNUSED_0x63 0x63
#define FLAG_UNUSED_0x64 0x64 
#define FLAG_UNUSED_0x65 0x65 
#define FLAG_UNUSED_0x66 0x66 
#define FLAG_UNUSED_0x67 0x67 
#define FLAG_UNUSED_0x68 0x68 
#define FLAG_UNUSED_0x69 0x69
#define FLAG_UNUSED_0x6A 0x6A
#define FLAG_UNUSED_0x6B 0x6B
#define FLAG_UNUSED_0x6C 0x6C
#define FLAG_UNUSED_0x6D 0x6D
#define FLAG_UNUSED_0x6E 0x6E
#define FLAG_UNUSED_0x6F 0x6F
#define FLAG_UNUSED_0x70 0x70
#define FLAG_UNUSED_0x71 0x71 
#define FLAG_UNUSED_0x72 0x72 
#define FLAG_UNUSED_0x73 0x73
#define FLAG_UNUSED_0x74 0x74 
#define FLAG_UNUSED_0x75 0x75 
#define FLAG_UNUSED_0x76 0x76

// Trainer Flags
// See constants/opponents.h. The values there + FLAG_TRAINER_FLAG_START are the flag IDs
#define TRAINER_FLAGS_START                                         (FLAG_UNUSED_0x76 + 1)
#define TRAINER_FLAGS_END                                           (TRAINER_FLAGS_START + MAX_TRAINERS_COUNT - 1) 

// System Flags
#define SYSTEM_FLAGS                                   (TRAINER_FLAGS_END + 1) 
#define FLAG_SYS_GAME_CLEAR                            (SYSTEM_FLAGS + 0x0)
#define FLAG_SYS_CTRL_OBJ_DELETE                       (SYSTEM_FLAGS + 0x1)
#define FLAGS_COUNT (FLAG_SYS_CTRL_OBJ_DELETE + 1)

// Special Flags (Stored in EWRAM (sSpecialFlags), not in the SaveBlock)
#define SPECIAL_FLAGS_START                     0x4000
#define FLAG_HIDE_MAP_NAME_POPUP                (SPECIAL_FLAGS_START + 0x0)
#define FLAG_DONT_TRANSITION_MUSIC              (SPECIAL_FLAGS_START + 0x1)
#define FLAG_ENABLE_MULTI_CORRIDOR_DOOR         (SPECIAL_FLAGS_START + 0x2)
#define FLAG_SPECIAL_FLAG_UNUSED_0x4003         (SPECIAL_FLAGS_START + 0x3) // Unused Flag
#define FLAG_STORING_ITEMS_IN_PYRAMID_BAG       (SPECIAL_FLAGS_START + 0x4)
#define FLAG_SAFE_FOLLOWER_MOVEMENT             (SPECIAL_FLAGS_START + 0x5) // When set, applymovement does not put the follower inside a pokeball
                                                                            // Also, scripted movements on the player will move follower(s), too
// FLAG_SPECIAL_FLAG_0x4005 - 0x407F also exist and are unused
#define SPECIAL_FLAGS_END                       (SPECIAL_FLAGS_START + 0x7F)
#define NUM_SPECIAL_FLAGS                       (SPECIAL_FLAGS_END - SPECIAL_FLAGS_START + 1)

#if TESTING
#define TESTING_FLAGS_START                     0x5000
#define TESTING_FLAG_SLEEP_CLAUSE               (TESTING_FLAGS_START + 0x0)
#define TESTING_FLAG_INVERSE_BATTLE             (TESTING_FLAGS_START + 0x1)
#define TESTING_FLAG_UNUSED_2                   (TESTING_FLAGS_START + 0x2)
#define TESTING_FLAG_UNUSED_3                   (TESTING_FLAGS_START + 0x3)
#define TESTING_FLAG_UNUSED_4                   (TESTING_FLAGS_START + 0x4)
#define TESTING_FLAG_UNUSED_5                   (TESTING_FLAGS_START + 0x5)
#define TESTING_FLAG_UNUSED_6                   (TESTING_FLAGS_START + 0x6)
#define TESTING_FLAG_UNUSED_7                   (TESTING_FLAGS_START + 0x7)
#endif // TESTING

#endif // GUARD_CONSTANTS_FLAGS_H
