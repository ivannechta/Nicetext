/*
 * Sample MTC++ program.
 *
 * Read unordered RAOF and generate unordered.alt -- alternate sorted .jmp file
 *
 * $Id: raofmalt.cc,v 1.3 1997/03/05 15:51:25 markc Exp $
 *
 * $Log: raofmalt.cc,v $
 * Revision 1.3  1997/03/05 15:51:25  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.2  1996/01/20 17:18:10  markc
 * converted to use rbt version of raofalt.h
 *
// Revision 1.1  1995/11/08  18:03:26  chapman
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: raofmalt.cc,v 1.3 1997/03/05 15:51:25 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/rbt.h"
#include "../include/raof.h"
#include "../include/raofalt.h"

int main()
{
	// intro

	cout << "MTC++ raofmalt utility." << endl;
	cout << "Copyright (c) 1995, Mark T. Chapman" << endl;
	cout << endl;

	cout << "Creating alternate sorted jump file unordered.alt..." << endl;

	// create alternate sorted jump file for unordered.dat 

	MTCcreateAltJmpRAOF <MTCstring> createAlt("unordered.dat", "unordered.jmp", "unordered.alt");

	createAlt.doIt();

	cout << "That's all folks!" << endl;

	return 0;
}
