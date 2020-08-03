
error:
	@echo 'ERROR: Use make wlist'

wlist:
	@(cd gutenberg; make wlist; cd ../web2; make wlist) |  ${NICETEXT_BIN_DIR}listword 
