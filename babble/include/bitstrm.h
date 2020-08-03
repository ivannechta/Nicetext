/*
 *
 * "Infinite" abstract bitstream class
 *
 * The idea is to first read/write from/to a stream for a while and then
 * either generate "random" junk for reads or ignore data for writes.
 *
 * Copyright (c) 1995, Mark T. Chapman
 *
 * $Id: bitstrm.h,v 1.4 1997/03/05 17:44:52 markc Exp $
 *
 * $Log: bitstrm.h,v $
 * Revision 1.4  1997/03/05 17:44:52  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1997/02/24 04:28:29  markc
 *  Added bit-input and output statistics
 *
 * Revision 1.2  1997/02/22 21:34:56  markc
 * Added BITSTRM_SIZE_MASK
 *
 * Revision 1.1  1995/11/05 00:06:09  chapman
 * Initial revision
 *
 */

#ifndef __BITSTRM_H_
#define __BITSTRM_H_

static char rcsid__BITSTRM_H_ []  = "@(#)$Id: bitstrm.h,v 1.4 1997/03/05 17:44:52 markc Exp $";

#define BITSTRM_SIZE_MASK 0xA4E376BF  	// used to help "mix-up" the first words in each file

typedef unsigned long bitBucketType;
typedef unsigned long bitCountType;

// abstract class for MTCbitStream operations 

class MTCbitStream {
private:
	bitCountType streamBitsLeft;	// how many more bits to/from stream?
	bitCountType bufferSize;	// how many bits in buffer are data?
protected:
	fstream *file;				// the file
	bitBucketType buffer;			// current buffer
	bitBucketType sigDigBitBucketMask;  	// most significant dig mask

	// open the file
	virtual void openFile(const char *)=0;	// open the file
	virtual void closeFile() { if (file!=NULL) { delete file; file=NULL; }};

	// set attribute methods
	void setStreamBitsLeft(bitCountType aCount) 	{ streamBitsLeft=aCount; };
	void setBuffer(bitBucketType aBucket) 		{ buffer=aBucket; };
	void setBufferSize(bitCountType aCount);		
	
	// decrement the value of an attribute
	void decStreamBitsLeft(bitCountType delta=1); 
	void decBufferSize(bitCountType delta=1);
	void incBufferSize(bitCountType delta=1);


public:
	// constructors and destructors
	MTCbitStream();
	virtual ~MTCbitStream();

	// get attribute methods
	bitCountType getStreamBitsLeft() const { return streamBitsLeft; };
	bitCountType getBufferSize() const { return bufferSize; };
};

#endif  __BITSTRM_H_
