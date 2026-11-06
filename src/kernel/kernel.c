#include "kernel.h"
#include "stdio.h"
#include "debug.h"
#include "../hardware/idt.h"
#include "../hardware/rtc.h"
#include "../hardware/keyboard.h"
#include "../hardware/key_code.h"
#include "../hardware/pit.h"

void handle_input(struct KeyboardEvent event) {
    if (event.type == PRESSED) {
        print_set_color(CYAN, BLACK);
        putc(key_to_ascii(event.code));
    } else if (event.type == RELEASED) {}
}

void start_kernel(void) {
    dbg_putln("Kernel Loaded in Long Mode");

    cursor_disable();

    print_set_color(YELLOW, BLACK);
    puts("Welcome to Oort OS!\n\n");
    print_reset_color();
    puts("\"Every atom in your body came from a star that exploded. And, the atoms in your left hand probably came from a different star than your right hand. It really is the most poetic thing I know about physics: You are all stardust.\"\n");
    print_set_color(MAGENTA, BLACK);
    puts("     - Lawrence M. Krauss\n\n");

    idt_set_pit_handler(pit_handler);
    keyboard_set_handler(handle_input);

    pit_init(1000);
    keyboard_init();

    u8 previous_seconds = 0;

    for(u8 i = 0; i < 3;) {
        u8 seconds = rtc_seconds();

        if (seconds != previous_seconds) {
            i++;
            print_set_color(GREEN, BLACK);
            printf("\n%s: %u, Hex: %x, Octal: %o", "Seconds", seconds, seconds, seconds);
        }

        previous_seconds = seconds;
    }
    puts("  - Seconds loop disabled.\n");

    u64 last_tick = 0;

    while(1) {
        u64 current_tick = pit_ticks();

        if (current_tick - last_tick >= 1000) {
            last_tick = current_tick;
            putln("One second elapsed");
        }
    }
}