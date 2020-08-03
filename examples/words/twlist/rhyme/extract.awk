/* This program extracts the "rhyming" part of words */ 

{ 
	ORS="";

	/* pc = how many trailing phonemes make a rhyme? */

	for(pc=2; pc<20; pc++)
	{
		if (NF>(pc))
		{
			print "rhymel" pc "_"
			for(i=NF-pc; i<NF; i++) 
			{
				print tolower($(i+1));
			}
			print " " tolower($1) 
			print "\n"
		}
	}
}

