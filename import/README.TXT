
This subdirectory contains the IMPORT module of the NICETEXT system.
See COPYRIGHT FILE in main distribution directory.

The purpose of the IMPORT module is to create master dictionary tables 
automatically from external sources.

webster : imports the output of the form "webster < /usr/share/dict/words"
kimmo   : imports the output of pckimmo (see ../doc/distribution.txt )
impmsc  : imports the output of a file with '"word"     TYPE' -- each line
impnum  : used to generate numbers for input -- not too useful, but it needed
          to go somewhere...

There are no changes required for the underlying systems.

