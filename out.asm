extern printf

section .data
	_format db '%d', 0xA

section .bss
	c: resb 4
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
	mov  	r8, 12		; r8 = 12
	mov  	[c], r8d
	mov  	r8, 18		; r8 = 18
	mov  	[d], r8d
	xor  	r8, r8
	mov  	r8d, dword [c]

	mov  	rdi, _format	; printf r8
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	lea  	r8, [c]
	mov  	r9, 1		; r9 = 1
	sal  	r9, 2
	add  	r9, r8		; r9 = r9 + r8
	mov  	[e], r9
	mov  	r8, [e]
	movzx	r8, word [r8]
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
