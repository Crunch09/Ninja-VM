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
	pushc	1
	pushc	2
	pushc	3
	call	f
	drop	3
	pushr
	popl	0
_L0:
	rsf
	ret

//
// integer f(integer, integer, integer)
//
f:
	asf	0
	pushl	-5
	pushl	-4
	add
	pushl	-3
	add
	popr
	jmp	_L1
_L1:
	rsf
	ret
