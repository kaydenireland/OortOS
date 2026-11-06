#include "kernel.h"
#include "stdio.h"
#include "../util/debug.h"
#include "../util/numbers.h"
#include "../arch/idt.h"
#include "../drivers/rtc.h"
#include "../drivers/keyboard.h"
#include "../drivers/key_code.h"
#include "../drivers/speaker.h"
#include "timer.h"
#include "random.h"

extern void rust_main(void);

void handle_input(struct KeyboardEvent event) {
    if (event.type == PRESSED) {
        print_set_color(CYAN, BLACK);
        putc(key_to_ascii(event.code));        
    } else if (event.type == RELEASED) {}
}

void kernel_init(void) {
    dbg_putln("Initializing Kernel");

    idt_set_pit_handler(timer_tick);
    keyboard_set_handler(handle_input);

    timer_init();
    keyboard_init();
}

void kernel_start(void) {
    dbg_putln("Kernel Loaded in Long Mode");
    kernel_init();

    cursor_disable();

    print_set_color(YELLOW, BLACK);
    puts("Welcome to Oort OS!\n\n");
    print_reset_color();
    puts("\"Every atom in your body came from a star that exploded. And, the atoms in your left hand probably came from a different star than your right hand. It really is the most poetic thing I know about physics: You are all stardust.\"\n");
    print_set_color(MAGENTA, BLACK);
    puts("     - Lawrence M. Krauss\n\n");

    print_set_color(CYAN, BLACK);
    printf("Hello from C!\n");
    print_set_color(BROWN, BLACK);
    rust_main();

    while(1);
}