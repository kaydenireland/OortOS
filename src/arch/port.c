#include "port.h"

void portw(void) {
    inb(0x80);     // causes slight delay in hardware
}