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
	lt
	dup
	brf	_L2
	drop	1
	pushl	-3
	pushc	17
	gt
_L2:
	brf	_L1
	pushc	2
	popl	0
_L1:
	pushl	-4
	pushl	-3
	lt
	dup
	brt	_L4
	drop	1
	pushl	-3
	pushc	17
	gt
_L4:
	brf	_L3
	pushc	3
	popl	0
_L3:
	pushc	1
	pushl	-4
	pushl	-3
	lt
	dup
	brf	_L6
	drop	1
	pushl	-3
	pushc	17
	gt
_L6:
	sub
	brf	_L5
	pushc	2
	popl	0
_L5:
	pushc	1
	pushl	-4
	pushl	-3
	lt
	sub
	dup
	brt	_L8
	drop	1
	pushc	1
	pushl	-3
	pushc	17
	gt
	sub
_L8:
	brf	_L7
	pushc	3
	popl	0
_L7:
	pushl	0
	popr
	jmp	_L0
_L0:
	rsf
	ret

//
// boolean g(boolean)
//
g:
	asf	0
	pushl	-3
	dup
	brf	_L10
	drop	1
	pushc	1
_L10:
	popl	-3
	pushl	-3
	dup
	brt	_L11
	drop	1
	pushc	0
_L11:
	popl	-3
	pushc	1
	pushl	-3
	sub
	popr
	jmp	_L9
_L9:
	rsf
	ret

//
// void main()
//
main:
	asf	0
_L12:
	rsf
	ret
