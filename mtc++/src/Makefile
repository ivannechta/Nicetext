#
# MTC++ MakeFile
# Copyright (c) Mark T. Chapman 1995
#

LIBTARGET=../lib/libmtc++.a
LIBOBJS= mstring.o stdermsg.o initfile.o
SRCS=mstring.cc stdermsg.cc initfile.cc raofmake.cc raofmalt.cc raofread.cc bsttest.cc rbttest.cc listtest.cc inittest.cc numsize.cc

CC=g++
CFLAGS+=$(CDEBUG) 
LFLAGS= -L../lib -lmtc++

.cc.o: 
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(LIBTARGET) raofmake raofmalt raofread bsttest rbttest listtest inittest numsize

install: all
	cp raofmake ../bin
	cp raofmalt ../bin
	cp raofread ../bin
	cp listtest ../bin
	cp bsttest ../bin
	cp rbttest ../bin
	cp inittest ../bin
	cp numsize ../bin
	strip ../bin/raofmake ../bin/raofmalt ../bin/raofread ../bin/listtest ../bin/bsttest ../bin/rbttest ../bin/inittest ../bin/numsize

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	rm -f $(LIBTARGET) *.o *.core inittest bsttest rbttest listtest raofmake raofmalt raofread numsize ../bin/inittest ../bin/bsttest ../bin/rbttest ../bin/listtest ../bin/raofmake ../bin/raofmalt ../bin/raofread ../bin/numsize

$(LIBTARGET): $(LIBOBJS)
	ar -rc $(LIBTARGET) $(LIBOBJS)
	ranlib $(LIBTARGET)

# THESE ARE SAMPLE PROGRAMS

raofmake: raofmake.o $(LIBTARGET)
	g++  $(CFLAGS) -o raofmake raofmake.o $(LFLAGS)

raofmalt: raofmalt.o $(LIBTARGET)
	g++  $(CFLAGS) -o raofmalt raofmalt.o $(LFLAGS)

raofread: raofread.o $(LIBTARGET)
	g++  $(CFLAGS) -o raofread raofread.o $(LFLAGS)

listtest: listtest.o $(LIBTARGET)
	g++  $(CFLAGS) -o listtest listtest.o $(LFLAGS)

bsttest: bsttest.o $(LIBTARGET)
	g++  $(CFLAGS) -o bsttest bsttest.o $(LFLAGS)

rbttest: rbttest.o $(LIBTARGET)
	g++  $(CFLAGS) -o rbttest rbttest.o $(LFLAGS)

inittest: inittest.o $(LIBTARGET)
	g++  $(CFLAGS) -o inittest inittest.o $(LFLAGS)

numsize: numsize.o $(LIBTARGET)
	g++  $(CFLAGS) -o numsize numsize.o $(LFLAGS)

