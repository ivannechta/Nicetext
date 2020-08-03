/*
 * MTC++ Character String Class
 * Copyright (c) Mark T. Chapman 1993, 1995
 *
 * $Id: mstring.cc,v 1.8 1998/08/25 18:11:50 markc Exp $
 *
 * $Log: mstring.cc,v $
 * Revision 1.8  1998/08/25 18:11:50  markc
 * added truncateLength()
 *
 * Revision 1.7  1998/08/23 03:48:26  markc
 * added lc=0 to deleteValue
 *
 * Revision 1.6  1998/08/22 20:12:56  markc
 * Added lc logic
 * Added default c-tor instead of using default parameter for char*=NULL
 *
 * Revision 1.5  1995/07/25 18:36:15  chapman
 * *** empty log message ***
 *
// Revision 1.4  1995/07/25  14:28:39  chapman
// Replaced NULL with '\0' to remove warning
//
// Revision 1.3  1995/07/24  23:02:52  markc
// Added toUpper() & toLower()
//
// Revision 1.2  1995/06/30  23:30:17  markc
// Updated for general MTC++.h
//
// Revision 1.1  1995/06/07  01:43:55  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: mstring.cc,v 1.8 1998/08/25 18:11:50 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"

// default constructor

MTCstring::MTCstring() : value(NULL), lc(0)
{
	return;
}

// copy constructor

MTCstring::MTCstring(const MTCstring & aString)
{
	init(aString.c_str());
}

// assignment constructor 

MTCstring::MTCstring(const char * aCharPtr)
{
	init(aCharPtr);
}

// shared constructor code

void MTCstring::init(const char * aCharPtr)
{
	// is this an empty string?

	if (aCharPtr==NULL)
	{
		aCharPtr = "";
	}

	// get the length 
	lc = strlen(aCharPtr);

	// allocate memory for a new copy of the aCharPtr string
				
	value=new char [lc+1];

	if (value==NULL)
	{
		errorMsg(EMT_ERROR, "I just ran out of memory", "MTCstring::MTCstring");
	}
	else
	{
		strcpy(value, aCharPtr);
	}
}

// constructor

MTCstring::MTCstring(char aChar)
{
	char buffer[2];

	buffer[0]=aChar;
	buffer[1]='\0';

	init(buffer);
}

// virtual destructor

MTCstring::~MTCstring()
{
	deleteValue();
}

// delete the memory pointed to by _value

void MTCstring::deleteValue()
{
	if (value!=NULL)
	{
		delete[] value;
		value=NULL;
		lc=0;
	}
}

// stream insertor

void MTCstring::print(ostream & s) const
{
	if (value!=NULL)
	{
		s << value;
	}
}

// stream extractor

void MTCstring::input(istream & s)
{
	// input a string

	char buffer[MAX_MSTRING_LENGTH+1];

	s >> buffer;

	if (strlen(buffer)>MAX_MSTRING_LENGTH)
	{
		errorMsg(EMT_ABORT, "The input stream has a string that is too long.  Programmer must increase MAX_MSTRING_LENGTH");
	}
					
	// create a new value equal to buffer

	deleteValue();
	init(buffer);
}

// string stream insertor operator

ostream & operator<<(ostream & anOStream, const MTCstring & aString)
{
	aString.print(anOStream); return anOStream;
}

// string stream extractor operator

istream & operator>>(istream & anIStream, MTCstring & aString)
{
	aString.input(anIStream); return anIStream;
}

// < comparison

int MTCstring::isLessThan(const MTCstring & aString) const
{
	if (aString.is_null())
	{
		return FALSE;
	}
					
	return value ? (strcmp(value, aString.value)<0) : TRUE;
}

// equivalence test

int MTCstring::isEqual(const MTCstring & aString) const
{
	if (aString.is_null())
	{
		if (is_null())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return (value ? (strcmp(value, aString.value)==0) : FALSE);
}

// == operator

BOOL MTCstring::operator==(const MTCstring & aString) const
{
	return (isEqual(aString));
}

// != operator

BOOL MTCstring::operator!=(const MTCstring & aString) const
{
	return (!isEqual(aString));
}

// String < operator

BOOL MTCstring::operator<(const MTCstring & aString)  const
{
	return (isLessThan(aString));
}

// String <= operator

BOOL MTCstring::operator<=(const MTCstring & aString) const
{
	return (isLessThan(aString)||isEqual(aString));
}

// String > operator

BOOL MTCstring::operator>(const MTCstring & aString)  const
{
	return (!(isLessThan(aString)||(isEqual(aString))));
}

// String >= operator

BOOL MTCstring::operator>=(const MTCstring & aString) const
{
	return (!(isLessThan(aString)));
}


// overloaded = operator

MTCstring & MTCstring::operator=(const char * aCharPtr)
{
	deleteValue();
	init(aCharPtr);

	return *this;
}

// overloaded = operator

MTCstring & MTCstring::operator=(const MTCstring &aString)
{
	deleteValue();
	init(aString.c_str());

	return *this;
}

// < comparison

int MTCstring::isLessThan(char aChar) const
{
	return value ? (value[0] < aChar) : TRUE;
}

// equivalence test

int MTCstring::isEqual(char aChar) const
{
	return value ? (value[0]==aChar) : FALSE;
}

// == operator

BOOL MTCstring::operator==(char aChar) const
{
	return (isEqual(aChar));
}

// != operator

BOOL MTCstring::operator!=(char aChar) const
{
	return (!isEqual(aChar));
}

// String < operator

BOOL MTCstring::operator<(char aChar)  const
{
	return (isLessThan(aChar));
}

// String <= operator

BOOL MTCstring::operator<=(char aChar) const
{
	return (isLessThan(aChar)||isEqual(aChar));
}

// String > operator

BOOL MTCstring::operator>(char aChar)  const
{
	return (!(isLessThan(aChar)||(isEqual(aChar))));
}

// String >= operator

BOOL MTCstring::operator>=(char aChar) const
{
	return (!(isLessThan(aChar)));
}

// < comparison

int MTCstring::isLessThan(const char * aCharPtr) const
{
	return value ? (strcmp(value, aCharPtr)<0) : TRUE;
}

// equivalence test

int MTCstring::isEqual(const char * aCharPtr) const
{
	return value ? (strcmp(value, aCharPtr)==0) : FALSE;
}

// == operator

BOOL MTCstring::operator==(const char * aCharPtr) const
{
	return (isEqual(aCharPtr));
}

// != operator

BOOL MTCstring::operator!=(const char * aCharPtr) const
{
	return (!isEqual(aCharPtr));
}

// String < operator

BOOL MTCstring::operator<(const char * aCharPtr)  const
{
	return (isLessThan(aCharPtr));
}

// String <= operator

BOOL MTCstring::operator<=(const char * aCharPtr) const
{
	return (isLessThan(aCharPtr)||isEqual(aCharPtr));
}

// String > operator

BOOL MTCstring::operator>(const char * aCharPtr)  const
{
	return (!(isLessThan(aCharPtr)||(isEqual(aCharPtr))));
}

// String >= operator

BOOL MTCstring::operator>=(const char * aCharPtr) const
{
	return (!(isLessThan(aCharPtr)));
}

// overloaded + operator

MTCstring MTCstring::operator+(const MTCstring & aString) const
{
	// allocate memory for a temporary buffer

	char * buffer = new char [length()+aString.length()+1];

	if (buffer==NULL)
	{
		errorMsg(EMT_ERROR, "I just ran out of memory", "MTCstring::operator+");
		return *this;
	} 

	// perform the addition
	
	if ((value)&&(aString.value))
	{
		// sprintf(buffer, "%s%s", value, aString.value);
		strcpy(buffer, value);
		strcpy(&buffer[length()], aString.value);
	}
	else
	{
		if (value)
		{
			strcpy(buffer, value);
		}
		else
		{
			if (aString.value)
			{
				strcpy(buffer, aString.value);
			}
			else
			{
				buffer[0]='\0';
			}
		}
	}
	
	// create the new string
	
	MTCstring temp(buffer);

	// don't forget to clean up the buffer!
	
	delete[] buffer;
	
	return temp;
}

// overloaded + operator

MTCstring MTCstring::operator+(const char * aCharPtr) const
{
	MTCstring temp(aCharPtr);

	return ((*this)+temp);
}

// overloaded += operator

MTCstring &MTCstring::operator+=(const MTCstring &aString)
{
	// create a temp copy of this

	MTCstring old(*this);

	// set my value to the sum

	deleteValue();
	init((old+aString).c_str());

	// return the current value

	return *this;
}

// operator +=

MTCstring & MTCstring::operator+=(const char * aCharPtr)
{
	MTCstring temp(aCharPtr);

	(*this)+=temp;

	return *this;
}

// return the current value as a const char *

const char * MTCstring::c_str() const
{
	return value;
}

// casting operator -- to const char *
/*
MTCstring::operator const char * () const
{
	return value;
}
*/

// convert to upper case

void MTCstring::toUpper()
{
	if (value!=NULL)
	{
		for(int i=0; i<length(); i++)
		{
			value[i]=toupper(value[i]);
		}
	}
}


// convert to lower case

void MTCstring::toLower()
{
	if (value!=NULL)
	{
		for(int i=0; i<length(); i++)
		{
			value[i]=tolower(value[i]);
		}
	}
}

// truncate the length of the string

unsigned MTCstring::truncateLength(unsigned aNum)
{
	if (aNum<length())
	{
		value[aNum] = (char)NULL;
		lc=aNum;
	}

	return length();
}

