#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../util/types.h"

enum KeyboardEventType {
    PRESSED = 0,
    RELEASED = 1
};

struct KeyboardEvent {
    u8 type;
    u16 code;
};


void keyboard_init(void);
void keyboard_set_handler(void (*handler) (struct KeyboardEvent event));


#endif