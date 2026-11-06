#include "ps2.h"

#include "../arch/port.h"

#define PS2_DATA_PORT 0x60

u8 ps2_read_scan_code(void) {
    return inb(PS2_DATA_PORT);
}