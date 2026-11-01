#ifndef PRINT_H
#define PRINT_H

#include "types.h"

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

void clear_row(u8 row);
void print_clear(void);
void print_char(char ch);
void print_string(char* str);
void print_newline(void);
void print_reset_color(void);
void print_set_color(u8 fg, u8 bg);
void print_u64_deci(u64 val);
void print_u64_hex(u64 val);
void print_u64_bin(u64 val);

#endif