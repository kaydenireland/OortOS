#include "port.h"

void port_wait(void) {
    port_inb(0x80);     // causes slight delay in hardware
}