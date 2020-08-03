/*
 *
 * Sort a set of raw dictionary entries by value
 * Useful because MTCstring may sort things differently than /usr/bin/sort
 *
 * Copyright (c) Mark T. Chapman, 1995
 *
 * $Id: sortdct.cc,v 1.18 1997/08/18 03:05:27 markc Exp $
 *
 * $Log: sortdct.cc,v $
 * Revision 1.18  1997/08/18 03:05:27  markc
 * Changed yy* to zz* to avoid multiple symbols for LEX parsers
 *
 * Revision 1.17  1997/03/09 18:39:32  markc
 * Fixed usage() for -r
 *
 * Revision 1.16  1997/03/08 17:06:37  markc
 * Added Rerun [-r] option
 *
 * Revision 1.15  1997/03/05 17:41:20  markc
 * Added static char rcsid []  = "@(#)$Id: sortdct.cc,v 1.18 1997/08/18 03:05:27 markc Exp $";
 *
 * Revision 1.14  1997/03/05 15:44:21  markc
 * Added nlAsPunct parameter.
 *
 * Revision 1.13  1997/03/04 16:08:51  markc
 * Removed cerr << "Usage:" line
 *
 * Revision 1.12  1997/03/03 03:20:36  markc
 * Added -e -u -q
 * Added parsing by lexword.l instead of just ignoring punctuation lines
 * Added modelset so that the order of the types no longer matters
 *
 * Revision 1.11  1996/02/11 17:30:45  markc
 * added isSubset logic to properly tokenize type to reduce duplicates
 * such as AD,N becoming AD,N,N if the word is defined twice as an N
 *
// Revision 1.10  1996/02/07  03:49:59  markc
// don't merge if a word appears multiple times defined the same way!
//
// Revision 1.9  1996/01/20  16:34:50  markc
// changed messages to every 1000 instead of every 100
//
// Revision 1.8  1996/01/20  16:28:53  markc
// removed sleep(30) from end
//
// Revision 1.7  1996/01/15  06:53:48  markc
// prototype with RBT support instead of BST
//
// Revision 1.6  1996/01/14  13:47:26  markc
// Added - parms
//
// Revision 1.5  1996/01/14  12:45:46  markc
// Only allow words such as [a-z0-9]+
// Added count display
//
// Revision 1.4  1996/01/14  11:29:33  markc
// added -x option
// merge types of duplicate words using comma-delimited list of types
//
// Revision 1.3  1995/11/08  19:37:39  chapman
// Updated to use rawdict.h instead of dictent.h
//
 *
 */

static char rcsid []  = "@(#)$Id: sortdct.cc,v 1.18 1997/08/18 03:05:27 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../include/rawdict.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/raof.h"
#include "../include/lexword.h"

int zzlex();
extern char *zztext;
extern short nlAsPunct;

#define STATE_READY	0	// ready to read next line
#define STATE_CODE	1	// we just read the code
#define STATE_VALUE	2	// we just read the value
#define STATE_BAD	3 	// something is amiss -- skip until next line

#define DEFAULT_PRINTSTAT_FREQ 1000

void dumpDictionary(MTCrawDictEnt *);
BOOL isSubset(const MTCstring & code, const MTCstring & baseString);
void usage();

MTCRBT <MTCstring> codeSet;	// used to merge types
MTCstring mergeTypeString;		// merged type result
void dumpCodeSet(MTCstring *);	// used to merge types

