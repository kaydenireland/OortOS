.globl _start

.code16 # 16 bit real mode
_start:

    # move stack pointer just before the bootloader, as the stack travels downward and we do not need much space currently
    movw $0x7C00, %bp
    movw %bp, %sp   # if the stack is empty, point to the sp

    movb $0x0E, %ah

    movb $'O', %al
    int $0x10
    movb $'o', %al
    int $0x10
    movb $'r', %al
    int $0x10
    movb $'t', %al
    int $0x10
    movb $'O', %al
    int $0x10
    movb $'S', %al
    int $0x10

.loop: # infinite loop
    jmp .loop 

# padding and magic num
.org 510 
.word 0xAA55
