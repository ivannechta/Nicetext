#
# Makefile
# Mark T. Chapman
# 

LIBDEPEND=../../babble/lib/libbabb.a ../../gendict/lib/libgndct.a ../../mtc++/lib/libmtc++.a 

CFLAGS+=$(CDEBUG) -I../include -I../../gendict/include -I../../mtc++/include 
LFLAGS= -L../lib -L../../babble/lib -L../../mtc++/lib -L../../gendict/lib -lbabb -lgndct -lmtc++ 

SRCS=nttpd.cc
CC=g++
LEX=flex -f -p $(LDEBUG)  
YACC=yacc -d -v $(YDEBUG) 

.cc.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: nttpd

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	rm -f *.o *.core $(LIBTARGET) nttpd 
	rm -f ../bin/nttpd ../../bin/nttpd

install: all
	cp nttpd ../../bin
	strip ../../bin/nttpd

nttpd: nttpd.o $(LIBDEPEND)
	g++  $(CFLAGS) -o nttpd nttpd.o $(LFLAGS) 

# DO NOT DELETE
