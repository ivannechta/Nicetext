/*
 * Sample MTC++ program.
 *
 * Read strings from stdin and create unordered.dat and ordered.dat
 *
 * $Id: raofmake.cc,v 1.3 1997/05/03 17:06:11 markc Exp $
 *
 * $Log: raofmake.cc,v $
 * Revision 1.3  1997/05/03 17:06:11  markc
 * Use RBT instead of BST
 *
 * Revision 1.2  1997/03/05 15:51:25  markc
 * Added static char rcsid []  = "@(#)$Id: raofmake.cc,v 1.3 1997/05/03 17:06:11 markc Exp $";
 *
 * Revision 1.1  1995/11/08 16:57:38  chapman
 * Initial revision
 *
 *
 */

static char rcsid []  = "@(#)$Id: raofmake.cc,v 1.3 1997/05/03 17:06:11 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/rbt.h"
#include "../include/raof.h"

MTCwriteRAOF <MTCstring> *ordered;	// the sorted RAOF

void appendToOrdered(MTCstring *);

int main()
{
	MTCstring temp;
	MTCwriteRAOF <MTCstring> *unordered;

	// intro

	cout << "MTC++ raofmake utility." << endl;
	cout << "Copyright (c) 1995, Mark T. Chapman" << endl;

	// create ordered raof

	if ((ordered=new MTCwriteRAOF <MTCstring> ("ordered"))==NULL)
	{
		cerr << "Not Enough RAM" << endl;
		return -1;
	}

	ordered->setNeedEOL(TRUE);

	// create unordered raof

	if ((unordered=new MTCwriteRAOF <MTCstring> ("unordered"))==NULL)
	{
		cerr << "Not Enough RAM" << endl;
		return -1;
	}

	unordered->setNeedEOL(TRUE);

	// input loop

	cerr << "Reading Strings from STDIN..." << endl;

	cin >> temp;

	MTCRBT <MTCstring> sort;
	int i=0;

	while (cin.eof()==0)
	{
		sort.add(temp);

		unordered->append(temp);

		cin >> temp;
	} 

	cerr << "Closing UNORDERED raof..." << endl;

	if (unordered!=NULL)
	{
		delete unordered;
		unordered=NULL;
	}

	cerr << "Creating ORDERED raof..." << endl;

	sort.forEach(appendToOrdered);

	return 0;
}

// used by the forEach() method of a RBT

void appendToOrdered(MTCstring *aStringPtr)
{
	ordered->append(*aStringPtr);
}
