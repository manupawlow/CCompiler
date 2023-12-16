extern printint
extern printchar

section	.text
global	param8
param8:
	push 	rbp
	mov  	rbp, rsp

	mov  	dword [rbp + -4], edi
	mov  	dword [rbp + -8], esi
	mov  	dword [rbp + -12], edx
	mov  	dword [rbp + -16], ecx
	mov  	dword [rbp + -20], r8d
	mov  	dword [rbp + -24], r9d
	add  	rsp, -32
	movsx 	r10, word [rbp + -4]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	movsx 	r10, word [rbp + -8]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	movsx 	r10, word [rbp + -12]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	movsx 	r10, word [rbp + -16]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	movsx 	r10, word [rbp + -20]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	movsx 	r10, word [rbp + -24]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	movsx 	r10, word [rbp + 16]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	movsx 	r10, word [rbp + 24]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	mov  	r10, 0		; r10 = 0
	mov  	eax, r10d
	jmp  	L1		; goto L1

L1:
	add  	rsp, 32
	pop  	rbp
	ret
section	.text
global	fred
fred:
	push 	rbp
	mov  	rbp, rsp

	mov  	dword [rbp + -4], edi
	mov  	dword [rbp + -8], esi
	mov  	dword [rbp + -12], edx
	add  	rsp, -16
	movsx 	r10, word [rbp + -4]
	movsxd	r10, r10d
	movsx 	r11, word [rbp + -8]
	movsxd	r11, r11d
	add  	r11, r10		; r11 = r11 + r10
	movsx 	r10, word [rbp + -12]
	movsxd	r10, r10d
	add  	r10, r11		; r10 = r10 + r11
	mov  	eax, r10d
	jmp  	L2		; goto L2

L2:
	add  	rsp, 16
	pop  	rbp
	ret
section	.text
global	main
main:
	push 	rbp
	mov  	rbp, rsp

	add  	rsp, -16
	mov  	r11, 34		; r11 = 34
	push 	r11
	mov  	r10, 21		; r10 = 21
	push 	r10
	mov  	r10, 13		; r10 = 13
	mov  	r9, r10
	mov  	r10, 8		; r10 = 8
	mov  	r8, r10
	mov  	r10, 5		; r10 = 5
	mov  	rcx, r10
	mov  	r10, 3		; r10 = 3
	mov  	rdx, r10
	mov  	r10, 2		; r10 = 2
	mov  	rsi, r10
	mov  	r10, 1		; r10 = 1
	mov  	rdi, r10
	call 	param8
	add  	rsp, 16
	mov  	r10, rax
	mov  	r10, 4		; r10 = 4
	mov  	rdx, r10
	mov  	r10, 3		; r10 = 3
	mov  	rsi, r10
	mov  	r10, 2		; r10 = 2
	mov  	rdi, r10
	call 	fred
	mov  	r10, rax
	mov  	dword [rbp + -4], r10d
	movsx 	r10, word [rbp + -4]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r10, rax
	mov  	r10, 0		; r10 = 0
	mov  	eax, r10d
	jmp  	L3		; goto L3

L3:
	add  	rsp, 16
	pop  	rbp
	ret
