.globl _start
.code16

_start:

    # move stack pointer just before the bootloader, as the stack travels downward
    movw $0x7C00, %sp
    mov $message, %si

puts:   # puts registers onto the stack
    pushw %si
    pushw %ax
    pushw %bx


.loop: # infinite loop
    lodsb   # loads string byte of %esi or %rsi and puts it at %al
    orb %al, %al # performs bitwise or on %al and puts it at %al to set ZF/SF

    jz .done    # jumps to .done if the ZF is set

    movb $0x0E, %ah
    movb $0, %bh

    int $0x10
    jmp .loop 

.done:  # pops registers off of the stack in LIFO
    popw %bx
    popw %ax
    popw %si
    ret


message:
    .asciz "OortOS"

# padding and magic num
.org 510 
.word 0xAA55
