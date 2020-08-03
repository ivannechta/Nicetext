/*
 * Test the Grammar Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: gramtest.cc,v 1.7 1997/04/22 16:35:48 markc Exp $
 *
 * $Log: gramtest.cc,v $
 * Revision 1.7  1997/04/22 16:35:48  markc
 * Fixed usage();
 *
 * Revision 1.6  1997/04/22 16:25:18  markc
 * Added -g, -d, -c
 *
 * Revision 1.5  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.4  1996/01/13 10:18:10  markc
 * use new sentnode enabled sentence models
 *
// Revision 1.3  1996/01/12  23:48:19  markc
// dynamic grammars work system wide -- next step, constant expressions
//
// Revision 1.2  1996/01/10  22:15:49  markc
// makeModel() works
//
// Revision 1.1  1996/01/08  22:07:07  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: gramtest.cc,v 1.7 1997/04/22 16:35:48 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/rbt.h"
#include "../../gendict/include/sentnode.h"
#include "../../gendict/include/sentmdl.h"
#include "../../gendict/include/typerec.h"
#include "../include/rule.h"
#include "../include/grammar.h"

void usage();

int main(int argc, char *argv[])
{
	MTCstring fileName("grammar.def");
	MTCstring typeName("mstr");
	char ch;
	int sampleCount=10;

        while ((ch = getopt(argc, argv, "g:d:c:h")) != EOF)
        {
                switch(ch) 
                {
                        case 'g':
                                fileName=optarg;
                                break;
			case 'd':
				typeName=optarg;
				break;
			case 'h':
				usage();
				return 0;
			case 'c':
				sampleCount=atoi(optarg);
				break;
			default:
				usage();
				return -1;
		}
	}

	argc -= optind;
	argv += optind;

	cout << "Testing Grammar Object" << endl;

	cout << "\tgrammar:  " << fileName << endl;
	cout << "\tdictionary: " << typeName << endl;
	cout << "\tloading..." << endl;

	typeName+="type";

	MTCgrammar grammar(fileName, typeName);

	grammar.allPunct();

	cout << "Dumping grammar to stdout..." << endl;

	cout << grammar << endl;

	cout << "Generating " << sampleCount << " random sentence models (each ends with 'e 0')..." << endl;

	if (grammar.isReady()==FALSE)
	{
		cout << "*** WARNING: Although the grammar is not ready, I will try to make models anyway" << endl;
		cout << "*** THIS MAY HAVE UNPREDICTABLE RESULTS!!!" << endl;
		cout << endl;
	}

	for(int i=0; i<sampleCount; i++)
	{
		MTCsentenceModel *model=grammar.makeModel(25, FALSE);

		if (model==NULL)
		{
			break;
		}
		else
		{
			cout << *model << endl;
		}
	}

	cout << "That's All Folks!" << endl;

	return 0;
}

void usage()
{
	cerr << "Usage: gramtest [-g grammarFile] [-d dictionary] [-c sampleCount]" << endl;
	cerr << "\t-g GrammarFile -- the name of the grammar definition file." << endl; 
	cerr << "\t   (defaults to grammar.def)" << endl;
	cerr << "\t-d Dictionary -- the name of the dictionary prefix" << endl;
 	cerr << "\t   (defaults to 'mstr')" << endl;
	cerr << "\t-c SampleCount -- print this number of sample sentence models" << endl;
}
