/*
 * Sentence Model Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: sentmdl.cc,v 1.10 1996/07/03 18:43:34 markc Exp $
 *
 * $Log: sentmdl.cc,v $
 * Revision 1.10  1996/07/03 18:43:34  markc
 * Fixed isEqual() method -- it didn't work if the getCounts() were equal
 *
 * Revision 1.9  1996/02/08 00:51:20  markc
 * allow multiple punctuation calls using SENTMDL_MORE_PUNCT
 * instead of a merged recursive call -- this lets {low} {shift}, etc
 * be treated like commands
 *
// Revision 1.8  1996/01/14  19:36:29  markc
// relaxed const
//
// Revision 1.7  1996/01/13  09:41:51  markc
// added sentnode logic instead of pure MTCstrings
//
// Revision 1.6  1996/01/12  23:53:35  markc
// optimized for numbers
//
// Revision 1.5  1996/01/10  21:57:33  markc
// Added default value for freq in constructor
// Modified copy ctor to use MTClist::MTClist(MTClist &) copy ctor instead
// of the for() loop.
//
// Revision 1.4  1995/11/08  19:07:11  chapman
// removed #include "distdict.h"
//
// Revision 1.3  1995/08/26  21:27:48  markc
// Added a clear() call within the stream extractor -
// otherwise it would just append to the list, which is incorrect for problem
// domain.
//
// Revision 1.2  1995/08/24  20:14:09  markc
// working version
//
// Revision 1.1  1995/08/24  18:32:49  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: sentmdl.cc,v 1.10 1996/07/03 18:43:34 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/raof.h"
#include "../../mtc++/include/list.h"
#include "../include/sentnode.h"
#include "../include/sentmdl.h"

// constructor

MTCsentenceModel::MTCsentenceModel() 
	: firstGetNext(TRUE), freq(0)
{
}

// copy constructor

MTCsentenceModel::MTCsentenceModel(MTCsentenceModel &aSrc) 
	: firstGetNext(TRUE), freq(aSrc.freq), list(aSrc.list)
{
}

// append a value to the sentence model 

void MTCsentenceModel::appendPunctuation(const MTCstring &aString)
{
	MTCsentenceNode node;

	node.setTypeIndex(-1);
	node.setPunctuation(aString);

	list.add(node);
}

// append a value to the sentence model

void MTCsentenceModel::appendTypeIndex(long aNum)
{
	MTCsentenceNode node;

	node.setTypeIndex(aNum);

	list.add(node);
}

// clear the sentence model

void MTCsentenceModel::clear()
{
	list.clear();
	firstGetNext=TRUE;
}

// get the next value from the sentence model
// fill in aPunctStr if there is punctuation _BEFORE_ current type

long MTCsentenceModel::getNext(MTCstring *aPunctStr)
{
	MTCsentenceNode *node;

	if (firstGetNext==TRUE)
	{
		firstGetNext=FALSE;
		node=list.getFirst();
	}
	else
	{
		node=list.getNext();
	}

	if (node==NULL)
	{
		return SENTMDL_GETNEXT_DONE;
	}

	if (node->getTypeIndex()==-1)
	{
	 	(*aPunctStr)=node->getPunctuation();
		return SENTMDL_MORE_PUNCT;
	}

	return node->getTypeIndex();
}

// sentenceModel stream insertor operator

ostream & operator<<(ostream & anOStream, MTCsentenceModel & aSrc)
{
	MTCsentenceNode *current;

	for(current=aSrc.list.getFirst(); current!=NULL; current=aSrc.list.getNext())
	{
		anOStream << *current << ' ';
	}

	anOStream << SENTMDL_EOS << ' ' << aSrc.getFreq() << endl;

	return anOStream;
}

// sentenceModel stream extractor operator

istream & operator>>(istream & anIStream, MTCsentenceModel & aSrc)
{
	aSrc.clear();

	MTCsentenceNode current;

	for(;;)
	{
		anIStream >> current;

		if (current.getPunctuation()==SENTMDL_EOS)
		{
			break;
		}

		aSrc.list.add(current);
	};

	long number;

	anIStream >> number;

	aSrc.setFreq(number);

	return anIStream;
}

// < comparison

int MTCsentenceModel::isLessThan(MTCsentenceModel & aSrc) 
{
	BOOL retVal=FALSE;
	MTCsentenceNode *str1=NULL;
	MTCsentenceNode *str2=NULL;

	if (aSrc.list.getCount()==list.getCount())
	{
		for(str1=aSrc.list.getFirst(), str2=list.getFirst(); str1!=NULL, str2!=NULL; str1=aSrc.list.getNext(), str2=list.getNext())
		{
			if (*str1<*str2)
			{
				retVal=TRUE;
				break;
			}
			if (*str1>*str2)
			{
				break;
			}
		}
	}
	else
	{
		retVal=(aSrc.list.getCount()<list.getCount());
	}

	return retVal;
}

// equivalence test

int MTCsentenceModel::isEqual(MTCsentenceModel & aSrc) 
{
	BOOL retVal=FALSE;
	MTCsentenceNode *str1;
	MTCsentenceNode *str2;

	if (aSrc.list.getCount()==list.getCount())
	{
		retVal=TRUE;

		for(str1=aSrc.list.getFirst(), str2=list.getFirst(); str1!=NULL, str2!=NULL; str1=aSrc.list.getNext(), str2=list.getNext())
		{
			if (*str1!=*str2)
			{
				retVal=FALSE;
				break;
			}
		}
	}

	return retVal;
}

// == operator

BOOL MTCsentenceModel::operator==(MTCsentenceModel & aSrc) 
{
	return (isEqual(aSrc));
}

// != operator

BOOL MTCsentenceModel::operator!=(MTCsentenceModel & aSrc) 
{
	return (!isEqual(aSrc));
}

// DictEnt < operator

BOOL MTCsentenceModel::operator<(MTCsentenceModel & aSrc) 
{
	return (isLessThan(aSrc));
}

// DictEnt <= operator

BOOL MTCsentenceModel::operator<=(MTCsentenceModel & aSrc) 
{
	return (isLessThan(aSrc)||isEqual(aSrc));
}

// DictEnt > operator

BOOL MTCsentenceModel::operator>(MTCsentenceModel & aSrc) 
{
	return (!(isLessThan(aSrc)||(isEqual(aSrc))));
}

// DictEnt >= operator

BOOL MTCsentenceModel::operator>=(MTCsentenceModel & aSrc) 
{
	return (!(isLessThan(aSrc)));
}

// overloaded = operator

MTCsentenceModel & MTCsentenceModel::operator=(MTCsentenceModel &aSrc)
{ 
	for(MTCsentenceNode *current=aSrc.list.getFirst(); current!=NULL; current=aSrc.list.getNext())
	{
		list.add(*current);
	}

	freq=aSrc.freq;

        return *this;
}

