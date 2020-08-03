/*
 * BitInput class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: bitinput.cc,v 1.6 1997/03/05 17:49:59 markc Exp $
 *
 * $Log: bitinput.cc,v $
 * Revision 1.6  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.5  1997/02/22 21:34:33  markc
 * Added BITSTRM_SIZE_MASK
 *
 * Revision 1.4  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.3  1996/01/12 23:48:19  markc
 * dynamic grammars work system wide -- next step, constant expressions
 *
// Revision 1.2  1995/11/13  07:11:34  markc
// Added random() call for data generation beyond end-of-stream
//
// Revision 1.1  1995/11/05  00:04:40  chapman
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: bitinput.cc,v 1.6 1997/03/05 17:49:59 markc Exp $";

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
#include "../include/bitinput.h"

// constructor

MTCinputBitStream::MTCinputBitStream(const char *aFileName) 
{
	openFile(aFileName);

	return;
}

// alt constructor

MTCinputBitStream::MTCinputBitStream(const MTCstring &aFileName)
{
	openFile(aFileName.c_str());

	return;
}

// destructor

MTCinputBitStream::~MTCinputBitStream()
{
	return;
}

// open the file

void MTCinputBitStream::openFile(const char *aFileName)
{
	// to start with we must close any open file

	closeFile();

	// stat the file to get the length and then open the file

	struct stat tempStat;

	if ((stat(aFileName, &tempStat)==0)&&((file=new fstream(aFileName, ios::in))!=NULL))
	{
		// add the length of the file as the first buffer 

		setStreamBitsLeft(tempStat.st_size*8);

		setBuffer((getStreamBitsLeft()^BITSTRM_SIZE_MASK));
#ifdef DEBUG
		printf("inputBitStream: %x\t xor'd with mask: %x\n",getStreamBitsLeft(), buffer);
#endif // DEBUG
		setBufferSize(sizeof(bitBucketType)*8);

	 	// check this machine to see of everything looks okay

		if ((tempStat.st_size*8)>((off_t)getStreamBitsLeft()))
		{
			errorMsg(EMT_WARNING, "I will not read the whole input file because the size of the stat.st_size does not fit in a bitBucketType.  Try splitting this large file into several smaller files.", "MTCinputBitStream::openFile()"); 
		}
	}
	else
	{
		MTCstring comment(strerror(errno));

		comment+=aFileName;

		errorMsg(EMT_WARNING, comment.c_str(), "MTCinputBitStream::openFile()");

		setStreamBitsLeft(0);
		setBufferSize(0);
	}

}

// read a number bits from the stream or from the generator

bitBucketType MTCinputBitStream::readBits(bitCountType aCount)
{
	// validate parameters

	if (aCount>(sizeof(bitBucketType)*8))
	{
		char message[300];

		sprintf(message, "readBits() overflow.  The calling function requested %d bits.  The bitBucketType can only hold %d bits.  I will only read %d bits from the stream.", aCount, sizeof(bitBucketType)*8); 

		errorMsg(EMT_WARNING, message, "MTCinputBitStream::readBits()");

		aCount=sizeof(bitBucketType)*8;
	}

	// fill a new bit bucket with aCount bits from the stream

	bitBucketType bucket(0);

#ifdef DEBUG
	cout << endl;
#endif DEBUG

	for(; aCount>0; aCount--)
	{
		bucket = bucket << 1;

		if (getBufferSize()==0)
		{
			loadBuffer();
		}

		bucket|=((buffer&sigDigBitBucketMask)!=0);

#ifdef DEBUG
		cout << ((buffer&sigDigBitBucketMask)!=0) << " ";
#endif DEBUG
		buffer = buffer << 1;

		decBufferSize();
	}

	return (bucket);
}

// load the buffer either from the file or from a random generator

void MTCinputBitStream::loadBuffer()
{
	// should we load the buffer from the stream?

	if (getStreamBitsLeft()>0)
	{
		unsigned char c;

		decStreamBitsLeft(sizeof(c)*8);

		if ((file==NULL)||(file->eof()))
		{
			errorMsg(EMT_ERROR, "I could not read from the input file, although there should be some stream bits left.  I will just switch to the non-read mode automatically now.  Keep in mind that the whole file might not have been read!", "MTCinputBitStream::loadBuffer()"); 

			setStreamBitsLeft(0); 
		} 
		else
		{
			file->read((char *)&c, 1);
			buffer=c;
			buffer=buffer << ((sizeof(bitBucketType)-sizeof(c))*8);

			setBufferSize(sizeof(c)*8);

			return;
		}
	}

	// other wise just load some junk into the buffer area

	buffer=random();

	setBufferSize(sizeof(buffer)*8);

	return; 
}
