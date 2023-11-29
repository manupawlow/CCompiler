extern printf

section .data
	_format db '%d', 0xA

section .bss
	d: resb 4
section .bss
	f: resb 4
section .bss
	e: resb 8
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
	mov  	r8, 3		; r8 = 3
	mov  	[b], r8d
	mov  	r8, 5		; r8 = 5
	mov  	[c], r8d
	xor  	r8, r8
	mov  	r8d, dword [b]
	xor  	r9, r9
	mov  	r9d, dword [c]
	mov  	r10, 10		; r10 = 10
	imul  	r10, r9		; r10 = r10 * r9
	add  	r10, r8		; r10 = r10 + r8
	mov  	[a], r10d
	xor  	r8, r8
	mov  	r8d, dword [a]

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

	lea  	r8, [d]
	mov  	[e], r8
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
