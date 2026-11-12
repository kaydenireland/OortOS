#ifndef STDIO_H
#define STDIO_H

#include "../util/numbers.h"

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

enum {
	ZERO = 0,
	BINARY = 2,
	OCTAL = 8,
	DECIMAL = 10,
	HEX = 16
};

void clear_row(u8 row);
void print_clear(void);

void putc(char ch);
void puts(const char* str);
void putln(const char* str);
void putnl(void);
void printf(char* fmt, ...);

void resetc(void);
void textc(u8 fg, u8 bg);

void print_u64(u64 val, u8 base);
void print_u64_deci(u64 val);
void print_u64_oct(u64 val);
void print_u64_hex(u64 val);
void print_u64_bin(u64 val);

void cursor_default(void);
void cursor_enable(u8 start, u8 end);
void cursor_disable(void);
void cursor_move(u8 x, u8 y);
u16 cursor_pos(void);

#endif