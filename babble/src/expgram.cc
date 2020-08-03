/*
 * Expand Grammar Rules Automatically to take into account merged types
 * (and frequencies) 
 *
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: expgram.cc,v 1.12 1998/08/17 20:24:28 markc Exp $
 *
 * $Log: expgram.cc,v $
 * Revision 1.12  1998/08/17 20:24:28  markc
 * modified usage()
 *
 * Revision 1.11  1997/05/03 14:10:36  markc
 * Added reference to dumptype.sh
 * Added {^xxxx^} Quoted punctuation syntax to output grammar file
 * /.
 * /
 *
 * Revision 1.10  1997/04/22 16:42:46  markc
 * Usage()
 *
 * Revision 1.9  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id: expgram.cc,v 1.12 1998/08/17 20:24:28 markc Exp $";
 *
 * Revision 1.8  1997/02/16 05:20:06  markc
 * *** empty log message ***
 *
 * Revision 1.7  1996/11/19 03:26:12  markc
 * Added -n option to normalize the frequency of merged types
 *
 * Revision 1.6  1996/11/08 04:47:54  markc
 * revised scramble warning
 *
 * Revision 1.5  1996/11/08 04:38:52  markc
 * removed scramble warning message
 *
 * Revision 1.4  1996/04/11 17:25:08  markc
 * added [-s sampleFile] for a particular purpose for development.
 * It will remain as an undocumented feature...no wait...
 *
 * Revision 1.3  1996/02/11 17:33:15  markc
 * added isSUbset to check for types
 *
// Revision 1.2  1996/02/07  03:52:05  markc
// it works for now!
//
// Revision 1.1  1996/02/07  03:11:09  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: expgram.cc,v 1.12 1998/08/17 20:24:28 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/raof.h"
#include "../../gendict/include/dconst.h"
#include "../../gendict/include/typerec.h"

void usage();

void baseLoop(MTCtypeRecord *);
void mergeLoop(MTCtypeRecord *);
BOOL isSubset(const MTCstring &code);
long countMerges(const MTCstring &code); 

MTCRBT <MTCtypeRecord> baseTypeList;
MTCRBT <MTCtypeRecord> mergeTypeList;
MTCstring baseString;
BOOL normalize(FALSE);
ostream *outputStream=&cout;
ofstream *sampleStream=NULL;

int main(int argc, char * argv[])
{
	// check command line arguments
	extern char *optarg;
	extern int optind;
	int ch;
	MTCstring typeFile("mstrtype");
	MTCstring outputFileName;
	MTCstring sampleFileName;

	while ((ch = getopt(argc, argv, "hd:o:s:n")) != EOF)
	{
		switch(ch) 
		{
			case 'd':
				typeFile=optarg;
				typeFile+="type";
				break;
			case 'o':
				outputFileName=optarg;
				break;
			case 's':
				sampleFileName=optarg;
				break;
			case 'n':
				normalize=TRUE;
				break;
			case 'h':
			default:
				usage();
				return (-1);
		}
	}

	argc -= optind;
 	argv += optind;

	cerr << "Expanding Grammar To Automatically Allow Merged Types..." << endl;

	cerr << "Stage 1: Setup"<< endl;

	cerr << "\tOpening type table: " << typeFile << endl;

	MTCreadRAOF <MTCtypeRecord> raof(typeFile);

	if (raof.getOkay()==FALSE)
	{
		exit(-1);
	}

	ofstream *outputFile=NULL;

	if (outputFileName.length()==0)
	{
		cerr << "\tUsing stdout for output." << endl;
	}
	else
	{
		cerr << "\tOpening output file: " << outputFileName << endl;

		outputFile=new ofstream(outputFileName.c_str());

	 	if (outputFile==NULL)
		{
			perror(outputFileName.c_str());
			exit(-1);
		}
		else
		{
			outputStream=outputFile;
		}
	}

	*outputStream << "// expanded grammar rules for merged types" << endl;
	*outputStream << endl;

	if (sampleFileName.length()!=0)
	{
		cerr << "\tOpening sample file: " << sampleFileName << endl;

		sampleStream=new ofstream(sampleFileName.c_str());

	 	if (sampleStream==NULL)
		{
			perror(sampleFileName.c_str());
			exit(-1);
		}
	}

	if (sampleStream!=NULL)
	{
		*sampleStream << "// A grammar set that outputs random samples of each LHS merged rule type" << endl;
		*sampleStream << "// use this grammar through nicetext to get examples of each type of word" << endl;
		*sampleStream << "// WARNING: the nicetext from this grammar will not scramble properly" << endl;
		*sampleStream << "// NOTE: do not forget to append the contents of the appropriate expanded grammar file with all the merged types!!!!" << endl;
		*sampleStream << "// Check out the dumptype.sh shell script.  It generates a grammar" << endl;
		*sampleStream << "// to show 3 examples of ALL types in a dictionary," << endl; 
		*sampleStream << "// including non-merged types" << endl;
		*sampleStream << endl;
		*sampleStream << "samples:" << endl;
	}

	cerr << "Stage 2: Sorting Rules into Base and Merged Lists (looking for ',')..." << endl;

	MTCtypeRecord *typeRecPtr=NULL;
	long i;

	for(i=0; i<raof.getCount(); i++)
	{
		if ((typeRecPtr=raof.read(i))==NULL)
		{
			errorMsg(EMT_ABORT, "The raof.read() returned NULL, when I should not be reading beyond the end!", "main()"); 
		}
		else
		{
			if (strchr(typeRecPtr->getCode().c_str(), ',')==NULL)
			{
				if (typeRecPtr->getCode()!=BAD_CODE)
				{
					baseTypeList.add(*typeRecPtr);
				}
			}
			else
			{
				mergeTypeList.add(*typeRecPtr);
			}
		}
	}

	cerr << "\t" << baseTypeList.getCount() << " base rules." << endl; 
	cerr << "\t" << mergeTypeList.getCount() << " merged rules." << endl; 

	cerr << "Stage 3: Creating Expanded Grammar Rules into Output File..." << endl;

	baseTypeList.forEach(baseLoop);

	if (outputFile!=NULL)
	{
		delete outputFile;
		outputFile=NULL;
	}

	if (sampleStream!=NULL)
	{
		*sampleStream << endl << "\t;" << endl;
		delete sampleStream;
		sampleStream=NULL;
	}

	cerr << "That's all folks!" << endl;

}

void usage()
{
	cerr << "Usage: expgram [-d dictionary] [-o outputFile] [-s sampleFile] [-n]" << endl;
	cerr << "\t-d \t dictionary prefix" << endl;
	cerr << "\t-o \t redirect from stdout" << endl;
	cerr << "\t-s \t create a sample grammar (use dumptype.sh instead...)" << endl;
	cerr << "\t-n \t `normalize' frequency based on the number of types in a merge" << endl;
}

// called in forEach loop for baseTypeList

void baseLoop(MTCtypeRecord *b)
{
	MTCstring lhs(b->getCode());

	lhs.toUpper();

	MTCstring lhsString("m");
	lhsString+=lhs;

	*outputStream << lhsString << ':' << endl;

	*outputStream << "\t\t" << b->getCode() << "\t@" << b->getFreq() << endl;

	baseString=b->getCode();

#ifdef DEBUG
	cout << "base: " << baseString << endl;
#endif // DEBUG

	mergeTypeList.forEach(mergeLoop);

	*outputStream << "\t;" << endl; 
	*outputStream << endl;

	if (sampleStream!=NULL)
	{
		*sampleStream << endl;
		*sampleStream << "\t{^" << lhsString << ":   ^} ";
		*sampleStream << lhsString << " {, } "; 
		*sampleStream << lhsString << " {, } "; 
		*sampleStream << lhsString << " {n} "; 
	}

	return;
}

// called in forEach loop for mergeTypeList 

void mergeLoop(MTCtypeRecord *m)
{
#ifdef DEBUG
	cout << "\tmerge: " << m->getCode() << endl;
#endif // DEBUG

	if (isSubset(m->getCode())==TRUE)
	{
#ifdef DEBUG
		cout << "\tYES -- IS SUBSET!" << endl;
#endif // DEBUG
		if (normalize==TRUE)
		{
			long freq=((m->getFreq()+1)/countMerges(m->getCode()));

			freq = freq ? freq : 1;

			*outputStream << "\t|\t" << m->getCode() << "\t@" << freq << endl;
		}
		else
		{
			*outputStream << "\t|\t" << m->getCode() << "\t@" << m->getFreq() << endl; 
		}
	}

	return;
}

// is the baseString a subset of the code?

BOOL isSubset(const MTCstring & code)
{
	if (code==baseString)
	{
		return FALSE;
	}

	char buffer[code.length()+1];

	strcpy(buffer, code.c_str());
	
	char *p;

	p=strtok(buffer, ",");

	while (p!=NULL)
	{
#ifdef DEBUG
		cout << "\t\t\t" << p << endl;
#endif // DEBUG

		if (baseString==p)
		{
			return TRUE;
		}

		p=strtok(NULL, ",");
	}

	return FALSE;
}

// Count Merged Types

long countMerges(const MTCstring &code) 
{
	long retVal=0;

	char buffer[code.length()+1];

	strcpy(buffer, code.c_str());
	
	char *p;

	p=strtok(buffer, ",");

	while (p!=NULL)
	{
		retVal++;
		p=strtok(NULL, ",");
	}

	#ifdef DEBUG
	cout << "RETVAL IS  " << retVal << endl;

	#endif // DEBUG
	return retVal;
}

