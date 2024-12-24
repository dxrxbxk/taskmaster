bits 64

section .data
	msg db "Hello, World!", 0

section .text
	global _start

_start:

	pusfq
	pop rax
	or rax, 0x40000
	push rax
	popfq

	lea rdi, [rel msg]

	inc rsi

	mov rax, [rsi]

	mov rax, 60
	xor edi, edi
	syscall

