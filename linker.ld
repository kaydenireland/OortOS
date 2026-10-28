ENTRY(_start)

SECTIONS
{
    /* BIOS loads boot sector at 0x7C00 */
    . = 0x7C00;

    .text : {
        /* Real mode entry is compiled first */
        *(.text.boot)
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    . = 0x7C00 + 510;
    .bootsig : {
        SHORT(0xAA55)
    }
}
