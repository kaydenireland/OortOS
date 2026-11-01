#include "kernel.h"
#include "print.h"
#include "../hardware/rtc.h"
#include "../hardware/keyboard.h"
#include "../hardware/key_code.h"

void handle_input(struct KeyboardEvent event) {
    if (event.type == PRESSED) {
        print_set_color(CYAN, BLACK);
        print_char(key_to_ascii(event.code));
    } else if (event.type == RELEASED) {}
}

void start_kernel(void) {
    cursor_disable();

    print_set_color(YELLOW, BLACK);
    print_string("Welcome to Oort OS!\n\n");
    print_reset_color();
    print_string("\"Every atom in your body came from a star that exploded. And, the atoms in your left hand probably came from a different star than your right hand. It really is the most poetic thing I know about physics: You are all stardust.\"\n");
    print_string("     - Lawrence M. Krauss\n");

    keyboard_set_handler(handle_input);
    keyboard_init();

    u8 previous_seconds = 0;

    for(u8 i = 0; i < 5;) {
        u8 seconds = rtc_seconds();

        if (seconds != previous_seconds) {
            i++;
            print_set_color(GREEN, BLACK);
            print_string("\nSeconds: ");
            print_u64_deci(seconds);
        }

        previous_seconds = seconds;
    }
    print_string("  - Seconds loop disabled.\n");

    while(1);
}