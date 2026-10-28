#include "print.h"

static const uint8_t NUM_COLS = 80;
static const uint8_t NUM_ROWS = 25;

typedef struct {
    uint8_t character;
    uint8_t color;
} Char;

volatile Char* buffer = (volatile Char*) 0xb8000ULL;
uint8_t col = 0;
uint8_t row = 0;
uint8_t color = WHITE | (BLACK << 4);

void print_newline() {
    col = 0;

    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (uint8_t row = 1; row < NUM_ROWS; row++) {
        for (uint8_t col = 0; col < NUM_COLS; col++) {
            Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row((uint8_t) (NUM_COLS - 1));
}

void print_clear(void) {
    for(uint8_t row = 0; row < NUM_ROWS; row++) {
        clear_row(row);
    }
}

void clear_row(uint8_t row) {
    Char empty;
    empty.character = ' ';
    empty.color = color;

    for(uint8_t col = 0; col < NUM_COLS; col++) {
        buffer[col + (NUM_ROWS * row)] = empty;
    }
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (col > NUM_COLS) {
        print_newline();
    }

    Char ch;
    ch.character = character;
    ch.color = color;

    buffer[col + (NUM_ROWS * row)] = ch;
    col++;
}

void print_string(char* str) {
    for(uint8_t i = 0; 1; i++) {
        char character = (uint8_t) str[i];

        if (character == '\0') {
            return;
        }
        print_char(character);
    }
}

void print_set_color(uint8_t fg, uint8_t bg) {
    color = fg + (bg << 4);
}