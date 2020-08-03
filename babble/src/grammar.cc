/*
 * Grammar Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: grammar.cc,v 1.9 1997/02/23 22:33:21 markc Exp $
 *
 * $Log: grammar.cc,v $
 * Revision 1.9  1997/02/23 22:33:21  markc
 * Fixed spelling errors in the punctutation routine ;)
 *
 * Revision 1.8  1997/02/23 20:37:13  markc
 * Added quotedPunctUsed messages and processing
 *
 * Revision 1.7  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.6  1996/01/13 13:56:02  markc
 * Added typeRec->getFreq()==0 check in validation of grammar
 *
// Revision 1.5  1996/01/13  12:00:33  markc
// added punctuation processing to grammar definition language using {}
//
// Revision 1.4  1996/01/13  10:18:10  markc
// use new sentnode enabled sentence models
//
// Revision 1.3  1996/01/12  23:48:19  markc
// dynamic grammars work system wide -- next step, constant expressions
//
// Revision 1.2  1996/01/10  22:15:49  markc
// makeModel() works
//
// Revision 1.1  1996/01/10  20:57:16  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: grammar.cc,v 1.9 1997/02/23 22:33:21 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/raof.h"
#include "../../mtc++/include/raof2rbt.h"
#include "../../gendict/include/sentnode.h"
#include "../../gendict/include/sentmdl.h"
#include "../../gendict/include/typerec.h"
#include "../include/rule.h"
#include "../include/grammar.h"
#include "y.tab.h"

// yyparse() information
int yyparse();		// function prototype
extern FILE *yyin; 	// within lexgram.o  -- the input file
MTCgrammar *yGram;	// used by yyparse() -- let yacc drive!

// stream insertor information
void dumpRule(MTCrule *);
ostream *dumpTarget;
MTCrule *dumpBaseRule;

// validate information
MTCRBT <MTCtypeRecord> *checkTypeRecRBT;
void checkRule(MTCrule *);
BOOL checkOkay;
BOOL foundQuotedRule;

// set allPunct 
void rulesAllPunct(MTCrule *);

// constructor

MTCgrammar::MTCgrammar(const MTCstring & aGramName, const MTCstring & aTypeName)
	: 	fileName(aGramName), inputFile(NULL), 
		okay(TRUE), baseRule(NULL), typeName(aTypeName),
		quotedPunctUsed(FALSE)
{
	// open the file

	if ((inputFile=fopen(fileName.c_str(), "r"))==NULL)
	{
		errorMsg(EMT_ABORT, "I could not open the grammar input file.", "MTCgrammar::MTCgrammar()");
	}

	// open the type RAOF and load into typeRecRBT

	loadTypeRecRBT(aTypeName);

	// use yacc to parse the grammar file

	yGram=this;

	yyin=inputFile;

	yyparse();

	// validate the grammar

	validate();
}

// destructor

MTCgrammar::~MTCgrammar()
{
	// close the input file

	if (inputFile!=NULL)
	{
		fclose(inputFile);
		inputFile=NULL;
	}
}

// load the type record RBT 

void MTCgrammar::loadTypeRecRBT(const MTCstring & aTypeName)
{
	MTCraofToRBT <MTCtypeRecord> loader;
	MTCreadRAOF <MTCtypeRecord> typeRecRAOF(aTypeName);

	if (typeRecRAOF.getOkay()==TRUE)
	{
		loader.doIt(typeRecRAOF, typeRecRBT);
	}
	else
	{
		errorMsg(EMT_ABORT, "I could not open the type table specified", "MTCgrammar::MTCgrammar()");
	}

	if (typeRecRBT.getCount()==0)
	{
		errorMsg(EMT_ABORT, "The type table specified is empty", "MTCgrammar::MTCgrammar()");
	}
}

// validate the rules in the grammar -- check for terminals, etc.

BOOL MTCgrammar::validate()
{
	checkOkay=TRUE;

	checkTypeRecRBT=&typeRecRBT;

	foundQuotedRule=FALSE;

	rules.forEach(checkRule);

	if ((quotedPunctUsed=foundQuotedRule)==TRUE)
	{
		errorMsg(EMT_WARNING, "At least one rule uses quoted punctuations with the {^xxxx^} syntax.  If 'xxxx' contains a word in the dictionary (where there are multiple words of that type) then the scramble program will not be able to recover the original text.","MTCgrammar::validate()");
	}

	if ((okay=checkOkay)==FALSE)
	{
		errorMsg(EMT_ERROR, "There is a problem with this grammar.  It may not generate models correctly", "MTCgrammar::validate()");
	}

	return okay;
}

// check each rule, if it is a terminal, then look it up in typeRecRBT

void checkRule(MTCrule *r)
{
	// check if rule is a terminal -- if so, assign terminal=typeIndex

	// if the rule is punctuation, make sure a RHS is not defined!
	// also, warn if quoted punctuation {^xxx^} was used
	// TODO: check if punctuation is in the dictionary -- complex...

	if (r->getIsPunct()==TRUE)
	{
		if (r->getRHSSetCount()!=0)
		{
			MTCstring message("A punctuation rule has a RHS defined: ");
			message+=r->getName();
			errorMsg(EMT_ERROR, message.c_str(), "MTCgrammar::checkRule()");
			checkOkay=FALSE;
		}

		// was quoted punctuation used?

		int nameLength=r->getName().length();

		if ((nameLength>2)
		&& (r->getName().c_str()[0]=='^')
		&& (r->getName().c_str()[nameLength-1]=='^'))
		{
			foundQuotedRule=TRUE;
		}
	}
	else
	{
		if (r->getRHSSetCount()==0)
		{
			MTCtypeRecord findTypeRec;
			findTypeRec.setCode(r->getName());
			MTCtypeRecord *trec=checkTypeRecRBT->find(findTypeRec);
	
			if (trec==NULL)
			{
				MTCstring message("I could not find a matching terminal in the type table for a rule: ");
				message+=r->getName();
				errorMsg(EMT_ERROR, message.c_str(), "MTCgrammar::checkRule()");
				checkOkay=FALSE;
			}
			else
			{
				if (trec->getFreq()==0)
				{
					MTCstring message("There are no words of this VALID type in the dictionary (typeFreq=0): ");
					message+=r->getName();
					errorMsg(EMT_ERROR, message.c_str(), "MTCgrammar::checkRule()");
					checkOkay=FALSE;
				}
				else
				{
					r->setTerminal(trec->getIndex());
				}
			}
		}
	}
}
	
// set all rules to just use punctuation, no terminals

void MTCgrammar::allPunct()
{
	rules.forEach(rulesAllPunct);
}

// ruleAllPunct -- set a rule to all punctuarion, no terminals

void rulesAllPunct(MTCrule *r)
{
	r->setAllPunct(TRUE);
}

// find a rule in the rules bst by name

MTCrule * MTCgrammar::findRule(const char * ruleName, BOOL isPunct)
{
	MTCrule *retVal=NULL;
	MTCrule tempRule(ruleName);
	
	if ((retVal=rules.find(tempRule))==NULL)
	{
		rules.add(tempRule);
		retVal=rules.find(tempRule);
	}

	if (isPunct==TRUE)
	{
		retVal->setIsPunct();
	}

	return retVal;
}

// commit the nextrule to the rules table

BOOL MTCgrammar::commitRule(const char * ruleName)
{
	// set the rule name

	nextRule.setName(ruleName);
	
	// add the rule -- with overwrite=ON

	rules.add(nextRule, TRUE);

	// if this is the first rule, then it is the base rule

	if (baseRule==NULL)
	{
		baseRule=rules.find(nextRule);
	}

	// get ready to define the next rule

	nextRule.clear();

	return TRUE;
}

// stream insertor -- used for dumping rules only! 
ostream & operator<<(ostream &s, MTCgrammar &g)
{
	s << "// DUMPING " << g.rules.getCount() << " GRAMMAR RULES in alphabetical order" << endl;

	s << "// (#'s are adjusted weights used to select a rule by the previous @ sum.)" << endl;

	s << endl;

	dumpBaseRule=g.baseRule;
	dumpTarget=&s;

	g.rules.forEach(dumpRule);

	cout << "// END OF GRAMMAR RULE DUMP" << endl;
}

// dump a rule to the dumpRuleTarget (ostream)
// purposely NOT part of the class

void dumpRule(MTCrule *r) 
{
	if (r==dumpBaseRule)
	{
		*dumpTarget << "// NEXT RULE IS BASE RULE * * * " << endl;
	}

	*dumpTarget << *r;
}

// ready to makeModel?

BOOL MTCgrammar::isReady()
{
	if (fileName.length()==0)
	{
		errorMsg(EMT_WARNING, "The grammar file name is empty", "MTCgrammar::isReady()");
		return FALSE;
	}

	if (inputFile==NULL)
	{
		errorMsg(EMT_WARNING, "The grammar file is not open", "MTCgrammar::isReady()");
		return FALSE;
	}

	if (getOkay()==FALSE)
	{
		errorMsg(EMT_WARNING, "The grammar state is not okay. (Maybe terminals are not in type master file...)", "MTCgrammar::isReady()");
		return FALSE;
	}

	if (rules.getCount()==0)
	{
		errorMsg(EMT_WARNING, "The grammar does not have any rules defined!)", "MTCgrammar::isReady()");
		return FALSE;
	}

	if (baseRule==NULL)
	{
		errorMsg(EMT_WARNING, "The grammar does not have a BASE rule defined!)", "MTCgrammar::isReady()");
		return FALSE;
	}

	if (typeName.length()==0)
	{
		errorMsg(EMT_WARNING, "The type table does not have a file name", "MTCgrammar::isReady()");
		return FALSE;
	}

	if (typeRecRBT.getCount()==0)
	{
		errorMsg(EMT_WARNING, "The type table is empty", "MTCgrammar::isReady()");
		return FALSE;
	}

	if (typeRecRBT.getCount()==0)
	{
		errorMsg(EMT_WARNING, "The type table is empty", "MTCgrammar::isReady()");
		return FALSE;
	}

	return TRUE;
}

// "randomly" generate a sentence model that corresponds to the grammar

MTCsentenceModel * MTCgrammar::makeModel(int maxSize, BOOL checkIfReady)
{
	// am I ready?

	if ((checkIfReady==TRUE)&&(isReady()!=TRUE))
	{
		return NULL;
	}

	// clear the sentence model

	model.clear();

	// fill the sentence model according to the grammar

	if (baseRule==NULL)
	{
		errorMsg(EMT_WARNING, "I cannot make a model from an empty grammar!", "MTCgrammar::makeModel()");
	}
	else
	{
		baseRule->makeModel(model, maxSize);
	}

	// return the sentence model

	return &model;
}

