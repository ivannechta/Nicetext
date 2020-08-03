
# UNIX Style Makefile for GCC 
# use to compile rinfo 

rinfo: rinfo.c
	gcc -DUNIX=1 rinfo.c -o rinfo

install: rinfo
	cp rinfo ../bin
	
clean:
	rm -f rinfo ../bin/rinfo

