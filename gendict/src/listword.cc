/*
 * List the words from stdin a line at a time to stdout
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: listword.cc,v 1.17 1998/08/18 15:52:27 root Exp $
 *
 * $Log: listword.cc,v $
 * Revision 1.17  1998/08/18 15:52:27  root
 * changed -d to -p
 *
 * Revision 1.16  1998/06/25 20:04:08  markc
 * *** empty log message ***
 *
 * Revision 1.15  1998/06/25 18:39:40  markc
 * excludeThisWord() TRUE/FALSE FIXED
 *
 * Revision 1.14  1998/06/25 18:03:42  markc
 * excludeNonWords fixed
 *
 * Revision 1.13  1998/06/25 17:29:01  markc
 * added isprint() to excludeNonWords
 *
 * Revision 1.12  1998/06/24 20:24:21  markc
 * added -u, -q, -x
 *
 * Revision 1.11  1998/06/24 19:44:56  markc
 * 100000 instead of 10000
 *
 * Revision 1.10  1998/06/24 19:43:15  markc
 * 10000 instead of 1000
 *
 * Revision 1.9  1998/06/23 20:24:33  markc
 * added -d option
 *
 * Revision 1.8  1997/08/18 03:05:27  markc
 * Changed yy* to zz* to avoid multiple symbols for LEX parsers
 *
 * Revision 1.7  1997/04/27 16:07:20  markc
 * added words read to output
 *
 * Revision 1.6  1997/04/27 15:57:25  markc
 * Added -l for lowercase
 *
 * Revision 1.5  1997/03/05 17:41:20  markc
 * Added static char rcsid []  = "@(#)$Id: listword.cc,v 1.17 1998/08/18 15:52:27 root Exp $";
 *
 * Revision 1.4  1997/02/16 06:21:06  markc
 * Added -c parameter to count word frequency
 *
 * Revision 1.3  1996/03/21 17:18:56  markc
 * program name changes and messages (minor)
 *
 * Revision 1.2  1996/02/13 20:25:52  markc
 * sort and output unique ones
 *
// Revision 1.1  1996/02/13  19:51:27  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: listword.cc,v 1.17 1998/08/18 15:52:27 root Exp $";

#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/rbt.h"
#include "../include/lexword.h"

int zzlex();
extern char *zztext;

BOOL printFreq=FALSE;

void dump(MTCstring *);
void dumpFreq(MTCstring *, long);
void usage();
BOOL excludeThisWord(const MTCstring &);

int main(int argc, char * argv[])
{
	BOOL lowercase=FALSE;
	BOOL dumpMode=FALSE;
	BOOL excludeNonWords=FALSE;
	long updateFreq=100000;
	char ch;

	while ((ch = getopt(argc, argv, "chlpxu:q")) != EOF)
        {
                switch(ch) 
                {
			case 'u':
				updateFreq = atol(optarg);
				break;
			case 'q':
				updateFreq = 0;
				break;
			case 'x':
				excludeNonWords = TRUE;
				break;
                        case 'c':
				printFreq=TRUE;
				break;
			case 'l':
				lowercase=TRUE;
				break;
			case 'p':
				dumpMode=TRUE;
				break;
			case 'h':
				usage();
				return 0;
			default:
				usage();
				return -1;
		}
	}

	argc -= optind;
        argv += optind;

	MTCRBT <MTCstring> * stringList;

	if (printFreq==TRUE)
	{
		MTCRBTFreq <MTCstring> * freqList = new MTCRBTFreq <MTCstring>;
	
		if (freqList==NULL)
		{
			errorMsg(EMT_ABORT, "I could not allocated the MTCRBTFreq() object -- not enough RAM?", "main()");
			return -2;
		}

		stringList=freqList;
	}
	else
	{
		stringList = new MTCRBT <MTCstring>;

		if (stringList==NULL)
		{
			errorMsg(EMT_ABORT, "I could not allocated the MTCRBTFreq() object -- not enough RAM?", "main()");
			return -2;
		}
	}

	int retval;
	long count=0;
	
	cerr << "listword started" << endl;
	cerr << "\tprinting options..." << endl;

	if (printFreq==TRUE)
	{
		cerr << "\t\tcount frequency option is ON" << endl;
	}
	else
	{
		cerr << "\t\tcount frequency option is OFF" << endl;
	}

	if (lowercase==TRUE)
	{
		cerr << "\t\tconverting all words to lowercase" << endl;
	}

	if (excludeNonWords==TRUE)
	{
		cerr << "\t\texcluding non-words (numbers, punct, etc.)" << endl;
	}

	if (dumpMode==TRUE)
	{
		cerr << "\t\tdumping words as they are read" << endl;
	}

	if (updateFreq)
	{
		cerr << "\t\tsending progress updates every " << updateFreq << " words" << endl;
	}
	else
	{
		cerr << "\t\tnot sending progress updates" << endl;
	}

	cerr << "\tparsing words from stdin..." << endl;

        for(;;)
        {
                retval=zzlex();

                if (zztext[0]=='\0')
                        break;

                switch (retval)
                {
		case END_OF_WORD:
			{
				MTCstring s(zztext);

				if (lowercase==TRUE)
				{
					s.toLower();
				}

				count++;

				if (dumpMode==TRUE)
				{
					if ((excludeNonWords==FALSE)||(excludeThisWord(s)==FALSE))
					{
						cout << s << endl;
					}
				}
				else
				{
					if ((excludeNonWords==FALSE)||(excludeThisWord(s)==FALSE))
					{
						stringList->add(s);
					}

					if ((updateFreq!=0)
					&&  ((count % updateFreq)==0))
					{
						cerr << "\rlw: " << stringList->getCount() << '/' << count << "             \r";
					}
				}
			}
			break;
                default:
                        break;
                }
        }

	cerr << "words read: " << count << "         " << endl;                                                                                            
	if (dumpMode==FALSE)
	{
		cerr << "\tdumping " << stringList->getCount() << " unique words to stdout..." << endl;

		if (printFreq==TRUE)
		{
			((MTCRBTFreq <MTCstring> *)stringList)->forEachFreq(dumpFreq);
		}
		else
		{
			stringList->forEach(dump);
		}
	}

	cerr << "That's All Folks!" << endl;

	if (stringList!=NULL)
	{
		delete stringList;
		stringList=NULL;
	}

	return 0;
}

void dump(MTCstring *s)
{
	cout << *s << endl;
}

void dumpFreq(MTCstring *s, long freq)
{
	cout << *s << ' ' << freq << endl;
}

BOOL excludeThisWord(const MTCstring &aString)
{
	for(int i=0; i<aString.length(); i++)
	{
		if ((!isalpha(aString.c_str()[i]))&&(aString.c_str()[i]!='\''))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void usage()
{
	cerr << "Usage: listword [-c] [-l] [-p] [-x] [[-u updateFreq] | [-q]]" << endl;
	cerr << "\t[-c] Count Words and Print Frequency on Output" << endl;
	cerr << "\t[-l] Convert all words to lower case" << endl;
	cerr << "\t[-p] Dump each word out as it is read" << endl;
	cerr << "\t[-q] Do not send updates to stderr (sames as -u 0)" << endl;
	cerr << "\t[-u] Send updates to stderr every updateFreq words" << endl;
	cerr << "\t[-x] Exclude non-words (numbers, punct, etc.)" << endl;
}
