#include "kernel.h"
#include "stdio.h"
#include "../util/debug.h"
#include "../util/numbers.h"
#include "../arch/idt.h"
#include "../drivers/keyboard.h"
#include "../drivers/key_code.h"
#include "../drivers/speaker.h"
#include "timer.h"

void handle_keyboard_input(struct KeyboardEvent event) {
    if (event.type == PRESSED) {
        // textc(CYAN, BLACK);
        // putc(key_to_ascii(event.code));      
        scancode_push(event.code);  
    } else if (event.type == RELEASED) {}
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

    beep(G4, 50);
    
    u8 code;
    while(1) {
        while(scancode_pop(&code)) {
            textc(CYAN, BLACK);
            putc(key_to_ascii(code));   
        }
    }
}