
/*
 *
 * Grammar Processing Class -- Used to randomly generate sentence models
 * Copyright (c) 1995, Mark T. Chapman
 *
 * $Id: grammar.h,v 1.11 1997/03/05 17:44:52 markc Exp $
 *
 * $Log: grammar.h,v $
 * Revision 1.11  1997/03/05 17:44:52  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.10  1997/02/23 20:40:45  markc
 * Added quotedPunctUsed BOOL
 *
 * Revision 1.9  1997/02/16 06:58:07  markc
 * BST replaced with RBT
 *
 * Revision 1.8  1996/02/08 01:28:07  markc
 * updated maxSize to 1024 from 50 in makeModel
 *
 * Revision 1.7  1996/01/13  12:01:06  markc
 * added punctuation processing to grammar definition language using {}
 *
 * Revision 1.6  1996/01/13  10:18:50  markc
 * use new sentnode enabled sentence models
 *
 * Revision 1.5  1996/01/12  23:51:25  markc
 * dynamic grammar generated models work -- next step punctuation?
 *
 * Revision 1.4  1996/01/10  22:16:20  markc
 * makeModel() works
 *
 * Revision 1.3  1996/01/10  20:42:55  markc
 *  correctly parses and sets up grammar rules -- does not yet makeModel()
 *
 * Revision 1.2  1996/01/08  21:53:59  markc
 * not quite ready
 *
 * Revision 1.1  1996/01/08  17:32:54  markc
 * Initial revision
 *
 *
 */

#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_

static char rcsid__GRAMMAR_H_ []  = "@(#)$Id: grammar.h,v 1.11 1997/03/05 17:44:52 markc Exp $";

// concrete class for randomly generating sentence models according to a grammar

class MTCgrammar {
private:
	BOOL quotedPunctUsed;	// was quoted punctuation used in a rule?
	MTCstring fileName;	// grammar definition file name
	FILE *inputFile;	// input file pointer
	BOOL okay;		// is *this okay?
	MTCsentenceModel model;	// the target model
	MTCRBT<MTCrule> rules;	// a storage place for rules instances
	MTCrule *baseRule;	// the start of it all...
	MTCrule nextRule; 	// used while parsing grammar in yyparse() 
	MTCstring typeName;	// used for RAOF
	MTCRBT<MTCtypeRecord> typeRecRBT;	// as in mstrtype.dat RAOF
	void loadTypeRecRBT(const MTCstring &); // used by ctor
	BOOL validate();		 	// check terminals, etc. 

	MTCrule * findRule(const char * ruleName, BOOL isPunct);

protected:
	void setOkay(BOOL aBool) { okay=(aBool!=FALSE); };

public:
	// actions used ONLY by yyparse()
	BOOL commitRule(const char * ruleName);

	void commitRHSSet(unsigned long aWeight)
	{
		nextRule.commitRHSSet(aWeight);
	};

	BOOL appendRHS(const char *ruleName, BOOL isPunct=FALSE)
	{
		return nextRule.appendRHS(findRule(ruleName, isPunct));
	};

public:
	// constructors and destructors
	MTCgrammar(const MTCstring &aGramName, const MTCstring &aTypeName);
	virtual ~MTCgrammar();	

	// attribute access methods
	BOOL getOkay() const { return okay; };
	MTCsentenceModel * getModel() { return &model; };
	const MTCstring & getFileName() const { return fileName; };
	BOOL getQuotedPunctUsed() const { return quotedPunctUsed; };

	// generate a "random" sentence model according to this grammar
	BOOL isReady();	
	MTCsentenceModel * makeModel(int maxSize=1024, BOOL checkIfReady=TRUE);
	void allPunct();	// do not recognize terminals in makeModel() 

	// stream insertor -- used for dumping rules only! 
	friend ostream & operator<<(ostream &s, MTCgrammar &);
};

#endif  __GRAMMAR_H_

