#
# Makefile
# Mark T. Chapman
# 

LIBTARGET=../lib/libbabb.a
LIBOBJS= bits2txt.o txt2bits.o bitstrm.o bitinput.o bitoutput.o grammar.o rule.o lexgram.o yaccgram.o
LIBDEPEND=../../gendict/lib/libgndct.a ../../mtc++/lib/libmtc++.a $(LIBTARGET) 

CFLAGS+=$(CDEBUG) -I../include -I../../gendict/include -I../../mtc++/include 
LFLAGS= -L../lib -L../../mtc++/lib -L../../gendict/lib -lbabb -lgndct -lmtc++ 

SRCS=bits2txt.cc txt2bits.cc nicetext.cc scramble.cc expgram.cc gramtest.cc bitcp.cc bitstrm.cc bitinput.cc bitoutput.cc grammar.cc rule.cc lexgram.cc yaccgram.cc
CC=g++
#LEX=lex
YACC=yacc -d 

.cc.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(LIBTARGET) nicetext scramble expgram gramtest bitcp

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	rm -f *.o *.core $(LIBTARGET) nicetext scramble expgram gramtest bitcp 
	rm -f y.output 
	rm -f ../../bin/nicetext ../../bin/scramble ../../bin/expgram ../../bin/gramtest ../../bin/bitcp
	rm -f ../bin/nicetext ../bin/scramble ../bin/expgram ../bin/gramtest ../bin/bitcp

install: all
	cp nicetext ../../bin
	cp scramble ../../bin
	cp gramtest ../../bin
	cp expgram ../../bin
	cp bitcp ../../bin
	strip ../../bin/nicetext ../../bin/scramble ../../bin/gramtest ../../bin/expgram ../../bin/bitcp

$(LIBTARGET): $(LIBOBJS)
	ar -rc $(LIBTARGET) $(LIBOBJS)
	ranlib $(LIBTARGET)

nicetext: nicetext.o $(LIBDEPEND)
	g++  $(CFLAGS) -o nicetext nicetext.o $(LFLAGS) 

scramble: scramble.o txt2bits.o $(LIBDEPEND)
	g++  $(CFLAGS) -o scramble scramble.o txt2bits.o -lgndct $(LFLAGS) 

bitcp: bitcp.o $(LIBDEPEND)
	g++  $(CFLAGS) -o bitcp bitcp.o $(LFLAGS) 

gramtest: gramtest.o $(LIBDEPEND)
	g++ $(CFLAGS) -o gramtest gramtest.o $(LFLAGS)

expgram: expgram.o $(LIBDEPEND)
	g++ $(CFLAGS) -o expgram expgram.o $(LFLAGS)

lexgram.cc: yaccgram.cc lexgram.l 
	rm -f lex.yy.c
	$(LEX) lexgram.l 
	mv lex.yy.c lexgram.cc

y.tab.h: yaccgram.cc

yaccgram.cc: yaccgram.y
	rm -f y.tab.c y.tab.h
	$(YACC) yaccgram.y
	mv y.tab.c yaccgram.cc

# Manual dependancies -- used for parallel make before makedepend...
grammar.o: y.tab.h

# makedepend from here on down...

