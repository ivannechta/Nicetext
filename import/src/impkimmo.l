%{

// import the output of kimmo into a raw definition entry format
// suitable for sortdct

static char rcsid []  = "@(#)$Id: impkimmo.l,v 1.10 1998/06/25 17:54:40 markc Exp $";


#define NEW_WORD	1
#define END_WORD	2
#define BAD_WORD	3
#define NEW_DEF		4

#define POS_N		10
#define POS_V		20
#define POS_AJ		30 
#define POS_AV 		40
#define POS_AUX 	50
#define POS_PP 		60
#define POS_PR		70 
#define POS_CJ 		80
#define POS_DT 		90
#define POS_IJ 		100
#define POS_INF		110

#define PERSON_1	120
#define PERSON_2	130
#define PERSON_3	140

#define SG3_MINUS       141
#define SG3_PLUS        142

#define NUMBER_SG	150
#define NUMBER_PL	160

#define PROPER_PLUS	170
#define PROPER_MINUS 	180

#define TENSE_PRES	190
#define TENSE_PAST	200

#define VFORM_S		210
#define VFORM_ED	220
#define VFORM_EN	230
#define VFORM_ING	240
#define VFORM_BASE      241

#define FINITE_PLUS	250
#define FINITE_MINUS	260 

#define AFORM_ABS	270
#define AFORM_COMP	280
#define AFORM_SUPER	290

#define VERBAL_PLUS	300
#define VERBAL_MINUS	310

#define CASE_NOM	320
#define CASE_ACC 	330
#define CASE_GEN	340
#define CASE_IND	350

#define REFLEX_PLUS	360
#define REFLEX_MINUS	370

#define WH_PLUS		380
#define WH_MINUS	390

#define REG_PLUS	400
#define REG_MINUS	410

#define MODAL_PLUS	420
#define MODAL_MINUS	430

#define NEG_PLUS	440
#define NEG_MINUS	450

#define CLITIC_CFORM	460
#define CFORM_GEN_PLUS	465

#define END_OF_FILE     470

#include <iostream.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/mstring.h"

#define MAX_WORDLEN 1024

void clear();
void show();
%}
%%
<<EOF>>			return END_OF_FILE;
^[a-zA-Z0-9'-]+$	return NEW_WORD;
^[0-9]+" parse".*found$	return END_WORD;
\*\*\*" NONE ".*$	return BAD_WORD; 
^Word:			return NEW_DEF;
" "pos:" "*N	return POS_N;
" "pos:" "*V	return POS_V;
" "pos:" "*AJ	return POS_AJ;
" "pos:" "*AV	return POS_AV;
" "pos:" "*AUX	return POS_AUX;
" "pos:" "*PP	return POS_PP;
" "pos:" "*PR	return POS_PR;
" "pos:" "*CJ	return POS_CJ;
" "pos:" "*DT	return POS_DT;
" "pos:" "*IJ	return POS_IJ;
" "pos:" "*INF	return POS_INF;
" "3sg:" "*-	return SG3_MINUS;
" "3sg:" "*\+	return SG3_PLUS;
" "person:" "*1	return PERSON_1;
" "person:" "*2	return PERSON_2;
" "person:" "*3	return PERSON_3;
" "number:" "*SG	return NUMBER_SG;
" "number:" "*PL	return NUMBER_PL;
" "proper:" "*\+	return PROPER_PLUS;
" "proper:" "*-	return PROPER_MINUS;
" "tense:" "*PRES	return TENSE_PRES;
" "tense:" "*PAST	return TENSE_PAST;
" "vform:" "*S	return VFORM_S;
" "vform:" "*ED	return VFORM_ED;
" "vform:" "*EN	return VFORM_EN;
" "vform:" "*ING	return VFORM_ING;
" "vform:" "*BASE  return VFORM_BASE;
" "finite:" "*\+	return FINITE_PLUS;
" "finite:" "*-	return FINITE_MINUS;
" "aform:" "*ABS	return AFORM_ABS;
" "aform:" "*COMP	return AFORM_COMP;
" "aform:" "*SUPER	return AFORM_SUPER;
" "verbal:" "*\+	return VERBAL_PLUS;
" "verbal:" "*-	return VERBAL_MINUS;
" "case:" "*NOM	return CASE_NOM;
" "case:" "*ACC	return CASE_ACC;
" "case:" "*GEN	return CASE_GEN;
" "case:" "*IND	return CASE_IND;
" "reflex:" "*\+	return REFLEX_PLUS;
" "reflex:" "*-		return REFLEX_MINUS;
" "wh:" "*\+		return WH_PLUS;
" "wh:" "*-		return WH_MINUS;
" "reg:" "*\+		return REG_PLUS;
" "reg:" "*-		return REG_MINUS;
" "modal:" "*\+		return MODAL_PLUS;
" "modal:" "*-		return MODAL_MINUS;
" "neg:" "*\+		return NEG_PLUS;
" "neg:" "*-		return NEG_MINUS;
" "cform:" +"GEN	return CFORM_GEN_PLUS;
" "clitic:" "*cform 	return CLITIC_CFORM;
.|\n		;
%%

MTCstring word;
MTCstring pos;
MTCstring person;
MTCstring sg3_plus;
MTCstring sg3_minus;
MTCstring number;
MTCstring proper;
MTCstring tense; 
MTCstring vform;
MTCstring finite;
MTCstring aform;
MTCstring verbal; 
MTCstring Case;
MTCstring reflex;
MTCstring wh;
MTCstring reg;
MTCstring modal;
MTCstring neg;
MTCstring clitic;
MTCstring cform_gen_plus;
BOOL defined=FALSE;

main()
{
	cerr << "reading kimmo output from stdin, writing dct format to stdout..." << endl; 

	int retval;
	BOOL badWord=TRUE; 

	for(;;) 
	{
		retval=yylex();

		if ((yytext[0]=='\0')||(retval==END_OF_FILE))
		{
			break;
		}

		switch (retval)
		{
		case NEW_WORD:
			clear();
			word=yytext;
			break;
		case END_WORD:
			show();
			clear();
			break;
		case BAD_WORD:
			clear();
			break;
		case NEW_DEF:
			if (defined==TRUE)
			{
				MTCstring saveWord(word);
				show();
				clear();
				word=saveWord;
			}
		case POS_N:
			defined=TRUE;
			pos="N";
			break;
		case POS_V:
			defined=TRUE;
			pos="V";
			break;
		case POS_AJ:
			defined=TRUE;
			pos="AJ";
			break;
		case POS_AV:
			defined=TRUE;
			pos="AV";
			break;
		case POS_AUX:
			defined=TRUE;
			pos="AUX";
			break;
		case POS_PP:
			defined=TRUE;
			pos="PP";
			break;
		case POS_PR:
			defined=TRUE;
			pos="PR";
			break;
		case POS_CJ:
			defined=TRUE;
			pos="CJ";
			break;
		case POS_DT:
			defined=TRUE;
			pos="DT";
			break;
		case POS_IJ:
			defined=TRUE;
			pos="IJ";
			break;
		case POS_INF:
			defined=TRUE;
			pos="INF";
			break;
		case PERSON_1:
			defined=TRUE;
			person="1";
			break;
		case PERSON_2:
			defined=TRUE;
			person="2";
			break;
		case PERSON_3:
			defined=TRUE;
			person="3";
			break;
		case SG3_PLUS:
			defined=TRUE;
			sg3_plus="3sg+";
			break;
		case SG3_MINUS:
			defined=TRUE;
			sg3_plus="3sg-";
			break;
		case NUMBER_SG:
			defined=TRUE;
			number="Sg";
			break;
		case NUMBER_PL:
			defined=TRUE;
			number="Pl";
			break;
		case PROPER_PLUS:
			defined=TRUE;
			proper="Prop+";
			break;
		case PROPER_MINUS:
			defined=TRUE;
			proper="Prop-";
			break;
		case TENSE_PRES:
			defined=TRUE;
			tense="Pres";
			break;
		case TENSE_PAST:
			defined=TRUE;
			tense="Past";
			break;
		case VFORM_S:
			defined=TRUE;
			vform="S";
			break;
		case VFORM_ED:
			defined=TRUE;
			vform="Ed";
			break;
		case VFORM_EN:
			defined=TRUE;
			vform="En";
			break;
		case VFORM_ING:
			defined=TRUE;
			vform="Ing";
			break;
		case VFORM_BASE:
			defined=TRUE;
			vform="Base";
			break;
		case FINITE_PLUS:
			defined=TRUE;
			finite="Fin+";
			break;
		case FINITE_MINUS:
			defined=TRUE;
			finite="Fin-";
			break;
		case AFORM_ABS:
			defined=TRUE;
			aform="Abs";
			break;
		case AFORM_COMP:
			defined=TRUE;
			aform="Comp";
			break;
		case AFORM_SUPER:
			defined=TRUE;
			aform="Super";
			break;
		case VERBAL_PLUS:
			defined=TRUE;
			verbal="Verbal+";
			break;
		case VERBAL_MINUS:
			defined=TRUE;
			verbal="Verbal-";
			break;
		case CASE_NOM:
			defined=TRUE;
			Case="Nom";
			break;
		case CASE_ACC:
			defined=TRUE;
			Case="Acc";
			break;
		case CASE_GEN:
			defined=TRUE;
			Case="Gen";
			break;
		case CASE_IND:
			defined=TRUE;
			Case="Ind";
			break;
		case REFLEX_PLUS:
			defined=TRUE;
			reflex="Reflex+";
			break;
		case REFLEX_MINUS:
			defined=TRUE;
			reflex="Reflex-";
			break;
		case WH_PLUS:
			defined=TRUE;
			wh="Wh+";
			break;
		case WH_MINUS:
			defined=TRUE;
			wh="Wh-";
			break;
		case REG_PLUS:
			defined=TRUE;
			reg="Reg+";
			break;
		case REG_MINUS:
			defined=TRUE;
			reg="Reg-";
			break;
		case MODAL_PLUS:
			defined=TRUE;
			modal="Mod+";
			break;
		case MODAL_MINUS:
			defined=TRUE;
			modal="Mod-";
			break;
		case NEG_PLUS:
			defined=TRUE;
			neg="Neg+";
			break;
		case NEG_MINUS:
			defined=TRUE;
			neg="Neg-";
			break;
		case CLITIC_CFORM:
			defined=TRUE;
			clitic="Cform";
			break;
		case CFORM_GEN_PLUS:
			defined=TRUE;
			cform_gen_plus="CFormGen+";
			break;
		default:
			defined=TRUE;
			break;
		}
	}

	show();

	return 0;
}

void clear()
{
	word=NULL;
	pos=NULL;
	person=NULL;
	sg3_plus=NULL;
	sg3_minus=NULL;
	number=NULL;
	proper=NULL;
	tense=NULL;
	vform=NULL;
	finite=NULL;
	aform=NULL;
	verbal=NULL;
	Case=NULL;
	reflex=NULL;
	wh=NULL;
	modal=NULL;
	reg=NULL;
	neg=NULL;
	clitic=NULL;
	cform_gen_plus=NULL;
	defined=FALSE;

	return;
}

void show()
{
	if ((word.length()>0)&&(defined==TRUE))
	{
		MTCstring type(pos);
		MTCstring temp(sg3_plus);

		temp+=sg3_minus;
		temp+=person;
		temp+=number;
		temp+=proper;
		temp+=tense;
		temp+=vform;
		temp+=finite;
		temp+=aform;
		temp+=verbal;
		temp+=Case;
		temp+=reflex;
		temp+=wh;
		temp+=reg;
		temp+=modal;
		temp+=neg;
		temp+=clitic;
		temp+=cform_gen_plus;

		if (temp.length()>0)
		{
			type+="_";
			type+=temp;
		}

		cout << type << ' ' << word << endl;
	}
}
