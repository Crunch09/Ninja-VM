//
// execution framework
//
_start:
	call	main
	call	exit
_stop:
	jmp	_stop

//
// integer read()
//
read:
	asf	0
	rdint
	popr
	rsf
	ret

//
// void write(integer)
//
write:
	asf	0
	pushl	-3
	wrint
	rsf
	ret

//
// void exit()
//
exit:
	asf	0
	halt
	rsf
	ret

//
// integer f(integer, integer)
//
f:
	asf	1
	pushl	-4
	pushl	-3
	add
	popl	0
	pushl	0
	pushl	0
	mul
	popl	0
	pushl	0
	pushl	-4
	sub
	popl	0
	pushc	1000
	pushl	0
	div
	popl	0
	pushl	0
	pushc	13
	mod
	popl	0
	pushl	0
	popl	0
	pushc	0
	pushl	0
	sub
	popl	0
	pushc	3
	pushc	2
	pushl	0
	pushc	5
	sub
	mul
	sub
	popl	0
	pushl	0
	popr
	jmp	_L0
_L0:
	rsf
	ret

//
// void main()
//
main:
	asf	0
_L1:
	rsf
	ret
