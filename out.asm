global main

extern printf

section .data
	formatStr	db '%d', 0xA

section .text

main:
	mov		r8, 20		; load 20 into r8
	mov		r9, 30		; load 30 into r9
	mov		r10, 2		; load 2 into r10
	mov		rax, r9		; div r9, r10
	xor		rdx, rdx
	idiv	r10
	mov		r9, rax		; result in rax, rest in rdx
	add		r9, r8		; add r9, r8
	push		rbp		; printint
	mov		rdi, formatStr
	mov		rsi, r9
	mov		al, 0
	call		printf
	pop		rbp

	; exit syscall
	mov		rax, 60		; número de syscall para exit
	xor		rdi, rdi		; código de salida 0
	syscall
