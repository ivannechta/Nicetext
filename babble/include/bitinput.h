
/*
 *
 * "Infinite Input" abstract bitstream class
 * Copyright (c) 1995, Mark T. Chapman
 *
 * The main purpose is the readBits(howmany) method.
 *
 * The idea is to first read from a stream for a set length 
 * afterwards generate "random" junk to be read from the "stream"
 *
 * $Id: bitinput.h,v 1.3 1997/03/05 17:44:52 markc Exp $
 *
 * $Log: bitinput.h,v $
 * Revision 1.3  1997/03/05 17:44:52  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.2  1996/01/12 23:51:25  markc
 * dynamic grammar generated models work -- next step punctuation?
 *
 * Revision 1.1  1995/11/05  00:06:09  chapman
 * Initial revision
 *
 */

#ifndef __BITINPUT_H_
#define __BITINPUT_H_

static char rcsid__BITINTPUT_H_ []  = "@(#)$Id: bitinput.h,v 1.3 1997/03/05 17:44:52 markc Exp $";

// concrete class for input operations

class MTCinputBitStream : public MTCbitStream {
private:
protected:
	virtual void openFile(const char *aFileName); // open a file
	void loadBuffer();	// load buffer from file or random source 
public:
	// constructors and destructors
	MTCinputBitStream(const char *aFileName);
	MTCinputBitStream(const MTCstring &aFileName);
	virtual ~MTCinputBitStream();	// virtual destructor

	// read a number bits from the stream or from the generator
	bitBucketType readBits(bitCountType aCount);

};

#endif  __BITINPUT_H_
