
error:
	@echo 'ERROR: The purpose of this makefile is to create a type-word list to stdout'
	@echo 'try make twlist'

twlist:
	@(cd impkimmo; make twlist; cd ../mitlist; make twlist; cd ../numeric; make twlist; cd ../rhyme; make twlist) | awk '/.* [AEIOUHaeiouh].*/ {print "begins_with_a_vowel " $$2;} /.*/ {print $$1 " " $$2;}'
 
