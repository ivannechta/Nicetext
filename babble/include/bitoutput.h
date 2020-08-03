
/*
 *
 * "Output" abstract bitstream class
 * Copyright (c) 1995, Mark T. Chapman
 *
 * The main purpose is the writeBits(howmany) method.
 *
 * The idea is to first write bits to a stream for a set length 
 * after that, ignore the writeBits() request
 *
 * $Id: bitoutput.h,v 1.3 1997/03/05 17:44:52 markc Exp $
 *
 * $Log: bitoutput.h,v $
 * Revision 1.3  1997/03/05 17:44:52  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.2  1996/01/13 14:19:25  markc
 * added MTCstring & ctor
 *
 * Revision 1.1  1995/11/05  00:06:09  chapman
 * Initial revision
 *
 */

#ifndef __BITOUTPUT_H_
#define __BITOUTPUT_H_

static char rcsid__BITOUTPUT_H_ []  = "@(#)$Id: bitoutput.h,v 1.3 1997/03/05 17:44:52 markc Exp $";

// concrete class for output operations

class MTCoutputBitStream : public MTCbitStream {
private:
protected:
	virtual void openFile(const char *aFileName); // open a file
	void saveBuffer();	// save buffer to the file or ignore
	BOOL streamBitsDefined;	// have we defined streamBits?
public:
	// constructors and destructors
	MTCoutputBitStream(const char *aFileName);
	MTCoutputBitStream(const MTCstring &aFileName);
	virtual ~MTCoutputBitStream();	// virtual destructor

	// read a number bits from the stream or from the generator
	BOOL writeBits(bitBucketType bitBucket, bitCountType aCount);
};

#endif  __BITOUTPUT_H_
