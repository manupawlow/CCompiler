extern printf

section .data
	_printint_format db '%d', 0xA

	section	.text
	global	main
main:
	push 	rbp
	mov  	rbp, rsp
	mov  	r8, 20		; r8 <- 20
	mov  	r9, 15		; r9 <- 15
	add  	r9, r8		; r9 + r8
	mov  	r8, 25		; r8 <- 25
	mov  	r10, 5		; r10 <- 5
	mov  	rax, r8		; r8 / r10
	xor  	rdx, rdx
	idiv 	r10
	mov  	r8, rax
	sub  	r9, r8		; r9 - r8

	mov  	rdi, _printint_format
	mov  	rsi, r9
	mov  	al, 0
	call 	printf

	mov  	eax, 0
	pop  	rbp
	ret
