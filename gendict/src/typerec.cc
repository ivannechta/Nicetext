/*
 * Type Record Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: typerec.cc,v 1.5 1996/01/16 21:30:43 markc Exp $
 *
 * $Log: typerec.cc,v $
 * Revision 1.5  1996/01/16 21:30:43  markc
 * fixed for(long bitString...) to use new gcc 2.7.0 bindings
 * (i.e. moved the declaration before the for() because it no longer is
 * good after the for() )
 *
// Revision 1.4  1995/11/08  20:33:13  chapman
// Added MTCtypeRecord::index processing
//
// Revision 1.3  1995/11/08  18:34:07  chapman
// Converted mstrtype.cc to typerec.cc
//
// Revision 1.2  1995/11/05  15:54:33  chapman
// Added bitCount logic
//
// Revision 1.1  1995/07/29  17:06:44  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: typerec.cc,v 1.5 1996/01/16 21:30:43 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../include/typerec.h"

// set the freq and computer the bitCount

void MTCtypeRecord::setFreq(long aNum)
{
	freq=aNum;

	if (freq<0)
	{
		freq=0;
	}

	bitCount=0;

	long bitString;

 	for(bitString=1; freq>=bitString; bitString = bitString << 1)
	{ 
		bitCount++; 
	} 
			 
	if ((bitCount>=1)&&(freq!=bitString))
	{ 
		bitCount--;
	}
}

// typeRecord stream insertor operator

ostream & operator<<(ostream & anOStream, const MTCtypeRecord & aSrc)
{
	// note: bitcount is computed from the setFreq in extractor

	anOStream << aSrc.getIndex() << " " << aSrc.getFreq() << " " <<  aSrc.getCode() << endl;
	return anOStream;
}

// typeRecord stream extractor operator

istream & operator>>(istream & anIStream, MTCtypeRecord & aSrc)
{
	// read index
	long tempIndex;
	anIStream >> tempIndex;
	aSrc.setIndex(tempIndex);

	// read frequency
	long tempFreq;
	anIStream >> tempFreq;
	aSrc.setFreq(tempFreq);	// this computes bitCount...

	// get code -- but skip the leading whitespace!

	MTCstring temp;
	char buffer[TYPEREC_CODE_MAX_LENGTH+1];
	char *startbuffer=NULL;

	anIStream.getline(buffer, TYPEREC_CODE_MAX_LENGTH);

	int i;

	for(i=0; i<strlen(buffer); i++)
	{
		if (isprint(buffer[i])&&(buffer[i]!=' '))
		{
			startbuffer=&buffer[i];
			break;
		}
	}

	temp=startbuffer;

	aSrc.setCode(temp);

	return anIStream;
}

// < comparison

int MTCtypeRecord::isLessThan(const MTCtypeRecord & aSrc) const
{
	return (code<aSrc.code);
}

// equivalence test

int MTCtypeRecord::isEqual(const MTCtypeRecord & aSrc) const
{
	return (code==aSrc.code);
}

// == operator

BOOL MTCtypeRecord::operator==(const MTCtypeRecord & aSrc) const
{
	return (isEqual(aSrc));
}

// != operator

BOOL MTCtypeRecord::operator!=(const MTCtypeRecord & aSrc) const
{
	return (!isEqual(aSrc));
}

// DictEnt < operator

BOOL MTCtypeRecord::operator<(const MTCtypeRecord & aSrc)  const
{
	return (isLessThan(aSrc));
}

// DictEnt <= operator

BOOL MTCtypeRecord::operator<=(const MTCtypeRecord & aSrc) const
{
	return (isLessThan(aSrc)||isEqual(aSrc));
}

// DictEnt > operator

BOOL MTCtypeRecord::operator>(const MTCtypeRecord & aSrc)  const
{
	return (!(isLessThan(aSrc)||(isEqual(aSrc))));
}

// DictEnt >= operator

BOOL MTCtypeRecord::operator>=(const MTCtypeRecord & aSrc) const
{
	return (!(isLessThan(aSrc)));
}

// overloaded = operator

MTCtypeRecord & MTCtypeRecord::operator=(const MTCtypeRecord &aSrc)
{
	setIndex(aSrc.getIndex());
	setCode(aSrc.getCode());
	setFreq(aSrc.getFreq());

	return *this;
}
