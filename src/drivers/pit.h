#include "../util/numbers.h"

// Programmable Interval Timer

#define PIT_DATA_PORT 0x40  // Channel 0
#define PIT_AUDIO_PORT 0x42  // Channel 2
#define PIT_COMMAND_PORT 0x43

#define PIT_FREQUENCY 1193182 // in hz

void pit_init(u32 frequency);