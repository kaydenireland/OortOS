#ifndef FLAGS_H
#define FLAGS_H

#include "numbers.h"

u8 flag_set(u8 flags, u8 flag);
u8 flag_clear(u8 flags, u8 flag);
u8 flag_toggle(u8 flags, u8 flag);
u8 flag_read(u8 flags, u8 flag);

#endif