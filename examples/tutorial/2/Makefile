
all: step5

# Step 1: Build dict.srt from dict.raw using sortdct

step1: dict.raw

dict.raw: words/name_family words/name_female words/name_male words/name_other words/num_cardinal_digits words/num_cardinal_words words/num_roman_digits words/num_years_digits words/place words/prepin words/prepto words/verbmeet words/verbmet words/verbwent 
	cd words; ${NICETEXT_BIN_DIR}txt2dct * > ../dict.raw

# Step 2: Build dict.srt from dict.raw and then make the mstrdict

step2: mstrdict.dat

mstrdict.dat: dict.srt
	${NICETEXT_BIN_DIR}dct2mstr -i dict.srt 

dict.srt: dict.raw
	${NICETEXT_BIN_DIR}sortdct < dict.raw > dict.srt

# Step 3: Build grambase.def and distdict.* from sample.txt and mstr*

step3: grambase.def

grambase.def: mstrdict.dat sample.txt
	${NICETEXT_BIN_DIR}genmodel -s -g grambase.def -i sample.txt


# Step 4: Build nicetxt*.out using mstr* and secret.txt

step4: nicetxt1.out nicetxt2.out

nicetxt1.out: secret.txt grambase.def mstrdict.dat  
	${NICETEXT_BIN_DIR}nicetext -s -d mstr -i secret.txt -g grambase.def -o nicetxt1.out

nicetxt2.out: secret.txt grambase.def mstrdict.dat
	${NICETEXT_BIN_DIR}nicetext -s -d mstr -i secret.txt -g grambase.def -o nicetxt2.out

# Step 5: Create scram1.txt and scram2.txt from mstr*, nicetxt*.out

step5: scram1.txt scram2.txt
	diff scram1.txt scram2.txt

scram1.txt: nicetxt1.out 
	${NICETEXT_BIN_DIR}scramble -s -d mstr -i nicetxt1.out -o scram1.txt
	diff scram1.txt secret.txt

scram2.txt: nicetxt2.out 
	${NICETEXT_BIN_DIR}scramble -s -d mstr -i nicetxt2.out -o scram2.txt
	diff scram2.txt secret.txt

# clean up 

clean:
	rm -f dict.raw dict.srt mstr* dist* grambase.def badword.out nicetxt*.out scram*.txt

