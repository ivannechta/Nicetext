/*
 * MTC++ Character String Class
 * Copyright (c) Mark T. Chapman 1993, 1994, 1995
 *
 * $Id: mstring.h,v 1.6 1998/08/25 18:11:32 markc Exp $
 *
 * $Log: mstring.h,v $
 * Revision 1.6  1998/08/25 18:11:32  markc
 * added truncateLength()
 *
 * Revision 1.5  1998/08/22 20:12:18  markc
 * Added length-cache (lc)
 * Added default c-tor instead of default parm to char *=NULL
 *
 * Revision 1.4  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id: mstring.h,v 1.6 1998/08/25 18:11:32 markc Exp $";
 *
 * Revision 1.3  1995/07/25 18:34:44  chapman
 * *** empty log message ***
 *
 * Revision 1.2  1995/07/24  23:02:37  markc
 * Added toUpper & toLower
 *
 * Revision 1.1  1995/06/07  01:50:06  markc
 * Initial revision
 *
 *
 */

#ifndef __MSTRING_H_
#define __MSTRING_H_

static char rcsid__MSTRING_H_ []  = "@(#)$Id: mstring.h,v 1.6 1998/08/25 18:11:32 markc Exp $";

// macros

#define MAX_MSTRING_LENGTH 512 // max length of an MTCstring -- used for stream extraction only!

// forward references to all classes in this header file

class MTCstring;

// string class

class MTCstring {
private:
	char *value;			// pointer to the "actual" string
	void deleteValue();		// if value is allocated, free the memory and set to NULL
	void init(const char *);	// shared constructor code
	unsigned lc;			// save the length in a "length cache"

protected:
	// stream insertor function
	virtual void print(ostream &) const;			//  virtual stream insertor
	virtual void input(istream &);					//  virtual stream extractor

public:
	// constructor and destructor
	MTCstring();					// default c-tor
	MTCstring(const MTCstring &);			// copy c-tor
	MTCstring(const char * c);			// assignment c-tor
	MTCstring(char);				// assignemtn c-tor
	virtual ~MTCstring();							// destructor

	// ANSI methods
	unsigned length() const { return lc; }; // get the string length
	int is_null() const { return (value==NULL||length()==0); };			// is the current string null?
	const char * c_str() const;							// return a "C" style string

	// stream insertor / extractors
	friend ostream & operator<<(ostream &, const MTCstring &);	// stream insertor operator
	friend istream & operator>>(istream &, MTCstring &);			// stream extractor operator

	// truncate the string length
	unsigned truncateLength(unsigned);

	// operator overloading methods
	virtual int isLessThan(const MTCstring &) const;
	virtual int isEqual(const MTCstring &) const;
	virtual int isLessThan(char) const;
	virtual int isEqual(char) const;
	virtual int isLessThan(const char *) const;
	virtual int isEqual(const char *) const;

	BOOL operator==(const MTCstring &) const;
	BOOL operator!=(const MTCstring &) const;
	BOOL operator<(const MTCstring &)  const;
	BOOL operator<=(const MTCstring &) const;
	BOOL operator>(const MTCstring &)  const;
	BOOL operator>=(const MTCstring &) const;

	BOOL operator==(char) const;
	BOOL operator!=(char) const;
	BOOL operator<(char)  const;
	BOOL operator<=(char) const;
	BOOL operator>(char)  const;
	BOOL operator>=(char) const;

	BOOL operator==(const char *) const;
	BOOL operator!=(const char *) const;
	BOOL operator<(const char *)  const;
	BOOL operator<=(const char *) const;
	BOOL operator>(const char *)  const;
	BOOL operator>=(const char *) const;

	MTCstring & operator=(const char *);
	MTCstring & operator=(const MTCstring &);

	MTCstring operator+(const MTCstring &) const;
	MTCstring & operator+=(const MTCstring &);

	MTCstring operator+(const char *) const;
	MTCstring & operator+=(const char *);

	// casting methods
	//operator const char *() const;		// cast the MTCstring as a "C" string

	// other methods
	void toUpper();		// convert to uppercase
	void toLower();		// convert to lowercase
};

#endif // __MSTRING_H_
