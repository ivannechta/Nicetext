/*
 *
 * Alternate Key Type Record Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * This is the MTCtypeRecord sorted by (index)
 *
 * $Id: typearec.h,v 1.1 1997/03/05 17:37:16 markc Exp $
 *
 * $Log: typearec.h,v $
 * Revision 1.1  1997/03/05 17:37:16  markc
 * Initial revision
 *
 *
 */

#ifndef __TYPEAREC_H_
#define __TYPEAREC_H_

static char rcsid__TYPEAREC_H_ []  = "@(#)$Id: typearec.h,v 1.1 1997/03/05 17:37:16 markc Exp $";

// class definition

class MTCtypeAltRecord : public MTCtypeRecord {
private:
protected:
public:
	// constructors and destructor
	MTCtypeAltRecord() {};
	MTCtypeAltRecord(const MTCtypeAltRecord &src) 
		: MTCtypeRecord(src) {};

	// overloaded operator methods
	MTCtypeAltRecord & operator=(const MTCtypeAltRecord &aSrc)
		{ 	MTCtypeRecord::operator=((const MTCtypeRecord)aSrc); 
			return *this;
		};

	virtual int isLessThan(const MTCtypeRecord &aSrc) const
		{
			return (getIndex()<aSrc.getIndex());
		}

        virtual int isEqual(const MTCtypeRecord &aSrc) const
		{
			return (getIndex()==aSrc.getIndex());
		}
};

#endif // __TYPEAREC_H_

