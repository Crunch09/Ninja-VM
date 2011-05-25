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
	asf	0
	pushc	1
	pushc	2
	pushc	3
	call	f
	drop	3
_L0:
	rsf
	ret

//
// void f(integer, integer, integer)
//
f:
	asf	0
_L1:
	rsf
	ret
