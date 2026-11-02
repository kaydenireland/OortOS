.extern idt_keyboard_handler
.globl idt_load

idt_load:
    lidt (%rdi)
    ret

.macro wrapped_handler handler
    .globl \handler\()_wrapped

    \handler\()_wrapped:

		cld		# C functions expect clear direction flag

        # save general purpose registers so they can be restored after the interrupt is handled

        push %rax
		push %rbx
		push %rcx
		push %rdx
		push %rbp
		push %rsi
		push %rdi
		push %r8
		push %r9
		push %r10
		push %r11
		push %r12
		push %r13
		push %r14
		push %r15
		
        mov %rsp, %r12	# preserve the current stack pointer
        and $-16, %rsp	# align the stack to 16 bytes
        call \handler	# call C handler
        mov %r12, %rsp	# restore original stack pointer
		
		# restore general-purpose registers
		pop %r15
		pop %r14
		pop %r13
		pop %r12
		pop %r11
		pop %r10
		pop %r9
		pop %r8
		pop %rdi
		pop %rsi
		pop %rbp
		pop %rdx
		pop %rcx
		pop %rbx
		pop %rax
		
		iretq
.endm

wrapped_handler idt_keyboard_handler
