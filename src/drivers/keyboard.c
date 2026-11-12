#include "keyboard.h"
#include "../arch/idt.h"
#include "ps2.h"
#include "../util/debug.h"
#include "../util/flags.h"

#define EXTENDED_SCAN_CODE 0xE0

static u8 keyboard_flags = 0b0000;

void (*keyboard_handler_user)(struct KeyboardEvent event);

void keyboard_handler(void) {
    static b8 is_extended = 0;

    u8 scan_code = ps2_read_scan_code();

    if (scan_code == EXTENDED_SCAN_CODE) {
        is_extended = true;
        return;
    }

    if (keyboard_handler_user == null) return;

    u16 fat_code = scan_code & 0x7F;

    if (is_extended) {
        is_extended = false;
        fat_code |= EXTENDED_SCAN_CODE << 8;
    }

    struct KeyboardEvent event;

    
    if ((scan_code & 0x80) == 0) {
        event.type = PRESSED;
    } else {
        event.type = RELEASED;
    }

    if (fat_code == 0x2A || fat_code == 0x36) {
        if (event.type == PRESSED) {
            keyboard_flags = flag_set(keyboard_flags, SHIFT);
        } else {
            keyboard_flags = flag_clear(keyboard_flags, SHIFT);
        }
    }

    if (fat_code == 0x3A) {
        if (event.type == PRESSED) {
            keyboard_flags = flag_toggle(keyboard_flags, CAPS_LOCK);
        }
    }

    event.code = fat_code;
    event.flags = keyboard_flags;

    keyboard_handler_user(event);
}

void keyboard_init(void) {
    dbg_putln("Initializing Keyboard");
    idt_set_keyboard_handler(keyboard_handler);
    idt_init();
}

void keyboard_set_handler(void (*handler)(struct KeyboardEvent event)) {
	keyboard_handler_user = handler;	
}

#define QUEUE_SIZE 128

static struct KeyboardEvent buffer[QUEUE_SIZE];
static volatile u8 head = 0;
static volatile u8 tail = 0;

void keyboard_event_push(struct KeyboardEvent event) {
    u8 next = (tail + 1) % QUEUE_SIZE;

    if (next == head) return;   // drop key press if queue is full

    buffer[tail] = event;
    tail = next;
}

b8 keyboard_event_pop(struct KeyboardEvent* event) {
    if (head == tail) return false; // queue is empty

    *event = buffer[head];
    head = (head + 1) % QUEUE_SIZE;

    return true;
}