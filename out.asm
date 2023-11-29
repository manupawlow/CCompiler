extern printf

section .data
	_format db '%d', 0xA

section .bss
	a: resb 1
section .bss
	b: resb 8
section .bss
	c: resb 1
section .bss
	d: resb 4
section .bss
	e: resb 8
section .bss
	f: resb 4
section	.text
global	main
main:
	push 	rbp
	mov  	rbp, rsp
	mov  	r8, 18		; r8 = 18
	mov  	[a], r8b
	movzx	r8, byte [a]

	mov  	rdi, _format	; printf r8
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, a
	mov  	[b], r8
	mov  	r8, [b]
	mov  	[c], r8b
	movzx	r8, byte [c]

	mov  	rdi, _format	; printf r8
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, 12		; r8 = 12
	mov  	[d], r8d
	xor  	r8, r8
	mov  	r8d, dword [d]

	mov  	rdi, _format	; printf r8
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, d
	mov  	[e], r8
	mov  	r8, [e]
	mov  	[f], r8d
	xor  	r8, r8
	mov  	r8d, dword [f]

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
