/*
 * Sample MTC++ program.
 *
 * Input Strings into a tree
 *
 * $Id: rbttest.cc,v 1.4 1997/03/05 15:51:25 markc Exp $
 *
 * $Log: rbttest.cc,v $
 * Revision 1.4  1997/03/05 15:51:25  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1997/02/16 02:56:01  markc
 * added test for findClosestSmaller()
 *
 * Revision 1.2  1996/01/20 16:54:11  markc
 * use cerr and cout for a single dump.
 *
// Revision 1.2  1996/01/14  22:52:18  markc
// added check for eof
//
// Revision 1.1  1996/01/14  22:34:55  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: rbttest.cc,v 1.4 1997/03/05 15:51:25 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/rbt.h"

void dumpString(MTCstring *aString);

int main(int argc, char *argv[])
{
	MTCstring string;
	MTCRBT <MTCstring> RBT;

	if (argc!=1)
	{
	}
	// title 

	cerr << "MTC++ Red-Black Tree Object Test Utility" << endl;
        cerr << "Copyright (c) 1995, Mark T. Chapman" << endl;
	cerr << endl;
	cerr << "Please enter a bunch of strings.  End with `quit'." << endl;

	cin >> string;

	while ((!cin.eof())&&(string!="quit"))
	{
		RBT.add(string);

		cin >> string;
	}

	cerr << "Dumping " << RBT.getCount() << " sorted strings to stdout..." << endl;

	RBT.forEach(dumpString);

	if (!cin.eof())
	{
		cerr << "Now we will test the 'findClosestSmaller() method'..." << endl;
		cerr << "Enter a set of strings.  Review list with `list'. End with `quit'." << endl;
		
		MTCstring *closest=NULL;

		string="";

		while ((!cin.eof())&&(string!="quit"))
		{
			cin >> string;

			if (string=="list")
			{
				RBT.forEach(dumpString);
			}
 			else
			{
				if ((closest=RBT.findClosestSmaller(string))==NULL)
				{
					cerr << "No strings we before '" << string << "' in the sorted list" << endl;	
				}
				else
				{
					cerr << *closest << " (is the closest string smaller than " << string << " in the sorted list" << endl;
				}
			}
		}
	}

	cerr << "That's All Folks!" << endl;

	return 0;
}

void dumpString(MTCstring *aString)
{
	cout << *aString << endl;
}
