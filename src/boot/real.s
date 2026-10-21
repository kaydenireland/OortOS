.section .text
.include "src/boot/bprint.s"
.include "src/boot/gdt.s"

.globl _start
.code16                 # 16 bit real mode

_start:

    cli                 # this section cannot be interrupted
    movw $0, %ax        # load 0 into %ax
    movw %ax, %ds       # set data segment to 0
    movw %ax, %es       #set extra stack segment to 0
    movw %ax, %ss       # set stack segment to 0

    movw $0x7C00, %bp   # set stack base pointer just before bootloader
    movw %bp, %sp       # set stack pointer
    sti                 # turn interrupts back on

    # enable A20 address line
    inb $0x92, %al
    orb $2, %al
    outb %al, $0x92

    jmp load_gdt        # load gdt table to switch to protected mode


loop16: # infinite loop
    jmp loop16

# padding and magic num
.org 510 
.word 0xAA55
