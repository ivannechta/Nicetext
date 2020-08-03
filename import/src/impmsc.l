%{
/*
 * Import from a misc file type (see below) 
 * Copyright (c) Mark T. Chapman 1995
 *
 **************************************
 * Example Format:
 * "word"             POS
 * "dog" 		N
 * "run"		V
 **************************************
 *
 * $Id: impmsc.l,v 1.3 1997/03/05 17:53:41 markc Exp $
 *
 * $Log: impmsc.l,v $
 * Revision 1.3  1997/03/05 17:53:41  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.2  1996/03/21 17:20:31  markc
 * modified program names and messages (minor)
 *
 * Revision 1.1  1996/02/14 00:19:26  markc
 * Initial revision
 *
 *
 */

static char rcsid []  = "@(#)$Id: impmsc.l,v 1.3 1997/03/05 17:53:41 markc Exp $";

#include <iostream.h>
#include <string.h>
#include <ctype.h>

#define IMPMSC_WORD 	1	
#define IMPMSC_TYPE	2
#define MAX_WORDLENGTH	511

void toLower(char *);

%}
%%
^\".*\"		return IMPMSC_WORD; 
" "[A-Z]+$	return IMPMSC_TYPE;
.|\n		;
%%

main()
{
	cerr << "reading msc format from stdin, writing dct format to stdout..." << endl;

	int retval;
	int x;
	char buffer[MAX_WORDLENGTH+1];

	buffer[MAX_WORDLENGTH]='\0';

	for(;;) 
	{
		retval=yylex();

		if (yytext[0]=='\0')
			break;

		switch (retval)
		{
			case IMPMSC_WORD:
						strncpy(buffer, yytext, MAX_WORDLENGTH);
						if ((x=strlen(buffer))>1)
						{
							buffer[x-1]='\0';
						}

						toLower(buffer);

						break;
			case IMPMSC_TYPE:
						fprintf(yyout, "%s %s\n", yytext, &buffer[1]);
						break;
		}
	}
}

void toLower(char *aCharPtr)
{
	for(int i=0; i< strlen(aCharPtr); i++)
		aCharPtr[i]=tolower(aCharPtr[i]);
}
