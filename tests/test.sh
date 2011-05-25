#!/bin/sh

#nj compile in asm
FILESNJ=nj/*.nj

for f in $FILESNJ; do
  #ASM=${f%.nj}.asm
  ASM=$(basename $f .nj).asm
  ./njc $f --output asm/$ASM
done

#asm compile in bin
FILESASM=asm/*.asm

for f in $FILESASM; do
  #BIN=${f%.asm}.bin
  BIN=$(basename $f .asm).bin
  ./nja $f bin/$BIN
done


#test bin
rm -f *.ausgabe

FILES=bin/*.bin
	
for f in $FILES
do
	echo "(--------------$f----------------)" >> njvm.ausgabe 
	./../njvm $f < tests.eingabe >> njvm.ausgabe
	echo "(------------------------------)" >> njvm.ausgabe 
done

for f in $FILES
do
	echo "(--------------$f----------------)" >> ref.ausgabe 
	./../njvm-linux $f < tests.eingabe >> ref.ausgabe
	echo "(------------------------------)" >> ref.ausgabe 
done

diff -yBw njvm.ausgabe ref.ausgabe
