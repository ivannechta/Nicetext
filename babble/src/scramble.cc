/*
 * Scramble Program 
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: scramble.cc,v 1.13 1998/08/18 18:10:17 root Exp $
 *
 * $Log: scramble.cc,v $
 * Revision 1.13  1998/08/18 18:10:17  root
 * *** empty log message ***
 *
 * Revision 1.12  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id: scramble.cc,v 1.13 1998/08/18 18:10:17 root Exp $";
 *
 * Revision 1.11  1997/03/03 19:46:32  markc
 * Added -v for verbose error reporting
 *
 * Revision 1.10  1997/02/27 17:52:25  markc
 * Fixed usage()
 *
 * Revision 1.9  1997/02/27 16:55:03  markc
 * Fixed small mode comments
 *
 * Revision 1.8  1997/02/27 15:56:17  markc
 * Added -s Small Mode
 *
 * Revision 1.7  1997/02/16 06:59:55  markc
 * BST replaced with RBT
 *
 * Revision 1.6  1996/01/13 15:13:56  markc
 * txt2bits.o now working, unbabble takes parameters like babble
 *
// Revision 1.5  1996/01/13  13:02:28  markc
// Cap first letter of first word in each sentence
//
// Revision 1.4  1996/01/12  23:48:19  markc
// dynamic grammars work system wide -- next step, constant expressions
//
// Revision 1.3  1995/11/13  14:28:08  markc
// Added output file parameter on command line.
//
// Revision 1.2  1995/11/13  02:15:26  chapman
// Added hugeMode -- much faster!!!
//
// Revision 1.1  1995/11/13  01:07:46  chapman
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: scramble.cc,v 1.13 1998/08/18 18:10:17 root Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/raof.h"
#include "../../mtc++/include/raof2rbt.h"
#include "../../gendict/include/dictrec.h"
#include "../../gendict/include/typerec.h"
#include "../../gendict/include/typearec.h"
#include "../../gendict/include/lexword.h"
#include "../include/bitstrm.h"
#include "../include/bitoutput.h"
#include "../include/txt2bits.h"

void usage();
BOOL smallMode=FALSE;

int main(int argc, char * argv[])
{
	// check command line arguments
	extern char *optarg;
	extern int optind;
	int ch;
	BOOL lowMemoryMode=FALSE;
	MTCstring dictFile("mstrdict");
	MTCstring typeFile("mstrtype");
	MTCstring outputFileName;
	MTCstring inputFileName;
	BOOL verboseMode=FALSE;

	while ((ch = getopt(argc, argv, "d:lho:i:se")) != EOF)
	{
		switch(ch) 
		{
			case 'd':
				dictFile=optarg;
				dictFile+="dict";
				typeFile=optarg;
				typeFile+="type";
				break;
			case 'o':
				outputFileName=optarg;
				break;
			case 'i':
				inputFileName=optarg;
				break;
			case 's':
				smallMode=TRUE;
				break;
			case 'e':
				verboseMode=TRUE;
				break;

			default:
				usage();
				return (-1);
		}
	}

	argc -= optind;
 	argv += optind;

	cerr << "Converting input text file into binary file..." << endl;

	MTCtextToBits converter;

	converter.setVerboseMode(verboseMode);

	cerr << "Stage 1: Setup"<< endl;
	
	if (inputFileName.length()==0)
	{
		cerr << "\tUsing stdin for input." << endl;
	}
	else
	{
		cerr << "\tOpening input file: " << inputFileName << endl;

		if (converter.openInputName(inputFileName)==FALSE)
		{
			exit(-1);
		}
	}

	if (converter.getVerboseMode()==TRUE)
	{
		cerr << "\tPrint undefined words to error output" << endl;
	}
	else
	{
		cerr << "\tDO NOT Print undefined words to error output" << endl;
	}

	if (outputFileName.length()==0)
	{
		cerr << endl;
		errorMsg(EMT_ABORT, "I was not given an output file (use -o outputFile)", "main()");
	}
	else
	{
		cerr << "\tOpening output file: " << outputFileName << endl;

		if (converter.openOutputName(outputFileName)==FALSE)
		{
			exit(-1);
		}
	}

	cerr << "\tOpening type table: " << typeFile << endl;

	if (converter.openTypeName(typeFile)==FALSE)
	{
		exit(-1);
	}

	cerr << "\tOpening dictionary table: " << dictFile << endl;

	if (smallMode==TRUE)
	{
        	cerr << "\t(small mode [-s] selected...I will NOT read dictionary into memory...)" << endl;
	}
	else
	{
                cerr << "\t(Reading dictionary into memory...if I run out of RAM try using [-s])" << endl;
	}

	if (converter.openDictName(dictFile, smallMode)==FALSE)
	{
		exit(-1);
	}

	cerr << "Stage 2: Process input file..." << endl;

	converter.doIt();

	cerr << "That's all folks!" << endl;

}

void usage()
{
	cerr << "Usage: scramble -o outputFile [-d dictionary] [-i inputFile] [-s] [-e]" << endl; 
       cerr << "\t-d\tSpecify the prefix for the dictionary file\n\t\t(i.e. mstr for mstrdict.dat, mstrtype.dat)" << endl; 
	cerr << "\t-e\tPrint undefined words to error output" << endl;
        cerr << "\t-i\tSpecify the full input file path (default to stdin)" << endl; 
        cerr << "\t-o\tSpecify the full output file path" << endl; 
	cerr << "\t-s\tSmall mode - use tables from disk, don't load them into RAM" << endl;
}
