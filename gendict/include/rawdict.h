/*
 *
 * Raw Dictionary Entry Header File
 * Copyright (c) Mark T. Chapman 1995
 *
 * Used to process raw "word, typecode" dictionary entries. 
 *
 * $Id: rawdict.h,v 1.3 1997/03/05 17:39:05 markc Exp $
 *
 * $Log: rawdict.h,v $
 * Revision 1.3  1997/03/05 17:39:05  markc
 * Added static char rcsid__DICTAREC_H_ []  = "@(#)$Id$";
 *
 * Revision 1.2  1995/11/08 19:43:16  chapman
 * Coverted from dictent.h
 *
 * Revision 1.1  1995/07/26  13:40:50  markc
 * Initial revision
 *
 *
 */

#ifndef __RAWDICT_H_
#define __RAWDICT_H_

static char rcsid__RAWDICT_H_ []  = "@(#)$Id: rawdict.h,v 1.3 1997/03/05 17:39:05 markc Exp $";

#define RAWDICT_MAX_LENGTH 511

class MTCrawDictEnt {
private:
	MTCstring value;
	MTCstring code;
protected:
public:
	// constructors and destructor
	MTCrawDictEnt() {};
	MTCrawDictEnt(const MTCrawDictEnt &de) { value=de.value; code=de.code; };

	// attribute set/get methods 
	const MTCstring & getValue() const { return value; };
	const MTCstring & getCode() const  { return code;  };
	void setValue(const MTCstring &aString) { value=aString; };
	void setCode(const MTCstring &aString)  { code=aString; };

	// overloaded operator methods
	MTCrawDictEnt & operator=(const MTCrawDictEnt &);
	int isLessThan(const MTCrawDictEnt &) const;
        int isEqual(const MTCrawDictEnt &) const;
     	BOOL operator==(const MTCrawDictEnt &) const;
        BOOL operator!=(const MTCrawDictEnt &) const;
        BOOL operator<(const MTCrawDictEnt &)  const;
        BOOL operator<=(const MTCrawDictEnt &) const;
        BOOL operator>(const MTCrawDictEnt &)  const;
        BOOL operator>=(const MTCrawDictEnt &) const;

        // stream insertor / extractors
        friend ostream & operator<<(ostream &, const MTCrawDictEnt &);
        friend istream & operator>>(istream &, MTCrawDictEnt &); 
};

#endif // __RAWDICT_H_
