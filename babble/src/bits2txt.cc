/*
 * Bitstream to text stream class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: bits2txt.cc,v 1.22 1998/08/23 02:47:07 markc Exp $
 *
 * $Log: bits2txt.cc,v $
 * Revision 1.22  1998/08/23 02:47:07  markc
 * fixed check for quoted punctuation in printPunct
 *
 * Revision 1.21  1998/02/19 19:31:25  markc
 * *** empty log message ***
 *
 * Revision 1.20  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id: bits2txt.cc,v 1.22 1998/08/23 02:47:07 markc Exp $";
 *
 * Revision 1.19  1997/03/02 04:23:26  markc
 * Added MINIMAL_EXTRA_BITS
 * otherwise the bits2txt may fall short
 *
 * Revision 1.18  1997/02/27 17:52:25  markc
 * Fixed usage()
 *
 * Revision 1.17  1997/02/27 17:12:00  markc
 * Shortened status G:, etc.
 *
 * Revision 1.16  1997/02/27 15:56:30  markc
 * Adjusted isReady()
 *
 * Revision 1.15  1997/02/27 13:35:31  chapman
 * Enabled -s parameter for smaller memory usage
 *
 * Revision 1.14  1997/02/24 05:01:40  markc
 * Increased PRINTSTAT_FREQ from 1000 to 10000
 *
 * Revision 1.13  1997/02/24 04:27:49  markc
 * Added bit-input and output statistics
 *
 * Revision 1.12  1997/02/24 00:12:19  markc
 * Added printStats and fixed maxModelLength
 *
 * Revision 1.11  1997/02/23 20:39:35  markc
 * Added quoted punctutation {^xxxxx^} processing
 *
 * Revision 1.10  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.9  1996/02/17 16:44:07  markc
 * eolAfterModel default changed to FALSE
 *
// Revision 1.8  1996/02/14  15:06:35  markc
// added space BOOL
// adjustment for "(" punctuation
//
// Revision 1.7  1996/02/08  01:26:02  markc
// always complete a model at the end of input stream + extra bits
// freshModel is used
//
// Revision 1.6  1996/02/08  00:53:48  markc
// added {low} {cap} {shift} to models
//
// Revision 1.5  1996/01/20  18:40:19  markc
// added eolAfterModel
//
// Revision 1.4  1996/01/13  13:33:36  markc
// -m option for babble added -- both models and grammars work!
//
// Revision 1.3  1996/01/13  13:02:28  markc
// Cap first letter of first word in each sentence
//
// Revision 1.2  1996/01/13  10:18:10  markc
// use new sentnode enabled sentence models
//
// Revision 1.1  1996/01/13  09:53:08  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: bits2txt.cc,v 1.22 1998/08/23 02:47:07 markc Exp $";

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

#define MINIMAL_EXTRA_BITS (sizeof(unsigned long)*8) 

int strlenNL=strlen("\n");

// constructor

MTCbitsToTextBase::MTCbitsToTextBase()
	: 	inputBitStream(NULL), outputStream(NULL), okay(TRUE), 
		model(NULL), capFirstWord(FALSE), firstWord(TRUE),
		eolAfterModel(FALSE), cap(FALSE), shift(FALSE), 
		freshModel(FALSE), space(FALSE), maxModelLength(1024),
		modelsSkipped(0), modelsUsed(0), totalLengthUsed(0),
		inputBits(-8*sizeof(bitBucketType)), 
		extraBits(8*sizeof(bitBucketType)), outputBytes(0),
		smallMode(FALSE), dictAltRecRAOF(NULL), 
		statFreq(DEFAULT_PRINTSTAT_FREQ)
{
	return;
}

// destructor

MTCbitsToTextBase::~MTCbitsToTextBase()
{
	if (inputBitStream!=NULL)
	{
		delete inputBitStream;
		inputBitStream=NULL;
	}

	if (outputStream!=NULL)
	{
		if (outputStream!=&cout)
		{
			delete outputStream;
		}
		outputStream=NULL;
	}

	if (dictAltRecRAOF!=NULL)
	{
		delete dictAltRecRAOF;
		dictAltRecRAOF=NULL;
	}

	return;
}

// set dictionary raof file name and open it

BOOL MTCbitsToTextBase::openDictName(const MTCstring &aFileName, BOOL useSmallMode)
{
	// clear the dictAltRecRBT
	dictAltRecRBT.clear();

	if (dictAltRecRAOF!=NULL)
	{
		delete dictAltRecRAOF;
		dictAltRecRAOF=NULL;
	}

	// if small mode then load the dictAltRecRBT from the RAOF
	// otherwise just set up dictAltRecRAOF

	smallMode=useSmallMode;

	dictName=aFileName;

	MTCraofToRBT <MTCdictAltRecord> loader;

	dictAltRecRAOF = new MTCreadRAOF <MTCdictAltRecord> (dictName+".dat", dictName+".alt");

	if (dictAltRecRAOF==NULL)
	{
		errorMsg(EMT_ERROR, "I could not open the dictionary specified", "MTCbitsToTextBase::openDictName()");
		return FALSE;
	}

	if (dictAltRecRAOF->getOkay()==TRUE)
	{
		if (smallMode==FALSE)
		{
			loader.doIt(*dictAltRecRAOF, dictAltRecRBT);
		}
	}
	else
	{
		errorMsg(EMT_ERROR, "I could not open the dictionary specified", "MTCbitsToTextBase::openDictName()");
		return FALSE;
	}

	if (((smallMode==FALSE)&&(dictAltRecRBT.getCount()==0))
	||(dictAltRecRAOF->getCount()==0))
	{
		errorMsg(EMT_ERROR, "The dictionary specified is empty", "MTCbitsToTextBase::openDictName()");
		return FALSE;
	}

	return TRUE;
}

// set type RAOF name and open it

BOOL MTCbitsToTextBase::openTypeName(const MTCstring &aFileName)
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
		errorMsg(EMT_ERROR, "I could not open the type table specified", "MTCbitsToTextBase::openTypeName()");
		return FALSE;
	}

	if (typeAltRecRBT.getCount()==0)
	{
		errorMsg(EMT_ERROR, "The type table specified is empty", "MTCbitsToTextBase::openDictName()");
		return FALSE;
	}

	return TRUE;
}

// open the input file bitstream

BOOL MTCbitsToTextBase::openInputName(const MTCstring &aFileName)
{
	// close it if one is already open...

	if (inputBitStream!=NULL)
	{
		delete inputBitStream;
	}

	// open the input file

	inputName=aFileName;
	inputBitStream=new MTCinputBitStream(inputName);

	if (inputBitStream->getStreamBitsLeft()==0)
	{
		errorMsg(EMT_WARNING, "I could not open the input file specified", "MTCbitsToTextBase::openInputName()"); 
		return FALSE;
	}

	return TRUE;
}

// open the output file 

BOOL MTCbitsToTextBase::openOutputName(const MTCstring &aFileName)
{
	// close it if one is already open...

	if ((outputStream!=NULL)&&(outputStream!=&cout))
	{
		delete outputStream;
	}

	// open the output file

	outputName=aFileName;
	outputStream=new ofstream(outputName.c_str());

	if (outputStream==NULL)
	{
		errorMsg(EMT_WARNING, "I could not open the output file specified", "MTCbitsToTextBase::openOutputName()"); 
		return FALSE;
	}

	return TRUE;
}

// get the next type
long MTCbitsToTextBase::getNextType()
{
	long answer;

	if (model==NULL)
	{
		getNextModel();
		if (eolAfterModel==TRUE)
		{
			*outputStream << endl;
			incOutputBytes();
		}
		firstWord=TRUE;
		return getNextType();
	}
	else
	{
		do
		{
			MTCstring punctuation;
			answer=model->getNext(&punctuation);
			printPunctuation(punctuation);
		} while (answer==SENTMDL_MORE_PUNCT);

		if (answer==SENTMDL_GETNEXT_DONE)
		{
			model=NULL;
			firstWord=TRUE;
			return getNextType();
		}
	}

	return answer;
}

// print punctuation

void MTCbitsToTextBase::printPunctuation(MTCstring &aStr)
{
	if (aStr==SENTMDL_CAP_NEXT_LETTER)
	{
		shift=TRUE;
		return;
	}

	if (aStr==SENTMDL_CAPSLOCK_ON)
	{
		cap=TRUE;
		return;
	}

	if (aStr==SENTMDL_CAPSLOCK_OFF)
	{
		cap=FALSE;
		return;
	}

	// processing for quoted punctuation {^xxxx^}
	// it will just be printed as-is without interpretation

	int i;

	if (aStr.length()>1)
	{
		if ((aStr.c_str()[0]=='^')&&(aStr.c_str()[aStr.length()-1]=='^'))
		{
			for(i=1; i<aStr.length()-1; i++)
			{
				*outputStream << aStr.c_str()[i];
			}

	 		incOutputBytes(aStr.length()-2);
	
			return;
		}

		if (aStr.c_str()[0]=='(')
		{
			*outputStream << ' ';
			incOutputBytes();
			space=FALSE;
		}
	}

	for(i=0; i<aStr.length(); i++)
	{
		switch (aStr.c_str()[i]) {

		case 'n':  	*outputStream << endl;
				incOutputBytes(strlenNL);
				space=FALSE;
				break;
		case 'e':	break;
		case ' ':
				if (space==TRUE)
				{
					*outputStream << ' ';
					incOutputBytes();
				}
				break;
		default:
				*outputStream << aStr.c_str()[i];
				incOutputBytes();
				space=TRUE;
				break;
		}
	}
}

// am I ready to doIt()?

BOOL MTCbitsToTextBase::isReady()
{
	if (getOkay()==FALSE)
	{
		errorMsg(EMT_ERROR, "I am not okay to convert.  See previous messages for a reason.", "MTCbitsToTextGrammar::isReady()"); 
		return FALSE;
	}

	if (dictName.length()==0)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because a dictionary has not been assigned", "MTCbitsToTextGrammar::isReady()"); 
		return FALSE;
	}

	if (typeName.length()==0)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because a type table name has not been assigned", "MTCbitsToTextGrammar::isReady()"); 
		return FALSE;
	}

	if (outputName.length()==0)
	{
		if ((outputStream!=NULL)&&(outputStream!=&cout))
		{
			delete outputStream;
		}

		outputStream=&cout;
	}

	if (inputBitStream==NULL)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because an input bitstream has not been assigned", "MTCbitsToTextGrammar::isReady()"); 
		return FALSE;
	}

	if (outputStream==NULL)
	{
		outputStream=&cout;
	}
	
	if (((smallMode==FALSE)&&(dictAltRecRBT.getCount()==0))
	||(dictAltRecRAOF->getCount()==0))
	{
		errorMsg(EMT_ERROR, "The dictionary specified is empty", "MTCbitsToTextBase::openDictName()");
		return FALSE;
	}

	return TRUE;
}

// take bits and generate text

BOOL MTCbitsToTextBase::doIt(BOOL checkIfReady)
{
	if (checkIfReady==TRUE)
	{
		if (isReady()==FALSE)
		{
			return FALSE;
		}
	}

	MTCtypeAltRecord typeAltRec;
	MTCtypeAltRecord *foundType;
	MTCdictAltRecord dictAltRec;
	const MTCdictAltRecord *foundDict;
	BOOL usingExtraBits=FALSE;

	for(;;)
	{
		freshModel=FALSE;

		typeAltRec.setIndex(getNextType());

		if ((foundType=typeAltRecRBT.find(typeAltRec))==NULL)
		{
			errorMsg(EMT_WARNING,"BAD TYPE FOUND","MTCbitsToTextBase::doIt()"); 
			cerr << typeAltRec << endl;
			continue;
		}

		if (foundType->getFreq()==0)
		{
			errorMsg(EMT_WARNING, "ZERO WORDS OF THIS TYPE FOUND IN DICTIONARY", "MTCbitsToTextBase::doIt()");
			cerr << typeAltRec << endl;
			continue;
		}

		// compute the inputBits properly and switch to counting extras

		if (usingExtraBits==FALSE)
		{
			if (inputBitStream->getStreamBitsLeft()==0)
			{
				long bitDelta=(foundType->getBitCount()-inputBitStream->getBufferSize());

				if (bitDelta>=0)
				{
					incInputBits(inputBitStream->getBufferSize());
					incExtraBits(-1*inputBitStream->getBufferSize());
					usingExtraBits=TRUE;
				}
			}
		}

		dictAltRec.setTypeIndex(foundType->getIndex());

		dictAltRec.setBitString(inputBitStream->readBits(foundType->getBitCount()));

		// update statistics

		if (usingExtraBits==TRUE)
		{
			// use extra bits just to the end of this model...
			// well -- add a few more to flush things out...

			if ((freshModel==TRUE)&&(getExtraBits()>=MINIMAL_EXTRA_BITS))
			{
			 	break;
			}
			else
			{
				incExtraBits(foundType->getBitCount());
			}
		}
		else
		{
			incInputBits(foundType->getBitCount());
		}

		if (smallMode==TRUE)
		{
			foundDict=dictAltRecRAOF->find(dictAltRec);
		}
 		else
		{
			foundDict=dictAltRecRBT.find(dictAltRec);
		}

		if (foundDict==NULL)
		{
			errorMsg(EMT_WARNING, "WORD NOT FOUND", "MTCbitsToTextBase::doIt()");
		}
		else
		{
			// put space between words?

			if (space==TRUE)
			{
				*outputStream << ' ';
				incOutputBytes();
			}

			space=TRUE;

			if (cap==TRUE)
			{
				MTCstring temp(foundDict->getValue());
				temp.toUpper();
				*outputStream << temp;
				incOutputBytes(temp.length());
			}
			else if (((firstWord==TRUE)&&(capFirstWord==TRUE))
				||(shift==TRUE))
			{
				if (foundDict->getValue().length()!=0)
				{
					*outputStream << (char)toupper(foundDict->getValue().c_str()[0]);
					*outputStream << &foundDict->getValue().c_str()[1];
					incOutputBytes(foundDict->getValue().length());
				}
	
				firstWord=shift=FALSE;
			}
			else
			{
				*outputStream << foundDict->getValue();
				incOutputBytes(foundDict->getValue().length());
			}
		}
	}

	*outputStream << endl;
	incOutputBytes(strlenNL);

	return TRUE;
}

// print statistics to stderr

void MTCbitsToTextBase::printStats()
{
	if ((getInputBits()+getExtraBits())==0)
	{
		return;
	}

	cerr << "I: " << getInputBits();
	cerr << " E: " << getExtraBits();
	cerr << " O: " << getOutputBytes()*8;
	cerr << " G: " << (long)(((long)getOutputBytes()*800L)/((long)getExtraBits()+(long)getInputBits()));
	cerr << " U: " << getModelsUsed();
	cerr << " S: " << getModelsSkipped();
	cerr << " N: " << getTotalLengthUsed();

	if (getModelsUsed()==0)
	{
		cerr << " A: 0";
	}
	else
	{
		cerr << " A: " << (getTotalLengthUsed()/getModelsUsed());
	}

	cerr << "     \r";
}

// constructor

MTCbitsToTextGrammar::MTCbitsToTextGrammar()
	:	grammar(NULL)
{
	return;
}

// destructor

MTCbitsToTextGrammar::~MTCbitsToTextGrammar()
{
	if (grammar!=NULL)
	{
		delete grammar;
		grammar=NULL;
	}

	return;
}

// open the grammar definition file and the type record table

BOOL MTCbitsToTextGrammar::openGrammarName(const MTCstring &aFileName)
{
	gramName=aFileName;

	// check type alt rec dependency

	if (getTypeName().length()==0)
	{
		errorMsg(EMT_ERROR, "I cannot open the grammar definition file until the type record alt table is loaded", "MTCbitsToTextGrammar::openGrammarName()");
		return FALSE;
	}

	// delete existing grammar (if any)

	if (grammar!=NULL)
	{
		delete grammar;
		grammar=NULL;
	}

	// create the new grammar

	grammar=new MTCgrammar(gramName, getTypeName()); 

	if (grammar==NULL)
	{
		errorMsg(EMT_ABORT, "I could not allocate a grammar object", "MTCbitsToTextGrammar::openGramName()");
	}

	if (grammar->getOkay()!=TRUE)
	{
		errorMsg(EMT_ERROR, "I could not create an 'okay' grammar object", "MTCbitsToTextGrammar::openGramName()");
		return FALSE;
	}

	return TRUE;
}

// get the next model using a grammar

void MTCbitsToTextGrammar::getNextModel()
{
	freshModel=TRUE;

	if (grammar==NULL)
	{
		errorMsg(EMT_ABORT, "I cannot use a NULL grammar", "MTCbitsToTextGrammar::getNextModel()");
	}

	if (getStatFreq()!=0)
	{
		if (((getModelsUsed()+getModelsSkipped())%getStatFreq())==0)
		{
			printStats();
		}
	}

	int modelLength=getMaxModelLength();

	if ((model=grammar->makeModel(modelLength, TRUE))==NULL)
	{
		errorMsg(EMT_ABORT, "The grammar did not return a model", "MTCbitsToTextGrammar::getNextModel()");
	}

	// check the length of the model

	if (model->getLength()>getMaxModelLength())
	{
		incModelsSkipped();
		getNextModel();
	}

	incModelsUsed();

	incTotalLengthUsed(model->getLength());

	return;
}

// am I ready to doIt()?

BOOL MTCbitsToTextGrammar::isReady()
{
	if (grammar==NULL)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because a grammar has not been assigned", "MTCbitsToTextGrammar::isReady()"); 
		return FALSE;
	}

	if (grammar->isReady()==FALSE)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because the grammar is not ready.", "MTCbitsToTextGrammar::isReady()"); 
		return FALSE;
	}

	if ((getSmallMode()==TRUE)&&(getDictAltRecRAOF()==NULL))
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because the dictionary is not ready.", "MTCbitsToTextGrammar::isReady()"); 
		return FALSE;
	}

	return MTCbitsToTextBase::isReady();
}

// constructor

MTCbitsToTextModel::MTCbitsToTextModel()
	: modelRAOF(NULL)
{
	return;
}

// destructor

MTCbitsToTextModel::~MTCbitsToTextModel()
{

	if (modelRAOF!=NULL)
	{
		delete modelRAOF;
		modelRAOF=NULL;
	}

	return;
}

// am I ready to doIt()?

BOOL MTCbitsToTextModel::isReady()
{
	if (modelRAOF==NULL)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because a model table has not been assigned", "MTCbitsToTextModel::isReady()"); 
		return FALSE;
	}

	if (modelRAOF->getCount()==0)
	{
		errorMsg(EMT_ERROR, "I am not ready to convert because an EMPTY model table has been assigned", "MTCbitsToTextModel::isReady()"); 
		return FALSE;
	}

	return MTCbitsToTextBase::isReady();
}

// get next model -- from the model RAOF

void MTCbitsToTextModel::getNextModel()
{
	freshModel=TRUE;

	long randomJump=random()%modelRAOF->getCount();

	if ((modelRAOF==NULL)||(modelRAOF->getCount()==0))
	{
		errorMsg(EMT_ABORT, "I cannot get the next model because the model is empty or not specified.", "MTCbitsToTextModel::getNextModel()");
	}

	model=modelRAOF->read(randomJump);

	if (getStatFreq()!=0)
	{ 
		if (((getModelsUsed()+getModelsSkipped())%getStatFreq())==0)
		{
			printStats();
		}
	}

	if (model->getLength()>getMaxModelLength())
	{
		incModelsSkipped();
		getNextModel();
	}

	if (model==NULL)
	{
		cerr << "* * * ERROR: getNextModel() failed...trying again..." << endl;
		getNextModel();
	}

	incModelsUsed();

	incTotalLengthUsed(model->getLength());

	return;
}

// open the model table

BOOL MTCbitsToTextModel::openModelName(const MTCstring &aFileName)
{
	modelName=aFileName;

	// close existing table

	if (modelRAOF!=NULL)
	{
		delete modelRAOF;
		modelRAOF=NULL;
	}

	// allocate a new table

	if ((modelRAOF=new MTCreadRAOF <MTCsentenceModel> (modelName))==NULL)
	{
		errorMsg(EMT_ERROR, "I could not allocate a model table access object", "MTCbitsToTextModel::openModelName()");
		return FALSE;
	}

	// is the model empty?

	if (modelRAOF->getCount()==0)
	{
		errorMsg(EMT_ERROR, "The model table specified is empty.", "MTCbitsToTextModel::openModelName()");
		return FALSE;
	}

	return TRUE;
}
