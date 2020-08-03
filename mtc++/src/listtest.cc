/*
 * Sample MTC++ program.
 *
 * Input Strings into a list object 
 *
 * $Id: listtest.cc,v 1.4 1997/03/05 15:51:25 markc Exp $
 *
 * $Log: listtest.cc,v $
 * Revision 1.4  1997/03/05 15:51:25  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1996/01/14 22:51:48  markc
 * added eof check
 *
// Revision 1.2  1995/12/16  13:15:46  markc
// test operator=()
//
// Revision 1.1  1995/12/05  21:14:21  chapman
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: listtest.cc,v 1.4 1997/03/05 15:51:25 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/list.h"

void dumpString(MTCstring *aString);

int main()
{
	MTCstring string;
	MTClist <MTCstring> list;
	MTClist <MTCstring> copy;

	// title 

	cout << "MTC++ List Object Test Utility" << endl;
        cout << "Copyright (c) 1995, Mark T. Chapman" << endl;
	cout << endl;
	cout << "Please enter a bunch of strings.  End with QUIT." << endl;

	cin >> string;

	while ((string!="QUIT")&&(!cin.eof()))
	{
		list.add(string);

		MTClist <MTCstring> tempList(list);

		cout << " * * * * Start List" << endl;

		tempList.forEach(dumpString);

		cout << " * * * * End List" << endl;

		cout << " * * * * Start Copy" << endl;
		copy=list;
		copy.forEach(dumpString);
		cout << " * * * * End Copy" << endl;

		cin >> string;

	}

	return 0;
}

void dumpString(MTCstring *aString)
{
	cout << '\t' << *aString << endl;
}
