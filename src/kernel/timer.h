#ifndef TIMER_H
#define TIMER_H

#include "../util/numbers.h"

void timer_init(void);
void timer_setf(u32 frequency);
void timer_tick(void);
u64 timer_ticks(void);
void timer_sleep(u64 milliseconds);
u64 timer_uptime(void);
u64 timer_milliseconds(void);

#endif