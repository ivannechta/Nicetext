
/*
 *
 * Sentence Node Class
 *
 * Copyright (c) Mark T. Chapman 1995
 *
 * (unsorted)
 *
 * $Id: sentnode.h,v 1.2 1997/03/05 17:39:05 markc Exp $
 *
 * $Log: sentnode.h,v $
 * Revision 1.2  1997/03/05 17:39:05  markc
 * Added static char rcsid__DICTAREC_H_ []  = "@(#)$Id$";
 *
 * Revision 1.1  1996/01/13 09:42:35  markc
 * Initial revision
 *
 *
 */

#ifndef __SENTNODE_H_
#define __SENTNODE_H_

static char rcsid__SENTNODE_H_ []  = "@(#)$Id: sentnode.h,v 1.2 1997/03/05 17:39:05 markc Exp $";

// sentence node class

class MTCsentenceNode {
private:
	long typeIndex;
	MTCstring punctuation;
protected:
public:
	// constructors and destructor
	MTCsentenceNode() : typeIndex(-1) {};
	MTCsentenceNode(const MTCsentenceNode &src) : typeIndex(src.typeIndex), punctuation(src.punctuation) {};

	// attribute set/get methods 
	const MTCstring & getPunctuation() const { return punctuation; };
	void setPunctuation(const MTCstring &aString) { punctuation=aString; };
	void setTypeIndex(long aNum) { typeIndex=aNum; };
	long getTypeIndex() const { return typeIndex; };

	// overloaded operator methods
	MTCsentenceNode & operator=(const MTCsentenceNode &);
	virtual int isEqual(const MTCsentenceNode &) const;
	virtual int isLessThan(const MTCsentenceNode &) const;
     	BOOL operator==(const MTCsentenceNode &) const;
        BOOL operator!=(const MTCsentenceNode &) const;
        BOOL operator<(const MTCsentenceNode &)  const;
        BOOL operator<=(const MTCsentenceNode &) const;
        BOOL operator>(const MTCsentenceNode &)  const;
        BOOL operator>=(const MTCsentenceNode &) const;

        // stream insertor / extractors
        friend ostream & operator<<(ostream &, const MTCsentenceNode &);
        friend istream & operator>>(istream &, MTCsentenceNode &); 
};

#endif // __SENTNODE_H_
