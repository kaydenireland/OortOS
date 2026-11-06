#include "pit.h"

#include "../util/debug.h"
#include "../arch/port.h"

void pit_init(u32 frequency) {
    dbg_putln("Initializing Programmable Interval Timer (PIT)");

    if (frequency == 0 || frequency > MAX_U16) return; 

    u16 divisor = PIT_FREQUENCY / frequency;

    port_outb(PIT_COMMAND_PORT, 0x34);
    port_outb(PIT_DATA_PORT, (u8)(divisor & 0xFF)); // send low byte first
    port_outb(PIT_DATA_PORT, (u8)(divisor >> 8)); // send high byte second
}