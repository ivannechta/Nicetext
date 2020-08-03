%{

/*
 *
 * import from webster program 
 *
 * $Id: impwbstr.l,v 1.3 1997/03/05 17:53:41 markc Exp $
 *
 * $Log: impwbstr.l,v $
 * Revision 1.3  1997/03/05 17:53:41  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.2  1996/03/21 17:20:31  markc
 * modified program names and messages (minor)
 *
 * Revision 1.1  1996/02/13 13:18:14  markc
 * Initial revision
 *
 */

static char rcsid []  = "@(#)$Id: impwbstr.l,v 1.3 1997/03/05 17:53:41 markc Exp $";

#include <iostream.h>

// set the mode to be one of FILTER_MODE, BAD_FILTER_MODE, or NORMAL_MODE

//#define FILTER_MODE
//#define BAD_FILTER_MODE
#define NORMAL_MODE
#define DEBUG

#ifdef DEBUG
#define DBG(x)	x
#else
#define DBG(x)
#endif

#ifdef FILTER_MODE		// filter out any IGNORE_IT lines
	#define IGNORE_IT	
	#define RETURN(x)	ECHO;	
	#define SKIP		ECHO;				
#else
#ifdef BAD_FILTER_MODE		// only print the IGNORE_IT lines
	#define IGNORE_IT	fprintf(yyout, "%s\n", yytext);
	#define RETURN(x)
	#define SKIP				
#else  // NORMAL MODE		// operate normally -- parsing the data
	#define IGNORE_IT
	#define RETURN(x) 	return(x);
	#define SKIP
#endif
#endif

#define ALT_FORM	0
#define NEW_WORD	5	
#define NEW_DEFINITION	7	
#define HYPH_WORD	10	
#define NO_DEFINITION	15
#define TYPE_ADJ	100
#define TYPE_ADV	105
#define TYPE_BIOGRAPHY  107
#define TYPE_CONJ	110	
#define TYPE_GEOGRAPHY  111
#define TYPE_INTERJ	115
#define TYPE_N		120
#define TYPE_NPL 	125	
#define TYPE_PREP	130	
#define TYPE_PRON	135
#define TYPE_VB		140
#define TYPE_VI		145
#define TYPE_VT		150
#define OFTEN_ATTRIB	300
#define OFTEN_CAP	305

#define BOOL short
#define TRUE 1
#define FALSE 0

#define MAX_WORDLEN 1024

char word[MAX_WORDLEN+1];
char base[MAX_WORDLEN+1];
void defWord(char *);
void setWord(char *);


#ifdef NOTNOT
\[((.*|((.*\n){1,5})).*)\]		/* fprintf(yyout, "I JUST ATE :%s:\n", yytext); */
\\((.*|((.*\n){1,5})).*)\\		/* fprintf(yyout, "I JUST ATE :%s:\n", yytext); */
.|\n		/* fprintf(yyout, "%s", yytext) */;
#endif NOTNOT
%}
%%
Word:" "(.*)				RETURN (NEW_WORD);
^"No definition"			RETURN (NO_DEFINITION);
(\n|" ")"adj"(" "|\n|,)			RETURN (TYPE_ADJ);
(\n|" ")"adv"(" "|\n|,)			RETURN (TYPE_ADV);
(\n|" ")"conj"(" "|\n|,)		RETURN (TYPE_CONJ);
(\n|" ")"Geographical Names"(" "|\n|,)	RETURN (TYPE_GEOGRAPHY);
(\n|" ")"Biographical Names"(" "|\n|,)	RETURN (TYPE_BIOGRAPHY);
(\n|" ")"interj"(" "|\n|,)		RETURN (TYPE_INTERJ);
(\n|" ")"n"(" "|\n|,)			RETURN (TYPE_N);
(\n|" ")"pl"(" "|\n|,)			RETURN (TYPE_NPL);
(\n|" ")"prep"(" "|\n|,)		RETURN (TYPE_PREP);
(\n|" ")"pron"(" "|\n|,)		RETURN (TYPE_PRON);
(\n|" ")"vb"(" "|\n|,)			RETURN (TYPE_VB);
(\n|" ")"vi"(" "|\n|,)			RETURN (TYPE_VI);
(\n|" ")"vt"(" "|\n|,)			RETURN (TYPE_VT);
^[0-9]+[a-z]*:.*$			IGNORE_IT;
^[0-9]+[a-z]*" --".*$			IGNORE_IT;
^[0-9]+[A-Za-z-]*			RETURN (NEW_DEFINITION);
^" "+.*$				IGNORE_IT;
^\(.*$					IGNORE_IT;
^\[.*$					IGNORE_IT;
^" "*syn.*$				IGNORE_IT;
^" "*usage.*$				IGNORE_IT;
^:.*$					IGNORE_IT;
"often attrib"				RETURN (OFTEN_ATTRIB);
"often cap"				RETURN (OFTEN_CAP);
^"-- "[A-Za-z]+((-[A-Za-z]*)*)		RETURN (ALT_FORM);
.|\n					SKIP;
%%

main()
{
	cerr << "reading webster data from stdin, writing dct format to stdout..." << endl;

	int retval;
	BOOL badWord=TRUE; 

	for(;;) 
	{
		retval=yylex();

		if (yytext[0]=='\0')
			break;

		switch (retval)
		{
		case NEW_WORD:
			badWord=FALSE;
			DBG(fprintf(yyout, "New Word: %s\n", &yytext[6]);)
			setWord(&yytext[6]);
			break;
		case NEW_DEFINITION:
			badWord=FALSE;
			DBG(fprintf(yyout, "\tNew Definition: %s\n", yytext);)
			/* setWord(yytext); */
			break;
		case HYPH_WORD:
			if (badWord==FALSE)
			{
				DBG(fprintf(yyout, "Hyph word: %s\n", yytext));
			}
			break;
		case ALT_FORM:
			DBG(fprintf(yyout, "ALT_FORM: %s\n", &yytext[3]);)
			setWord(&yytext[3]);
			badWord=FALSE;
			break;
		case NO_DEFINITION:
			DBG(fprintf(yyout, "\tNo definition\n");)
			badWord=TRUE;
			break;
		case TYPE_ADJ:
			if (badWord==FALSE)
				defWord("adj");
			break;
		case TYPE_ADV:
			if (badWord==FALSE)
				defWord("adv");
			break;
		case TYPE_BIOGRAPHY:
			if (badWord==FALSE)
				defWord("bio");
			break;
		case TYPE_CONJ:
			if (badWord==FALSE)
				defWord("conj");
			break;
		case TYPE_GEOGRAPHY:
			if (badWord==FALSE)
			{
				defWord("geo");
			}
			break;
		case TYPE_INTERJ:
			if (badWord==FALSE)
			{
				defWord("ij");
			}
			break;
		case TYPE_N:
			if (badWord==FALSE)
			{
				defWord("ns");
			}
			break;
		case TYPE_NPL:
			if (badWord==FALSE)
			{
				defWord("np");
			}
			break;
		case TYPE_PREP:
			if (badWord==FALSE)
			{
				defWord("pp");
			}
			break;
		case TYPE_PRON:
			if (badWord==FALSE)
			{
				defWord("pn");
			}
			break;
		case TYPE_VB:
			if (badWord==FALSE)
			{
				defWord("vb");
			}
			break;
		case TYPE_VI:
			if (badWord==FALSE)
			{
				defWord("vi");
			}
			break;
		case TYPE_VT:
			if (badWord==FALSE)
			{
				defWord("vt");
			}
			break;
		case OFTEN_ATTRIB:
			if (badWord==FALSE)
			{ 
				DBG(fprintf(yyout, "\t\toften ATTRIB!\n");)
			}
			break;
		case OFTEN_CAP:
			if (badWord==FALSE)
			{
				DBG(fprintf(yyout, "\t\toften CAP!\n");)
			}
			break;
		default:
			fprintf(yyout, "ERROR\n", yytext);
			break;
		}
	}

	return 0;
}

void setWord(char *rawWord)
{
	int i;
	int j;
	int l;

	for(l=0; l<strlen(rawWord); l++)
	{
		if ((rawWord[l]>'9')||(rawWord[l]<'0'))
				break;
	}

	j=0;

	for(i=l; i<strlen(rawWord); i++)
	{
		if (rawWord[i]!='-')
		{
			word[j++]=rawWord[i];
		}
	}

	word[j]=NULL;
}

void defWord(char *defStr)
{
	DBG(fprintf(yyout, "\t");)

	fprintf(yyout, "%s %s\n", defStr, word); 
}
