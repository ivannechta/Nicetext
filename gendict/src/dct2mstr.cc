/*
 * Raw Dictionary to Master Tables Program 
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: dct2mstr.cc,v 1.16 1997/03/05 17:41:20 markc Exp $
 *
 * $Log: dct2mstr.cc,v $
 * Revision 1.16  1997/03/05 17:41:20  markc
 * Added static char rcsid []  = "@(#)$Id: dct2mstr.cc,v 1.16 1997/03/05 17:41:20 markc Exp $";
 *
 * Revision 1.15  1997/02/22 20:30:13  markc
 * changed default input file to "dct.srt" from "dictionary"
 *
 * Revision 1.14  1997/02/16 06:57:29  markc
 * BST replaced with RBT
 *
 * Revision 1.13  1996/03/21 17:18:56  markc
 * program name changes and messages (minor)
 *
 * Revision 1.12  1996/01/20 17:23:28  markc
 * use rbt version of raofalt instead of bst
 *
// Revision 1.11  1996/01/20  08:59:44  markc
// fixed default dictionary name to be mstrdict instead of mstrtype :(
//
// Revision 1.10  1996/01/14  13:47:33  markc
// added parameters
//
// Revision 1.9  1996/01/14  12:27:45  markc
// added count display
//
// Revision 1.8  1995/11/10  12:25:43  chapman
// I works! -- use new typeREcord, rawDictEnt, dictREcord,
// and most important, MTCcreateAltJmpRAOF -- seems to go!
//
// Revision 1.7  1995/11/08  19:34:46  chapman
// Updated to use rawdict.h instead of dictent.h
//
// Revision 1.6  1995/11/08  18:49:28  chapman
// Updated to dictRecord and typeRecord from mstr*
//
// Revision 1.5  1995/11/08  04:24:14  chapman
// Removed #include <String.h>
//
// Revision 1.4  1995/08/31  02:34:54  markc
// Added suffixes to mstrdict -- THIS DOES NOT WORK -- USE OLD VERSION
//
// Revision 1.3  1995/08/13  23:02:17  markc
// Changed jump to typeIndex -- to be more clean on purpose
//
// Revision 1.2  1995/08/13  22:38:36  markc
// Added ../include/dconst.h and the BAD_CODE and BAD_VALUE items
// into the mstrdict and mstrtype.
// This is an example of how undefined words will be handled.
//
// Revision 1.1  1995/08/13  22:03:55  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: dct2mstr.cc,v 1.16 1997/03/05 17:41:20 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/raof.h"
#include "../../mtc++/include/raofalt.h"
#include "../../mtc++/include/list.h"
#include "../include/rawdict.h"
#include "../include/dictrec.h"
#include "../include/dictarec.h"
#include "../include/typerec.h"
#include "../include/dconst.h"

MTCwriteRAOF <MTCtypeRecord> *typeRecordWriteRAOF=NULL;
MTCwriteRAOF <MTCdictRecord> *dictRecordWriteRAOF=NULL;
MTCRBTFreq  <MTCtypeRecord>  typeRecordRBTFreq;

void createMstrType(MTCtypeRecord *, long); 
void createMstrDict(MTCrawDictEnt *);
void usage();

int main(int argc, char *argv[])
{
	MTCtypeRecord tempTypeRecord;
	MTCrawDictEnt rawDictEnt;

	// check command line arguments
	extern char *optarg;
	extern int optind;
	int ch;
	MTCstring inputFileName;
	MTCstring prefix("mstr");
	MTCstring typeName("mstrtype");
	MTCstring dictName("mstrdict");

	while ((ch = getopt(argc, argv, "d:i:h")) != EOF)
	{
		switch(ch) 
		{
			case 'd':
				prefix=optarg;
				dictName=optarg;
				dictName+="dict";
				typeName=optarg;
				typeName+="type";
				break;
			case 'i':
				inputFileName=optarg;
				break;
			case 'h':
			default:
				usage();
				return (-1);
		}
	}

	if (inputFileName.length()==0)
	{
		errorMsg(EMT_ERROR, "I need to have an input file name specified with the -i option", "main()");
		usage();
		return -1;
	}

	argc -= optind;
 	argv += optind;

	cerr << "Processing Pre-Sorted Dictionary (see sortdct)..." << endl;
	cerr << "\tInput File: " << inputFileName << endl;
	cerr << "\tOutput Dictionary Table: " << prefix << endl; 

	cerr << "Stage 1: read dictionary for type information..." << endl;

	// add a stub for all bad values

	tempTypeRecord.setCode(BAD_CODE);
	typeRecordRBTFreq.add(tempTypeRecord);

	// read all entries in dictionary file

	ifstream infile(inputFileName.c_str());

	if (!infile)
	{
		perror(inputFileName.c_str());
		usage();
		return -1;
	}

	infile >> rawDictEnt;

	int i=0;

	while (infile.eof()==0)
	{
		tempTypeRecord.setCode(rawDictEnt.getCode());
		typeRecordRBTFreq.add(tempTypeRecord);
		infile >> rawDictEnt;
	} 

	cerr << "Stage 2: output master type table..." << endl;

	typeRecordWriteRAOF = new MTCwriteRAOF <MTCtypeRecord> (typeName);

	typeRecordRBTFreq.forEachFreq(createMstrType);

	cerr << "\t" << typeRecordWriteRAOF->getCount() << " types. " << endl;
	delete typeRecordWriteRAOF;
	typeRecordWriteRAOF=NULL;

	cerr << "Stage 3: Create master dictionary table..." << endl;

	dictRecordWriteRAOF = new MTCwriteRAOF <MTCdictRecord> (dictName);

	if (dictRecordWriteRAOF==NULL)
	{
		cerr << "not Enough RAM" << endl;
		return (-1);
	}

	infile.close();
	infile.open(inputFileName.c_str());

	rawDictEnt.setCode(BAD_CODE);	// note: stub not really needed in dict
	rawDictEnt.setValue(BAD_VALUE);	// just in the typeRecordRBTFreq 

	do {
		createMstrDict(&rawDictEnt);	
		infile >> rawDictEnt;
	} while (infile.eof()==0);

	typeRecordRBTFreq.clear();

	cerr << "\t" << dictRecordWriteRAOF->getCount() << " words." << endl;

	delete dictRecordWriteRAOF;
	dictRecordWriteRAOF=NULL;

	cerr << "Stage 4: Create Alternate Dictionary Index" << endl;

	MTCcreateAltJmpRAOF <MTCdictAltRecord> makeAlt(dictName);

	makeAlt.doIt();

	cerr << "That's All Folks!" << endl;

	return 0;
}

void createMstrType(MTCtypeRecord *aTypeRecord, long freq) 
{
	static long typeIndex=0;

	aTypeRecord->setIndex(typeIndex++);
	aTypeRecord->setFreq(freq);

	typeRecordWriteRAOF->append(*aTypeRecord);
}

void createMstrDict(MTCrawDictEnt *aRawDictEnt)
{
	MTCdictRecord dictRecord;
	MTCtypeRecord typeRecord;
	MTCtypeRecord *foundTypeRecord;

	dictRecord.setValue(aRawDictEnt->getValue());
	typeRecord.setCode(aRawDictEnt->getCode());

	foundTypeRecord=typeRecordRBTFreq.find(typeRecord);

	if (foundTypeRecord==NULL)
	{
		cerr << "PROGRAM ERROR: I could not find a record type:" << typeRecord << endl;
		exit(-1);
	}

	dictRecord.setTypeIndex(foundTypeRecord->getIndex());

	// decrement the foundTypeRecord and use index as bitstring...
	// this causes a "reverse ordering" of bitStrings, but it does
	// not matter because we are ultimately accessing things randomly anyway

	foundTypeRecord->setFreq(foundTypeRecord->getFreq()-1);

	dictRecord.setBitString(foundTypeRecord->getFreq());

	dictRecordWriteRAOF->append(dictRecord);
}

void usage()
{
	cerr << "Usage: dct2mstr -i inputFile [-d dictionary]" << endl;
}
