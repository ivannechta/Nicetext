# Note: set NICETEXT_BIN_DIR or set your path

all: mstrdict.dat wizozdict.dat shakdict.dat fabldict.dat jfkdict.dat 

mstrdict.dat:
	cd ../words/twlist; make twlist | ${NICETEXT_BIN_DIR}sortdct -u 10000 > /tmp/dict.raw
	${NICETEXT_BIN_DIR}dct2mstr -i /tmp/dict.raw
	rm -f /tmp/dict.raw

wizozdict.dat: mstrdict.dat
	(cd ../texts/wizoz; make) | ${NICETEXT_BIN_DIR}genmodel -o wizoz -s  

shakdict.dat: mstrdict.dat
	(cd ../texts/shak; make) | ${NICETEXT_BIN_DIR}genmodel -o shak -s  

fabldict.dat: mstrdict.dat
	(cd ../texts/fabl; make) | ${NICETEXT_BIN_DIR}genmodel -o fabl -s  

jfkdict.dat: mstrdict.dat
	(cd ../texts/jfk; make) | ${NICETEXT_BIN_DIR}genmodel -o jfk -s  

clean:
	rm -f /tmp/dict.raw

veryclean: clean
	rm -f *.jmp *.dat *.alt

