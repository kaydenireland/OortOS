.code32

.globl _start32
_start32:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %ss
    movw %ax, %gs
    
    # set stack pointers
    movl $0x9C00, %ebp
    movl %ebp, %esp

loop32:
    jmp loop32
