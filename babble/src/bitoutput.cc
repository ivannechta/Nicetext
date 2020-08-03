/*
 * BitInput class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: bitoutput.cc,v 1.6 1997/03/05 17:49:59 markc Exp $
 *
 * $Log: bitoutput.cc,v $
 * Revision 1.6  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.5  1997/02/22 21:34:33  markc
 * Added BITSTRM_SIZE_MASK
 *
 * Revision 1.4  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.3  1996/01/13 14:19:39  markc
 *  added MTCstring & ctor
 *
// Revision 1.2  1995/11/05  01:10:30  chapman
// removed cout << "stream bitsize...
//
// Revision 1.1  1995/11/05  00:05:10  chapman
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: bitoutput.cc,v 1.6 1997/03/05 17:49:59 markc Exp $";

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/raof.h"
#include "../include/bitstrm.h"
#include "../include/bitoutput.h"

// constructor

MTCoutputBitStream::MTCoutputBitStream(const char *aFileName) 
	: streamBitsDefined(FALSE)
{
	openFile(aFileName);

	return;
}

// constructor

MTCoutputBitStream::MTCoutputBitStream(const MTCstring &aFileName)
	: streamBitsDefined(FALSE)
{
	openFile(aFileName.c_str());

	return;
}

// destructor

MTCoutputBitStream::~MTCoutputBitStream()
{
	saveBuffer();

	return;
}

// open the file

void MTCoutputBitStream::openFile(const char *aFileName)
{
	// close any open file

	closeFile();

	// open the file

	if ((file=new fstream(aFileName, ios::out))==NULL)
	{
		MTCstring comment(strerror(errno));

		comment+=aFileName;

		errorMsg(EMT_WARNING, comment.c_str(), "MTCoutputBitStream::openFile()");

	}
}

// write a number bits to the stream or eat bits

BOOL MTCoutputBitStream::writeBits(bitBucketType aBucket, bitCountType aCount)
{
	// validate parameters

	if (aCount>(sizeof(bitBucketType)*8))
	{
		char message[300];

		sprintf(message, "writeBits() overflow.  The calling function requested %d bits.  The bitBucketType can only hold %d bits.  I will only write %d bits to the stream.", aCount, sizeof(bitBucketType)*8); 

		errorMsg(EMT_WARNING, message, "MTCoutputBitStream::writeBits()");

		aCount=sizeof(bitBucketType)*8;
	}

	// should we eat the bits?

	if ((streamBitsDefined==TRUE)&&(getStreamBitsLeft()==0))
	{
		return FALSE;
	}

	// we need to shift aBucket to get most sig digits on left 

	aBucket = (aBucket << ((sizeof(bitBucketType)*8)-aCount));

	// fill in bitbucket with aCount bits from the stream

#ifdef DEBUG
	cout << ":";   
#endif DEBUG

	for(; aCount>0; aCount--)
	{
		buffer = buffer << 1;
		buffer |= ((aBucket&sigDigBitBucketMask)!=0);

#ifdef DEBUG
		cout << ((aBucket&sigDigBitBucketMask)!=0) << " ";
#endif DEBUG

		incBufferSize();

		aBucket = aBucket << 1;

		if (streamBitsDefined==TRUE)
		{
			if (getBufferSize()==(sizeof(unsigned char)*8))
			{
				saveBuffer();
			}

			if (getStreamBitsLeft()==0)
			{
				break;
			}
		}
		else
		{
			if (getBufferSize()==(sizeof(bitBucketType)*8))
			{
				saveBuffer();
			}
		}

	}

	return TRUE;
}

// save the buffer 

void MTCoutputBitStream::saveBuffer()
{
	// should we set the streamSize?

	if (streamBitsDefined==FALSE)
	{
		streamBitsDefined=TRUE;
		setStreamBitsLeft(buffer^BITSTRM_SIZE_MASK);
	}
	else
	{
		if (file==NULL)
		{
			errorMsg(EMT_ERROR, "I could not write to the file", "MTCoutputBitStream::saveBuffer()");
		}
		else
		{
			if (getBufferSize()!=0)
			{
				unsigned char c=buffer;

				*file << c;

				decStreamBitsLeft(sizeof(c)*8);
			}
		}
	}

	setBufferSize(0);

	return; 
}
