#include "../util/types.h"

// Programmable Interval Timer

void pit_init(u32 frequency);
void pit_handler(void);
u64 pit_ticks(void);