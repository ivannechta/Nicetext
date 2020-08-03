
/*
 *
 * Grammar Rule Class -- Used by grammar
 * Copyright (c) 1995, Mark T. Chapman
 *
 * $Id: rule.h,v 1.10 1997/03/05 17:44:52 markc Exp $
 *
 * $Log: rule.h,v $
 * Revision 1.10  1997/03/05 17:44:52  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.9  1996/04/11 17:26:06  markc
 * increased MAX_RULE_RHS to 1024
 *
 * Revision 1.8  1996/02/17 16:39:49  markc
 * changed MAX_RULE_RHS to 256
 *
 * Revision 1.7  1996/01/13  12:01:06  markc
 * added punctuation processing to grammar definition language using {}
 *
 * Revision 1.6  1996/01/13  10:18:50  markc
 * use new sentnode enabled sentence models
 *
 * Revision 1.5  1996/01/12  23:51:57  markc
 * dynamic grammar generated models work now
 *
 * Revision 1.4  1996/01/10  22:16:20  markc
 * makeModel() works
 *
 * Revision 1.3  1996/01/10  20:42:55  markc
 *  correctly parses and sets up grammar rules -- does not yet makeModel()
 *
 * Revision 1.2  1996/01/08  21:54:07  markc
 * not quite ready
 *
 * Revision 1.1  1996/01/08  17:33:01  markc
 * Initial revision
 *
 *
 */

#ifndef __RULE_H_
#define __RULE_H_

static char rcsid__RULE_H_ []  = "@(#)$Id: rule.h,v 1.10 1997/03/05 17:44:52 markc Exp $";

#define MAX_RULE_RHS 	1024	// how many rules can go on RHS?

// forward references

class MTCrule;			// this class
class MTCruleRHS;		// right hand side of rule

// rhs of rule 

class MTCruleRHS {
private:
	MTCrule *rules[MAX_RULE_RHS+1];
	int max;
	int current;
	unsigned long weight;
protected:
public:
	// constructors and destructors
	MTCruleRHS();
	MTCruleRHS(const MTCruleRHS &s);
	virtual ~MTCruleRHS();

	// actions 
	void clear();	
	MTCrule *getFirstRHS()	{ current=-1; return(getNextRHS()); };
	MTCrule *getNextRHS();   
	BOOL appendRHS(MTCrule *r); 

	// attribute access methods
	int getCount() const 			{ return max+1; }; 
	unsigned long getWeight() const		{ return weight; };
	void setWeight(unsigned long l)		{ weight=l; };

	// overloaded assignment operator
	const MTCruleRHS & operator=(const MTCruleRHS &s);

	// dummy comparison operator for use in list class
	int operator==(const MTCruleRHS &) const;

	// stream insertor -- used for dumping rules only! 

	friend ostream & operator<<(ostream &s, MTCruleRHS &r);
};

// rule class used by a grammar

class MTCrule {
private:
	MTCstring name;			// rules are sorted by LHS name 
	MTClist <MTCruleRHS> *rhs;	// list of possible rhs
	MTCruleRHS nextRHS;		// next rhs
	long terminal;			// if this is a terminal, here is value
	unsigned long totalWeight;	// used for probabilities
	void init();			// shared ctor code
	void init(MTCrule &);		// shared ctor and operator=() code
	MTCruleRHS * pickRHSSet();	// used by makeModel()
	BOOL allPunct;			// make a model using all punctuation?
	BOOL isPunct;			// is the rule punct only?

protected:

public:
	// constructors and destructors
	MTCrule();				// default ctor
	MTCrule(const MTCstring &aName);	// parameterized ctor
	MTCrule(MTCrule &);			// copy ctor
	virtual ~MTCrule();			// virtual dtor	

	// actions
	void clear();
	BOOL appendRHS(MTCrule *r)	{ return nextRHS.appendRHS(r); };
	void commitRHSSet(unsigned long w); 	

	// attribute access methods
	long getRHSSetCount() const;
	void setTerminal(long aNum)	{ terminal=aNum; };
	long getTerminal() const	{ return terminal; };
	void setAllPunct(BOOL aBool)	{ allPunct=(aBool!=FALSE); };
	BOOL getAllPunct() const	{ return allPunct; };
	void setIsPunct()		{ isPunct=TRUE; };
	BOOL getIsPunct() const	 	{ return isPunct; }; 

	const MTCstring & getName() const 	{ return name; };
	void setName(const MTCstring &s) 	{ name=s; };

	// probably only used by grammar object 
       	void makeModel(MTCsentenceModel &model, int &maxSize);

	// overloaded operator methods
	MTCrule & operator=(MTCrule &);
	int isLessThan(const MTCrule &) const;
        int isEqual(const MTCrule &) const;
     	BOOL operator==(const MTCrule &) const;
        BOOL operator!=(const MTCrule &) const;
        BOOL operator<(const MTCrule &) const;
        BOOL operator<=(const MTCrule &) const;
        BOOL operator>(const MTCrule &) const;
        BOOL operator>=(const MTCrule &) const;

	// stream insertor -- used to dump rule only!
	friend ostream & operator<<(ostream &s, MTCrule &r);
};

#endif  __RULE_H_
