extern printf

section .data
	_format db '%d', 0xA

section .bss
	a: resb 4
section .bss
	b: resb 8
section	.text
global	main
main:
	push 	rbp
	mov  	rbp, rsp
	mov  	r8, 12		; r8 = 12
	lea  	r9, [b]
	mov  	r10, 3		; r10 = 3
	sal  	r10, 2
	add  	r10, r9		; r10 = r10 + r9
	mov  	[r10], r8
	lea  	r8, [b]
	mov  	r9, 3		; r9 = 3
	sal  	r9, 2
	add  	r9, r8		; r9 = r9 + r8
	movzx	r9, word [r9]
	mov  	[a], r9d
	lea  	r8, [b]
	mov  	r9, 3		; r9 = 3
	sal  	r9, 2
	add  	r9, r8		; r9 = r9 + r8
	movzx	r9, word [r9]

	mov  	rdi, _format	; printf r9
	mov  	rsi, r9
	mov  	al, 0
	call 	printf

	lea  	r8, [b]
	mov  	r9, 4		; r9 = 4
	sal  	r9, 2
	add  	r9, r8		; r9 = r9 + r8
	movzx	r9, word [r9]

	mov  	rdi, _format	; printf r9
	mov  	rsi, r9
	mov  	al, 0
	call 	printf

	xor  	r8, r8
	mov  	r8d, dword [a]

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
