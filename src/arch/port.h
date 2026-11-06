#ifndef PORT_H
#define PORT_H

#include "../util/types.h"

u8 port_inb(u16 port_id);   // get input from given port
void port_outb(u16 port_id, u8 data);   // send output to port
void port_wait(void);

#endif