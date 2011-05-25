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
	call	g
	drop	1
	pushr
	pushc	2
	call	g
	drop	1
	pushr
	pushc	3
	call	g
	drop	1
	pushr
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
	call	h
	drop	1
	pushr
	popr
	jmp	_L1
_L1:
	rsf
	ret

//
// integer g(integer)
//
g:
	asf	0
	pushl	-3
	pushc	1
	add
	popr
	jmp	_L2
_L2:
	rsf
	ret

//
// integer h(integer)
//
h:
	asf	0
	pushc	2
	pushl	-3
	mul
	popr
	jmp	_L3
_L3:
	rsf
	ret
