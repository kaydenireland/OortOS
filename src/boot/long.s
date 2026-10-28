.code64

.extern start_kernel

.globl _start64
_start64:

    movw $0x10, %ax
    movw %ax, %ss
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    mov $0x90000, %rsp  
    
    call start_kernel


loop64:
    jmp loop64
