#ifndef lint
static char const yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#if defined(c_plusplus) || defined(__cplusplus)
#include <stdlib.h>
#else
extern char *getenv();
extern void *realloc();
#endif
static int yygrowstack();
#define YYPREFIX "yy"
#line 2 "yaccgram.y"
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
#endif /* DEBUG_YACC_VERBOSE*/

extern MTCgrammar *yGram;
extern long lexline;
unsigned long saveWeight=1;

#line 75 "yaccgram.y"
typedef union {
	char *string;
	unsigned long  number;
} YYSTYPE;
#line 99 "y.tab.c"
#define TOKEN 257
#define PUNCT 258
#define WEIGHT 259
#define BAD 260
#define YYERRCODE 256
const short yylhs[] = {                                        -1,
    0,    0,    1,    2,    2,    2,    2,    3,    3,    3,
    3,    4,
};
const short yylen[] = {                                         2,
    1,    2,    4,    1,    1,    3,    3,    1,    1,    2,
    2,    2,
};
const short yydefred[] = {                                      0,
    0,    0,    1,    0,    2,    8,    9,    0,    0,    5,
    3,    0,   10,   11,   12,    0,    7,
};
const short yydgoto[] = {                                       2,
    3,    8,    9,   10,
};
const short yysindex[] = {                                   -249,
  -49, -249,    0, -251,    0,    0,    0,  -59, -254,    0,
    0, -251,    0,    0,    0, -254,    0,
};
const short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,  -58,    0,
    0,    0,    0,    0,    0,  -57,    0,
};
const short yygindex[] = {                                      0,
    8,    0,   -1,    1,
};
#define YYTABLESIZE 67
const short yytable[] = {                                      11,
    4,    6,   13,   14,   15,    6,    7,    1,    4,    5,
   16,    0,   17,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   12,    4,    6,
};
const short yycheck[] = {                                      59,
   59,   59,  257,  258,  259,  257,  258,  257,   58,    2,
   12,   -1,   12,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  124,  124,  124,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#elif YYDEBUG
#include <stdio.h>
#endif
#define YYMAXTOKEN 260
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"':'","';'",0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'|'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"TOKEN","PUNCT",
"WEIGHT","BAD",
};
const char * const yyrule[] = {
"$accept : grammar",
"grammar : rule",
"grammar : grammar rule",
"rule : TOKEN ':' rhs ';'",
"rhs : rhsTokens",
"rhs : weightedRhsTokens",
"rhs : rhs '|' rhsTokens",
"rhs : rhs '|' weightedRhsTokens",
"rhsTokens : TOKEN",
"rhsTokens : PUNCT",
"rhsTokens : rhsTokens TOKEN",
"rhsTokens : rhsTokens PUNCT",
"weightedRhsTokens : rhsTokens WEIGHT",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
#line 188 "yaccgram.y"

int yyerror(char *s)
{
	char buffer[1024];

	sprintf(buffer, "Syntax Error on line # %d of %s", lexline, yGram->getFileName().c_str());

	errorMsg(EMT_ABORT, buffer, "yyerror()");
}
#line 216 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    if ((newss = (short *)realloc(yyss, newsize * sizeof *newss)) == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    if ((newvs = (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs)) == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 88 "yaccgram.y"
{ D(cerr << "rule inserted to grammar" << endl;) }
break;
case 2:
#line 89 "yaccgram.y"
{ D(cerr << "rule appended to grammar" << endl;)}
break;
case 3:
#line 94 "yaccgram.y"
{	
			D( cerr << "Rule completed:" << yyvsp[-3].string << endl;) 
			if (yGram->commitRule(yyvsp[-3].string)==FALSE)
			{
				errorMsg(EMT_ERROR, "I could not commit a rule", "yyparse()");
			}
			free(yyvsp[-3].string); 
		}
break;
case 4:
#line 106 "yaccgram.y"
{ 
			D(cerr << "rhsTokens completed: " << endl;) 
			yGram->commitRHSSet(saveWeight);
		}
break;
case 5:
#line 112 "yaccgram.y"
{
			D(cerr << "weightedRhsTokens completed: " << endl;) 
			yGram->commitRHSSet(saveWeight);
		}
break;
case 6:
#line 118 "yaccgram.y"
{	
			D(cerr << "rhs|rhstokens" << endl;) 
			yGram->commitRHSSet(saveWeight);
		}
break;
case 7:
#line 124 "yaccgram.y"
{ 
			D(cerr <<  "rhs|weightedRhsTokens" << endl;) 
			yGram->commitRHSSet(saveWeight);
		}
break;
case 8:
#line 132 "yaccgram.y"
{
			D(cerr << "token: " << yyvsp[0].string << endl;) 

			if (yGram->appendRHS(yyvsp[0].string)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}

			free(yyvsp[0].string);
			saveWeight=1;
		}
break;
case 9:
#line 145 "yaccgram.y"
{
			D(cerr << "punctuation:" << yyvsp[0].string << ':' <<  endl;) 

			if (yGram->appendRHS(yyvsp[0].string, TRUE)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}

			free(yyvsp[0].string);
			saveWeight=1;
		}
break;
case 10:
#line 158 "yaccgram.y"
{
			D(cerr << "token: " << yyvsp[0].string << endl;)
			if (yGram->appendRHS(yyvsp[0].string)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}
			free(yyvsp[0].string);
			saveWeight=1;
		}
break;
case 11:
#line 169 "yaccgram.y"
{
			D(cerr << "punctuation:" << yyvsp[0].string << ':' << endl;)
			if (yGram->appendRHS(yyvsp[0].string, TRUE)==FALSE)
			{
				errorMsg(EMT_WARNING, "I could not append a RHS rule", "yyparse()");
			}
			free(yyvsp[0].string);
			saveWeight=1;
		}
break;
case 12:
#line 182 "yaccgram.y"
{
			D(cerr << "weight: " << yyvsp[0].number << endl;) 
			saveWeight=yyvsp[0].number;
		}
break;
#line 491 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
