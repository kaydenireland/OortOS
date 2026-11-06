#include "timer.h"

#include "../drivers/pit.h"

#define BASE_FREQUENCY 1000

static volatile u64 ticks = 0;
static u32 frequency = BASE_FREQUENCY;

void timer_init(void) {
    pit_init(BASE_FREQUENCY);
}

void timer_setf(u32 _frequency) {
    frequency = _frequency;
    pit_init(_frequency);
}

void timer_tick(void) {
    ticks++;
}

u64 timer_ticks(void) {
    return ticks;
}

void timer_sleep(u64 milliseconds) {
    if (milliseconds == 0) return;

    u64 start = timer_milliseconds();

    while (timer_milliseconds() - start < milliseconds) {
        asm volatile("hlt");
    }
}

// in seconds
u64 timer_uptime(void) {
    return frequency != 0 ? ticks / frequency : 0;
}

u64 timer_milliseconds(void) {
    return frequency != 0 ? (ticks * 100) / frequency : 0;
}