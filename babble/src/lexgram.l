%{
/*
 * 
 * Lex Input File for Grammar Scanning
 * Copyright (c) 1995, Mark T. Chapman
 * 
 * $Id: lexgram.l,v 1.8 1998/06/22 18:16:01 markc Exp $
 *
 * $Log: lexgram.l,v $
 * Revision 1.8  1998/06/22 18:16:01  markc
 * *** empty log message ***
 *
 * Revision 1.7  1998/03/13 04:02:34  root
 * removed yywrap()
 *
 * Revision 1.6  1996/02/07 03:51:48  markc
 * Allowed commas in tokens
 *
 * Revision 1.5  1996/01/13  12:00:33  markc
 * added punctuation processing to grammar definition language using {}
 *
 * Revision 1.4  1996/01/10  22:43:29  markc
 * Allowed '+' to be valid in token names
 *
 * Revision 1.3  1996/01/10  20:41:59  markc
 * correctly parses and sets up grammar rules -- does not yet makeModel()
 *
 * Revision 1.2  1996/01/08  21:54:25  markc
 * parses tokens for grammar.def correctly
 *
 * Revision 1.1  1996/01/08  17:32:14  markc
 * Initial revision
 *
 *
 */

#include <string.h>
#include "y.tab.h"

static char rcsid []  = "@(#)$Id: lexgram.l,v 1.8 1998/06/22 18:16:01 markc Exp $";

extern YYSTYPE yylval;
long lexline=1;

%}

%s QUOTE
%%
<QUOTE>[^\{]*\}			{yylval.string=strdup(yytext); yylval.string[strlen(yylval.string)-1]=NULL; BEGIN 0; return PUNCT; }
\/\/.*$				{/* ignore single-line comments */}
[A-Za-z_\-]+[A-Za-z_\-0-9\+,]*	{yylval.string=strdup(yytext);	 return TOKEN;}
@[0-9]+				{yylval.number=atol(&yytext[1]); return WEIGHT;}
":"				{yylval.number=0;	return ':';}
";"				{yylval.number=0;	return ';';}
"|"				{yylval.number=0;	return '|';}
\{				{BEGIN QUOTE; }
\n				{lexline++;}
(\t|" ")*			;
.				{yylval.number=0;	return BAD;}
%%

int yywrap()
{
	return 1;
}
