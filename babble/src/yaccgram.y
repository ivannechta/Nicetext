%{
/*
 * YACC Grammar for inputting BABBLE Grammars 
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: yaccgram.y,v 1.10 1997/03/05 17:47:51 markc Exp $
 *
 * $Log: yaccgram.y,v $
 * Revision 1.10  1997/03/05 17:47:51  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.9  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.8  1996/01/13 12:00:33  markc
 * added punctuation processing to grammar definition language using {}
 *
 * Revision 1.7  1996/01/13  10:18:10  markc
 * use new sentnode enabled sentence models
 *
 * Revision 1.6  1996/01/12  23:48:19  markc
 * dynamic grammars work system wide -- next step, constant expressions
 *
 * Revision 1.5  1996/01/10  22:15:49  markc
 * makeModel() works
 *
 * Revision 1.4  1996/01/10  20:41:59  markc
 * correctly parses and sets up grammar rules -- does not yet makeModel()
 *
 * Revision 1.3  1996/01/08  22:12:17  markc
 * Modified language to only allow weights at the end of a RHS set
 *
 * Revision 1.2  1996/01/08  21:54:52  markc
 * parses the grammar definition language correctly -- but with not meaning!
 *
 * Revision 1.1  1996/01/08  17:32:22  markc
 * Initial revision
 *
 *
 */

static char rcsid []  = "@(#)$Id: yaccgram.y,v 1.10 1997/03/05 17:47:51 markc Exp $";

int yyerror(char *);
int yylex();
#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
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

#ifdef DEBUG_YACC_VERBOSE
#define D(x) x
#else
#define D(x) 
#endif // DEBUG_YACC_VERBOSE

extern MTCgrammar *yGram;
extern long lexline;
unsigned long saveWeight=1;

%}

%union {
	char *string;
	unsigned long  number;
}

%token <string> TOKEN
%token <string> PUNCT
%token <number> WEIGHT
%token <number> BAD

%%

grammar: 
		rule { D(cerr << "rule inserted to grammar" << endl;) };
	| 	grammar rule { D(cerr << "rule appended to grammar" << endl;)};
	;

rule: 
		TOKEN ':' rhs ';' 
		{	
			D( cerr << "Rule completed:" << $1 << endl;) 
			if (yGram->commitRule($1)==FALSE)
			{
				errorMsg(EMT_ERROR, "I could not commit a rule", "yyparse()");
			}
			free($1); 
		};
	;

rhs:
		rhsTokens		
		{ 
			D(cerr << "rhsTokens completed: " << endl;) 
			yGram->commitRHSSet(saveWeight);
		};

	|	weightedRhsTokens	
		{
			D(cerr << "weightedRhsTokens completed: " << endl;) 
			yGram->commitRHSSet(saveWeight);
		};

	|	rhs '|' rhsTokens	
		{	
			D(cerr << "rhs|rhstokens" << endl;) 
			yGram->commitRHSSet(saveWeight);
		};

	|	rhs '|' weightedRhsTokens	
		{ 
			D(cerr <<  "rhs|weightedRhsTokens" << endl;) 
			yGram->commitRHSSet(saveWeight);
		};
	;

rhsTokens:
		TOKEN 
		{
			D(cerr << "token: " << $1 << endl;) 

			if (yGram->appendRHS($1)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}

			free($1);
			saveWeight=1;
		}

	|	PUNCT 
		{
			D(cerr << "punctuation:" << $1 << ':' <<  endl;) 

			if (yGram->appendRHS($1, TRUE)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}

			free($1);
			saveWeight=1;
		}

	|	rhsTokens TOKEN	
		{
			D(cerr << "token: " << $2 << endl;)
			if (yGram->appendRHS($2)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}
			free($2);
			saveWeight=1;
		};

	|	rhsTokens PUNCT	
		{
			D(cerr << "punctuation:" << $2 << ':' << endl;)
			if (yGram->appendRHS($2, TRUE)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}
			free($2);
			saveWeight=1;
		};
	;

weightedRhsTokens:
		rhsTokens WEIGHT 
		{
			D(cerr << "weight: " << $2 << endl;) 
			saveWeight=$2;
		};
	;
%%

int yyerror(char *s)
{
	char buffer[1024];

	sprintf(buffer, "Syntax Error on line # %d of %s", lexline, yGram->getFileName().c_str());

	errorMsg(EMT_ABORT, buffer, "yyerror()");
}
