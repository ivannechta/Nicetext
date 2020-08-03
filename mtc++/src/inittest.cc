/*
 * Sample MTC++ program.
 *
 * Read and display sections, keys, values in an .ini file
 *
 * $Id: inittest.cc,v 1.6 1997/03/05 15:51:25 markc Exp $
 *
 * $Log: inittest.cc,v $
 * Revision 1.6  1997/03/05 15:51:25  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.5  1996/04/24 16:46:19  markc
 * added return -1 if argc!=2
 *
 * Revision 1.4  1995/12/19 20:43:12  markc
 * removed #include <String.h>
 *
// Revision 1.3  1995/12/19  20:11:48  markc
// *** empty log message ***
//
// Revision 1.2  1995/12/19  20:10:41  markc
// Fixed usage statement
//
// Revision 1.1  1995/12/19  09:28:54  markc
// Initial revision
//
 */

static char rcsid []  = "@(#)$Id: inittest.cc,v 1.6 1997/03/05 15:51:25 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/bst.h"
#include "../include/list.h"
#include "../include/initfile.h"

int main(int argc, char *argv[])
{

	if (argc!=2)
	{
		cerr << "Usage: inittest file.ini" << endl;
		return -1;
	}

	MTCinitFile initFile(argv[1]);	// used to input parameters

	if (initFile.firstSection()==0)
	{
		do
		{
			cout << '[' << initFile.getSection() << ']' << endl;

			if (initFile.firstKey()==0)
			{
				do
				{
					cout << initFile.getKey() << '=' << initFile.getValue() << endl;
				} while (initFile.nextKey()==0);
			}

		} while (initFile.nextSection()==0);
	}

	return 0;
}
