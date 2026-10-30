#include "kernel.h"
#include "print.h"
#include "../hardware/rtc.h"

void start_kernel(void) {

    print_string("Welcome to Oort OS!");
    print_newline();
    print_newline();
    print_string("\"Every atom in your body came from a star that exploded. And, the atoms in your left hand probably came from a different star than your right hand. It really is the most poetic thing I know about physics: You are all stardust.\"");
    print_newline();
    print_string("     - Lawrence M. Krauss");
    print_newline();
    print_newline();


    u8 previous_seconds = 0;

    for(u8 i = 0; i < 5;) {
        u8 seconds = rtc_seconds();

        if (seconds != previous_seconds) {
            i++;
            print_set_color(GREEN, BLACK);
            print_string("Seconds: ");
            print_u64_deci(seconds);
            print_newline();
        }

        previous_seconds = seconds;
    }
}