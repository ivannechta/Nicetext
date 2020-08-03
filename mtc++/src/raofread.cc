/*
 * Sample MTC++ program.
 *
 * Read strings from a RAOF utility.
 *
 * $Id: raofread.cc,v 1.3 1997/03/05 15:51:25 markc Exp $
 *
 * $Log: raofread.cc,v $
 * Revision 1.3  1997/03/05 15:51:25  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.2  1996/01/14 22:33:35  markc
 * fixed 'quit' to be 'QUIT' -- be consitent ;)
 *
// Revision 1.1  1995/11/08  16:57:49  chapman
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: raofread.cc,v 1.3 1997/03/05 15:51:25 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/bst.h"
#include "../include/raof.h"


int main()
{
	MTCstring datName;
	MTCstring jmpName;

	// which raof are we using?

	cout << "MTC++ RAOF Read Utility" << endl;
        cout << "Copyright (c) 1995, Mark T. Chapman" << endl;
	cout << endl;
	cout << "Please enter the name of the .dat file (i.e. ordered.dat)" << endl;
	cin >> datName; 
	cout << "Please enter the name of the .jmp file (i.e. ordered.jmp)" << endl;
	cin >> jmpName;

	// get the input file

	MTCstring temp;
	MTCreadRAOF <MTCstring> readRAOF(datName, jmpName);

	cout << "Size of long on this machine: " << sizeof(long) << endl;
	cout << "Number of records in .jmp=" << readRAOF.getCount() << endl;

	cout << endl;

	cout << "To access the .dat by value the .jmp must be sorted in MTCstring order. I do not check that at this time.";
	cout << "You may always access by index." << endl;

	cout << "Do you want to access by number (y/n)?" << endl;
	char c;
	cin >> c;

	const MTCstring *answer;

	if (toupper(c)!='Y')
	{
		for(;;)
		{
			cout << "Enter a Word, 'QUIT' to quit: " << endl;
			cin >> temp;

			answer=readRAOF.find(temp);

			if (answer==NULL)
			{
				cout << "No Answer! (Maybe this RAOF isn't sorted) " << endl;
			}
			else
			{
				cout << (*answer) << endl;
			}

			if (temp=="QUIT")
			{
				break;
			}

		}
	}
	else
	{
		long index;

		for (;;)
		{
			cout << "Enter an index number, '-1' to quit: " << endl;
			cin >> index;

			answer=readRAOF.read(index);

			if (answer==NULL)
			{
				cout << "No Answer!" << endl;
			}
			else
			{
				cout << (*answer) << endl;
			}

			if (index==-1)
			{
				break;
			}
		}
	}

	return 0;
}

