/*
 *
 * Dictionary Record Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * class MTCdictRecord - value, typeIndex, bitstring, freq
 *
 * The MTCdictRecord class is sorted by (value).
 * The MTCdictAltRecord class is sorted by (typeINdex, bitstring)
 *
 * $Id: dictrec.h,v 1.6 1997/03/05 17:39:05 markc Exp $
 *
 * $Log: dictrec.h,v $
 * Revision 1.6  1997/03/05 17:39:05  markc
 * Added static char rcsid__DICTAREC_H_ []  = "@(#)$Id$";
 *
 * Revision 1.5  1996/02/17 18:46:39  markc
 * added freq
 *
 * Revision 1.4  1995/11/08  18:45:04  chapman
 * Converted from mstrdict.h
 *
 * Revision 1.3  1995/11/08  04:21:47  chapman
 * temp version
 *
 * Revision 1.2  1995/08/24  20:14:40  markc
 * bitstring is now a long
 *
 * Revision 1.1  1995/08/13  23:08:31  markc
 * Initial revision
 *
 *
 */

#ifndef __DICTREC_H_
#define __DICTREC_H_

static char rcsid__DICTREC_H_ []  = "@(#)$Id: dictrec.h,v 1.6 1997/03/05 17:39:05 markc Exp $";

#define DICTREC_VALUE_MAX_LENGTH 512

class MTCdictRecord {
private:
	MTCstring value;
	long typeIndex;
	long bitstring;
	long freq;
protected:
public:
	// constructors and destructor
	MTCdictRecord() : typeIndex(0), bitstring(0), freq(0) {};
	MTCdictRecord(const MTCdictRecord &src) : value(src.value), typeIndex(src.typeIndex), bitstring(src.bitstring), freq(0) { };

	// attribute set/get methods 
	const MTCstring & getValue() const { return value; };
	long getTypeIndex() const  { return typeIndex;  };
	long getBitString() const { return bitstring; };
	long getFreq() const { return freq; };
	void setValue(const MTCstring &aString) { value=aString; };
	void setTypeIndex(long aNum)  { typeIndex=aNum; };
	void setBitString(long aNum) { bitstring=aNum; };
	void setFreq(long aNum) { freq=aNum; };

	// overloaded operator methods
	MTCdictRecord & operator=(const MTCdictRecord &);
	virtual int isLessThan(const MTCdictRecord &) const;
        virtual int isEqual(const MTCdictRecord &) const;
     	BOOL operator==(const MTCdictRecord &) const;
        BOOL operator!=(const MTCdictRecord &) const;
        BOOL operator<(const MTCdictRecord &)  const;
        BOOL operator<=(const MTCdictRecord &) const;
        BOOL operator>(const MTCdictRecord &)  const;
        BOOL operator>=(const MTCdictRecord &) const;

        // stream insertor / extractors
        friend ostream & operator<<(ostream &, const MTCdictRecord &);
        friend istream & operator>>(istream &, MTCdictRecord &); 
};

#endif // __DICTREC_H_
