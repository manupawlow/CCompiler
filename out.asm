global main

extern printf

section .data
	_printint_format db '%d', 0xA

section .text

main:
	common	i 8:8
	mov  	r8, 1		; load 1 into r8
	mov  	[i], r8
L1:
	mov  	r8, [i]	mov  	r9, 10		; load 10 into r9
	cmp	r8, r9
	jg	L2
	mov  	r8, [i]	push 	rbp		; printint
	mov  	rdi, _printint_format
	mov  	rsi, r8
	mov  	al, 0
	call 	printf
	pop  	rbp
	mov  	r8, [i]	mov  	r9, 1		; load 1 into r9
	add  	r9, r8		; add r9, r8
	mov  	[i], r9
	jmp  	L1
L2:

	mov  	rax, 60		; exit syscall
	xor  	rdi, rdi		; exit code 0
	syscall
