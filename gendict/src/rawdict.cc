/*
 * Dictionary Entry Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: rawdict.cc,v 1.2 1995/11/08 19:32:11 chapman Exp $
 *
 * $Log: rawdict.cc,v $
 * Revision 1.2  1995/11/08 19:32:11  chapman
 * Converted from dictent.cc
 *
// Revision 1.1  1995/07/29  17:06:12  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: rawdict.cc,v 1.2 1995/11/08 19:32:11 chapman Exp $";

#include <iostream.h>
#include <fstream.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../include/rawdict.h"

// rawDictEnt stream insertor operator

ostream & operator<<(ostream & anOStream, const MTCrawDictEnt & aRawDictEnt)
{
	anOStream << aRawDictEnt.getCode() << " " << aRawDictEnt.getValue() << endl;
	return anOStream;
}

// rawDictEnt stream extractor operator

istream & operator>>(istream & anIStream, MTCrawDictEnt & aRawDictEnt)
{
	MTCstring temp;
	char buffer[RAWDICT_MAX_LENGTH+1];
	char *startbuffer=NULL;

	anIStream >> temp;

	aRawDictEnt.setCode(temp);

	anIStream.getline(buffer, RAWDICT_MAX_LENGTH);

	int i;

	for(i=0; i<strlen(buffer); i++)
	{
//		if (isalnum(buffer[i]))
		if (isprint(buffer[i])&&(buffer[i]!=' '))
		{
			startbuffer=&buffer[i];
			break;
		}
	}

	temp=startbuffer;

	aRawDictEnt.setValue(temp);

	return anIStream;
}

// < comparison

int MTCrawDictEnt::isLessThan(const MTCrawDictEnt & aRawDictEnt) const
{
	return (value<aRawDictEnt.value);
}

// equivalence test

int MTCrawDictEnt::isEqual(const MTCrawDictEnt & aRawDictEnt) const
{
	return (value==aRawDictEnt.value);
}

// == operator

BOOL MTCrawDictEnt::operator==(const MTCrawDictEnt & aRawDictEnt) const
{
	return (isEqual(aRawDictEnt));
}

// != operator

BOOL MTCrawDictEnt::operator!=(const MTCrawDictEnt & aRawDictEnt) const
{
	return (!isEqual(aRawDictEnt));
}

// RawDictEnt < operator

BOOL MTCrawDictEnt::operator<(const MTCrawDictEnt & aRawDictEnt)  const
{
	return (isLessThan(aRawDictEnt));
}

// RawDictEnt <= operator

BOOL MTCrawDictEnt::operator<=(const MTCrawDictEnt & aRawDictEnt) const
{
	return (isLessThan(aRawDictEnt)||isEqual(aRawDictEnt));
}

// RawDictEnt > operator

BOOL MTCrawDictEnt::operator>(const MTCrawDictEnt & aRawDictEnt)  const
{
	return (!(isLessThan(aRawDictEnt)||(isEqual(aRawDictEnt))));
}

// RawDictEnt >= operator

BOOL MTCrawDictEnt::operator>=(const MTCrawDictEnt & aRawDictEnt) const
{
	return (!(isLessThan(aRawDictEnt)));
}

// overloaded = operator

MTCrawDictEnt & MTCrawDictEnt::operator=(const MTCrawDictEnt &aRawDictEnt)
{
	value=aRawDictEnt.value;
	code=aRawDictEnt.code;

	return *this;
}
