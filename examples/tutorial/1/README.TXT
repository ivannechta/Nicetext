
- - - - - - - - - - - - - - - - - - Sample 1 - - - - - - - - - - - - - - - - -

An overview of the nicetext system.  

(If you just want to see a file converted to nicetext and back again, skip to 
step 4.)


To start:
--------------------------

We start with just a few simple files.  Be sure to view dict.raw and
sample.txt.  

Makefile       - used to make sample files
README         - this file
dict.raw       - the input dictionary in an unsorted raw format
sample.txt     - a sample text file used to create the sentence models 
secret.txt     - a "secret" that is used to demonstrate functionality of system



"make step1" - Builds dict.srt from dict.raw using sortdct
-----------------------------------------------------------

If you type "make step1" it will run the sortdct program with the dict.raw
file as the input.  The output file dict.srt will be used in step2 by the
dct2mstr process.  Note that sortdct is much more useful on very large raw
dictionaries as it does many consistency checks to make sure that the
output is useable by dct2mstr.  

Be sure to view the file dict.srt as it compares to dict.raw.

dict.srt       - a sorted and merged version of dict.raw 

It is important to look at how sortdct handles the word "bill" which is
both an object and a person.  The merged type of "object,person" was 
created.  This means that the word "bill" is classified neither as an "object"
nor a "person" but rather "bill" belongs to the new category of "object,person".  
The reason why the types are merged is so that there is no ambiguity when
converting the nicetext output back into the original file.  (We will see in 
sample 3 how the expgram program can easily allow the selection of "bill" when 
nicetext wants to use either "person" or "object" or "person,object".)

"make step2" - Builds master dictionary tables from dict.srt
------------------------------------------------------------

If you type "make step2", the dct2mstr program builds the "mstr" dictionary 
database from dict.srt.  

(Note that each step is smart enough to run the previous steps if necessary.)  

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

The mstrmodel.dat format is best suited for a large number of models.  
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

Questions:
---------------------------

Q: What happens if you edit the nicetxt1.out file and then run "make step5"?

A: If all you change is the punctuation or case of words, or if you
add words that are not in the dictionary, or if you add ANYTHING to the
end of the nicetxt1.out files scramble will recover the input to nicetext...)

Q: Why are the nicetxt*.out files so large?

A: The dictionary is too small.

Q: How could you add words to the dictionary?

A: Edit dict.raw.

Q: Are there automated techniques for making large dictionaries?

A: See sample 2.

Q: What if we wanted more variety in sentence models?

A: Edit sample.txt or go to sample 3 that deals with grammars.

Q: How come the same words show up in the nicetxt*.out?  Shouldn't it be more
"random"? 

A: It depends on the distribution on bits in the input file.  If you were to 
take the output of bdes < secret.txt > secret.des and run nicetext on secret.des
the word selection of the nicetext would be more random.

