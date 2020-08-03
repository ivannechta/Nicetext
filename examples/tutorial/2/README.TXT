
- - - - - - - - - - - - - - - - - - Sample 2 - - - - - - - - - - - - - - - - -

The purpose of sample 2 is to show how to make larger dictionaries
using the txt2dct program.  Step 2 and beyond are similar to the steps
in sample 1.

To start:
--------------------------

We start with just a few simple files.  Be sure to view the files in the
words directory.

Makefile       - used to make sample files
README         - this file
sample.txt     - a sample text file used to create the sentence models 
secret.txt     - a "secret" that is used to demonstrate functionality of system
words/name_family           - a list of family names
words/name_female           - a list of female names
words/name_male             - ...
words/name_other            - ...
words/num_cardinal_digits   - ...
words/num_cardinal_words    - ...
words/num_roman_digits      - ...
words/num_years_digits      - ...
words/place                 - ...


"make step1" - Builds dict.raw from words/* using txt2dct
-----------------------------------------------------------

If you type "make step1" it will run the txt2dct program on the files in
the words directory.  The output is put into dict.raw so we can see
what it looks like.

The txt2dct program is a useful way to make a raw dct file for input to
sortdct. (It is easy to make a raw dct file from a variety of sources.  Feel
free to try the ../../bin/imp* programs).

"make step2" - Build dict.srt from dict.raw using sortdct and then build mstrdct using dct2mstr
---------------------------------------------------------------------

If you type "make step2" it will run the sortdct program with the dict.raw
file as the input.  The output file dict.srt will then be used by the
dct2mstr process to make the "mstr" dictionary.  

Be sure to view the file dict.srt as it compares to dict.raw.

dict.srt       - a sorted and merged version of dict.raw 

Be sure to view mstrdict.dat and mstrtype.dat. (The *.alt and *.jmp hash files
are in a binary format).

mstrdict.alt	- alternate index to mstrdict.dat (sorted by type,code)
mstrdict.dat    - master dictionary data table
mstrdict.jmp    - primary index to mstrdict.dat (sorted by word)
mstrtype.dat    - master type data table (used to "normalize" mstrdict.dat) 
mstrtype.jmp    - alternate index to mstrtype.dat (sorted by description)


"make step3" - Builds grambase.def and dist* from sample.txt and mstr*  
------------------------------------------------------------------------

If you type "make step3", the genmodel program build a table of sentence
models using the sentences in sample.txt and the words in the master 
dictionary.  

badword.out    -  a list of words in sample.txt not found in master dictionary.
                  (this is supposed to be empty)

The dist* files are a subset of the mstr* table containing only the words
used as input the the genmodel process (in other words, sample.txt).  The
purpose is that the nicetext and scramble processes can be run with either
(as long as the table used for nicetext matches that of the scramble process). 

As normal, you may view the *.dat files.

distdict.alt   -  alternate index to distdict.dat (sorted by type,code)
distdict.dat   -  a dictionary table with only the words from sample.txt
distdict.jmp   -  primary index to disttype.dat (sorted by word)
disttype.dat   -  type data table (used to "normalize distdict.dat) 
disttype.jmp   -  alternate index to disttype.dat (sorted by description)

A sentence model is mostly a list of types with some formatting information.

mstrmodel.dat  -  a table of sentence models created from sample.txt 
mstrmodel.jmp  -  primary index into mstrmodel.dat (sorted sequentially) 

The genmodel program can also output all of the models in mstrmodel.dat as 
right-hand-side (RHS) options for a single grammar rule in the file 
grambase.def.  This may be useful if you want to get a quick-start on creating
custom grammars.

The mstrmodel.dat format is best for when there are a large number of models.
The nicetext program reads the models on demand from the mstrmodel.dat file
when the -m option is used.  Otherwise all the rules in the grambase.def format
are read into program memory with the -g option.

grambase.def   -  the sentence models in mstrmodel.dat in a single grammar rule 

If you use "nicetext -m mstrmodel.dat ..." compared to a corresponding
"nicetest -g grambase.def ..." you will get similar results.

"make step4" -- convert secret.txt into nicetxt1.out and nicetxt2.out
-----------------------------------------------------------------------

If you run "make step4", you will convert the secret.txt file into
nicetext output.  This demonstration actually runs nicetext twice so that
you can see how secret.txt may be mapped to different nicetext even though
the sample dictionary and grammar are used.

nicetxt1.out  -  the output of the first run of nicetext
nicetxt2.out  -  the output of the second run of nicetext

Note that each time you do a "make clean", "make step4" you may get a different
sent of nicetxt*.out files.

"make step5" -- convert nicetxt*.out into scram*.txt
-----------------------------------------------------

If you run "make step5", you will witness the amazing trick of converting
the "English Looking" output of nicetext back into secret.txt. 

scram1.txt    - this should be a copy of secret.txt generated from nicetxt1.out 
scram2.txt    - this should be a copy of secret.txt generated from nicetxt2.out 

Step 5 also compares scram1.txt scram2.txt and secret.txt.  

Notes:
--------------------------------------------------------

The size of the nicetxt*.out files are on average much smaller compared those 
of sample 1.  This is because our dictionary is much larger.  Some other factors
that affect the size of the nicetext are the use of punctuation (which, is
completely ignored by the scramble process); and the average length of
the words in a category compared to how many words are in the category.

