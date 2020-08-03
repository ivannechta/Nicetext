
all: step5

# Step 1: Build dict.srt from dict.raw using sortdct

step1: dict.srt

dict.srt: dict.raw
	${NICETEXT_BIN_DIR}sortdct < dict.raw > dict.srt


# Step 2: Build mstrdict.* from dict.srt

step2: mstrdict.dat

mstrdict.dat: dict.srt
	${NICETEXT_BIN_DIR}dct2mstr -i dict.srt 


# Step 3: Build grambase.def and distdict.* from sample.txt and mstr*

step3: grambase.def

grambase.def: mstrdict.dat sample.txt
	${NICETEXT_BIN_DIR}genmodel -s -g grambase.def -i sample.txt


# Step 4: Build nicetxt*.out using mstr* and secret.txt

step4: nicetxt1.out nicetxt2.out

nicetxt1.out: secret.txt grambase.def mstrdict.dat  
	${NICETEXT_BIN_DIR}nicetext -d mstr -i secret.txt -g grambase.def -o nicetxt1.out

nicetxt2.out: secret.txt grambase.def mstrdict.dat
	${NICETEXT_BIN_DIR}nicetext -d mstr -i secret.txt -g grambase.def -o nicetxt2.out

# Step 5: Create scram1.txt and scram2.txt from mstr*, nicetxt*.out

step5: scram1.txt scram2.txt
	diff scram1.txt scram2.txt

scram1.txt: nicetxt1.out 
	${NICETEXT_BIN_DIR}scramble -d mstr -i nicetxt1.out -o scram1.txt
	diff scram1.txt secret.txt

scram2.txt: nicetxt2.out 
	${NICETEXT_BIN_DIR}scramble -d mstr -i nicetxt2.out -o scram2.txt
	diff scram2.txt secret.txt

# clean up 

clean:
	rm -f dict.srt mstr* dist* grambase.def badword.out nicetxt*.out scram*.txt

