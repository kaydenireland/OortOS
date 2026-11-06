#include "debug.h"
#include <stdarg.h>

#include "../util/types.h"


void dbg_putc(char c) {
    port_outb(0xE9, c);
}

void dbg_puts(char* str) {
    while (*str) {
        dbg_putc(*str);
        str++;
    }
}

void dbg_putln(char* str) {
    dbg_puts(str);
    dbg_putc('\n');
}

void dbg_err(char* code, char* msg) {
    dbg_puts("ERROR ");
    dbg_puts(code);
    dbg_puts(": ");
    dbg_putln(msg);
}