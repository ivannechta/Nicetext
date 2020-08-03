#
# Makefile
# Mark T. Chapman
# 

LIBTARGET=
LIBOBJS=
LIBDEPEND=$(LIBTARGET) ../../mtc++/lib/libmtc++.a

CFLAGS+=$(DEBUG) -I../include -I../../mtc++/include 
LFLAGS= -L../../mtc++/lib -L../lib 

SRCS=impkimmo.cc impwbstr.cc impmsc.cc printint.cc
CC=g++
LEX=flex -f 

.cc.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(LIBTARGET) impkimmo impwbstr impmsc printint  

install: all
	cp impkimmo ../../bin
	cp impwbstr ../../bin
	cp impmsc ../../bin
	cp printint ../../bin
	strip ../../bin/impkimmo ../../bin/impwbstr ../../bin/impmsc ../../bin/printint

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	rm -f *.o impkimmo impwbstr impmsc printint $(LIBTARGET) 
	rm -f ../bin/impkimmo ../../bin/impkimmo
	rm -f ../bin/impwbstr ../../bin/impwbstr
	rm -f ../bin/impmsc ../../bin/impmsc
	rm -f ../bin/printint ../../bin/printint

impkimmo: impkimmo.o $(LIBDEPEND)
	g++  $(CFLAGS) -o impkimmo impkimmo.o -ll -lmtc++ $(LFLAGS)

impkimmo.cc: impkimmo.l impmsc.cc  
	rm -f lex.yy.c
	$(LEX) impkimmo.l 
	mv lex.yy.c impkimmo.cc

impmsc: impmsc.o $(LIBDEPEND)
	g++  $(CFLAGS) -o impmsc impmsc.o -ll $(LFLAGS)

impmsc.cc: impmsc.l impwbstr.cc  
	rm -f lex.yy.c
	$(LEX) impmsc.l 
	mv lex.yy.c impmsc.cc
	
impwbstr: impwbstr.o $(LIBDEPEND)
	g++  $(CFLAGS) -o impwbstr impwbstr.o -ll $(LFLAGS)

impwbstr.cc: impwbstr.l 
	rm -f lex.yy.c
	$(LEX) impwbstr.l 
	mv lex.yy.c impwbstr.cc

printint: printint.o $(LIBDEPEND)
	g++  $(CFLAGS) -o printint printint.o -ll $(LFLAGS)

# DO NOT DELETE
