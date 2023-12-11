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

	mov  	r8, 42		; r8 = 42
	mov  	[a], r8d
	mov  	r8, 19		; r8 = 19
	mov  	[b], r8d
	xor  	r8, r8
	mov  	r8d, dword [a]
	xor  	r9, r9
	mov  	r9d, dword [b]
	and  	r9, r8

	mov  	rdi, _format	; printf r9
	mov  	rsi, r9
	mov  	al, 0
	call 	printf

	xor  	r8, r8
	mov  	r8d, dword [a]
	xor  	r9, r9
	mov  	r9d, dword [b]
	or   	r9, r8

	mov  	rdi, _format	; printf r9
	mov  	rsi, r9
	mov  	al, 0
	call 	printf

	xor  	r8, r8
	mov  	r8d, dword [a]
	xor  	r9, r9
	mov  	r9d, dword [b]
	xor  	r9, r8

	mov  	rdi, _format	; printf r9
	mov  	rsi, r9
	mov  	al, 0
	call 	printf

	mov  	r8, 1		; r8 = 1
	mov  	r9, 3		; r9 = 3
	mov  	cl, r9b
	shl  	r8, cl

	mov  	rdi, _format	; printf r8
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, 63		; r8 = 63
	mov  	r9, 3		; r9 = 3
	mov  	cl, r9b
	shr  	r8, cl

	mov  	rdi, _format	; printf r8
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, 0		; r8 = 0
	mov  	eax, r8d
	jmp  	L1		; goto L1

L1:
	pop  	rbp
	ret
