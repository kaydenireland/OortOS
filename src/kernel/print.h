#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

enum {
    BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GRAY = 7,
	DARK_GRAY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	PINK = 13,
	YELLOW = 14,
	WHITE = 15,
};

void clear_row(uint8_t row);
void print_clear(void);
void print_char(char ch);
void print_string(char* str);
void print_newline(void);
void print_set_color(uint8_t fg, uint8_t bg);

#endif