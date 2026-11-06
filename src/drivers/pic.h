#ifndef PIC_H
#define PIC_H

// Programmable Interrupt Controller

void pic_remap(void);
void pic_eoi_master(void);
void pic_eoi_slave(void);

#endif