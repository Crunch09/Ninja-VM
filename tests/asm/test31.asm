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
	eq
	brf	_L1
	pushc	2
	popl	0
_L1:
	pushl	-4
	pushl	-3
	ne
	brf	_L2
	pushc	3
	popl	0
_L2:
	pushl	-4
	pushl	-3
	lt
	brf	_L3
	pushc	4
	popl	0
_L3:
	pushl	-4
	pushl	-3
	le
	brf	_L4
	pushc	5
	popl	0
_L4:
	pushl	-4
	pushl	-3
	gt
	brf	_L5
	pushc	6
	popl	0
_L5:
	pushl	-4
	pushl	-3
	ge
	brf	_L6
	pushc	7
	popl	0
_L6:
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
_L7:
	rsf
	ret
