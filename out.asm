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
	movsx 	r10, word [a]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [b]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [c]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [d]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [e]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [f]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [g]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [h]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	mov  	r10, 0		; r10 = 0
	mov  	eax, r10d
	jmp  	L1		; goto L1

L1:
	add  	rsp, 32
	pop  	rbp
	ret
section	.text
global	param5
param5:
	push 	rbp
	mov  	rbp, rsp

	mov  	dword [rbp + -4], edi
	mov  	dword [rbp + -8], esi
	mov  	dword [rbp + -12], edx
	mov  	dword [rbp + -16], ecx
	mov  	dword [rbp + -20], r8d
	mov  	dword [rbp + -24], r9d
	add  	rsp, -32
	movsx 	r10, word [a]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [b]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [c]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [d]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [e]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	mov  	r10, 0		; r10 = 0
	mov  	eax, r10d
	jmp  	L2		; goto L2

L2:
	add  	rsp, 32
	pop  	rbp
	ret
section	.text
global	param2
param2:
	push 	rbp
	mov  	rbp, rsp

	mov  	dword [rbp + -4], edi
	mov  	dword [rbp + -8], esi
	mov  	dword [rbp + -12], edx
	mov  	dword [rbp + -16], ecx
	mov  	dword [rbp + -20], r8d
	mov  	dword [rbp + -24], r9d
	add  	rsp, -32
	mov  	r10, 3		; r10 = 3
	mov  	[c], r10d
	mov  	r10, 4		; r10 = 4
	mov  	[d], r10d
	mov  	r10, 5		; r10 = 5
	mov  	[e], r10d
	movsx 	r10, word [a]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [b]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [c]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [d]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [e]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	mov  	r10, 0		; r10 = 0
	mov  	eax, r10d
	jmp  	L3		; goto L3

L3:
	add  	rsp, 32
	pop  	rbp
	ret
section	.text
global	param0
param0:
	push 	rbp
	mov  	rbp, rsp

	mov  	dword [rbp + -4], edi
	mov  	dword [rbp + -8], esi
	mov  	dword [rbp + -12], edx
	mov  	dword [rbp + -16], ecx
	mov  	dword [rbp + -20], r8d
	mov  	dword [rbp + -24], r9d
	add  	rsp, -32
	mov  	r10, 1		; r10 = 1
	mov  	[a], r10d
	mov  	r10, 2		; r10 = 2
	mov  	[b], r10d
	mov  	r10, 3		; r10 = 3
	mov  	[c], r10d
	mov  	r10, 4		; r10 = 4
	mov  	[d], r10d
	mov  	r10, 5		; r10 = 5
	mov  	[e], r10d
	movsx 	r10, word [a]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [b]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [c]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [d]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	movsx 	r10, word [e]
	movsxd	r10, r10d
	mov  	rdi, r10
	call 	printint
	mov  	r11, rax
	mov  	r10, 0		; r10 = 0
	mov  	eax, r10d
	jmp  	L4		; goto L4

L4:
	add  	rsp, 32
	pop  	rbp
	ret
