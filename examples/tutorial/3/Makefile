
all: step2

# Step 1: Build nicetxt1.out and scram1.txt using gram1.def,  

step1: ../2/mstrdict.dat nicetxt1.out scram1.txt

../2/mstrdict.dat:
	cd ../2; gmake step2

nicetxt1.out: gram1.def secret.txt 
	${NICETEXT_BIN_DIR}nicetext -s -i secret.txt -d ../2/mstr -g gram1.def -o nicetxt1.out

scram1.txt: nicetxt1.out
	${NICETEXT_BIN_DIR}scramble -e -s -d ../2/mstr -i nicetxt1.out -o scram1.txt
	diff scram1.txt secret.txt

# Step 2: Build expgram.inc and gram2.def 

step2: step1 gram2.def nicetxt2.out scram2.txt
	diff scram1.txt scram2.txt

gram2.def: expgram.inc gram2.inc
	cat gram2.inc expgram.inc > gram2.def

expgram.inc:
	cd ../2; ${NICETEXT_BIN_DIR}expgram > ../3/expgram.inc

nicetxt2.out: gram2.def secret.txt 
	${NICETEXT_BIN_DIR}nicetext -s -i secret.txt -d ../2/mstr -g gram2.def -o nicetxt2.out

scram2.txt: nicetxt2.out
	${NICETEXT_BIN_DIR}scramble -e -s -d ../2/mstr -i nicetxt2.out -o scram2.txt
	diff scram2.txt secret.txt

# clean up 

clean:
	rm -f nicetxt*.out scram*.txt expgram.inc gram2.def 

