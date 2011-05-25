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
	pushc	0
	popl	0
_L1:
	pushl	0
	pushc	1
	add
	popl	0
	pushl	0
	pushc	10
	lt
	brt	_L1
_L2:
_L0:
	rsf
	ret
