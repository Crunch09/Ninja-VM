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
// void f(boolean, boolean)
//
f:
	asf	1
	pushl	-4
	brf	_L1
	pushl	-3
	brf	_L2
	pushc	2
	popl	0
	jmp	_L3
_L2:
	pushc	3
	popl	0
_L3:
_L1:
_L0:
	rsf
	ret

//
// void main()
//
main:
	asf	0
_L4:
	rsf
	ret
