/*
 * BitTest program -- test the MTCbitStream classe
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: bitcp.cc,v 1.8 1998/08/17 18:34:54 markc Exp $
 *
 * $Log: bitcp.cc,v $
 * Revision 1.8  1998/08/17 18:34:54  markc
 * added return -20 if the compare failed
 *
 * Revision 1.7  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id: bitcp.cc,v 1.8 1998/08/17 18:34:54 markc Exp $";
 *
 * Revision 1.6  1997/02/22 22:17:01  markc
 * Added validation step
 *
 * Revision 1.5  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.4  1996/03/21 17:17:11  markc
 * renamed to bitcp, fixed messages
 *
 * Revision 1.3  1995/11/13 01:07:08  chapman
 * Added command-line file selection
 * Added extra-bits at the end to test output bitstream
 *
// Revision 1.2  1995/11/05  01:09:11  chapman
// clean up
//
// Revision 1.1  1995/11/05  00:05:00  chapman
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: bitcp.cc,v 1.8 1998/08/17 18:34:54 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/raof.h"
#include "../include/bitstrm.h"
#include "../include/bitinput.h"
#include "../include/bitoutput.h"

int main(int argc, char * argv[])
{
	// check parameters

	if (argc!=3)
	{
		cerr << "Usage: " << argv[0] << " inputFile outputFile" << endl;
		return -1;
	}

	// open files

	MTCinputBitStream *input=new MTCinputBitStream(argv[1]);
	MTCoutputBitStream *output=new MTCoutputBitStream(argv[2]);

	if (input==NULL)
	{
		perror(argv[1]);
		return -2;
	}

	if (output==NULL)
	{
		perror(argv[2]);
		return -3;
	}

	// copy input file to output file bit by bit

	bitBucketType c;
	long size;

	cerr << "bitcp program started" << endl;
	cerr << "\tcopying bits from " << argv[1] << " to " << argv[2] << "..." << endl;

	while (input->getStreamBitsLeft()>0)
	{
		size=random()%21;		// how many bits to read?
		c=input->readBits(size);	 	// read them	
		output->writeBits(c, size);	// write them
	}

	// why not dump a few more bits to output? 

	cerr << "\tdumping (ignorable) zeroes into the output bitstream" << endl;

	c=0;

	for(int i=0; i<1000; i++)
	{
		size=random()%21;		// how many bits to "write"?
		output->writeBits(c, size);	// this should do nothing!
	}

	if (input!=NULL)
	{
		delete input;
		input=NULL;
	}

	if (output!=NULL)
	{
		delete output;
		output=NULL;
	}

	sync();

	cerr << "Comparing Files..." << endl;

	ifstream compIn(argv[1]);
	ifstream compOut(argv[2]);

	char inChar;
	char outChar;
	BOOL equal=TRUE;

	do
	{
		compIn >> inChar;
		compOut >> outChar;

		if (inChar!=outChar)
		{
			equal=FALSE;
		}
	} while ((!compIn.eof())&&(!compOut.eof())&&(equal==TRUE));

	if ((equal==FALSE)||(compIn.eof()!=compOut.eof()))
	{
		cerr << "\tWARNING: files are NOT equal -- bitcp FAILED" << endl;
		return -20;
	}
	else
	{
		cerr << "\tFiles ARE equal -- bitcp succeeded" << endl;
	}

	cerr << "That's All Folks!" << endl;

	return 0;
}