int main(int argc, char *argv[])
{
	cerr << "Sortdct Program Started..." << endl;

	int ch;
	BOOL expand=FALSE;
	BOOL rerun=FALSE;
	int errorReportingLevel=1;
	int printStatFreq=DEFAULT_PRINTSTAT_FREQ;

	nlAsPunct=1;	// tell lexword.l to return \n as punctuation
	
	while ((ch = getopt(argc, argv, "xe:u:qr")) != EOF)
	{
		switch(ch) 
		{
			case 'x':
				expand=TRUE;
				break;
			case 'e':
				errorReportingLevel=atoi(optarg);
				break;
			case 'u':
				printStatFreq=atoi(optarg);
				break;
			case 'q':
				errorReportingLevel=0;
				printStatFreq=0;
				break;
			case 'r':
				rerun=TRUE;
				break;
			case 'h':
			default:
				usage();
				return (-1);
		}
	}

	argc -= optind;
 	argv += optind;

	MTCRBT <MTCrawDictEnt> dictionary;
	MTCrawDictEnt inputDE, morphDE;
	MTCrawDictEnt *found;

	cerr << "Step 1: Process Dictionary..." << endl;
	cerr << "\tOutput to standard output" << endl;
	cerr << "\tReading from standard input" << endl;
	cerr << "\tPrint Status Every " << printStatFreq << " good entries" << endl;
	cerr << "\tError Reporting Level: " << errorReportingLevel << endl;

	if (expand==TRUE)
	{
		cerr << "\t * * * Suffix Expansion Enabled." << endl;
	}

	if (rerun==TRUE)
	{
		cerr << "\tRerun [-r] enabled. Input assumed to be output of a previous run." << endl;
	}

	BOOL good; 
	int count=0;
	int l;
	int retval;
 	int state=STATE_READY;
	int errorLevel=0;

        for(;;)
        {
                retval=zzlex();

                if (zztext[0]=='\0')
		{
                        break;
		}

		MTCstring zztextString(zztext);

                switch (retval)
                {
		case END_OF_WORD:
			switch (state)
			{
			case STATE_READY:
				if (rerun==TRUE)
				{
					MTCstring tempStr(inputDE.getCode());
					inputDE.setCode(tempStr+zztextString);
				}
				else
				{
					inputDE.setCode(zztextString);
				}
				state=STATE_CODE;
				break;
			case STATE_CODE:
				zztextString.toLower();	
				inputDE.setValue(zztextString);
				state=STATE_VALUE;
				break;
			case STATE_VALUE:
				if (errorLevel++<errorReportingLevel)
				{
					errorMsg(EMT_WARNING, "I parsed two words on a single line\nI will EXCLUDE this entry from the dictionary.", "main()");
					cerr << "\tPrevious:" << inputDE.getValue() << endl;
					cerr << "\tNext: " << zztextString << endl;
				}
				state=STATE_BAD;
				break;

			default:
				if (errorLevel++<errorReportingLevel)
				{
					errorMsg(EMT_WARNING, "I am ignoring this word because the entry is bad", "main()");
					cerr << "\tWord: " << zztextString << endl;
				}
				state=STATE_BAD;
				break;
			}
			break;

		case PUNCTUATION:
			if (zztextString=="\n") 
			{
				switch (state)
				{
				case STATE_VALUE:
					// is the word undefined -- if so define it

					if ((found=dictionary.find(inputDE))==NULL)
					{
						dictionary.add(inputDE);
					}
					else	// otherwise merge the type
					{
						// don't expand if a word appears multi-times
						// defined the same way!

						if (isSubset(found->getCode(), inputDE.getCode())==FALSE)
						{
							found->setCode(mergeTypeString);
	
							if (expand==TRUE)
							{
								inputDE.setCode(mergeTypeString);
							}
						}
					}

					if (expand==TRUE)
					{
						morphDE.setValue(inputDE.getValue()+"ary");
						morphDE.setCode(inputDE.getCode()+"-ARY");
						dictionary.add(morphDE, TRUE);

						morphDE.setValue(inputDE.getValue()+"s");
						morphDE.setCode(inputDE.getCode()+"-PLURAL");
						dictionary.add(morphDE, TRUE);
	
						morphDE.setValue(inputDE.getValue()+"es");
						morphDE.setCode(inputDE.getCode()+"-PLURAL");
						dictionary.add(morphDE, TRUE);

						morphDE.setValue(inputDE.getValue()+"ing");
						morphDE.setCode(inputDE.getCode()+"-GERUND");
						dictionary.add(morphDE, TRUE);

						morphDE.setValue(inputDE.getValue()+"ism");
						morphDE.setCode(inputDE.getCode()+"-ISM");
						dictionary.add(morphDE, TRUE);

						morphDE.setValue(inputDE.getValue()+"ly");
						morphDE.setCode(inputDE.getCode()+"-LY");
						dictionary.add(morphDE, TRUE);
					}

					if ((printStatFreq!=0)&&((++count%printStatFreq)==0))
					{
  						cerr << "sd: " << count << "    \r";   
					}

					errorLevel=0;

					state=STATE_READY;

					inputDE.setCode("");
					inputDE.setValue("");
					break;
					
				case STATE_READY:
					if (errorLevel++<errorReportingLevel)
					{
						errorMsg(EMT_WARNING, "Empty line ignored", "main()");
					}
					break;

				case STATE_CODE:
					if (errorLevel++<errorReportingLevel)
					{
						errorMsg(EMT_WARNING, "A code was read, but there was no word found", "main()");
						cerr << "\tCode:" << inputDE.getCode() << endl;
					}
					state=STATE_READY;
					inputDE.setCode("");
					inputDE.setValue("");
					break;
		
				default:
					if (errorLevel++<errorReportingLevel)
					{
						errorMsg(EMT_WARNING, "The input state is being reset because of errors.", "main()");
						cerr << "\tPrevious: " << inputDE << endl;
					}
					state=STATE_READY;
					inputDE.setCode("");
					inputDE.setValue("");
					break;
				}
			}
			else
			{
				if ((zztextString==",")
				&&  (rerun==TRUE)
				&&  (state==STATE_CODE)) 
				{
					MTCstring tempStr(inputDE.getCode());
					inputDE.setCode(tempStr+zztextString);
					state=STATE_READY;
					// note: do NOT clear code here!
					inputDE.setValue("");
					break;
				}

				if (errorLevel++<errorReportingLevel)
				{
					errorMsg(EMT_WARNING, "I read some unparseable punctuation.  It is not part of a word nor is it a valid end-of-line. I am excluding this entry from the dictionary", "main()");
					cerr << "\tPunctuation:" << zztextString <<  ":" << endl;
					cerr << "\tPrevious or Current Entry: " << inputDE << endl;
				}
				state=STATE_BAD;
			}

			break;

		case END_OF_SENTENCE:
			if (errorLevel++<errorReportingLevel)
			{
				errorMsg(EMT_WARNING, "The punctuation within a word or type was mis-interpretable as an END-OF-SENTENCE marker.  I will ignore the current entry.", "main()"); 
				cerr << "\tPrevious or Current Entry: " << inputDE << endl;
			}
			state=STATE_BAD;
                        break;

                default:
			if (errorLevel++<errorReportingLevel)
			{
				errorMsg(EMT_WARNING, "The punctuation within a word or type was mis-interpretable as an UNDEFINED-PARSER-STATE.  I will ignore the current entry.", "main()"); 
				cerr << "\tPrevious or Current Entry: " << inputDE << endl;
			}
			state=STATE_BAD;
                        break;
                }
        }

	cerr << "Dumping " << dictionary.getCount() << " words." << endl;

	dictionary.forEach(dumpDictionary);

	cerr << "That's All Folks!" << endl;

	return 0;
}

