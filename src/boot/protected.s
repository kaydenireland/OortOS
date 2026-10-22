.code32

.equ EFLAGS_ID, 1 << 21
.equ CPUID_EXT_LM, 1 << 29  # if this is set, the cpu supports long mode

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

    call check_cpuid
    testw %ax, %ax
    jz no_cpuid


    call check_long_mode
    cmpw $2, %ax
    jne no_long_mode

    call setup_page_tables
    call enable_paging

    ljmpl $0x18, $_start64


    jmp loop32

check_cpuid:
    pushfl              # push flags register onto stack
    popl %eax           # pop flags register into %eax
    movl %eax, %ecx     # original value is copied so it can be saved for restoration

    xorl $EFLAGS_ID, %eax

    # storing the eflags and then retrieving it again will show whether or not the bit could successfully be flipped
    pushl %eax          # save to eflags
    popfl
    pushfl              # restore from eflags
    popl %eax

    pushl %ecx
    popfl

    # if the bit in %eax was sucessfully flipped such that %eax != %ecx, cpuid is supported
    xorl %ecx, %eax
    jnz .supported

    .not_supported:
        movw $0, %ax
        ret
    .supported:
        movw $1, %ax
        ret

check_long_mode:
    # cpuid will see that magic number and place a number larger back into %eax if cpu supports extended processor info
    movl $0x80000000, %eax
    cpuid   
    cmp $0x80000001, %eax
    jb .no_long_mode

    movl $0x80000001, %eax
    cpuid
    testl $0x20000000, %edx
    jz .no_long_mode
    
    .has_long_mode:
        movw $2, %ax
        ret

    .no_long_mode:
        movw $3, %ax
        ret

no_cpuid:
    jmp no_long_mode

no_long_mode:
    jmp loop32

.include "src/boot/gdt.s"
.include "src/boot/page_table.s"
.include "src/boot/long.s"

loop32:
    jmp loop32
