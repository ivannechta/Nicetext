/*
 * Sentence Node Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: sentnode.cc,v 1.2 1996/01/13 09:47:22 markc Exp $
 * 
 * $Log: sentnode.cc,v $
 * Revision 1.2  1996/01/13 09:47:22  markc
 * removed cerr <<'s
 *
// Revision 1.1  1996/01/13  09:41:24  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: sentnode.cc,v 1.2 1996/01/13 09:47:22 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/list.h"
#include "../include/sentnode.h"
#include "../include/sentmdl.h"

// sentenceNode stream insertor operator

ostream & operator<<(ostream & anOStream, const MTCsentenceNode & aSrc)
{
	if (aSrc.getTypeIndex()==-1)
	{
		anOStream << aSrc.getPunctuation() << ' ';
	}
	else
	{
		anOStream << aSrc.getTypeIndex() << ' ';
	}

	return anOStream;
}

// sentenceNode stream extractor operator

istream & operator>>(istream & anIStream, MTCsentenceNode & aSrc)
{
	MTCstring tempStr;

	anIStream >> tempStr;

	long tempLong=atol(tempStr.c_str());

	if (tempLong==0)
	{
		aSrc.setPunctuation(tempStr);
		aSrc.setTypeIndex(-1);
	}
	else
	{
		aSrc.setPunctuation("");
		aSrc.setTypeIndex(tempLong);
	}

	return anIStream;
}

// < comparison

int MTCsentenceNode::isLessThan(const MTCsentenceNode & aSrc) const
{
	return ((typeIndex<aSrc.typeIndex)||
	((typeIndex==aSrc.typeIndex)&&(punctuation<aSrc.punctuation)));
}

// equivalence test

int MTCsentenceNode::isEqual(const MTCsentenceNode & aSrc) const
{
	return ((typeIndex==aSrc.typeIndex)&&(punctuation==aSrc.punctuation));
}

// == operator

BOOL MTCsentenceNode::operator==(const MTCsentenceNode & aSrc) const
{
	return (isEqual(aSrc));
}

// != operator

BOOL MTCsentenceNode::operator!=(const MTCsentenceNode & aSrc) const
{
	return (!isEqual(aSrc));
}

// DictEnt < operator

BOOL MTCsentenceNode::operator<(const MTCsentenceNode & aSrc)  const
{
	return (isLessThan(aSrc));
}

// DictEnt <= operator

BOOL MTCsentenceNode::operator<=(const MTCsentenceNode & aSrc) const
{
	return (isLessThan(aSrc)||isEqual(aSrc));
}

// DictEnt > operator

BOOL MTCsentenceNode::operator>(const MTCsentenceNode & aSrc)  const
{
	return (!(isLessThan(aSrc)||(isEqual(aSrc))));
}

// DictEnt >= operator

BOOL MTCsentenceNode::operator>=(const MTCsentenceNode & aSrc) const
{
	return (!(isLessThan(aSrc)));
}

// overloaded = operator

MTCsentenceNode & MTCsentenceNode::operator=(const MTCsentenceNode &aSrc)
{
	typeIndex=aSrc.typeIndex;
	punctuation=aSrc.punctuation;

	return *this;
}
