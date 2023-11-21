extern printf

section .data
	_printint_format db '%d', 0xA

	common	i 8:8
	common	j 1:1
	section	.text
	global	main
main:
	push 	rbp
	mov  	rbp, rsp
	mov  	r8, 20		; r8 <- 20
	mov  	[j], r8b
	movzx	r8, byte [j]
	mov  	rdi, _printint_format
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, 10		; r8 <- 10
	mov  	[i], r8
	mov  	r8, [i]
	mov  	rdi, _printint_format
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, 1		; r8 <- 1
	mov  	[i], r8
L1:
	mov  	r8, [i]	mov  	r9, 5		; r9 <- 5
	cmp	r8, r9
	jg	L2
	mov  	r8, [i]
	mov  	rdi, _printint_format
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	mov  	r8, [i]	mov  	r9, 1		; r9 <- 1
	add  	r9, r8		; r9 + r8
	mov  	[i], r9
	jmp  	L1
L2:
	mov  	r8, 253		; r8 <- 253
	mov  	[j], r8b
L3:
	movzx	r8, byte [j]	mov  	r9, 2		; r9 <- 2
	cmp	r8, r9
	je	L4
	movzx	r8, byte [j]
	mov  	rdi, _printint_format
	mov  	rsi, r8
	mov  	al, 0
	call 	printf

	movzx	r8, byte [j]	mov  	r9, 1		; r9 <- 1
	add  	r9, r8		; r9 + r8
	mov  	[j], r9b
	jmp  	L3
L4:
	mov  	eax, 0
	pop  	rbp
	ret
