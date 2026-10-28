#include "kernel.h"
#include "print.h"

void start_kernel(void) {

    print_string("Welcome to Oort OS!");
    print_newline();
    print_newline();
    print_string("\"Every atom in your body came from a star that exploded. And, the atoms in your left hand probably came from a different star than your right hand. It really is the most poetic thing I know about physics: You are all stardust.\"");
    print_newline();
    print_string("     - Lawrence M. Krauss");

    while(1) {}
}