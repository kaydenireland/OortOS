#include "print.h"

static const u8 NUM_COLS = 80;
static const u8 NUM_ROWS = 25;

typedef struct {
    u8 character;
    u8 color;
} Char;

volatile Char* buffer = (volatile Char*) 0xb8000ULL;
u8 vga_col = 0;
u8 vga_row = 0;
u8 color = WHITE | (BLACK << 4);

void print_newline(void) {
    vga_col = 0;

    if (vga_row < NUM_ROWS - 1) {
        vga_row++;
        return;
    }

    for (u8 row = 1; row < NUM_ROWS; row++) {
        for (u8 col = 0; col < NUM_COLS; col++) {
            Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row((u8) (NUM_ROWS - 1));
}

void print_clear(void) {
    for(u8 row = 0; row < NUM_ROWS; row++) {
        clear_row(row);
    }
}

void clear_row(u8 row) {
    Char empty;
    empty.character = ' ';
    empty.color = color;

    for(u8 col = 0; col < NUM_COLS; col++) {
        buffer[col + (NUM_ROWS * row)] = empty;
    }
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (vga_col >= NUM_COLS) {
        print_newline();
    }

    Char ch;
    ch.character = character;
    ch.color = color;

    buffer[vga_col + (NUM_COLS * vga_row)] = ch;
    vga_col++;
}

void print_string(char* str) {
    for(u8 i = 0; 1; i++) {
        char character = (u8) str[i];

        if (character == '\0') {
            return;
        }
        print_char(character);
    }
}

void print_reset_color(void) {
    print_set_color(WHITE, BLACK);
}

void print_set_color(u8 fg, u8 bg) {
    color = fg + (bg << 4);
}

void print_u64_deci(u64 val) {
    if (val == 0) {
        print_char('0');
        return;
    }

    char buffer[20];    // 20 is max number of digits of a u64 decimal
    int i = 0;

    while (val > 0) {
        buffer[i++] = (val % 10) + '0';     // use '0' ascii to align ascii with our values
        val /= 10;
    }

    while (i-- > 0) {
        print_char(buffer[i]);
    }

}

void print_u64_hex(u64 val) {
    if (val == 0) {
        print_char('0');
        return;
    }

    char buffer[16];
    int i = 0;

    while (val > 0) {
        u8 digit = val & 0xF;

        if (digit < 10) {
            buffer[i++] = digit + '0';
        } else {
            buffer[i++] = digit - 10 + 'A';
        }

        val >>= 4;
    }

    while (i-- > 0) {
        print_char(buffer[i]);
    }

}

void print_u64_bin(u64 val) {
    char buffer[64];

    for(u8 i = 0; i < 64; i++) {
        buffer[i] = (val & 0x1) + '0';
        val >>= 1;
    }

    for(u8 i = 64; i > 0; i--) {
        print_char(buffer[i - 1]);
    }
}