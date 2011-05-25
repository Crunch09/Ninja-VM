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
	asf	2
	call	read
	pushr
	popl	0
	call	read
	pushr
	popl	1
	jmp	_L2
_L1:
	pushl	0
	pushl	1
	gt
	brf	_L4
	pushl	0
	pushl	1
	sub
	popl	0
	jmp	_L5
_L4:
	pushl	1
	pushl	0
	sub
	popl	1
_L5:
_L2:
	pushl	0
	pushl	1
	ne
	brt	_L1
_L3:
	pushl	0
	call	write
	drop	1
_L0:
	rsf
	ret
