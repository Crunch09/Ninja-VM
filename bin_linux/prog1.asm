//
// prog1.asm -- an assembler example
//

// local integer x;
// local integer y;
// x = 2;
// y = x + 3;
// x = 7 * y + x;
// write(x + -33);

	asf	2
	pushc	2
	popl	0
	pushl	0
	pushc	3
	add
	popl	1
	pushc	7
	pushl	1
	mul
	pushl	0
	add
	popl	0
	pushl	0
	pushc	-33
	add
	wrint
	rsf
	halt
