#!/bin/sh
#
# Usage: nicetest.sh filename
#
# The purpose of this shell script is to quickly convert a file
# and then compare the results
#
# One possible use is to do a "find / -type f | xargs -n1 nicetest.sh"
# (The above find command will try to convert every file on the system)
#
# You can then look at the file /tmp/diff.nt to see that there are no
# differences between the original and the converted file
# NOTE: to use less RAM, add "-s" after both "nicetext" and "scramble"  
#

#nice -20 ./nicetext -i $1 -d ../../examples/database/mstr -m ../../examples/database/shak -q | nice -20 ./scramble -o /tmp/ntest -d ../../examples/database/mstr  
#diff -q -s /tmp/ntest $1 --brief >> /tmp/diff.nt
./nicetext -i $1.rnd -d ../../examples/database/mstr -m ../../examples/database/shak -q  -o $1.stg
