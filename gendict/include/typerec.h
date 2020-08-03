
/*
 *
 * Type Record Class
 *
 * Copyright (c) Mark T. Chapman 1995
 *
 * class MTCtypeRecord - code, index, freq  
 *
 * SORTED BY (code)
 * Note: Alternate Type Record Class is sorted by (index)
 *
 * $Id: typerec.h,v 1.5 1997/03/05 17:39:05 markc Exp $
 *
 * $Log: typerec.h,v $
 * Revision 1.5  1997/03/05 17:39:05  markc
 * Added static char rcsid__DICTAREC_H_ []  = "@(#)$Id$";
 *
 * Revision 1.4  1995/11/08 20:33:40  chapman
 * Added index parameter
 *
 * Revision 1.3  1995/11/08  18:34:36  chapman
 * Converted mstrtype.h to typerec.h
 *
 * Revision 1.2  1995/11/05  15:54:48  chapman
 * Added bitCount logic
 *
 * Revision 1.1  1995/07/29  17:05:32  markc
 * Initial revision
 *
 *
 */

#ifndef __TYPEREC_H_
#define __TYPEREC_H_

static char rcsid__TYPEREC_H_ []  = "@(#)$Id: typerec.h,v 1.5 1997/03/05 17:39:05 markc Exp $";

#define TYPEREC_CODE_MAX_LENGTH 512

class MTCtypeRecord {
private:
	MTCstring code;
	long index;
	long freq;
	long bitCount;
protected:
public:
	// constructors and destructor
	MTCtypeRecord() : freq(0), bitCount(0), index(0) {};
	MTCtypeRecord(const MTCtypeRecord &src) : code(src.code), 
		freq(src.freq), bitCount(src.bitCount), index(src.index) {};

	// attribute set/get methods 
	const MTCstring & getCode() const { return code; };
	long getFreq() const  { return freq;  };
	void setCode(const MTCstring &aString) { code=aString; };
	void setFreq(long aNum);
	long getBitCount() const { return bitCount; };
	void setIndex(long aNum) { index=aNum; };
	long getIndex() const { return index; };

	// overloaded operator methods
	MTCtypeRecord & operator=(const MTCtypeRecord &);
	virtual int isLessThan(const MTCtypeRecord &) const;
        virtual int isEqual(const MTCtypeRecord &) const;
     	BOOL operator==(const MTCtypeRecord &) const;
        BOOL operator!=(const MTCtypeRecord &) const;
        BOOL operator<(const MTCtypeRecord &)  const;
        BOOL operator<=(const MTCtypeRecord &) const;
        BOOL operator>(const MTCtypeRecord &)  const;
        BOOL operator>=(const MTCtypeRecord &) const;

        // stream insertor / extractors
        friend ostream & operator<<(ostream &, const MTCtypeRecord &);
        friend istream & operator>>(istream &, MTCtypeRecord &); 
};

#endif // __TYPEREC_H_
