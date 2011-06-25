//
// sum.asm -- compute the sum of numbers in an array
//

//********************************************
//**                                        **
//**   Your task: find suitable comments!   **
//**                                        **
//********************************************
sum:
	asf	3		// 0: array, 1: index, 2: sum
	pushc	10
	newa
	popl	0
	pushc	9
	popl	1
sum1:
	pushl	0
	pushl	1
	pushc	10
	pushl	1
	sub
	putfa
	pushl	1
	pushc	1
	sub
	popl	1
	pushl	1
	pushc	0
	lt
	brf	sum1
	pushc	0
	popl	1
	pushc	0
	popl	2
sum2:
	pushl	0
	pushl	1
	getfa
	pushl	2
	add
	popl	2
	pushl	1
	pushc	1
	add
	popl	1
	pushl	1
	pushc	10
	eq
	brf	sum2
	pushl	2
	wrint
	rsf
	halt
