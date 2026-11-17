#ifndef PORT_H
#define PORT_H

#include "../util/numbers.h"

u8 inb(u16 port_id);   // get input from given port
void outb(u16 port_id, u8 data);   // send output to port
void portw(void);   // shotr delay

#endif