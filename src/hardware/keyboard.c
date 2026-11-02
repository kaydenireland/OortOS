#include "keyboard.h"
#include "idt.h"
#include "ps2.h"
#include "../kernel/debug.h"

#define EXTENDED_SCAN_CODE 0xE0

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

    event.code = fat_code;

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