global main

extern printf

section .data
	formatStr	db '%d', 0xA

section .text

main:
	mov		r8, 5		; load 5 into r8
	mov		r9, 0		; load 0 into r9
	mov		r8, 12		; load 12 into r8
	mov		r9, 1		; load 1 into r9
	mov	fred, [r8]
	mov	jim, [r9]
	add		r9, r8		; add r9, r8
	push	rbp		; printint
	mov		rdi, formatStr
	mov		rsi, r9
	mov		al, 0
	call	printf
	pop		rbp

	; exit syscall
	mov		rax, 60		; número de syscall para exit
	xor		rdi, rdi		; código de salida 0
	syscall
