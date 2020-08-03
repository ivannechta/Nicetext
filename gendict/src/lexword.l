%{
/*
 * 
 * Lex Input File for Word and Sentence Parsing
 * Copyright (c) 1995, Mark T. Chapman
 * 
 * $Id: lexword.l,v 1.10 1998/06/22 18:16:12 markc Exp $
 *
 * $Log: lexword.l,v $
 * Revision 1.10  1998/06/22 18:16:12  markc
 * *** empty log message ***
 *
 * Revision 1.9  1998/03/13 04:02:07  root
 * removed yywrap()
 *
 * Revision 1.8  1997/03/05 15:44:21  markc
 * Added nlAsPunct parameter.
 *
 * Revision 1.7  1997/03/04 16:09:50  markc
 * Added uppercase to 'x and x' options
 *
 * Revision 1.6  1997/03/03 19:09:41  markc
 * Do NOT return unrecognized letters as PUNCTUATION
 * This causes problems such as when "johnsons'" is really part of "'johnsons'"
 *
 * Revision 1.5  1997/03/03 03:20:15  markc
 * Added D'word O'word, prefixes for names
 *
 * Revision 1.4  1997/03/02 04:22:17  markc
 * Fixed can't from ca'nt to can't
 *
 * Revision 1.3  1997/03/01 22:38:10  markc
 * Allow contractions, hyphenation, http://, etc.
 *
 * Revision 1.2  1995/08/31 00:45:52  markc
 * Adjusted end-of-sentence punctuation rule to ignore punctuation after \n
 *
 * Revision 1.1  1995/07/29  17:20:54  markc
 * Initial revision
 *
 *
 */

static char rcsid []  = "@(#)$Id: lexword.l,v 1.10 1998/06/22 18:16:12 markc Exp $";

#define END_OF_WORD	0
#define END_OF_SENTENCE 1
#define PUNCTUATION	3

short nlAsPunct=0;

%}

%%
((((D|d)')*)|(((O|o)')*)|(((L|l)')*))(([A-Za-z0-9&#@$%\*\+_]+)(((('(s|S))|((n|N)'(t|T))|((s|S)')|('((ve)|(VE)))|(((in)|(IN))')|('((re)|(RE))))*)|((((\.)|(\-)|(:\/\/)){1}([A-Za-z0-9&#@$%\*\+_]+)+)*)))	 return END_OF_WORD;
(\.(" "|\n)*){3}|,|;|:|\(|\)|\>|\<|\"|=|~|\+	return PUNCTUATION;;
(\.|\?|!)+(" "|\")*(\n)*	return END_OF_SENTENCE;
\n{2}				return END_OF_SENTENCE;
\n				if (nlAsPunct!=0) { return PUNCTUATION;}
.				;

%%

int zzwrap()
{
	return 1;
}
