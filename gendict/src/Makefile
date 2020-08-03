#
# Makefile
# Mark T. Chapman
# 

LIBTARGET=../lib/libgndct.a
LIBOBJS= rawdict.o lexword.o dictrec.o typerec.o sentmdl.o sentnode.o 
LIBDEPEND=$(LIBTARGET) ../../mtc++/lib/libmtc++.a

CFLAGS+=$(CDEBUG) -I../include -I../../mtc++/include
LFLAGS= -L../../mtc++/lib -L../lib -lmtc++ -lgndct   

CC=g++
#LEX=lex -fp

SRCS=dct2mstr.cc rawdict.cc genmodel.cc lexword.cc dictrec.cc typerec.cc sentmdl.cc sentnode.cc txt2dct.cc listword.cc sortdct.cc

.cc.o:
	$(CC) $(CFLAGS) -c -o $@ $<


all: $(LIBTARGET) dct2mstr genmodel txt2dct listword sortdct 

install: all
	cp dct2mstr ../../bin
	cp genmodel ../../bin
	cp listword ../../bin
	cp sortdct ../../bin
	cp txt2dct  ../../bin
	strip ../../bin/dct2mstr ../../bin/genmodel ../../bin/listword ../../bin/sortdct ../../bin/txt2dct


depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	rm -f *.o $(LIBTARGET) dct2mstr genmodel txt2dct listword sortdct txt2dct *.core 
	rm -f ../bin/dct2mstr ../bin/genmodel ../bin/listword ../bin/sortdct ../../bin/dct2mstr ../../bin/genmodel ../../bin/listword ../../bin/sortdct ../bin/txt2dct ../../bin/txt2dct

master: dct2mstr
	dct2mstr

$(LIBTARGET): $(LIBOBJS)
	ar -rc $(LIBTARGET) $(LIBOBJS)
	ranlib $(LIBTARGET)

dct2mstr: dct2mstr.o $(LIBDEPEND)
	g++  $(CFLAGS) -o dct2mstr dct2mstr.o $(LFLAGS) 

listword: listword.o $(LIBDEPEND)
	g++  $(CFLAGS) -o listword listword.o $(LFLAGS)

sortdct: sortdct.o $(LIBDEPEND)
	g++  $(CFLAGS) -o sortdct sortdct.o $(LFLAGS)

txt2dct: txt2dct.o $(LIBDEPEND)
	g++  $(CFLAGS) -o txt2dct txt2dct.o $(LFLAGS)

genmodel: genmodel.o $(LIBDEPEND)
	g++  $(CFLAGS) -o genmodel genmodel.o $(LFLAGS)

lexword.cc: lexword.l
	rm -f lex.yy.c
	$(LEX) lexword.l 
	sed s/yy/zz/g < lex.yy.c | sed s/YY/ZZ/g > lexword.cc
	rm -f lex.yy.c

