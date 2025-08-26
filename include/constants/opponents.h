#ifndef GUARD_CONSTANTS_OPPONENTS_H
#define GUARD_CONSTANTS_OPPONENTS_H

#include "constants/battle_partner.h"

#define TRAINER_NONE                          0
#define TRAINER_EXAMINER_FIONN                1
#define TRAINER_EXAMINER_SORA                 2
#define TRAINER_EXAMINER_RUNE                 3
#define TRAINER_EXAMINER_DANTE                4
#define TRAINER_DEITY_TENJIN                  5
#define TRAINER_BERUS                         6

// NOTE: Because each Trainer uses a flag to determine when they are defeated, there is only space for 9 additional trainers before trainer flag space overflows
//       More space can be made by shifting flags around in constants/flags.h or changing how trainer flags are handled
//       MAX_TRAINERS_COUNT can be increased but will take up additional saveblock space

#define TRAINERS_COUNT                      7
#define MAX_TRAINERS_COUNT                  15
#define TRAINER_PARTNER(partner)           (MAX_TRAINERS_COUNT + partner)

#endif  // GUARD_CONSTANTS_OPPONENTS_H
