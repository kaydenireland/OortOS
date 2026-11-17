#include "kernel.h"
#include "stdio.h"
#include "../util/debug.h"
#include "../util/numbers.h"
#include "../util/flags.h"
#include "../arch/idt.h"
#include "../drivers/keyboard.h"
#include "../drivers/key_code.h"
#include "../drivers/mouse.h"
#include "../drivers/speaker.h"
#include "timer.h"
#include "music.h"

void handle_keyboard_input(struct KeyboardEvent event) {
    keyboard_event_push(event);
}

// I found a secret Mickey 8o8
#define MICKEYS_PER_COL 8
#define MICKEYS_PER_ROW 16
#define SCREEN_COLS 80
#define SCREEN_ROWS 25

static i32 mouse_x = (SCREEN_COLS * MICKEYS_PER_COL) / 2;
static i32 mouse_y = (SCREEN_ROWS * MICKEYS_PER_ROW) / 2;
static b8 prev_left = 0;

void handle_mouse_input(struct MouseEvent event) {
    mouse_x += event.x_mov;
    mouse_y += event.y_mov;

    // Clamp to screen dimensions
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x > SCREEN_COLS * MICKEYS_PER_COL - 1) mouse_x = SCREEN_COLS * MICKEYS_PER_COL - 1;

    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y > SCREEN_ROWS * MICKEYS_PER_ROW - 1) mouse_y = SCREEN_ROWS * MICKEYS_PER_ROW - 1;

    if (event.left && !prev_left) {
        dbg_putln("Left Mouse Button Clicked!");
    }
    prev_left = event.left;

}

void kernel_init(void) {
    dbg_putln("Initializing Kernel");

    idt_set_pit_handler(timer_tick);
    keyboard_set_handler(handle_keyboard_input);
    mouse_set_handler(handle_mouse_input);

    timer_init();
    idt_init();
    keyboard_init();
    mouse_init();
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
                    char ascii = key_to_ascii(event.code, shift, caps);
                    if (ascii != 0) putc(ascii);
                } 
        }
    }
}