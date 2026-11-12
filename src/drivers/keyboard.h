#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../util/numbers.h"

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


void scancode_push(u8 code);
b8 scancode_pop(u8* code);

#endif