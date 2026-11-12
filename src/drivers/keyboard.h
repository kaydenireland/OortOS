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

    u8 flags;
};

enum KeyFlag {
    SHIFT = 0,
    CAPS_LOCK = 1,
    CONTROL = 2,
    ALT = 3,
    FUNCTION = 4
};


void keyboard_init(void);
void keyboard_set_handler(void (*handler) (struct KeyboardEvent event));

void keyboard_event_push(struct KeyboardEvent event);
b8 keyboard_event_pop(struct KeyboardEvent* event);

#endif