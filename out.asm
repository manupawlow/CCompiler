extern printf

section .data
	_format db '%d', 0xA, 0
	_format2 db '%c', 0

section .bss
	a: resb 4

section .bss
	b: resb 4

section .bss
	c: resb 4

section	.text
global	main
main:
	push 	rbp
	mov  	rbp, rsp
	add  	rsp, -16

	mov  	r8, 10		; r8 = 10
	mov  	dword [rbp + -12], r8d
	mov  	r8, 20		; r8 = 20
	mov  	dword [rbp + -8], r8d
	mov  	r8, 30		; r8 = 30
	mov  	byte [rbp + -4], r8b
	movzx	r8, byte [rbp + -4]
	movsx 	r9, word [rbp + -12]
	movsxd	r9, r9d
	add  	r9, r8		; r9 = r9 + r8
	mov  	r8, 2		; r8 = 2
	add  	r8, r9		; r8 = r8 + r9
	mov  	dword [rbp + -8], r8d
	movsx 	r8, word [rbp + -12]
	movsxd	r8, r8d
	movzx	r9, byte [rbp + -4]
	add  	r9, r8		; r9 = r9 + r8

	mov  	rdi, _format	; printf r9
	mov  	rsi, r9
	mov  	al, 0
	call 	printf

	movsx 	r8, word [rbp + -8]
	movsxd	r8, r8d

	mov  	rdi, _format	; printf r8
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, 0		; r8 = 0
	mov  	eax, r8d
	jmp  	L1		; goto L1

L1:
	add  	rsp, 16
	pop  	rbp
	ret
