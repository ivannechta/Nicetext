
RINFO - Copyright (c) Mark T. Chapman 1995

This program gives you a little information about files checked out
with RCS across multiple platforms.  Only RCS repositories located in an
RCS subdirectory are analyzed.  You may use the ",v" convention within
the RCS directories, but as of now, the program DOES NOT give rinfo on ",v"
files anywhere outside of the RCS subdirectory.  

WARNING: This program will crash if you have cyclic symbolic links!

Feel free to change it to work as you may like.

This ANSI C program should be very easy to compile.

Makefile   - UNIX style makefile -- use with cc
RCS        - subdirectory for RCS versions of rinfo.c 
README.TXT - this file
rinfo.c    - the complete program source

