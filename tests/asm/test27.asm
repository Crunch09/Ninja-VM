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
	call	f
	pushr
	popl	0
_L0:
	rsf
	ret

//
// integer f()
//
f:
	asf	0
	pushc	42
	popr
	jmp	_L1
_L1:
	rsf
	ret
