.code64

.globl _start64
_start64:

    movw $0x10, %ax
    movw %ax, %ss
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    movabsq $0x9C00, %rsp

    movabsq $0xB8000, %rdi
    movw $0x0F4C, (%rdi)    # L
    movw $0x0F4D, 2(%rdi)   # M
    movw $0x0F20, 4(%rdi)   # space
    movw $0x0F4F, 6(%rdi)   # O
    movw $0x0F4B, 8(%rdi)   # K


loop64:
    jmp loop64