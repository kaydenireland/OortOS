.text
.include "src/boot/print.s"

.globl _start
.code16 # 16 bit real mode

_start:

    xor %ax, %ax
    movw %ax, %ds
    movw %ax, %ss   # stack starts at zero

    movw $0x7C00, %bp   
    movw %bp, %sp   # set stack pointer

    cld     # clears DF flag

    bprintln welcome
    bprintln hello

hang: # infinite loop
    jmp hang

welcome:
    .asciz "Welcome to OortOS!"

hello:
    .asciz "Hello, World!"

# padding and magic num
.org 510 
.word 0xAA55
