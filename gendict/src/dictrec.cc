/*
 * Dictionary Record Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: dictrec.cc,v 1.4 1996/02/17 18:46:28 markc Exp $
 *
 * $Log: dictrec.cc,v $
 * Revision 1.4  1996/02/17 18:46:28  markc
 * added freq
 *
// Revision 1.3  1995/11/08  18:44:06  chapman
// Converted mstrdict.cc to dictrec.cc
//
// Revision 1.4  1995/08/24  20:13:36  markc
// bitstring now is a long
//
// Revision 1.3  1995/08/23  19:04:56  markc
// Changed order of data in << and >> streams
//
// Revision 1.2  1995/08/13  23:09:31  markc
// Change jump to typeIndex
//
// Revision 1.1  1995/07/30  17:04:33  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: dictrec.cc,v 1.4 1996/02/17 18:46:28 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/raof.h"
#include "../include/dictrec.h"

// dictRecord stream insertor operator

ostream & operator<<(ostream & anOStream, const MTCdictRecord & aSrc)
{
	anOStream << aSrc.getTypeIndex() << ' ' << aSrc.getBitString() << ' ' << aSrc.getFreq() << ' ' << aSrc.getValue() << endl;

	return anOStream;
}

// dictRecord stream extractor operator

istream & operator>>(istream & anIStream, MTCdictRecord & aSrc)
{
	// get typeIndex

	long tempTypeIndex;

	anIStream >> tempTypeIndex;
	aSrc.setTypeIndex(tempTypeIndex);

	// get bitstring

	long tempBitstring;

	anIStream >> tempBitstring;
	aSrc.setBitString(tempBitstring);

	// get freq

	long tempFreq;

	anIStream >> tempFreq;
	aSrc.setFreq(tempFreq);

	// get value -- but skip the leading whitespace!

	MTCstring temp;
	char buffer[DICTREC_VALUE_MAX_LENGTH+1];
	char *startbuffer=NULL;

	anIStream.getline(buffer, DICTREC_VALUE_MAX_LENGTH);

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

	aSrc.setValue(temp);

	return anIStream;
}

// < comparison

int MTCdictRecord::isLessThan(const MTCdictRecord & aSrc) const
{
	return (value<aSrc.value);
}

// equivalence test

int MTCdictRecord::isEqual(const MTCdictRecord & aSrc) const
{
	return (value==aSrc.value);
}


// == operator

BOOL MTCdictRecord::operator==(const MTCdictRecord & aSrc) const
{
	return (isEqual(aSrc));
}

// != operator

BOOL MTCdictRecord::operator!=(const MTCdictRecord & aSrc) const
{
	return (!isEqual(aSrc));
}

// DictEnt < operator

BOOL MTCdictRecord::operator<(const MTCdictRecord & aSrc)  const
{
	return (isLessThan(aSrc));
}

// DictEnt <= operator

BOOL MTCdictRecord::operator<=(const MTCdictRecord & aSrc) const
{
	return (isLessThan(aSrc)||isEqual(aSrc));
}

// DictEnt > operator

BOOL MTCdictRecord::operator>(const MTCdictRecord & aSrc)  const
{
	return (!(isLessThan(aSrc)||(isEqual(aSrc))));
}

// DictEnt >= operator

BOOL MTCdictRecord::operator>=(const MTCdictRecord & aSrc) const
{
	return (!(isLessThan(aSrc)));
}

// overloaded = operator

MTCdictRecord & MTCdictRecord::operator=(const MTCdictRecord &aSrc)
{
	value=aSrc.value;
	typeIndex=aSrc.typeIndex;
	bitstring=aSrc.bitstring;
	freq=aSrc.freq;

	return *this;
}
