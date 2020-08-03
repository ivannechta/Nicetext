
/*
 *
 * Sentence Model Class  
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: sentmdl.h,v 1.9 1997/03/05 17:39:05 markc Exp $
 *
 * $Log: sentmdl.h,v $
 * Revision 1.9  1997/03/05 17:39:05  markc
 * Added static char rcsid__DICTAREC_H_ []  = "@(#)$Id$";
 *
 * Revision 1.8  1997/03/02 14:17:24  markc
 * Changed cap->Cap capslockon->CAPSLOCKON caplockoff->capslockoff
 *
 * Revision 1.7  1997/03/02 04:24:24  markc
 * Added SENTMDL_CAP* defines
 *
 * Revision 1.6  1997/02/24 00:13:08  markc
 * Added getLength()
 *
 * Revision 1.5  1996/02/08 00:52:14  markc
 * added SENTMDL_MORE_PUNCT
 * ,
 *
 * Revision 1.4  1996/01/14  19:36:44  markc
 * relaxed const
 *
 * Revision 1.3  1996/01/13  09:42:24  markc
 * added sentnode object
 *
 * Revision 1.2  1996/01/12  23:53:22  markc
 * optimized
 *
 * Revision 1.1  1995/11/08  18:25:28  chapman
 * Initial revision
 *
 *
 */

#ifndef __SENTMDL_H_
#define __SENTMDL_H_

static char rcsid__SENTMDL_H_ []  = "@(#)$Id: sentmdl.h,v 1.9 1997/03/05 17:39:05 markc Exp $";

#define SENTMDL_EOS		"e"
#define SENTMDL_GETNEXT_DONE	-69
#define SENTMDL_MORE_PUNCT	-21	

#define SENTMDL_CAP_NEXT_LETTER "Cap"
#define SENTMDL_CAPSLOCK_ON     "CAPSLOCKON"
#define SENTMDL_CAPSLOCK_OFF    "capslockoff"

class MTCsentenceModel {
private:
	MTClist <MTCsentenceNode> list;
	BOOL firstGetNext;
	long freq;
protected:
public:
	// constructors and destructors
	MTCsentenceModel();
	MTCsentenceModel(MTCsentenceModel &);

	// attribute set/get methods 
	void appendPunctuation(const MTCstring &); 	// add punctuation
	void appendTypeIndex(long);			// add type index
	long getNext(MTCstring *punctuation=NULL);
	void setFreq(long aNum) { freq=aNum; };
	long getFreq() const    { return freq; };
	const MTCstring & getPunctuation();
	void clear();
	long getLength() const  { return list.getCount(); }; 

	// overloaded operator methods
	MTCsentenceModel & operator=(MTCsentenceModel &);
	int isLessThan(MTCsentenceModel &);
        int isEqual(MTCsentenceModel &);
     	BOOL operator==(MTCsentenceModel &);
        BOOL operator!=(MTCsentenceModel &);
        BOOL operator<(MTCsentenceModel &);
        BOOL operator<=(MTCsentenceModel &);
        BOOL operator>(MTCsentenceModel &);
        BOOL operator>=(MTCsentenceModel &);

        // stream insertor / extractors
        friend ostream & operator<<(ostream &, MTCsentenceModel &);
        friend istream & operator>>(istream &, MTCsentenceModel &); 
};

#endif // __SENTMDL_H_
