/*
 *
 * Read lines in each type file to create output suitable for sortdct
 *
 * Copyright (c) Mark T. Chapman, 1995
 *
 * $Id: txt2dct.cc,v 1.4 1997/03/05 17:41:20 markc Exp $
 *
 * $Log: txt2dct.cc,v $
 * Revision 1.4  1997/03/05 17:41:20  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1997/03/01 21:34:59  markc
 * Changed usage() messaage
 *
 * Revision 1.2  1996/03/21 17:18:56  markc
 * program name changes and messages (minor)
 *
 * Revision 1.1  1996/01/14 11:29:22  markc
 * Initial revision
 *
 *
 */

static char rcsid []  = "@(#)$Id: txt2dct.cc,v 1.4 1997/03/05 17:41:20 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH	1024

int processFile(const char *);
void usage();

int main(int argc, char *argv[])
{
	int okay=1;

	if (argc==1)
	{
		usage();
		return -1;
	}

	for(int i=1; i<argc; i++)
	{
		if (processFile(argv[i])!=0)
		{
			cerr << "* * * * FILE SKIPPED: " << argv[i] << endl;
			okay=0;
		}
	}

	if (okay==0)
	{
		usage();
	}

	return 0;
}

int processFile(const char *fileName)
{
	ifstream input(fileName);
	char buffer[MAX_LINE_LENGTH+1];

	if ((!input)||(input.bad()))
	{
		perror(fileName);
		return 1;
	}

	do
	{
		input.getline(buffer, MAX_LINE_LENGTH);

		if ((strlen(buffer)!=0)&&(buffer[0]!='#'))
		{
			cout << fileName << " " << buffer << endl;
		}

	} while (!input.eof());

	return 0;
}

void usage()
{
	cerr << "Usage: txt2dct file1 [file2] [file3] [...]" << endl;
	cerr << endl;
}
