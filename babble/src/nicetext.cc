/*
 * NiceText Program 
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: nicetext.cc,v 1.26 1998/08/18 21:34:14 markc Exp $
 *
 * $Log: nicetext.cc,v $
 * Revision 1.26  1998/08/18 21:34:14  markc
 * babble -> nicetext
 *
 * Revision 1.25  1997/05/03 16:30:38  markc
 * Added stats doc to usage()
 *
 * Revision 1.24  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id: nicetext.cc,v 1.26 1998/08/18 21:34:14 markc Exp $";
 *
 * Revision 1.23  1997/02/27 17:52:25  markc
 * Fixed usage()
 *
 * Revision 1.22  1997/02/27 16:55:03  markc
 * Fixed small mode comments
 *
 * Revision 1.21  1997/02/27 13:35:31  chapman
 * Enabled -s parameter for smaller memory usage
 *
 * Revision 1.20  1997/02/24 04:27:49  markc
 * Added bit-input and output statistics
 *
 * Revision 1.19  1997/02/24 00:12:19  markc
 * Added printStats and fixed maxModelLength
 *
 * Revision 1.18  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.17  1996/07/03 22:02:42  markc
 * added stub for small mode (-s)
 *
 * Revision 1.16  1996/02/08 01:09:38  markc
 * added srandom() call
 *
// Revision 1.15  1996/01/13  14:30:01  markc
// fixed usage()
//
// Revision 1.14  1996/01/13  14:25:21  markc
// removed -t from getopts call
//
// Revision 1.13  1996/01/13  13:56:22  markc
// merged -d to mean -t and -d -- it must be this way
//
// Revision 1.12  1996/01/13  13:33:36  markc
// -m option for babble added -- both models and grammars work!
//
// Revision 1.11  1996/01/13  10:18:10  markc
// use new sentnode enabled sentence models
//
// Revision 1.10  1996/01/12  23:50:20  markc
// dynamic grammar makeModel() workds -- cleaned up parameters in babble.cc
// next step, punctuation, etc.
//
// Revision 1.9  1996/01/12  16:37:37  markc
// BAD BAD BAD -- REMOVE REMOVE REMOVE THIS EDIT
//
// Revision 1.8  1995/11/13  11:04:27  markc
// Added option for stdout if no outfileName specified on command line
//
// Revision 1.7  1995/11/13  01:50:26  chapman
// Added HugeMode -- significant speed improvement!!!!
//
// Revision 1.6  1995/11/13  01:06:22  chapman
// Converted to use new output from genmodel.
// Added extrabits -- otherwise the last byte of input stream was not used
// for some reason.
//
// Revision 1.5  1995/11/07  18:54:30  chapman
// Added MTCinputBitStream logic and file selection
//
// Revision 1.4  1995/11/04  18:46:16  chapman
// removed #include <String>
//
// Revision 1.3  1995/09/25  03:12:15  markc
// Check it in
//
// Revision 1.2  1995/08/26  21:34:58  markc
// Just about working for a first cut...
//
// Revision 1.1  1995/08/24  20:14:57  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: nicetext.cc,v 1.26 1998/08/18 21:34:14 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/raof.h"
#include "../../mtc++/include/raof2rbt.h"
#include "../../gendict/include/dictrec.h"
#include "../../gendict/include/dictarec.h"
#include "../../gendict/include/typerec.h"
#include "../../gendict/include/typearec.h"
#include "../../gendict/include/sentnode.h"
#include "../../gendict/include/sentmdl.h"
#include "../../gendict/include/dconst.h"
#include "../include/bitstrm.h"
#include "../include/bitinput.h"
#include "../include/rule.h"
#include "../include/grammar.h"
#include "../include/bits2txt.h"

#define DEFAULT_MAX_MODEL_LENGTH 1024

void usage();
void randomize();

int main(int argc, char * argv[])
{
	// check command line arguments
	extern char *optarg;
	extern int optind;
	int ch;
	BOOL smallMode=FALSE;
	BOOL grammarMode=TRUE;
	long statFreq=DEFAULT_PRINTSTAT_FREQ;
	MTCstring grammarFile("grammar.def");
	MTCstring modelFile("model");
	MTCstring dictFile("mstrdict");
	MTCstring typeFile("mstrtype");
	MTCstring outputFileName;
	MTCstring inputFileName;
	int maxModelLength=DEFAULT_MAX_MODEL_LENGTH; 

	while ((ch = getopt(argc, argv, "g:m:d:sho:i:l:qu:")) != EOF)
	{
		switch(ch) 
		{
			case 'g':
				grammarMode=TRUE;
				grammarFile=optarg;
				break;
			case 'm':
				grammarMode=FALSE;
				modelFile=optarg;
				modelFile+="model";
				break;
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
			case 'q': 
				statFreq=0;
				break;
			case 'u':
				statFreq=atoi(optarg);
				break;
			case 'l':
				maxModelLength=atoi(optarg);

				if (maxModelLength<=0)
				{
					errorMsg(EMT_WARNING, "Invalid maximum model length specified, setting back to default", "main()"); 
					maxModelLength=DEFAULT_MAX_MODEL_LENGTH;
				}

				break;

			default:
				usage();
				return (-1);
		}
	}

	argc -= optind;
 	argv += optind;

	cerr << "Converting input file into nicetext..." << endl;

	MTCbitsToTextBase *converter;

	if (grammarMode==TRUE)
	{
		converter=new MTCbitsToTextGrammar;
	}
	else
	{
		converter=new MTCbitsToTextModel;
	}

	if (converter==NULL)
	{
		errorMsg(EMT_ABORT, "I could not allocate a converter object", "main()"); 
	}

	cerr << "Stage 1: Setup"<< endl;

	if (inputFileName.length()==0)
	{
		cerr << endl;
		errorMsg(EMT_ABORT, "I was not given an input file to process (use -i inputFile)", "main()");
	}
	else
	{
		cerr << "\tOpening input file: " << inputFileName << endl;

		if (converter->openInputName(inputFileName)==FALSE)
		{
			exit(-1);
		}
	}

	if (outputFileName.length()==0)
	{
		cerr << "\tUsing stdout for output." << endl;
	}
	else
	{
		cerr << "\tOpening output file: " << outputFileName << endl;

		if (converter->openOutputName(outputFileName)==FALSE)
		{
			exit(-1);
		}
	}

	cerr << "\tOpening type table: " << typeFile << endl;

	if (converter->openTypeName(typeFile)==FALSE)
	{
		exit(-1);
	}

	if (grammarMode==TRUE)
	{
		cerr << "\tOpening grammar definition file: " << grammarFile << endl;

		if (((MTCbitsToTextGrammar *)converter)->openGrammarName(grammarFile)==FALSE)
		{
			exit(-1);
		}
	}
	else
	{
		cerr << "\tOpening model table: " << modelFile << endl;

		if (((MTCbitsToTextModel *)converter)->openModelName(modelFile)==FALSE)
		{
			exit(-1);
		}
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

	if (converter->openDictName(dictFile, smallMode)==FALSE)
	{
		exit(-1);
	}

	converter->setStatFreq(statFreq);

	if (statFreq!=0)
	{
		cerr << "\tUpdate Status Every " << converter->getStatFreq() << " Sentences " << endl;
	}
	else
	{
		cerr << "\tUpdate Status Disabled with [-q] or [-u 0]" << endl;
	}

	cerr << "\tMax Model Length: " << maxModelLength << endl;
	converter->setMaxModelLength(maxModelLength);

	cerr << "Stage 2: Process input file..." << endl;

	randomize();

	converter->doIt();

	cerr << "Stage 3: Print Final Stats                                                    " << endl;

	converter->printStats();

	cerr << endl;

	cerr << "That's all folks!" << endl;

}

void randomize()
{
	struct timeval t;
	struct timezone tz; 

	if (gettimeofday(&t, &tz)==-1)
	{
		errorMsg(EMT_WARNING, "random seed generation failed...no big deal though...", "randomize()");
	}

	srandom(t.tv_sec|t.tv_usec);
}

void usage()
{
	cerr << "Usage: nicetext -i inputfile [[-g grammarDefFile] | [-m model]] [-d dictionary] [-o outputFile] [-s] [-l maxModelLength] [[-u updateFreq] | [-q]]" << endl; 
	cerr << "\t-d\tSpecify the prefix for the dictionary file\n\t\t(i.e. mstr for mstrdict.dat, mstrtype.dat)" << endl; 
	cerr << "\t-g\tSpecify the full grammar definition file path" << endl; 
	cerr << "\t-i\tSpecify the full input file path" << endl; 
	cerr << "\t-l\tDo not use sentences with more than this number of components" << endl;
	cerr << "\t-m\tSpecify the prefix for the model file\n\t\t(i.e. mstr for mstrmodel.dat)" << endl; 
	cerr << "\t-o\tSpecify the full output file path (default to stdout)" << endl; 
	cerr << "\t-q\tDo not print status updates. (same as [-u 0])" << endl;
	cerr << "\t-s\tSmall mode - use tables from disk, don't load them into RAM" << endl;
	cerr << "\t-u\tPrint status updates every 'updateFreq' sentences." << endl;
	cerr << "The following statistics are printed because of [-u]:" << endl;
	cerr << "\tI: Number of Input Bits Read" << endl;
	cerr << "\tE: Number of Extra Bits Appended to Input" << endl; 
	cerr << "\tO: Number of Output Bits" << endl;
	cerr << "\tG: Growth Ratio: (100*O)/(I+E)" << endl;
	cerr << "\tU: Number of Models Used" << endl;
	cerr << "\tS: Number of Models Skipped Because of [-l]" << endl;
	cerr << "\tN: Total Number of Model Elements Used" << endl;
	cerr << "\tA: Average Number of Elements per Model: N/S" << endl;
}
