.section .text

.globl _start
.code16                 # 16 bit real mode

# modules
.include "src/boot/bprint.s"
.include "src/boot/disk_sector.s"

_start:

    cli                 # this section cannot be interrupted
    movw $0, %ax        # load 0 into %ax
    movw %ax, %ds       # set data segment to 0
    movw %ax, %es       #set extra stack segment to 0
    movw %ax, %ss       # set stack segment to 0
    movw $0x7C00, %bp   # set stack base pointer just before bootloader
    movw %bp, %sp       # set stack pointer
    movb %dl, boot_drive
    sti                 # turn interrupts back on

    bprintln oort

    disk_load 40, 0x9000

    bprintln loaded

    # Set 80x25 color text mode
    movb $0x00, %ah
    movb $0x03, %al
    int $0x10

    # enable A20 address line
    inb $0x92, %al
    orb $2, %al
    outb %al, $0x92

    jmp load_gdt        # load gdt table to switch to protected mode

load_gdt:
    cli
    lgdt (gdt_descriptor)
    movl %cr0, %eax
    orb $1, %al
    movl %eax, %cr0
    ljmpl $0x08, $0x9000

loop16: # infinite loop
    jmp loop16


.include "src/boot/gdt.s"

oort:
    .asciz "Booting OortOS."
loaded:
    .asciz "Disk Read Success"

boot_drive:
    .byte 0

# padding and magic num
.org 510
.word 0xAA55
