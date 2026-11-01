#include "ps2.h"

#include "port.h"

#define PS2_DATA_PORT 0x60

u8 ps2_read_scan_code(void) {
    return port_inb(PS2_DATA_PORT);
}