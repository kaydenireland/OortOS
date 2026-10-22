.code32

.equ TABLE_SIZE, 4096

.globl setup_page_tables
setup_page_tables:
    # Clear all three page tables.
    cld
    xorl %eax, %eax
    movl $page_table_l4, %edi
    movl $(3 * TABLE_SIZE / 4), %ecx
    rep stosl

    movl $page_table_l3, %eax
    orl $0b11, %eax     # present, writable flags
    movl %eax, page_table_l4

    movl $page_table_l2, %eax
    orl $0b11, %eax     # present, writable flags
    movl %eax, page_table_l3

    # fill every page 2 entry
    movl $0, %ecx        # counter

    .fill_pages:

        movl %ecx, %eax
        shll $21, %eax
        orl $0x83, %eax
        movl %eax, page_table_l2(,%ecx, 8)

        incl %ecx            # increment counter
        cmp $512, %ecx      # check if whole table is mapped
        jne .fill_pages
        ret

enable_paging:
    # pass page table location to cpu
    movl $page_table_l4, %eax
    movl %eax, %cr3

    # enable pae flag in %cr4
    movl %cr4, %eax
    orl $(1 << 5), %eax
    movl %eax, %cr4

    # enable long mode
    movl $0xC0000080, %ecx
    rdmsr
    orl $(1 << 8), %eax
    wrmsr

    # enable paging
    movl %cr0, %eax
    orl $(1 << 31), %eax
    movl %eax, %cr0
    ret


.align TABLE_SIZE

page_table_l4: 
    .space TABLE_SIZE
page_table_l3: 
    .space TABLE_SIZE
page_table_l2: 
    .space TABLE_SIZE
