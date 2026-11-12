#ifndef DEBUG_H
#define DEBUG_H

void dbg_putc(char c);
void dbg_puts(char* str);
void dbg_putln(char* str);
void dbg_err(char* code, char* msg);

void dumpr(void);

// void dbg_dumpr(Registers* regs); // dump register values

#endif