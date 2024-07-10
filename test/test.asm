	.globl _start
_start:
	push	%rbp
	mov		%rsp,%rbp

	call	main	

	sub		$0x20, %rsp
	mov		%rsp, %rsi
	mov		$4, %rdx
	mov		$1,%rdi

	movl	$0x000A3936, (%rsp)

	mov		$0x1,%rax
	syscall

	mov		$0x7,%rsp

	mov		$0x0,%rdi
	mov		$0x3c,%rax
	syscall
	ret

	.globl	main
	.type	main, @function
main:
	mov		$0, %rax
	ret
