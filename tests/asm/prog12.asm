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
// void main()
//
main:
	asf	1
	call	read
	pushr
	popl	0
	pushl	0
	call	factorial
	drop	1
	pushr
	call	write
	drop	1
_L0:
	rsf
	ret

//
// integer factorial(integer)
//
factorial:
	asf	1
	pushc	1
	popl	0
	jmp	_L3
_L2:
	pushl	0
	pushl	-3
	mul
	popl	0
	pushl	-3
	pushc	1
	sub
	popl	-3
_L3:
	pushl	-3
	pushc	0
	gt
	brt	_L2
_L4:
	pushl	0
	popr
	jmp	_L1
_L1:
	rsf
	ret
