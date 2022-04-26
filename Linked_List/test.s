	.file	"LinkedList.c"
	.text
	.def	___main;	.scl	2;	.type	32;	.endef
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB19:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	call	___main
	movl	$0, 28(%esp)
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	_makeList
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	_printList
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	_delIndexElement
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	_printList
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	_freeMemory
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE19:
	.section .rdata,"dr"
	.align 4
LC0:
	.ascii "Enter list elements (whole numbers) one by one. To finish the input enter 0\0"
	.align 4
LC1:
	.ascii "Bad input. Make sure you enter only whole numbers \0"
	.text
	.globl	_makeList
	.def	_makeList;	.scl	2;	.type	32;	.endef
_makeList:
LFB20:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -20(%ebp)
	movl	$0, -12(%ebp)
	movl	$LC0, (%esp)
	call	_puts
	jmp	L4
L5:
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_getInt
	movl	%eax, -16(%ebp)
	movl	-24(%ebp), %eax
	testl	%eax, %eax
	jne	L4
	movl	$LC1, (%esp)
	call	_puts
L4:
	movl	-24(%ebp), %eax
	cmpl	$1, %eax
	jne	L5
	movl	$0, -24(%ebp)
	cmpl	$0, -16(%ebp)
	je	L12
	movl	$8, (%esp)
	call	_malloc
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	L8
	movl	8(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	%edx, (%eax)
	jmp	L9
L8:
	movl	-12(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	%edx, 4(%eax)
L9:
	movl	-20(%ebp), %eax
	movl	$0, 4(%eax)
	movl	-20(%ebp), %eax
	movl	%eax, -12(%ebp)
	jmp	L4
L12:
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE20:
	.section .rdata,"dr"
LC2:
	.ascii "%9d%c\0"
	.text
	.globl	_getInt
	.def	_getInt;	.scl	2;	.type	32;	.endef
_getInt:
LFB21:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	nop
	leal	-13(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC2, (%esp)
	call	_scanf
	cmpl	$2, %eax
	je	L19
L17:
	nop
L15:
	call	_getchar
	cmpl	$10, %eax
	jne	L15
	movl	8(%ebp), %eax
	movl	$0, (%eax)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	jmp	L18
L19:
	movzbl	-13(%ebp), %eax
	cmpb	$10, %al
	jne	L17
	movl	8(%ebp), %eax
	movl	$1, (%eax)
	movl	-12(%ebp), %eax
L18:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE21:
	.section .rdata,"dr"
	.align 4
LC3:
	.ascii "\12================ Linked list elements =============\0"
LC4:
	.ascii "Element %d is: %d\12\0"
	.text
	.globl	_printList
	.def	_printList;	.scl	2;	.type	32;	.endef
_printList:
LFB22:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	$1, -16(%ebp)
	movl	$LC3, (%esp)
	call	_puts
L21:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC4, (%esp)
	call	_printf
	addl	$1, -16(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	L21
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC4, (%esp)
	call	_printf
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE22:
	.section .rdata,"dr"
	.align 4
LC5:
	.ascii "\12Enter which element would you like to delete\0"
	.align 4
LC6:
	.ascii "Chosen element exceeds the size of the list\0"
LC7:
	.ascii "Deleted element %d was: %d\12\0"
	.text
	.globl	_delIndexElement
	.def	_delIndexElement;	.scl	2;	.type	32;	.endef
_delIndexElement:
LFB23:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$56, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	$0, -16(%ebp)
L23:
	movl	$LC5, (%esp)
	call	_puts
	movl	$0, -32(%ebp)
	jmp	L24
L25:
	leal	-32(%ebp), %eax
	movl	%eax, (%esp)
	call	_getInt
	movl	%eax, -24(%ebp)
	movl	-32(%ebp), %eax
	testl	%eax, %eax
	jne	L24
	movl	$LC1, (%esp)
	call	_puts
L24:
	movl	-32(%ebp), %eax
	cmpl	$1, %eax
	jne	L25
	cmpl	$1, -24(%ebp)
	jne	L28
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -20(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	jmp	L27
L31:
	cmpl	$0, -24(%ebp)
	jle	L29
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	L30
L29:
	movl	$LC6, (%esp)
	call	_printf
	movl	$0, -16(%ebp)
	movl	$0, -32(%ebp)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	jmp	L23
L30:
	addl	$1, -16(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -12(%ebp)
L28:
	movl	-16(%ebp), %eax
	addl	$2, %eax
	cmpl	%eax, -24(%ebp)
	jne	L31
	movl	-12(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -20(%ebp)
	addl	$1, -16(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %edx
	movl	-28(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L27:
	movl	-16(%ebp), %eax
	leal	1(%eax), %edx
	movl	-20(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	$LC7, (%esp)
	call	_printf
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE23:
	.section .rdata,"dr"
LC8:
	.ascii "\12====== Memory freed ======\0"
	.text
	.globl	_freeMemory
	.def	_freeMemory;	.scl	2;	.type	32;	.endef
_freeMemory:
LFB24:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	jmp	L33
L34:
	movl	8(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, 8(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L33:
	cmpl	$0, 8(%ebp)
	jne	L34
	movl	$LC8, (%esp)
	call	_puts
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE24:
	.ident	"GCC: (MinGW.org GCC Build-20200227-1) 9.2.0"
	.def	_puts;	.scl	2;	.type	32;	.endef
	.def	_malloc;	.scl	2;	.type	32;	.endef
	.def	_scanf;	.scl	2;	.type	32;	.endef
	.def	_getchar;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_free;	.scl	2;	.type	32;	.endef
