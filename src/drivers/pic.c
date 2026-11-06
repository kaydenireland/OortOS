#include "pic.h"

#include "../util/types.h"
#include "../arch/port.h"

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

#define IRQ0_TIMER 0x00
#define IRQ1_KEYBOARD 0x01
#define IRQ2_SLAVE 0x02

#define ICW1_INIT 0x10
#define ICW1_HAS_ICW4 0x01

#define ICW3_IRQ2_HAS_SLAVE (1 << IRQ2_SLAVE)

#define ICW4_MODE_8086 (1 << 0)

#define PIC_EOI 0x20
#define PIC_OFFSET_MASTER 0x20
#define PIC_OFFSET_SLAVE 0x28


void pic_eoi_master(void) {
	port_outb(PIC1_COMMAND_PORT, PIC_EOI);
	port_wait();
}

void pic_eoi_slave(void) {
	port_outb(PIC2_COMMAND_PORT, PIC_EOI);
	port_wait();
}

void pic_remap(void) {
    // ICW1 - begin initialization sequence
	port_outb(PIC1_COMMAND_PORT, ICW1_INIT | ICW1_HAS_ICW4); // master
	port_wait();
	port_outb(PIC2_COMMAND_PORT, ICW1_INIT | ICW1_HAS_ICW4); // slave
	port_wait();
	
	// ICW2 - update interrupt vector offsets
	port_outb(PIC1_DATA_PORT, PIC_OFFSET_MASTER);
	port_wait();
	port_outb(PIC2_DATA_PORT, PIC_OFFSET_SLAVE);
	port_wait();
	
	// ICW3 - configure cascading between master and slave
	port_outb(PIC1_DATA_PORT, ICW3_IRQ2_HAS_SLAVE);
	port_wait();
	port_outb(PIC2_DATA_PORT, IRQ2_SLAVE);
	port_wait();
	
	// ICW4 - configure 8086 mode
	port_outb(PIC1_DATA_PORT, ICW4_MODE_8086);
	port_wait();
	port_outb(PIC2_DATA_PORT, ICW4_MODE_8086);
	port_wait();
	
	// Mask (ignore) all interrupts
	port_outb(PIC1_DATA_PORT, 0xFF);
	port_wait();
	port_outb(PIC2_DATA_PORT, 0xFF);
	port_wait();
	
	// Unmask desired interrupts
	u8 mask_master = port_inb(PIC1_DATA_PORT);
	u8 mask_slave = port_inb(PIC2_DATA_PORT);
	mask_master &= ~(1 << IRQ0_TIMER);
	mask_master &= ~(1 << IRQ1_KEYBOARD);
	port_outb(PIC1_DATA_PORT, mask_master);
	port_wait();
	port_outb(PIC2_DATA_PORT, mask_slave);
	port_wait();
	
	// Wait for outstanding interrupts to clear
	pic_eoi_master();
	pic_eoi_slave();
}
