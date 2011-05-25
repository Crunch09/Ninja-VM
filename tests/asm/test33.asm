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
// integer f(integer, integer, integer)
//
f:
	asf	2
	pushc	3
	popl	0
	pushc	5
	popl	1
	pushl	-5
	pushl	-4
	add
	pushl	-3
	add
	pushl	0
	add
	pushl	1
	add
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
	pushc	1
	pushc	2
	pushc	3
	call	f
	drop	3
	pushr
	call	write
	drop	1
_L1:
	rsf
	ret