void dumpDictionary(MTCrawDictEnt *aRawDictEnt)
{
	cout << (*aRawDictEnt);
}

// is the baseString a subset of the code?

BOOL isSubset(const MTCstring & code, const MTCstring & baseString)
{
	if (code==baseString)
	{
		return TRUE;
	}

	MTCRBT <MTCstring> codeSet;
	MTCstring tempStr;

	codeSet.clear();

	char buffer[code.length()+1];

	strcpy(buffer, code.c_str());
	
	char *p;

	p=strtok(buffer, ",");
	tempStr=p;
	codeSet.add(tempStr);

	while (p!=NULL)
	{
		if (baseString==p)
		{
			codeSet.clear();
			return TRUE;
		}

		p=strtok(NULL, ",");
		tempStr=p;
		codeSet.add(tempStr);
	}

	tempStr=baseString;
	codeSet.add(tempStr);

	mergeTypeString="";

	codeSet.forEach(dumpCodeSet);

	codeSet.clear();

	return FALSE;
}

void dumpCodeSet(MTCstring *aString)
{
	if (mergeTypeString.length()==0)
	{
		mergeTypeString=*aString;
	}
	else
	{
		mergeTypeString+=",";
		mergeTypeString+=(*aString);
	}
}

void usage()
{
	cerr << "Usage: sortdict [-x] [-e errorLevel] [-u updateFreq] [-q] [-r] " << endl;
	cerr << "\t-e\tPrint error messages up to this level (0-9). Default is 1." << endl;
	cerr << "\t-q\tDo NOT print status updates or errors (same as [-u 0] [-e 0]" << endl;
	cerr << "\t-r\tRerun option: input should be the output of previous run." << endl;
	cerr << "\t \t(commas are interpreted as merging types)." << endl;
	cerr << "\t-u\tPrint status update every 'updateFreq' successful lines " << endl;
	cerr << "\t-x\tExpand dictionary with suffixes (experimental)" << endl;
}
