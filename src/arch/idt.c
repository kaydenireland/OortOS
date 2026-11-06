#include "idt.h"
#include "../drivers/pic.h"
#include "gdt.h"
#include "../util/debug.h"

#define IDT_IRQ0_TIMER 0x20
#define IDT_IRQ1_KEYBOARD 0x21

#define IDT_GATE_PRESENT (1 << 7)

// privilege levels
#define IDT_GATE_DPL0 (0b00 << 5)   // ring 0
#define IDT_GATE_DPL1 (0b01 << 5)   // ring 1
#define IDT_GATE_DPL2 (0b10 << 5)   // ring 2
#define IDT_GATE_DPL3 (0b11 << 5)   // ring 3

#define IDT_GATE_TYPE_INTERRUPT 0xE

#define IDT_ENTRY_TYPE_INTERRUPT (IDT_GATE_PRESENT | IDT_GATE_DPL0 | IDT_GATE_TYPE_INTERRUPT)

extern void idt_pit_handler_wrapped(void);
extern void idt_keyboard_handler_wrapped(void);

struct IdtEntry {
    u16 offset_low;
    u16 selector;
    u8 ist;
    u8 type;
    u16 offset_mid;
    u32 offset_high;
    u32 reserved;
} __attribute__((packed)); // disables possible padding from C

struct IdtPtr {
    u16 limit;
    u64 base;
} __attribute__((packed));  

struct IdtEntry idt[256] __attribute__((aligned(16)));
struct IdtPtr idt_ptr;

extern void idt_load(struct IdtPtr *idt_ptr);

void (*idt_pit_handler_user)();
void (*idt_keyboard_handler_user)();


void idt_set_entry(u8 vector, u64 isr_addr, u16 selector, u8 type) {
	idt[vector] = (struct IdtEntry) {
		.offset_low = (u16) (isr_addr >> 0),
		.selector = selector,
		.ist = 0,
		.type = type,
		.offset_mid = (u16) (isr_addr >> 16),
		.offset_high = (u32) (isr_addr >> 32),
		.reserved = 0,
	};
}

void idt_pit_handler(void) {
    if (idt_pit_handler_user != null) {
        idt_pit_handler_user();
    }

    pic_eoi_master();
}

void idt_keyboard_handler(void) {
    if (idt_keyboard_handler_user != null) {
        idt_keyboard_handler_user();
    }

    pic_eoi_master();
}

void idt_init(void) {
    dbg_putln("Initializng Interrupt Descriptor Table");
    pic_remap();

    idt_ptr.limit = (sizeof(struct IdtEntry) * 256) - 1;
    idt_ptr.base = (u64) &idt[0];

    idt_set_entry(
        IDT_IRQ0_TIMER,
        (u64) idt_pit_handler_wrapped,
        GDT_SELECTOR_CS_KERNEL,
        IDT_ENTRY_TYPE_INTERRUPT
    );

    idt_set_entry(
        IDT_IRQ1_KEYBOARD,
        (u64) idt_keyboard_handler_wrapped,
        GDT_SELECTOR_CS_KERNEL,
        IDT_ENTRY_TYPE_INTERRUPT
    );

    idt_load(&idt_ptr);

    asm volatile("sti");
}

void idt_set_pit_handler(void (*handler)(void)) {
    idt_pit_handler_user = handler;
}

void idt_set_keyboard_handler(void (*handler)()) {
	idt_keyboard_handler_user = handler;
}
