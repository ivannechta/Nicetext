# Master Makefile
# NiceText

all: 
	cd rinfo; gmake
	cd mtc++/src; gmake 
	cd gendict/src; gmake 
	cd babble/src; gmake 
#	cd import/src; gmake 
	cd nttpd/src; gmake

database:
	cd examples/database; gmake

depend: 
	cd mtc++/src; gmake depend 
	cd gendict/src; gmake depend 
	cd babble/src; gmake depend 
	cd import/src; gmake depend 
	cd nttpd/src; gmake depend

clean:
	cd mtc++/src; gmake clean 
	cd gendict/src; gmake clean 
	cd babble/src; gmake clean 
	cd nttpd/src; gmake clean
	cd import/src; gmake clean 
	cd rinfo; gmake clean 
	rm -f bin/qstart-nttpd

backup: clean
	rm -f ../nicetext.zip; zip -9 -r ../nicetext.zip *; 

snapshot: RINFO
	rinfo/rinfo -s

RINFO:  
	cd rinfo; gmake 

tarball: clean
	rm -f ../nicetext-0.9.tar ../nicetext-0.9.tar.gz
	@echo "Creating tar archive..."
	cd ..; tar -cvf nicetext-0.9.tar nicetext-0.9; 
	@echo "Compressing tar archive..."
	cd ..; gzip --best nicetext-0.9.tar

install: quickstart
	cd rinfo; gmake install
	cd mtc++/src; gmake install 
	cd gendict/src; gmake install 
	cd babble/src; gmake install 
	cd import/src; gmake install 
	cd nttpd/src; gmake install

quickstart:
	@echo "#!/bin/sh" > bin/qstart-nttpd
	@echo `pwd`/bin/"nttpd -b `pwd`"/examples/database >> bin/qstart-nttpd
	chmod a+rx bin/qstart-nttpd
