#include "flags.h"

u8 flag_set(u8 flags, u8 flag) {
    return  flags | (1 << flag);
}

u8 flag_clear(u8 flags, u8 flag) {
    return flags & ~(1 << flag);
}

u8 flag_toggle(u8 flags, u8 flag) {
    return flags ^ (1 << flag);
}

u8 flag_read(u8 flags, u8 flag) {
    return flags & (1 << flag);
}