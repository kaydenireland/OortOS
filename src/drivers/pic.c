#include "pic.h"

#include "../util/numbers.h"
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
	outb(PIC1_COMMAND_PORT, PIC_EOI);
	portw();
}

void pic_eoi_slave(void) {
	outb(PIC2_COMMAND_PORT, PIC_EOI);
	portw();
}

void pic_remap(void) {
    // ICW1 - begin initialization sequence
	outb(PIC1_COMMAND_PORT, ICW1_INIT | ICW1_HAS_ICW4); // master
	portw();
	outb(PIC2_COMMAND_PORT, ICW1_INIT | ICW1_HAS_ICW4); // slave
	portw();
	
	// ICW2 - update interrupt vector offsets
	outb(PIC1_DATA_PORT, PIC_OFFSET_MASTER);
	portw();
	outb(PIC2_DATA_PORT, PIC_OFFSET_SLAVE);
	portw();
	
	// ICW3 - configure cascading between master and slave
	outb(PIC1_DATA_PORT, ICW3_IRQ2_HAS_SLAVE);
	portw();
	outb(PIC2_DATA_PORT, IRQ2_SLAVE);
	portw();
	
	// ICW4 - configure 8086 mode
	outb(PIC1_DATA_PORT, ICW4_MODE_8086);
	portw();
	outb(PIC2_DATA_PORT, ICW4_MODE_8086);
	portw();
	
	// Mask (ignore) all interrupts
	outb(PIC1_DATA_PORT, 0xFF);
	portw();
	outb(PIC2_DATA_PORT, 0xFF);
	portw();
	
	// Unmask desired interrupts
	u8 mask_master = inb(PIC1_DATA_PORT);
	u8 mask_slave = inb(PIC2_DATA_PORT);
	mask_master &= ~(1 << IRQ0_TIMER);
	mask_master &= ~(1 << IRQ1_KEYBOARD);
	outb(PIC1_DATA_PORT, mask_master);
	portw();
	outb(PIC2_DATA_PORT, mask_slave);
	portw();
	
	// portw for outstanding interrupts to clear
	pic_eoi_master();
	pic_eoi_slave();
}
