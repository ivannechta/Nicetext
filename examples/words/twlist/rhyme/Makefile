
error:
	@echo "ERROR - Use 'make twlist' or 'make create'"

twlist:
	@gunzip < rhyme.twlist.gz

clean:
	rm -f many.typ dictsel.srt dict.srt

# The RHYME twlist is created from a joining of the cmudict and
# an existing master dictionary database (../../../databast/mstrdict.dat) 
# This means that in order to create rhyme.twlist we need to first
# create the master database.  
# But - the master database uses the rhyme.twlist
# hmmmm.... :) 

create: ../../../database/mstrdict.dat
	gunzip < cmudict.0.4.gz | awk -f extract.awk | sort +1 > dict.srt
	join -1 2 -2 4 -o"1.1 1.2" dict.srt ../../../database/mstrdict.dat | sort >  dictsel.srt
	awk -f rev2.awk dictsel.srt | uniq -c -f 1 | awk -f gt3.awk > many.typ 
	join dictsel.srt many.typ | gzip --best > rhyme.twlist.gz
	rm -f many.typ dictsel.srt dict.srt

../../../database/mstrdict.dat:
	rm -f rhyme.twlist.gz
	cd ../../../database; make

