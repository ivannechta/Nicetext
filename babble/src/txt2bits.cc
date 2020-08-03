/*
 * Text to Bitstream Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: txt2bits.cc,v 1.7 1998/02/19 19:31:25 markc Exp $
 *
 * $Log: txt2bits.cc,v $
 * Revision 1.7  1998/02/19 19:31:25  markc
 * *** empty log message ***
 *
 * Revision 1.6  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id: txt2bits.cc,v 1.7 1998/02/19 19:31:25 markc Exp $";
 *
 * Revision 1.5  1997/03/03 19:46:32  markc
 * Added -v for verbose error reporting
 *
 * Revision 1.4  1997/02/27 15:56:17  markc
 * Added -s Small Mode
 *
 * Revision 1.3  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.2  1996/02/08 00:57:56  markc
 * do not warn if a word is not found in dictionary table
 *
// Revision 1.1  1996/01/13  15:13:56  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: txt2bits.cc,v 1.7 1998/02/19 19:31:25 markc Exp $";

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
#include "../../gendict/include/dconst.h"
#include "../include/bitstrm.h"
#include "../include/bitoutput.h"
#include "../include/txt2bits.h"

int zzlex();
extern char *zztext;
extern FILE *zzin;

// constructor

MTCtextToBits::MTCtextToBits()
	: 	outputBitStream(NULL), inputStream(NULL), 
		okay(TRUE), smallMode(FALSE), dictRecRAOF(NULL),
		verboseMode(FALSE)
{
	return;
}

// destructor

MTCtextToBits::~MTCtextToBits()
{
	if (outputBitStream!=NULL)
	{
		delete outputBitStream;
		outputBitStream=NULL;
	}

	if (inputStream!=NULL)
	{
		if (inputStream!=stdin)
		{
			if (fclose(inputStream)==EOF)
			{
				perror(inputName.c_str());
			}
		}
		inputStream=NULL;
	}

	if (dictRecRAOF!=NULL)
	{
		delete dictRecRAOF;
		dictRecRAOF=NULL;
	}

	return;
}

// set dictionary raof file name and open it

BOOL MTCtextToBits::openDictName(const MTCstring &aFileName, BOOL useSmallMode)
{
	// set small mode

	setSmallMode(useSmallMode);

	if (dictRecRAOF!=NULL)
	{
		delete dictRecRAOF;
		dictRecRAOF=NULL;
	}

	// clear the dictRecRBT
	dictRecRBT.clear();

	// load the dictRecRBT from the RAOF

	dictName=aFileName;


	dictRecRAOF=new MTCreadRAOF <MTCdictRecord> (dictName);

	if (dictRecRAOF==NULL)
	{
		errorMsg(EMT_ERROR, "I could not open the dictionary specified", "MTCtextToBits::openDictName()");
		return FALSE;
	}

	if (dictRecRAOF->getOkay()==TRUE)
	{
		if (getSmallMode()==FALSE)
		{
			MTCraofToRBT <MTCdictRecord> loader;
			loader.doIt(*dictRecRAOF, dictRecRBT);
		}
	}
	else
	{
		errorMsg(EMT_ERROR, "I could not open the dictionary specified", "MTCtextToBits::openDictName()");
		return FALSE;
	}

	if (((dictRecRBT.getCount()==0)&&(getSmallMode()==FALSE))
	|| (dictRecRAOF->getCount()==0))
	{
		errorMsg(EMT_ERROR, "The dictionary specified is empty", "MTCtextToBits::openDictName()");
		return FALSE;
	}

	return TRUE;
}

// set type RAOF name and open it

BOOL MTCtextToBits::openTypeName(const MTCstring &aFileName)
{
	// clear the typeAltRecRBT
	typeAltRecRBT.clear();

	// assign type name
	typeName=aFileName;

	// load the typeAltRecRBT from the RAOF

	MTCraofToRBT <MTCtypeAltRecord> loader;
	MTCreadRAOF <MTCtypeAltRecord> typeAltRecRAOF(typeName);

	if (typeAltRecRAOF.getOkay()==TRUE)
	{
		loader.doIt(typeAltRecRAOF, typeAltRecRBT);
	}
	else
	{
		errorMsg(EMT_ERROR, "I could not open the type table specified", "MTCtextToBits::openTypeName()");
		return FALSE;
	}

	if (typeAltRecRBT.getCount()==0)
	{
		errorMsg(EMT_ERROR, "The type table specified is empty", "MTCtextToBits::openDictName()");
		return FALSE;
	}

	return TRUE;
}

// open the output file bitstream

BOOL MTCtextToBits::openOutputName(const MTCstring &aFileName)
{
	// close it if one is already open...

	if (outputBitStream!=NULL)
	{
		delete outputBitStream;
	}

	// open the output file

	outputName=aFileName;
	outputBitStream=new MTCoutputBitStream(outputName);

	return TRUE;
}

// open the input file 

BOOL MTCtextToBits::openInputName(const MTCstring &aFileName)
{
	// close it if one is already open...

	if ((inputStream!=NULL)&&(inputStream!=stdin))
	{
		delete inputStream;
	}

	// open the input file

	inputName=aFileName;
	inputStream=fopen(inputName.c_str(), "r");

	if (inputStream==NULL)
	{
		errorMsg(EMT_WARNING, "I could not open the input file specified", "MTCtextToBits::openInputName()"); 
		return FALSE;
	}

	return TRUE;
}

// am I ready to doIt()?

BOOL MTCtextToBits::isReady()
{
	if (getOkay()==FALSE)
	{
		errorMsg(EMT_ERROR, "I am not okay to convert.  See previous messages for a reason.", "MTCtextToBits::isReady()"); 
		return FALSE;
	}

	if (dictName.length()==0)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because a dictionary has not been assigned", "MTCtextToBits::isReady()"); 
		return FALSE;
	}

	if (typeName.length()==0)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because a type table name has not been assigned", "MTCtextToBits::isReady()"); 
		return FALSE;
	}

	if (outputBitStream==NULL)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because an output bitstream has not been assigned", "MTCtextToBits::isReady()"); 
		return FALSE;
	}

	if (inputStream==NULL)
	{
		inputStream=stdin;
	}
	
	if (((dictRecRBT.getCount()==0)&&(getSmallMode()==FALSE))
	|| (dictRecRAOF==NULL)||(dictRecRAOF->getCount()==0))
	{
		errorMsg(EMT_ERROR, "The dictionary specified is empty", "MTCtextToBits::openDictName()");
		return FALSE;
	}

	return TRUE;
}

// take bits and generate text

BOOL MTCtextToBits::doIt(BOOL checkIfReady)
{
	if ((checkIfReady==TRUE)&&(isReady()==FALSE))
	{
		errorMsg(EMT_ERROR, "I am not ready", "MTCtextToBits::doIt()");
		return FALSE;
	}

	const MTCdictRecord *foundDict;
	MTCdictRecord tempDictRec;
	MTCtypeAltRecord *foundType;
	MTCtypeAltRecord tempTypeAltRec;
	int retval;
	zzin=inputStream;

	for(;;)
	{
		retval=zzlex();		// read the next token from stdin

		if (zztext[0]=='\0')	// is it eof?
		{
			break;
		}

		if (retval==END_OF_WORD)
		{
			// use lower case of word for now...

			MTCstring zztextString(zztext);
			zztextString.toLower();	
			tempDictRec.setValue(zztextString);

			// look up the word in the dictionary table

			if (getSmallMode()==TRUE)
			{
				foundDict=dictRecRAOF->find(tempDictRec);
			}
			else
			{
				foundDict=dictRecRBT.find(tempDictRec);
			}

			if (foundDict!=NULL)
			{
				// look up the type

				tempTypeAltRec.setIndex(foundDict->getTypeIndex());
				if ((foundType=typeAltRecRBT.find(tempTypeAltRec))!=NULL)
				{
					outputBitStream->writeBits(foundDict->getBitString(), foundType->getBitCount());
				}
				else
				{
					errorMsg(EMT_WARNING, "ERROR: I could not find the type in the index.  DICTIONARY MAY BE CORRUPTED", "MTCtextToBits::doIt");
					cerr << "\t" << *foundDict << endl;
				}
			}
			else
			{
				if (getVerboseMode()==TRUE)
				{
					cerr << "Ignoring Undefined Word: " << zztextString << endl;
				}
			}
		}
		else
		{
			if ((retval!=END_OF_SENTENCE)&&(retval!=PUNCTUATION))
			{
				errorMsg(EMT_WARNING, "IGNORING INPUT: NOT AN END_OF_WORD! -- Is the lexword.o the one loaded in? ", "MTCtextToBits::doIt()");
			}
		}
	}

	outputBitStream->writeBits(0xFFFFFFFF, 32);

	return TRUE;
}
