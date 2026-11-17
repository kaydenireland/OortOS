#ifndef MOUSE_H
#define MOUSE_H

#include "../util/numbers.h"

enum MouseResolution {
    ONE = 0,
    TWO = 1,
    FOUR = 2,
    EIGHT = 3
};

struct MouseEvent {
    i16 x_mov;
    i16 y_mov;

    b8 left;
    b8 middle;
    b8 right;
};

void mouse_init(void);
void mouse_set_handler(void (*handler) (struct MouseEvent event));

b8 mouse_resolution(u8 resolution);

#endif