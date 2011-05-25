CFLAGS=-Wall -g -ansi -pedantic 
SOURCES=njvm.c 
HEADERS=njvm.h 
RFOLDER=hstr63

all:	 
	gcc $(CFLAGS) -o njvm $(SOURCES)
	# Fuer das Compilieren ohne sich an den Standard zu halten
	
debug:
	gcc -Wall -g -o njvm $(SOURCES)


test:
	(cd tests; sh test.sh)

release:
	mkdir $(RFOLDER)
	chmod 755 mknjvm
	cp README $(RFOLDER)
	cp team.txt $(RFOLDER)
	cp debug.txt $(RFOLDER)
	cp njvm.c $(RFOLDER)
	cp njvm.h $(RFOLDER)
	cp Makefile $(RFOLDER)
	cp mknjvm $(RFOLDER) 
	tar -cvf $(RFOLDER).tar $(RFOLDER)
	gzip $(RFOLDER).tar

clean:
	rm -f *~  njvm
	rm -r $(RFOLDER)

