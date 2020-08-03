
// this is part of an example grammar
// the output of expgram must be appended to this file to complete

sentence:
	  	{Cap} PICK_NAMES verbwent prepto {Cap} mPLACE WHEN ACTION {Cap} PICK_NAMES {.n} @100
	 | 	{Cap} WHENSTART PICK_NAMES verbmet PICK_NAMES {.n} @31
	;
		
WHENSTART:
		prepin YEAR {,} @20	// "in 1972," 20/23 times 
	| 	{} @3			// blank -- 3/23 times 
	;

WHEN:
		prepin YEAR @17		// "in 1972" 17/37 times
	|	{} @20			// blank 20/37 times
	;

PICK_NAMES:
	 	SIMPLE_NAME @100 
	|	SIMPLE_NAME mCONJUNCTION SIMPLE_NAME @7
	|	SIMPLE_NAME {,} SIMPLE_NAME mCONJUNCTION SIMPLE_NAME @2
	|	SIMPLE_NAME {,} SIMPLE_NAME {,} SIMPLE_NAME mCONJUNCTION SIMPLE_NAME @1
	;

SIMPLE_NAME:
		MALE @8
	|	FEMALE @12
	;

MALE:
		{Cap} mNAME_MALE @51
	|	{Cap} mNAME_MALE {Cap} mNAME_FAMILY @27
	|	{Cap} mNAME_MALE {Cap} mNAME_MALE {Cap} mNAME_FAMILY @5
	;

FEMALE:
		{Cap} mNAME_FEMALE @49
	|	{Cap} mNAME_FEMALE {Cap} mNAME_FAMILY @23
	|	{Cap} mNAME_FEMALE {Cap} mNAME_FEMALE {Cap} mNAME_FAMILY @5
	;

YEAR:
		num_cardinal_digits,num_years_digits	
	;

ACTION:
		mCONJUNCTION verbmet @20
	|	prepto verbmeet @30
	;

