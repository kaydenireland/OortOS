#ifndef IDT_H
#define IDT_H

#include "../util/numbers.h"

void idt_init(void);
void idt_set_pit_handler(void (*handler)(void));
void idt_set_keyboard_handler(void (*handler)(void));
void idt_set_mouse_handler(void (*handler)(void));


#endif