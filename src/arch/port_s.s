.code64

.globl port_inb
.globl port_outb

port_inb:
    movw %di, %dx       # read input parameter
    inb %dx, %al        # reads port from %dx
    movzx %al, %rax     # zero extend
    ret

port_outb:
    movw %di, %dx
    movb %sil, %al
    outb %al, %dx
    ret
