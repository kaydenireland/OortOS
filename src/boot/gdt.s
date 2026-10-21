.section .rodata    # static structures: read-only data
.align 8            # align gdt to an 8-byte boundary

gdt_start:
    # null descriptor
    .long 0x00000000
    .long 0x00000000

    # Code segment descriptor: 0x8
    .word 0xFFFF        # limit
    .word 0x0000        # base
    .byte 0x00          # base
    .byte 0b10011010    # access byte: P DPL S E DC RW A
    .byte 0b11001111    # flags: G DB L Reserved
    .byte 0x00          # base

    # Data segment descriptor: 0x10
    .word 0xFFFF        # limit
    .word 0x0000        # base
    .byte 0x00          # base
    .byte 0b10010010    # access byte: P DPL S E DC RW A
    .byte 0b11001111    # flags: G DB L Reserved
    .byte 0x00          # base

gdt_end:

# global gdt pointer needed by lgdt instruction
.globl gdt_descriptor
gdt_descriptor:
    .word (gdt_end - gdt_start) - 1     # size of gdt table minus 1
    .long gdt_start + 0x7C00 


.section .text  # code

.include "src/boot/protected.s"

.globl load_gdt
load_gdt:
    cli
    lgdt (gdt_descriptor)
    movl %cr0, %eax
    orb $1, %al
    movl %eax, %cr0
    ljmp $0x08, $_start32
    ret
