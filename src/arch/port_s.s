.code64

.globl inb
.globl outb

inb:
    movw %di, %dx       # read input parameter
    inb %dx, %al        # reads port from %dx
    movzx %al, %rax     # zero extend
    ret

outb:
    movw %di, %dx
    movb %sil, %al
    outb %al, %dx
    ret
