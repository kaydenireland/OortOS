#include "pit.h"

#include "../kernel/debug.h"
#include "port.h"

#define PIT_DATA_PORT 0x40  // Channel 0
#define PIT_COMMAND_PORT 0x43

#define PIT_FREQUENCY 1193182 // in hz


static volatile u64 ticks = 0;

void pit_init(u32 frequency) {
    dbg_putln("Initializing Programmable Interval Timer (PIT)");

    if (frequency == 0) return; 

    u16 divisor = PIT_FREQUENCY / frequency;

    if (divisor == 0 || divisor > MAX_U16) return;

    port_outb(PIT_COMMAND_PORT, 0x34);
    port_outb(PIT_DATA_PORT, (u8)(divisor & 0xFF)); // send low byte first
    port_outb(PIT_DATA_PORT, (u8)(divisor >> 8)); // send high byte second
}

void pit_handler(void) {
    ticks++;
}

u64 pit_ticks(void) {
    return ticks;
}