
- - - - - - - - - - - - - - - - - - Sample 3 - - - - - - - - - - - - - - - - -

The purpose of sample 3 is to demonstrate the use of grammars.
Sample 3 depends on a non-modified version of the sample 2 directory.

To start:
--------------------------

We start with just a few simple files.  Be sure to view the files in the
words directory.

Makefile       - used to make sample files
README         - this file
gram1.def      - a sample grammar
gram2.inc      - the start of a second sample grammar
secret.txt     - a "secret" that is used to demonstrate functionality of system

"make step1" - Step 1: Build nicetxt1.out and scram1.txt using gram1.def
-------------------------------------------------------------------------

Step 1 tests nicetext using a hand-typed grammar.  
It also runs scramble to prove it works. 

If you type "make step1" it will:

1.1  Make the mstr dictionary in step 2 by running "../2/make step2"
1.2  Run nicetext using the grammar in gram1.def.  Dictionary is ../2/mstr.
     Input is secret.txt. 

nicetxt1.out  - output of nicetext using gram1.def

1.3  Run scramble on nicetxt1.out using ../2/mstr dictionary. 

scram1.txt    - output of scramble using nicetxt1 -- it should equal secret.txt

1.4  Compare scram1.txt and secret.txt to make sure they match.


"make step2" -  
---------------------------------------------------------------------

Step 2 is essentially the same as Step 1 with one extra step.  The grammar
file is partly created by hand and partly generated.

2.1 Create expgram.inc using the expgram program.  This program reads the
    ../2/mstrtype.dat table to understand the components of merged types. 
    It then creates a set of rules in which the LHS is the "merged base type"
    and the RHS consists of the supertypes that contain the base type.
    (If this doesn't make any sense, take a look at expgram.inc -- it will
    become much more clear.)

expgram.inc      -- merge type rules created from dictionary by expgram
    
2.2 Concatenate gram2.inc (a hand-made grammar) with the remaining expgram.inc
    to create gram2.def.

gram2.def        -- concatenation of gram2.inc+expgram.inc - a complete grammar 

2.3  Run nicetext using the grammar in gram2.def.  Dictionary is ../2/mstr.
     Input is secret.txt. 

nicetxt2.out  - output of nicetext using gram2.def

2.4  Run scramble on nicetxt2.out using ../2/mstr dictionary. 

scram2.txt    - output of scramble using nicetxt2 -- it should equal secret.txt

2.5  Compare scram2.txt and secret.txt to make sure they match.

2.6  Compare scram1.txt and scram2.txt just to show off.


Notes:
--------------------------------------------------------

Grammars can be cyclic.  Grammars can be ambuiguous.  This is a good thing 
for nicetext.  Scramble does not use sentence models or grammars!  It just
recognizes words and outputs the bit-value from the dictionary.

