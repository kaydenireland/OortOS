.code16
.macro disk_load num_sectors, load_offset
    jmp load

    DISK_ERROR_MSG: 
            .asciz "Disk Read Error"

    dap:
        .byte 0x10, 0
        .word \num_sectors
        .word \load_offset
        .word 0
        .long 1
        .long 0
    
    load:
        pusha                       # push all 16-bit registers to the stack
        
        # location
        xorw %ax, %ax
        movw %ax, %dx
        movw %ax, %es
        movw $dap, %si      

        movb boot_drive, %dl
        movb $0x42, %ah             # what task to perform: 0x02 'read', 0x42 'extended read'


        int $0x13   
        jc disk_error               # checks carry bit

        popa
        jmp load_finished


    disk_error:
        popa
        bprintln DISK_ERROR_MSG
        jmp load_finished
    
    load_finished:

.endm
