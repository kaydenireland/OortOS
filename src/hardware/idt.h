#ifndef IDT_H
#define IDT_H

#include "../kernel/types.h"

void idt_init(void);
void idt_set_pit_handler(void (*handler)());
void idt_set_keyboard_handler(void (*handler)());

#endif