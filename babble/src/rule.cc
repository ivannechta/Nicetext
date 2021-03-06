/*
 * Grammar Rules
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: rule.cc,v 1.8 1997/02/24 00:12:19 markc Exp $
 *
 * $Log: rule.cc,v $
 * Revision 1.8  1997/02/24 00:12:19  markc
 * Added printStats and fixed maxModelLength
 *
 * Revision 1.7  1996/01/13 12:00:33  markc
 * added punctuation processing to grammar definition language using {}
 *
// Revision 1.6  1996/01/13  10:18:10  markc
// use new sentnode enabled sentence models
//
// Revision 1.5  1996/01/12  23:48:19  markc
// dynamic grammars work system wide -- next step, constant expressions
//
// Revision 1.4  1996/01/10  22:15:49  markc
// makeModel() works
//
// Revision 1.3  1996/01/10  20:41:59  markc
// correctly parses and sets up grammar rules -- does not yet makeModel()
//
// Revision 1.2  1996/01/08  21:54:42  markc
// not quite ready
//
// Revision 1.1  1996/01/08  17:32:38  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: rule.cc,v 1.8 1997/02/24 00:12:19 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/list.h"
#include "../../gendict/include/sentnode.h"
#include "../../gendict/include/sentmdl.h"
#include "../include/rule.h"

// constructor
MTCrule::MTCrule()
	: 	name("UNDEFINED_RULE"), terminal(-1), totalWeight(0), 
		rhs(NULL), allPunct(FALSE), isPunct(FALSE)
{
	init();

	return;
}

// parameterized constructor

MTCrule::MTCrule(const MTCstring &s)
	: 	name(s), terminal(-1), totalWeight(0),
		rhs(NULL), allPunct(FALSE), isPunct(FALSE)
{
	init();

	return;
}

// copy constructor

MTCrule::MTCrule(MTCrule &r)
{
	init(r);
	
	return;
}

// shared constructor code

void MTCrule::init()
{
	if ((rhs=new MTClist <MTCruleRHS>)==NULL)
	{
		errorMsg(EMT_ABORT, "I do not have enough memory to allocate a MTClist of MTCruleRHS's", "MTCrule::init()");
		return;
	}
}

// shared constructor and operator=() code

void MTCrule::init(MTCrule &r)
{
	if (r.rhs==NULL)
	{
		errorMsg(EMT_WARNING, "Why did I just copy an MTCrule with a NULL rhs?", "MTCrule::operator=()");
		rhs=NULL;
	}
	else
	{
		if ((rhs=new MTClist <MTCruleRHS>(*r.rhs))==NULL)
		{
			errorMsg(EMT_ABORT, "I do not have enough memory to allocate a MTClist of MTCruleRHS's", "MTCrule::init()");
			return;
		}
	}

	name=r.name;
	terminal=r.terminal;
	totalWeight=r.totalWeight;
	allPunct=r.allPunct;
	isPunct=r.isPunct;
}

// destructor

MTCrule::~MTCrule()
{
	if (rhs!=NULL)
	{
		delete rhs;
		rhs=NULL;
	}

	return;
}

// clear the rule

void MTCrule::clear()
{ 
	name=""; 
	nextRHS.clear(); 
	terminal=-1; 
	totalWeight=0; 

	if (rhs!=NULL)
	{
		rhs->clear(); 
	}
}

// commit the nextRHS to this rule

void MTCrule::commitRHSSet(unsigned long w)
{ 
	totalWeight+=w;
	nextRHS.setWeight(totalWeight);

	if (rhs==NULL)
	{
		errorMsg(EMT_WARNING, "I cannot commit a RHSSet to a rule with a NULL MTClist <MTCruleRHS> *rhs", "MTCrule::commitRHSSet()");
	}
	else
	{
		rhs->add(nextRHS); 
	}

	nextRHS.clear(); 
};

// < comparison

int MTCrule::isLessThan(const MTCrule & aSrc) const
{
	return (name<aSrc.name);
}

// equivalence test

int MTCrule::isEqual(const MTCrule & aSrc) const
{
	return (name==aSrc.name);
}

// == operator

BOOL MTCrule::operator==(const MTCrule & aSrc) const
{
	return (isEqual(aSrc));
}

// != operator

BOOL MTCrule::operator!=(const MTCrule & aSrc) const
{
	return (!isEqual(aSrc));
}

//  < operator

BOOL MTCrule::operator<(const MTCrule & aSrc)  const
{
	return (isLessThan(aSrc));
}

//  <= operator

BOOL MTCrule::operator<=(const MTCrule & aSrc) const
{
	return (isLessThan(aSrc)||isEqual(aSrc));
}

//  > operator

BOOL MTCrule::operator>(const MTCrule & aSrc)  const
{
	return (!(isLessThan(aSrc)||(isEqual(aSrc))));
}

//  >= operator

BOOL MTCrule::operator>=(const MTCrule & aSrc) const
{
	return (!(isLessThan(aSrc)));
}

// stream insertor operators -- used to dump rules only!!

ostream & operator<<(ostream &s, MTCruleRHS &r)
{
	for(int i=0; i<=r.max; i++)
	{
		if (r.rules[i]!=NULL)
		{
			s << r.rules[i]->getName() << ' ';
		}
	}

	s << '#' << r.weight;

	return s;
}

ostream & operator<<(ostream &s, MTCrule &r)
{
	if (r.getIsPunct()==TRUE)
	{
		s << r.name << ": (punctuation rule only) ;" << endl << endl;
	}
	else
	{
		MTCstring delimeter="\t \t";

		s << r.name << ": " << endl;

		if (r.rhs==NULL)
		{
			s << "* * * * * THIS RULE'S RHS IS A NULL MTClist <MTCruleRHS> * ";
		}
		else
		{
			if (r.rhs->getCount()==0)
			{
				s << "\t\tterminal(" << r.getTerminal() << ")" << endl; 
			}
			else
			{
				for(MTCruleRHS *trhs=r.rhs->getFirst(); trhs!=NULL; trhs=r.rhs->getNext())
				{
					s << delimeter;
					s << *trhs << endl;
					delimeter="\t|\t";
				}
			}
		}

		s << "\t;" << endl << endl;
	}

	return s;
}

// overloaded = operator

MTCrule & MTCrule::operator=(MTCrule &aSrc)
{
	init(aSrc);

	return *this;
}

// default constructor 

MTCruleRHS::MTCruleRHS() : max(-1), weight(1) 
{ 
	for(int i=0; i<MAX_RULE_RHS; i++) 
	{
		rules[i]=NULL; 
	}

	return;
}

// copy constructor

MTCruleRHS::MTCruleRHS(const MTCruleRHS &s) : max(s.max), weight(s.weight)
{ 
	for(int i=0; i<MAX_RULE_RHS; i++) 
	{
		rules[i]=s.rules[i]; 
	}
}

// destructor

MTCruleRHS::~MTCruleRHS() 
{ 
	return; 
}

// clear the current rule rhs
void MTCruleRHS::clear()	
{ 	
	max=-1; 
	current=-1; 
	weight=1;

	for(int i=0; i<MAX_RULE_RHS; i++) 
	{
		rules[i]=NULL;
	}
}

// return the next RHS of the ruleRHS

MTCrule * MTCruleRHS::getNextRHS()   
{ 
	if ((++current)<=max) 
	{
		return rules[current]; 
	}

	return NULL; 
}
	
// get the RHSSet count

long MTCrule::getRHSSetCount() const
{
	if (rhs==NULL)
	{
		errorMsg(EMT_WARNING, "I cannot return the count of the NULL rhs -- I will return 0 instead", "MTCrule::getRHSSetCount()");
		return 0;
	}

	return rhs->getCount();
}

// overloaded assignment operator

const MTCruleRHS & MTCruleRHS::operator=(const MTCruleRHS &s)
{ 
	max=s.max; 
	weight=s.weight;

	for(int i=0; i<MAX_RULE_RHS; i++) 
	{
		rules[i]=s.rules[i]; 
	}

	return *this;
}

// dummy comparison operator for use in list class

int MTCruleRHS::operator==(const MTCruleRHS &) const 
{ 
	return FALSE; 
}

// append a RHS to the ruleRHS

BOOL MTCruleRHS::appendRHS(MTCrule *r) 
{ 
	if (r==NULL) 
	{
		errorMsg(EMT_ERROR, "I cannot append a NULL rule to the RHS -- I will ignore it instead");
		return FALSE;
	}

	if ((max+1)<MAX_RULE_RHS)
	{
		rules[++max]=r;
		return TRUE;
	}
	else
	{
		errorMsg(EMT_WARNING, "I cannot handle that many RHS's for a rule.  Please adjust MAX_RULE_RHS in rules.h", "MTCruleRHS::appendRHS()");
		return FALSE;
	}
}

// make a sentence model from this rule set

void MTCrule::makeModel(MTCsentenceModel &model, int &maxSize)
{
	// check the facts, ma'am

	if (rhs==NULL)
	{
		errorMsg(EMT_WARNING, "I cannot add anything to a sentence model from a rule with no rhs list!", "MTCrule::makeModel()");
		return;
	}

	// is this a terminal?

	if (rhs->getCount()==0)
	{
		maxSize--;

		if ((terminal==-1)||(isPunct==TRUE)||(allPunct==TRUE))
		{
			model.appendPunctuation(name);
		}
		else
		{
			model.appendTypeIndex(terminal);
		}
	}
	else	// non-terminal -- use rhs to fill in model
	{
		MTCruleRHS *trhs=pickRHSSet();

		if (trhs==NULL)
		{
			errorMsg(EMT_WARNING, "I cannot add anything to a sentence model because I picked a NULL RHSSET!", "MTCrule::makeModel()");
			return;
		}

		for(MTCrule *tRule=trhs->getFirstRHS(); tRule!=NULL; tRule=trhs->getNextRHS())
		{
			tRule->makeModel(model, maxSize);

			// give up if we are already over the max size limit

			if (maxSize<0)
			{
				break;
			}
		}
	}

	return;
}

// pick a RHSSET from rhs randomly using weights -- used by makeModel()

MTCruleRHS * MTCrule::pickRHSSet()
{
	unsigned long randVal=random()%totalWeight;

	if (rhs==NULL)
	{
		errorMsg(EMT_WARNING, "I could not pick a RHSSet because the rule.rhs is NULL", "MTCrule::pickRHSSet()");
		return NULL;
	}

	for(MTCruleRHS *tRule=rhs->getFirst(); tRule!=NULL; tRule=rhs->getNext())
	{
		if (randVal<tRule->getWeight())
		{
			return tRule;
		}
	}

	errorMsg(EMT_WARNING, "I could not pick a RHSSet", "MTCrule::pickRHSSet()");

	return NULL;
}

