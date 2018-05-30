	.file	"protect.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	" "
.LC2:
	.string	"Excption! --> "
.LC3:
	.string	"\n\n"
.LC4:
	.string	"EFLAGS: "
.LC5:
	.string	"CS: "
.LC6:
	.string	"EIP: "
.LC7:
	.string	"Error Code:"
	.section	.rodata
	.align 32
.LC0:
	.string	"#DE Divide Error"
	.zero	47
	.string	"#DB RESERVED"
	.zero	51
	.string	"- NMI Interrupt"
	.zero	48
	.string	"#BP Breakpoint"
	.zero	49
	.string	"#OF Overflow"
	.zero	51
	.string	"#BR BOUND Range Exeeded"
	.zero	40
	.string	"#UD Invalid Opcode (Undefined Opcode)"
	.zero	26
	.string	"#NM Device Not Available (No Math Coprocessor)"
	.zero	17
	.string	"#DF Double Fault"
	.zero	47
	.string	"copprocessor SegmentOverrun(reserved)"
	.zero	26
	.string	"#TS Invalid TSS"
	.zero	48
	.string	"#NP Segment Not Present"
	.zero	40
	.string	"#SS Stack-segment Fault"
	.zero	40
	.string	"#GP General Protection"
	.zero	41
	.string	"#PF General Protection"
	.zero	41
	.string	"- (Intel reserved. Do not use.)"
	.zero	32
	.string	"#MF x87 FPU Floating-point Error(Math Fault)"
	.zero	19
	.string	"#AC Alignment Check"
	.zero	44
	.string	"#MC Machine Check"
	.zero	46
	.string	"#XM SIMD Floation-Point Execption"
	.zero	30
	.string	"#VE Virtualization Exception"
	.zero	35
	.text
	.p2align 4,,15
	.globl	exception_handler
	.type	exception_handler, @function
exception_handler:
.LFB0:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movl	%esi, %r14d
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movslq	%edi, %r13
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rbp
	movl	%edx, %r15d
	movl	$400, %ebx
	subq	$1368, %rsp
	.cfi_def_cfa_offset 1424
	movl	$0, disp_pos(%rip)
	leaq	16(%rsp), %r12
	movl	%ecx, 8(%rsp)
	movl	$168, %ecx
	movl	%r8d, 12(%rsp)
	movq	%r12, %rdi
	rep movsq
	.p2align 4,,10
	.p2align 3
.L2:
	movq	%rbp, %rdi
	call	disp_str@PLT
	subl	$1, %ebx
	jne	.L2
	leaq	.LC2(%rip), %rsi
	movl	$116, %edi
	salq	$6, %r13
	movl	$0, disp_pos(%rip)
	call	disp_color_str@PLT
	leaq	(%r12,%r13), %rsi
	movl	$116, %edi
	call	disp_color_str@PLT
	leaq	.LC3(%rip), %rdi
	call	disp_str@PLT
	leaq	.LC4(%rip), %rsi
	movl	$116, %edi
	call	disp_color_str@PLT
	movl	%r15d, %edi
	call	disp_int@PLT
	leaq	.LC5(%rip), %rsi
	movl	$116, %edi
	call	disp_color_str@PLT
	movl	12(%rsp), %edi
	call	disp_int@PLT
	leaq	.LC6(%rip), %rsi
	movl	$116, %edi
	call	disp_color_str@PLT
	movl	8(%rsp), %edi
	call	disp_int@PLT
	cmpl	$-1, %r14d
	je	.L1
	leaq	.LC7(%rip), %rsi
	movl	$116, %edi
	call	disp_color_str@PLT
	movl	%r14d, %edi
	call	disp_int@PLT
.L1:
	addq	$1368, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	exception_handler, .-exception_handler
	.p2align 4,,15
	.globl	init_idt_desc
	.type	init_idt_desc, @function
init_idt_desc:
.LFB1:
	.cfi_startproc
	leaq	idt(%rip), %rax
	movzbl	%dil, %edi
	movl	$8, %r8d
	orl	%esi, %ecx
	movw	%dx, (%rax,%rdi,8)
	shrq	$16, %rdx
	movw	%r8w, 2(%rax,%rdi,8)
	movb	$0, 4(%rax,%rdi,8)
	movb	%cl, 5(%rax,%rdi,8)
	movw	%dx, 6(%rax,%rdi,8)
	ret
	.cfi_endproc
.LFE1:
	.size	init_idt_desc, .-init_idt_desc
	.p2align 4,,15
	.globl	init_prot
	.type	init_prot, @function
init_prot:
.LFB3:
	.cfi_startproc
	movq	divide_error@GOTPCREL(%rip), %rax
	movl	$-1912602616, 2+idt(%rip)
	movw	%ax, idt(%rip)
	shrq	$16, %rax
	movw	%ax, 6+idt(%rip)
	ret
	.cfi_endproc
.LFE3:
	.size	init_prot, .-init_prot
	.p2align 4,,15
	.globl	seg2phys
	.type	seg2phys, @function
seg2phys:
.LFB4:
	.cfi_startproc
	leaq	gdt(%rip), %rdx
	shrw	$3, %di
	andl	$8191, %edi
	movzbl	7(%rdx,%rdi,8), %eax
	movzbl	4(%rdx,%rdi,8), %ecx
	movzwl	2(%rdx,%rdi,8), %edx
	sall	$24, %eax
	sall	$16, %ecx
	orl	%ecx, %eax
	orl	%edx, %eax
	ret
	.cfi_endproc
.LFE4:
	.size	seg2phys, .-seg2phys
	.ident	"GCC: (Ubuntu 7.2.0-8ubuntu3.2) 7.2.0"
	.section	.note.GNU-stack,"",@progbits
