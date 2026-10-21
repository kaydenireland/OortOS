.macro bprint msg
    movw $\msg, %si   # loads address of msg into %si

    1:
        lodsb   # loads string byte of %esi or %rsi and puts it at %al, also increment %si
        orb %al, %al # performs bitwise or on %al and puts it at %al to set ZF/SF
        # zero is end of string
        jz 2f    # jumps to done if the ZF is set

        movb $0x0E, %ah
        movb $0, %bh

        int $0x10
        jmp 1b

    2:
        
.endm


.macro bprintln msg
    bprint \msg

    movb $0x0E, %ah
    movb $0x0A, %al   # Line Feed
    int $0x10
    movb $0x0D, %al   # Carriage Return
    int $0x10
.endm
