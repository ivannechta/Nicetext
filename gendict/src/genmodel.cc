/*
 * Generate Model Program 
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: genmodel.cc,v 1.27 1998/07/13 16:06:42 markc Exp $
 *
 * $Log: genmodel.cc,v $
 * Revision 1.27  1998/07/13 16:06:42  markc
 * Major changes
 * -m,-d,-o,-u,-q,-i,-b,-g
 *
 * Revision 1.26  1997/08/18 03:05:27  markc
 * Changed yy* to zz* to avoid multiple symbols for LEX parsers
 *
 * Revision 1.25  1997/05/03 16:16:59  markc
 * Added statis doc to usage()
 *
 * Revision 1.24  1997/03/05 17:41:20  markc
 * Added static char rcsid []  = "@(#)$Id: genmodel.cc,v 1.27 1998/07/13 16:06:42 markc Exp $";
 *
 * Revision 1.23  1997/03/02 04:45:46  markc
 * Big changes: [-s] added, printStats, SENTMDL_CAP*, capWord fixed, usage()
 *
 * Revision 1.22  1997/02/16 06:57:12  markc
 * BST replaced with RBT
 *
 * Revision 1.21  1996/02/20 03:44:23  markc
 * *** empty log message ***
 *
// Revision 1.20  1996/02/17  18:46:13  markc
// use freq for dictrecords
//
// Revision 1.19  1996/02/17  16:42:31  markc
// added grambase.def generation automatically
//
// Revision 1.18  1996/02/14  15:07:31  markc
// use shift and cap
// ignore quotation marks
// output mstrmodel instead of model
//
// Revision 1.17  1996/02/07  04:02:19  markc
// removed #include "rbt.h" -- duplicate -- only needed once ;)
//
// Revision 1.16  1996/01/20  18:18:47  markc
// replaced BST with RBT for dictionary
//
// Revision 1.15  1996/01/20  17:23:28  markc
// use rbt version of raofalt instead of bst
//
// Revision 1.14  1996/01/13  09:45:52  markc
// removed cout << Model
//
// Revision 1.13  1996/01/13  09:41:29  markc
// use new sentnode enabled sentmdl
//
// Revision 1.12  1995/11/12  23:04:28  chapman
// Converted to use the .alt files and everything
// it actually works now.
//
// Revision 1.11  1995/11/08  19:40:58  chapman
// Removed #include "dictent.h"
//
// Revision 1.10  1995/11/08  19:25:10  chapman
// Compiled with new "records" instead of "mstr*"
//
// Revision 1.9  1995/10/12  16:04:46  markc
// Fixed the repeat-last-type-when-end-of-sentence bug./
//
// Revision 1.8  1995/09/25  03:10:22  markc
// Check It In -- thank you.
//
// Revision 1.7  1995/08/24  20:13:47  markc
// update
//
// it now works well with babble 1.1
//
// Revision 1.6  1995/08/24  18:32:59  markc
// Use sentenceModel class instead of just an array of strings for Model
//
// Revision 1.5  1995/08/24  02:02:27  markc
// Revised Model, added MARK_'s,
// Next I need to optimize the sentence model format.
//
// Revision 1.4  1995/08/24  01:02:51  markc
// Everything seems to work now -- needs clean-up though
//
// Revision 1.3  1995/08/23  19:12:37  markc
// Correctly creates distdict.dat -- does not yet deal with disttype.dat
//
// Revision 1.2  1995/08/14  01:01:54  markc
// In progress -- just back it up because it is kinda stable -- but wrong!
//
// Revision 1.1  1995/07/30  17:06:55  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: genmodel.cc,v 1.27 1998/07/13 16:06:42 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/raof.h"
#include "../../mtc++/include/raofalt.h"
#include "../../mtc++/include/raof2rbt.h"
#include "../include/lexword.h"
#include "../include/dictrec.h"
#include "../include/typerec.h"
#include "../include/typearec.h"
#include "../include/dictarec.h"
#include "../include/sentnode.h"
#include "../include/sentmdl.h"
#include "../include/dconst.h"

int zzlex();
extern char *zztext;
extern FILE *zzin;

long badWordCounter=0;
long modelCounter=0;
long wordCounter=0;

void usage();
void printStats();

long loadTypeAltRecords(MTCRBT <MTCtypeAltRecord> &, const MTCstring &);
void clearTypeAltRecFreq(MTCtypeAltRecord *);

void createDistDict(MTCdictRecord *);
void dumpDictionary(MTCdictRecord *, long);
void dumpModelSet(MTCsentenceModel *, long);
void dumpTypeAltRecSet(MTCtypeAltRecord *);
void dumpBadWords(MTCstring *str);
const MTCstring removeNewLines(const MTCstring & aString);

ofstream *badWordFile=NULL;
ofstream *grammarFile=NULL;
MTCwriteRAOF <MTCdictRecord> *dictionaryWriteRAOF=NULL;
MTCwriteRAOF <MTCtypeAltRecord>  *typeWriteRAOF=NULL;
MTCwriteRAOF <MTCsentenceModel>  *modelWriteRAOF=NULL;
MTCRBT <MTCtypeAltRecord> typeAltRecSet;
MTCRBTFreq <MTCsentenceModel> modelSet;
MTCRBTFreq <MTCdictRecord> dictionary;	// new dictionary 
MTCRBT <MTCstring> badWords;

int main(int argc, char *argv[])
{
	BOOL smallMode=FALSE;
	MTCstring inputDictPrefix("mstr");
	MTCstring outputDictPrefix("dist");
	MTCstring outputModelPrefix("dist");
	MTCstring grammarName;
	MTCstring inputFileName;
	long jumpCount=0;
	MTCstring badWordFileName;
	int updateFreq=1000;
	char ch;

	while ((ch = getopt(argc, argv, "hsd:i:j:b:g:o:u:qm:")) != EOF)
	{
		switch(ch) 
		{
		case 'd':
			inputDictPrefix=optarg;
			break;
		case 'i':
			inputFileName=optarg;
			break;
		case 'j':
			jumpCount=atoi(optarg);	
			break;
		case 'u':
			updateFreq=atoi(optarg);	
			break;
		case 'q':
			updateFreq=0;
			break;
		case 'b':
			badWordFileName=optarg;
			break;
		case 's':
			smallMode=TRUE;
			break;
		case 'o':
			outputDictPrefix=optarg;
			outputModelPrefix=optarg;
			break;
		case 'g':
			grammarName=optarg;
			break;
		case 'm':
			outputModelPrefix=optarg;
			break;
		case 'h':
		default:
			usage();	
			exit(-1);
		}
	}

	argc -= optind;
	argv += optind;

	cerr << "Sentence Model Analysis Started..." << endl;

	cerr << "\tprogram options:" <<endl;
	
	if (smallMode==TRUE)
	{
		cerr << "\t\tsmall memory mode [-s] selected" << endl;
	}
	else
	{
		cerr << "\t\tload tables into RAM -- If I run out of memory, try [-s]" << endl;
	}

	cerr << "\t\tinput dictionary prefix: " << inputDictPrefix << endl;

	if (inputFileName.length())
	{
		cerr << "\t\tinput file name: " << inputFileName << endl;

		if ((zzin=fopen(inputFileName.c_str(),"r"))==NULL)
		{
			perror(inputFileName.c_str());
			errorMsg(EMT_ABORT, "I could not open the input file","main()");
			exit(-1);
		}
	}
	else
	{
		cerr << "\t\tread from stdin" << endl;
	}

	if (jumpCount)
	{
		cerr << "\t\t(-j NOT YET IMPLEMENTED) skip the first " << jumpCount << " lines" << endl;
	}

	if (badWordFileName.length())
	{
		cerr << "\t\tsend bad word list to: " << badWordFileName << endl;
		if ((badWordFile = new ofstream(badWordFileName.c_str()))==NULL)
		{
			perror(badWordFileName.c_str());
			errorMsg(EMT_ABORT, "I could not create the bad word file", "main()");
			exit(-1);
		}
	}

	if (grammarName.length())
	{
		cerr << "\t\tdump a (probably useless) grammar to: " << grammarName << endl; 

		if ((grammarFile = new ofstream(grammarName.c_str()))==NULL)
		{
			perror(grammarName.c_str());
			errorMsg(EMT_ABORT, "I could not create the grammar output file", "main()");
			exit(-1);
		}
	}

	if (outputDictPrefix.length())
	{
		cerr << "\t\toutput dictionary prefix: " << outputDictPrefix << endl;
		dictionaryWriteRAOF= new MTCwriteRAOF <MTCdictRecord> (outputDictPrefix + "dict");

		if (dictionaryWriteRAOF==NULL)
		{
			perror(outputDictPrefix.c_str());
			errorMsg(EMT_ABORT,"I could not create the output dictionary twlist record table","main()"); 
			exit (-1);
		}

		typeWriteRAOF= new MTCwriteRAOF <MTCtypeAltRecord> (outputDictPrefix + "type");
		if (typeWriteRAOF==NULL)
		{
			perror(outputDictPrefix.c_str());
			errorMsg(EMT_ABORT,"I could not create the output dictionary type table","main()"); 
			exit (-1);
		}
	}

	if (outputModelPrefix.length())
	{
		cerr << "\t\toutput model prefix: " << outputModelPrefix << endl;
		modelWriteRAOF= new MTCwriteRAOF <MTCsentenceModel> (outputModelPrefix + "model");

		if (modelWriteRAOF==NULL)
		{
			perror(outputModelPrefix.c_str());
			errorMsg(EMT_ABORT,"I could not create the model table","main()"); 
			exit (-1);
		}
	}

	BOOL good;
	MTCreadRAOF <MTCdictRecord> dictRecordReadRAOF (inputDictPrefix + "dict");
	MTCRBT <MTCdictRecord> dictRecordReadRBT;
	MTCdictRecord currentDictRec;
	MTCtypeAltRecord tempTypeAltRecord;
	MTCtypeAltRecord *foundTypeAltRec;
	long badTypeIndex;
	MTCstring zztextString;
	MTCsentenceModel model;
	MTCdictRecord find;
	const MTCdictRecord *found;
	int retval;
	char buffer[20];
	MTCstring bufferString;
	long i;
	BOOL capWord=FALSE;
 
	// load type alternate records

	cerr << "Step 1: Load type information from input dictionary..." << endl;

	badTypeIndex=loadTypeAltRecords(typeAltRecSet, inputDictPrefix);

	cerr << "\t" << typeAltRecSet.getCount() << " types in dictionary" << endl;

	cerr << "Step 2: Load input dictionary..." << endl;

	if (smallMode==FALSE)
	{
		MTCraofToRBT <MTCdictRecord> loader;

		if (loader.doIt(dictRecordReadRAOF, dictRecordReadRBT)==FALSE)
		{
			errorMsg(EMT_ABORT, "I could not load the dictionary table into Memory", "main()"); 
			return -1;
		}
	}

	cerr << "\t" << dictRecordReadRAOF.getCount() << " words in dictionary" << endl;

	// process words from stdin

	cerr << "Step 3: Scanning sentences from stdin..." << endl;

	good=TRUE;

	for(;;)
	{
		retval=zzlex();		// read the next token from stdin

		if (zztext[0]=='\0')	// is it eof?
		{
			break;
		}

		zztextString=zztext;
		zztextString.toLower();	// everything lower case for now...

		currentDictRec.setValue(zztextString);

		switch (retval)
		{
			case END_OF_WORD:	
				// uppercase?

				wordCounter++;

				if ((zztext[0]>='A')&&(zztext[0]<='Z'))
				{
					MTCstring tstr(zztext);

					tstr.toUpper();

					if (tstr==zztext)
					{
						capWord=TRUE;
					}
					else
					{
						model.appendPunctuation(SENTMDL_CAP_NEXT_LETTER);
					}
				}

				// is it already in dictionary?

				if ((found=dictionary.find(currentDictRec))!=NULL)
				{
					// increment frequency

					MTCdictRecord d(*found);
					dictionary.add(d, FALSE);

					// append type index to model and exit

					if (capWord==TRUE)
					{
						model.appendPunctuation(SENTMDL_CAPSLOCK_ON);
					}

					model.appendTypeIndex(found->getTypeIndex());
					if (capWord==TRUE)
					{
						model.appendPunctuation(SENTMDL_CAPSLOCK_OFF);
						capWord=FALSE;
					}

					break;
				}

				// otherwise look it up in master RAOF

				if (smallMode==TRUE)
				{
					found=dictRecordReadRAOF.find(currentDictRec);
				}
				else
				{
					found=dictRecordReadRBT.find(currentDictRec);
				}

				if (found!=NULL)
				{
					currentDictRec=*found;
				}
				else
				{
					// bad word processing 
					
					badWordCounter++;

					if (badWordFile!=NULL)
					{
						MTCstring bad(currentDictRec.getValue());
						badWords.add(bad);
						currentDictRec.setTypeIndex(badTypeIndex);
						good=FALSE;
					}

					// do not add bad words to dist dict

					break;
				}

				// look up the the type frequency

				tempTypeAltRecord.setIndex(currentDictRec.getTypeIndex());

				foundTypeAltRec=typeAltRecSet.find(tempTypeAltRecord);

				if (foundTypeAltRec==NULL)
				{
					cerr << "ERROR: Type Not Found" << tempTypeAltRecord << " -- The type table does not match the dictionary." << endl;
					break;
				}

				// set the bitstring in the dictrec
	
				currentDictRec.setBitString(foundTypeAltRec->getFreq());

				// increment the type freq
			
				foundTypeAltRec->setFreq(foundTypeAltRec->getFreq()+1);

				// add the currentDictRec

				dictionary.add(currentDictRec);

				// add the type index to the model

				if (capWord==TRUE)
				{
					model.appendPunctuation(SENTMDL_CAPSLOCK_ON);
				}

				model.appendTypeIndex(currentDictRec.getTypeIndex());

				// if all caps, go back to lowercase

				if (capWord==TRUE)
				{
					model.appendPunctuation(SENTMDL_CAPSLOCK_OFF);
					capWord=FALSE;
				}

				break;

			case END_OF_SENTENCE:	
				if (good==TRUE)
				{
					// add the punctuation to model

					model.appendPunctuation(removeNewLines(currentDictRec.getValue()));
					// add the model to the modelSet

					modelSet.add(model);
				}

				modelCounter++;

				if ((updateFreq)&&(modelCounter%updateFreq)==0)
				{
					printStats();
				}
			
				model.clear();
				good=TRUE;
				break;

			case PUNCTUATION:
				model.appendPunctuation(removeNewLines(currentDictRec.getValue()));
				break;
		}
	}

	// close the dictionary

	printStats();

	cerr << endl; 

	if (smallMode==FALSE)
	{
		cerr << "\tremoving mstrdict.dat from memory..." << endl;
		dictRecordReadRBT.clear();
	}
	

	// warn about bad words

	cerr << "Step 4: Process bad word list" << endl;

	if (badWordCounter==0)
	{
		cerr << "\tAll words in sample texts were defined! " << endl;
	}
	else
	{
		if (badWordFile!=NULL)
		{
			cerr << "\tWarning: " << badWordCounter << " instances of undefined words." << endl;
			cerr << "\t         " << badWords.getCount() << " unique undefined words." << endl; 
			cerr << "\t          Please see " << badWordFileName << endl;
			badWords.forEach(dumpBadWords);
		}
	}

	// dump the model set

	cerr << "Step 5: Store the sentence models..." << endl;

	cerr << "\tStoring " << modelSet.getCount() << " Models..." << endl;

	if (grammarFile!=NULL)
	{
		cerr << "\tStoring " << modelSet.getCount() << " Models..." << endl;

		*grammarFile << "// rules generated by genmodel" << endl;
		*grammarFile << "// these should match mstrmodel.dat" << endl;
		*grammarFile << "// this grammar will use a LOT of memory if used with nicetext" << endl;
		*grammarFile << "// the real purpose is to assist in manual grammar generation" << endl;
		*grammarFile << endl;
		*grammarFile << "SENTENCE:" << endl;
		modelSet.forEachFreq(dumpModelSet);

		*grammarFile << "\t;" << endl;
		*grammarFile << endl;
	}
	else
	{
		if (modelWriteRAOF!=NULL)
		{
			modelSet.forEachFreq(dumpModelSet);
		}
	}

	modelSet.clear();

	// make this dump dictionary into a distdict.dat, distdict.jmp

	if (dictionaryWriteRAOF!=NULL)
	{
		cerr << "Step 6: dump the 'distribution' dictionary..." << endl;
		cerr << "\tStoring " << dictionary.getCount() << " words into the distribution dictionary..." << endl;

		dictionary.forEachFreq(dumpDictionary);
		dictionary.clear();

		delete dictionaryWriteRAOF;
		dictionaryWriteRAOF=NULL;
	}


	// store the disttype.dat and disttype.jmp

	cerr << "\tStoring " << typeAltRecSet.getCount() << " types into the distribution type table..." << endl;

	typeAltRecSet.forEach(dumpTypeAltRecSet);
	typeAltRecSet.clear();

	// clean up

	cerr << "Step 7: Free some resources before indexing... " << endl; 

	if (badWordFile!=NULL)
	{
		delete badWordFile;
		badWordFile=NULL;
	}

	if (grammarFile!=NULL)
	{
		delete grammarFile;
		grammarFile=NULL;
	}

	if (dictionaryWriteRAOF!=NULL)
	{
		delete dictionaryWriteRAOF;
		dictionaryWriteRAOF=NULL;
	}

	if (typeWriteRAOF!=NULL)
	{
		delete typeWriteRAOF;
		typeWriteRAOF=NULL;
	}

	if (modelWriteRAOF!=NULL)
	{
		delete modelWriteRAOF;	
		modelWriteRAOF=NULL;
	}

	typeAltRecSet.clear();
	modelSet.clear();
	dictionary.clear();
	badWords.clear();

	// create the alternate index for the distdict table

	cerr << "Step 8: Create alternate index for dictionary table... " << endl; 

	if (outputDictPrefix.length()>0)
	{
		MTCcreateAltJmpRAOF < MTCdictAltRecord > makeDictIndex(outputDictPrefix + "dict");

		makeDictIndex.doIt();
	}
	else
	{
		cerr << "\t\tNo dictionary created - skipping index step" << endl;
	}


	cerr << "That's all folks!" << endl;

	return 0;
}

// load the type alt records from the mstrtype.RAOF into a MTCRBTFreq
// return the typeIndex for BAD_CODE

long loadTypeAltRecords(MTCRBT <MTCtypeAltRecord> &typeAltRecSet, const MTCstring & aDictPrefix)
{
	// readRAOF as a typeAltRecord

	MTCreadRAOF <MTCtypeAltRecord> typeAltRecordReadRAOF("mstrtype");

	// "optimally" load all typeAltRecords from readRAOF into RBT

	MTCraofToRBT <MTCtypeAltRecord> typeAltRecordRAOFToRBT;

	typeAltRecordRAOFToRBT.doIt(typeAltRecordReadRAOF, typeAltRecSet);

	// clear the freqency of each typeAltRecord

	typeAltRecSet.forEach(clearTypeAltRecFreq);

	// look up and return the badTypeIndex

	MTCreadRAOF <MTCtypeRecord> typeRecordReadRAOF(aDictPrefix + "type");
	MTCtypeRecord temp;

	temp.setCode(BAD_CODE);
	typeRecordReadRAOF.find(temp);

	return typeRecordReadRAOF.getIndex();
}

void clearTypeAltRecFreq(MTCtypeAltRecord *aTypeAltRec)
{
	aTypeAltRec->setFreq(0);
}

const MTCstring removeNewLines(const MTCstring &aString)
{
	MTCstring output;

	for(int i=0; i<aString.length(); i++)
	{
		if (aString.c_str()[i]=='\n')
		{
			output+="n";
		}
		else if (aString.c_str()[i]!='\"')
		{
			if ((aString.c_str()[i]==' ')&&(i>0)&&(aString.c_str()[i-1]==' '))
			{
				break;
			}
			output+=aString.c_str()[i];
		}
	}

	return output; // +" ";
}

// dump the dictionary 

void dumpDictionary(MTCdictRecord *aDictRec, long freq)
{
	aDictRec->setFreq(freq);
	dictionaryWriteRAOF->append(*aDictRec);
}

void dumpTypeAltRecSet(MTCtypeAltRecord *aTypeAltRec)
{
	if (typeWriteRAOF!=NULL)
	{
		typeWriteRAOF->append(*aTypeAltRec);
	}
}

void dumpModelSet(MTCsentenceModel *aModel, long freq)
{
	static BOOL firstRule=TRUE;

	aModel->setFreq(freq);

	if (modelWriteRAOF!=NULL)
	{
		modelWriteRAOF->append(aModel);
	}

	long answer;
	MTCtypeAltRecord tempTypeAltRecord;
	MTCtypeAltRecord *foundTypeAltRec;

	if (grammarFile!=NULL)
	{
		if (firstRule==TRUE)
		{
			firstRule=FALSE;
			*grammarFile << "\t\t";
		}
		else
		{
			*grammarFile << "\t|\t";
		}

		do 
		{
			do
			{
				MTCstring punctuation;
				answer=aModel->getNext(&punctuation);
	
				if (punctuation.length()>0)
				{
					*grammarFile << "{" << punctuation << "} ";
				}
			} while (answer==SENTMDL_MORE_PUNCT);

			if (answer!=SENTMDL_GETNEXT_DONE)
			{
				tempTypeAltRecord.setIndex(answer);

				if ((foundTypeAltRec=typeAltRecSet.find(tempTypeAltRecord))==NULL)
				{
					errorMsg(EMT_ERROR, "I could not find a type index.  The grammar definition file may be incompleted", "dumpModelSet()"); 
				}
				else
				{
					*grammarFile << foundTypeAltRec->getCode() << ' ';
				}
			}
		} while (answer!=SENTMDL_GETNEXT_DONE);

		*grammarFile << '@' << aModel->getFreq() << endl;
	}
}

void dumpBadWords(MTCstring *str)
{
	if (badWordFile!=NULL)
	{
		*badWordFile << *str << endl;
	}
}

void usage()
{
	cerr << "Usage: genmodel [-s] [-d inputDictionaryPrefix] [-i inputFile] [-j jumpOverFirstLineCount] [-b badWordFile] [-g outputGrammarFile] [-o outputDictionaryPrefix] [-m outputModelPrefix] [[-u updateFreq] | [-q]]" << endl;
 
	cerr << "\t-s\tDo not load tables into RAM -- read it as needed from disk" << endl;
	cerr << "\t-d\tinput dictionary prefix (default: mstr)" << endl;	
	cerr << "\t-i\tinput file (default: stdin)" << endl;
	cerr << "\t-j\tjump over (ignore) this number of lines at start of input" << endl; 
	cerr << "\t-b\toutput list of words not found in input dictionary" << endl; 
	cerr << "\t-g\toutput a (probably useless) grammar with 1 big rule" << endl;
	cerr << "\t-m\toutput a sentence model table with this prefix (default: dist or -o)" << endl;
	cerr << "\t-o\toutput a dictionary table with this prefix (default: dist)" << endl;
	cerr << "\t-u\tprint updates after processing this many models" << endl;
	cerr << "\t-q\tquiet - same as -u 0" << endl;
	cerr << endl;
	cerr << "The statistics output during processing are:" << endl;
	cerr << "\tSR:\tNumber of Sentences Read" << endl;
	cerr << "\tUS:\tNumber of Unique Sentences Read" << endl;
	cerr << "\tWR:\tNumber of Words Read" << endl;
	cerr << "\tUW:\tNumber of Unique Words Read" << endl;
	cerr << "\tBWR:\tNumber of Bad Words Read (not in dictionary)" << endl;
	cerr << "\tUBW:\tNumber of Unique Bad Words Read" << endl;
}

void printStats()
{
	cerr << "\rSR: " << modelCounter << " US: " << modelSet.getCount() << " WR: " << wordCounter << " UW: " << dictionary.getCount() << " BWR: " << badWordCounter << " UBW: " << badWords.getCount() << "        \r";  
}
