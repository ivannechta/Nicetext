#!/bin/sh
# 
# Dumptype Program 
# Copyright (c) Mark T. Chapman 1997
# This calls an awk program which makes a grammar file from a mstrtype.dat file.
# The purpose of the grammar is to force NICETEXT to print three examples
# of each type.  
# WARNING: THE OUTPUT FROM NICETEXT MAY NOT SCRAMBLE IF THE TYPES ARE REAL 
# WORDS IN THE DICTIONARY!
cat ../../database/mstrtype.dat | awk -f dumptype.awk > /tmp/dumptype.def
../../bin/nicetext -i ../../database/mstrtype.jmp -d ../../database/mstr -g /tmp/dumptype.def -l 25000 -u 50 -s | sort 
rm -f /tmp/dumptype.def
