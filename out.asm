extern printf

section .data
	_printint_format db '%d', 0xA

section .bss
	i: resb 4
section .bss
	j: resb 4
section	.text
global	main
main:
	push 	rbp
	mov  	rbp, rsp
	mov  	r8, 6		; r8 = 6
	mov  	[i], r8		; i = r8
	mov  	r8, 12		; r8 = 12
	mov  	[j], r8		; j = r8
	mov  	r8, [i]
	mov  	r9, [j]
	cmp	r8, r9
	jge	L1
	mov  	r8, [i]

	mov  	rdi, _printint_format
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	jmp  	L2
L1:
	mov  	r8, [j]

	mov  	rdi, _printint_format
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

L2:
	mov  	eax, 0
	pop  	rbp
	ret
