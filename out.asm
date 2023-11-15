global main

extern printf

section .data
	formatStr	db '%d', 0xA

section .text

main:
	mov		r8, 12		; load 12 into r8
	mov		r9, 3		; load 3 into r9
	imul		r9, r8		; mul r9, r8
	push	rbp		; printint
	mov		rdi, formatStr
	mov		rsi, r9
	mov		al, 0
	call	printf
	pop		rbp
	mov		r8, 18		; load 18 into r8
	mov		r9, 2		; load 2 into r9
	mov		r10, 4		; load 4 into r10
	imul		r10, r9		; mul r10, r9
	sub		r8, r10		; sub r8, r10
	push	rbp		; printint
	mov		rdi, formatStr
	mov		rsi, r8
	mov		al, 0
	call	printf
	pop		rbp
	mov		r8, 1		; load 1 into r8
	mov		r9, 2		; load 2 into r9
	add		r9, r8		; add r9, r8
	mov		r8, 9		; load 9 into r8
	add		r8, r9		; add r8, r9
	mov		r9, 5		; load 5 into r9
	mov		r10, 2		; load 2 into r10
	mov		rax, r9		; div r9, r10
	xor		rdx, rdx
	idiv	r10
	mov		r9, rax		; result in rax, rest in rdx
	sub		r8, r9		; sub r8, r9
	mov		r9, 3		; load 3 into r9
	mov		r10, 5		; load 5 into r10
	imul		r10, r9		; mul r10, r9
	add		r10, r8		; add r10, r8
	push	rbp		; printint
	mov		rdi, formatStr
	mov		rsi, r10
	mov		al, 0
	call	printf
	pop		rbp

	; exit syscall
	mov		rax, 60		; número de syscall para exit
	xor		rdi, rdi		; código de salida 0
	syscall
