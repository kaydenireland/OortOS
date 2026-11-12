#include "kernel.h"
#include "stdio.h"
#include "../util/debug.h"
#include "../util/numbers.h"
#include "../util/flags.h"
#include "../arch/idt.h"
#include "../drivers/keyboard.h"
#include "../drivers/key_code.h"
#include "../drivers/speaker.h"
#include "timer.h"
#include "music.h"

void handle_keyboard_input(struct KeyboardEvent event) {
    keyboard_event_push(event);
}

void kernel_init(void) {
    dbg_putln("Initializing Kernel");

    idt_set_pit_handler(timer_tick);
    keyboard_set_handler(handle_keyboard_input);

    timer_init();
    keyboard_init();
}

void kernel_start(void) {
    dbg_putln("Kernel Loaded in Long Mode");
    kernel_init();

    cursor_disable();

    putln("OortOS v0.1.0\n");

    textc(YELLOW, BLACK);
    puts("Welcome to Oort OS!\n\n");
    resetc();
    puts("\"Every atom in your body came from a star that exploded. And, the atoms in your left hand probably came from a different star than your right hand. It really is the most poetic thing I know about physics: You are all stardust.\"\n");
    textc(MAGENTA, BLACK);
    puts("     - Lawrence M. Krauss\n\n");

    play_tetris();
    
    struct KeyboardEvent event;
    b8 shift, caps;
    while(1) {
        while(keyboard_event_pop(&event) && event.type == PRESSED) {
            if (event.code == KEY_CAPS_LOCK) beep(G4, 50);
            textc(CYAN, BLACK);
            shift = flag_read(event.flags, SHIFT);
            caps = flag_read(event.flags, CAPS_LOCK);
            if (event.code != KEY_LEFT_SHIFT 
                && event.code != KEY_RIGHT_SHIFT 
                && event.code != KEY_CAPS_LOCK) {
                    putc(key_to_ascii(event.code, shift, caps)); 
                } 
        }
    }
}