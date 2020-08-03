/*
 * Abstract Bit Stream class
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: bitstrm.cc,v 1.4 1997/03/05 17:49:59 markc Exp $
 *
 * $Log: bitstrm.cc,v $
 * Revision 1.4  1997/03/05 17:49:59  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1997/02/22 21:34:33  markc
 * Added BITSTRM_SIZE_MASK
 *
 * Revision 1.2  1997/02/16 06:58:59  markc
 * BST replaced with RBT
 *
 * Revision 1.1  1995/11/05 00:05:17  chapman
 * Initial revision
 *
 *
 */

static char rcsid []  = "@(#)$Id: bitstrm.cc,v 1.4 1997/03/05 17:49:59 markc Exp $";

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


// constructor

MTCbitStream::MTCbitStream() : 
	streamBitsLeft(0), buffer(0), bufferSize(0), 
	file(NULL), sigDigBitBucketMask(1)
{ 
	// shift the 0x1 left to most sig dig

	sigDigBitBucketMask = sigDigBitBucketMask << ((sizeof(bitBucketType)*8)-1); 
};

// destructor

MTCbitStream::~MTCbitStream()
{
	closeFile();
}

// set the buffer size

void MTCbitStream::setBufferSize(bitCountType aCount)		
{ 	
	bufferSize=aCount; 

	if (bufferSize>(sizeof(bitBucketType)*8))
	{
		bufferSize=sizeof(bitBucketType)*8;
		errorMsg(EMT_ERROR, "buffer size overflow error -- this should never happend -- porting needed (check bitCountType)", "MTCbitStream::setBufferSize()");
	}
}

// decrement the value of an attribute

void MTCbitStream::decStreamBitsLeft(bitCountType delta) 
{ 	
	if (streamBitsLeft>=delta)
	{
		streamBitsLeft-=delta; 
	}
	else 	
	{
		streamBitsLeft=0;
		errorMsg(EMT_WARNING, "I tried to decStreamBitsLeft() more than I could.  I will set streamBitsLeft to 0.", "MTCbitStream::decStreamBitsLeft()");
	}
}

// decrement buffer size

void MTCbitStream::decBufferSize(bitCountType delta)
{
	if (bufferSize>=delta)
	{
		bufferSize-=delta; 
	}
	else 	
	{
		bufferSize=0;
		errorMsg(EMT_WARNING, "I tried to decBufferSize() more than I could.  I will set bufferSize to 0.", "MTCbitStream::decBufferSize()");
	}
}

// increment buffer size

void MTCbitStream::incBufferSize(bitCountType delta)
{
	bufferSize+=delta;	

	if (bufferSize>(sizeof(bitBucketType)*8))
	{
		bufferSize=(sizeof(bitBucketType)*8);
		errorMsg(EMT_WARNING, "I tried to incBufferSize() more than I could.  I will set bufferSize to maximum.", "MTCbitStream::incBufferSize()");
	}
}

