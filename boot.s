.globl _start

_start:

# infinite loop
loop:
    jmp loop 

# padding and magic num
.org 510 
.word 0xAA55
