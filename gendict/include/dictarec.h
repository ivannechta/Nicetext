/*
 *
 * Alternate Key Dictionary Record Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * This is the MTCdictRecord sorted by (type, bitstring) instread of (value)
 *
 * $Id: dictarec.h,v 1.5 1997/03/05 17:39:05 markc Exp $
 *
 * $Log: dictarec.h,v $
 * Revision 1.5  1997/03/05 17:39:05  markc
 * Added static char rcsid__DICTAREC_H_ []  = "@(#)$Id$";
 *
 * Revision 1.4  1995/11/08 19:24:43  chapman
 * Converted from distdict.h to dictarec.h
 *
 * Revision 1.3  1995/11/07  19:56:17  chapman
 * DO NOT USE ANYMORE -- SEE UPDATED MSTRDICT.H
 *
 * Revision 1.2  1995/08/24  20:14:40  markc
 * bitstring is now a long
 *
 * Revision 1.1  1995/08/13  23:08:31  markc
 * Initial revision
 *
 *
 */

#ifndef __DICTAREC_H
#define __DICTAREC_H

static char rcsid__DICTAREC_H_ []  = "@(#)$Id: dictarec.h,v 1.5 1997/03/05 17:39:05 markc Exp $";

class MTCdictAltRecord : public MTCdictRecord {
private:
protected:
public:
	// constructors and destructor
	MTCdictAltRecord() {};
	MTCdictAltRecord(const MTCdictAltRecord &src) 
		: MTCdictRecord(src) {};

	// overloaded operator methods
	MTCdictAltRecord & operator=(const MTCdictAltRecord &aSrc)
		{ 	MTCdictRecord::operator=((const MTCdictRecord)aSrc); 
			return *this;
		};

	virtual int isLessThan(const MTCdictRecord &aSrc) const
		{
			return ((getTypeIndex()<aSrc.getTypeIndex())||
			((getTypeIndex()==aSrc.getTypeIndex())&&
			(getBitString()<aSrc.getBitString())));
		}

        virtual int isEqual(const MTCdictRecord &aSrc) const
		{
			return ((getTypeIndex()==aSrc.getTypeIndex())&&
			(getBitString()==aSrc.getBitString()));
		}
};

#endif // __DICTAREC_H

