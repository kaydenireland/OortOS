#include "stdio.h"
#include "../hardware/port.h"
#include <stdarg.h>

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

void putnl(void) {
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
        buffer[col + (NUM_COLS * row)] = empty;
    }
}

void backspace(void) {
    if (vga_col <= 0) return;

    Char empty;
    empty.character = ' ';
    empty.color = color;

    vga_col -= 1;
    buffer[vga_col + (NUM_COLS * vga_row)] = empty;
}

void putc(char character) {
    if (character == '\n') {
        putnl();
        return;
    } else if (character == '\b') {
        backspace();
        return;
    }

    if (vga_col >= NUM_COLS) {
        putnl();
    }

    Char ch;
    ch.character = character;
    ch.color = color;

    buffer[vga_col + (NUM_COLS * vga_row)] = ch;
    vga_col++;
}

void puts(const char* str) {
    for(u8 i = 0; 1; i++) {
        char character = (u8) str[i];

        if (character == '\0') {
            return;
        }
        putc(character);
    }
}

void putln(const char* str) {
    puts(str);
    putnl();
}

void print_reset_color(void) {
    print_set_color(WHITE, BLACK);
}

void print_set_color(u8 fg, u8 bg) {
    color = fg + (bg << 4);
}

void print_u64_deci(u64 val) {
    if (val == 0) {
        putc('0');
        return;
    }

    char buffer[20];    // 20 is max number of digits of a u64 decimal
    int i = 0;

    while (val > 0) {
        buffer[i++] = (val % 10) + '0';     // use '0' ascii to align ascii with our values
        val /= 10;
    }

    while (i-- > 0) {
        putc(buffer[i]);
    }

}

void print_u64_oct(u64 val) {
    if (val == 0) {
        putc('0');
        return;
    }

    char buffer[22];
    int i = 0;

    while (val > 0) {
        u8 digit = val & 0x07;
        buffer[i++] = digit + '0';
        val >>= 3;
    }

    while (i-- > 0) {
        putc(buffer[i]);
    }
}

void print_u64_hex(u64 val) {
    if (val == 0) {
        putc('0');
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
        putc(buffer[i]);
    }

}

void print_u64_bin(u64 val) {
    char buffer[64];

    for(u8 i = 0; i < 64; i++) {
        buffer[i] = (val & 0x1) + '0';
        val >>= 1;
    }

    for(u8 i = 64; i > 0; i--) {
        putc(buffer[i - 1]);
    }
}

void printf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            switch (*fmt) {
                case 'c': {
                    char c = (char) va_arg(args, int);
                    putc(c);
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    puts(str);
                    break;
                }
                case 'u': {
                    u32 val = va_arg(args, u32);
                    print_u64_deci(val);
                    break;
                }
                case 'x': {
                    u32 val = va_arg(args, u32);
                    print_u64_hex(val);
                    break;
                }
                case 'o': {
                    u32 val = va_arg(args, u32);
                    print_u64_oct(val);
                    break;
                }
                case 'p':   {
                    u64 val = (u64)va_arg(args, void*);

                    puts("0x");
                    print_u64_hex(val);
                    break;
                }
                case '%': {
                    putc('%');
                    break;
                }
                default: {
                    putc('%');
                    putc(*fmt);
                    break;
                }
            }
        } else {
            putc(*fmt);
        }
        
        fmt++;
    }
    
    va_end(args);
}

void cursor_default(void) {
    cursor_enable(14, 15);
}

// 0 - 15: top of char - bottom
void cursor_enable(u8 start, u8 end) {
    port_outb(0x3D4, 0x0A);
    port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | start);

    port_outb(0x3D4, 0x0B);
    port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | end);
}

void cursor_disable(void) {
    port_outb(0x3D4, 0x0A);
    port_outb(0x3D5, 0x20);
}

void cursor_move(u8 x, u8 y) {
    u16 pos = x + (NUM_COLS * y);

    port_outb(0x3D4, 0x0F);
    port_outb(0x3D5, (u8) (pos & 0xFF));

    port_outb(0x3D4, 0x0E);
    port_outb(0x3D5, (u8) ((pos >> 8) & 0xFF));
}

u16 cursor_pos(void) {
    u16 pos = 0;

    port_outb(0x3d4, 0x0F);
    pos |= port_inb(0x3D5);

    port_outb(0x3D4, 0x0E);
    pos |= ((u16) port_inb(0x3D5)) << 8;

    return pos;
}