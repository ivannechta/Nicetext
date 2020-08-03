/*
 * Sample MTC++ program.
 *
 * Input Strings into a tree
 *
 * $Id: bsttest.cc,v 1.5 1997/03/05 15:51:25 markc Exp $
 *
 * $Log: bsttest.cc,v $
 * Revision 1.5  1997/03/05 15:51:25  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.4  1996/01/20 16:55:11  markc
 * removed \t from output
 *
// Revision 1.3  1996/01/20  16:52:13  markc
// modified output to use cerr and cout with one dump
//
// Revision 1.2  1996/01/14  22:52:18  markc
// added check for eof
//
// Revision 1.1  1996/01/14  22:34:55  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: bsttest.cc,v 1.5 1997/03/05 15:51:25 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/bst.h"

void dumpString(MTCstring *aString);

int main()
{
	MTCstring string;
	MTCBST <MTCstring> BST;

	// title 

	cerr << "MTC++ Binary Search Tree Object Test Utility" << endl;
        cerr << "Copyright (c) 1995, Mark T. Chapman" << endl;
	cerr << endl;
	cerr << "Please enter a bunch of strings.  End with Control-D." << endl;

	cin >> string;

	while (!cin.eof())
	{
		BST.add(string);

		cin >> string;
	}

	cerr << "Dumping " << BST.getCount() << " sorted strings to stdout..." << endl;

	BST.forEach(dumpString);

	cerr << "That's All Folks!" << endl;

	return 0;
}

void dumpString(MTCstring *aString)
{
	cout << *aString << endl;
}
