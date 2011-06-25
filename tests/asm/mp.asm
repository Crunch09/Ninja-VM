//
// mp.asm -- compute midpoint of a line segment
//

mp:
	asf	0
	pushc	2
	pushc	2
	call	mkpoint		// point(2,2)
	drop	2
	pushr
	pushc	10
	pushc	6
	call	mkpoint		// point(10,6)
	drop	2
	pushr
	call	mkline		// line(point(2,2),point(10,6))
	drop	2
	pushr
	call	midpoint	// midpoint of this line
	drop	1
	pushr
	call	showpoint	// show coordinates of midpoint
	drop	1
	rsf
	halt

mkpoint:
	asf	0
	new	2		// a point object has 2 instance vars
	dup
	pushl	-4		// get first argument
	putf	0		// this is the point's x coordinate
	dup
	pushl	-3		// get second argument
	putf	1		// this is the point's y coordinate
	popr
	rsf
	ret

showpoint:
	asf	0
	pushl	-3		// get point argument
	getf	0		// x coordinate
	wrint
	pushl	-3		// get point argument
	getf	1		// y coordinate
	wrint
	rsf
	ret

mkline:
	asf	0
	new	2		// a line object has 2 instance vars
	dup
	pushl	-4		// get first argument
	putf	0		// this is the line's start point
	dup
	pushl	-3		// get second argument
	putf	1		// this is the line's end point
	popr
	rsf
	ret

//********************************************
//**                                        **
//**   Your task: find suitable comments!   **
//**                                        **
//********************************************
midpoint:
	asf	2
	pushl	-3
	getf	0
	popl	0
	pushl	-3
	getf	1
	popl	1
	pushl	0
	getf	0
	pushl	1
	getf	0
	add
	pushc	2
	div
	pushl	0
	getf	1
	pushl	1
	getf	1
	add
	pushc	2
	div
	call	mkpoint
	drop	2
	pushr
	popr
	rsf
	ret
