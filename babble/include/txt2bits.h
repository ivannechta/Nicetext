/*
 * Text to Bitstream Engine Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * "The engine for the unbabble process"
 *
 * $Id: txt2bits.h,v 1.5 1997/03/05 17:44:52 markc Exp $
 *
 * $Log: txt2bits.h,v $
 * Revision 1.5  1997/03/05 17:44:52  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.4  1997/03/03 19:46:48  markc
 * Added -v for verbose error reporting
 *
 * Revision 1.3  1997/02/27 15:56:45  markc
 * Added -s Small Mode
 *
 * Revision 1.2  1997/02/16 06:58:07  markc
 * BST replaced with RBT
 *
 * Revision 1.1  1996/01/13 15:14:40  markc
 * Initial revision
 *
 *
 */

#ifndef __TXT2BITS_H_
#define __TXT2BITS_H_

static char rcsid__TXT2BITS_H_ []  = "@(#)$Id: txt2bits.h,v 1.5 1997/03/05 17:44:52 markc Exp $";

// forward reference
class MTCtextToBits;

// concrete class for conversion objects

class MTCtextToBits
{
private:
	MTCstring dictName;
	MTCstring typeName;
	MTCstring outputName;
	MTCstring inputName;
	MTCRBT <MTCdictRecord> dictRecRBT;
	MTCRBT <MTCtypeAltRecord> typeAltRecRBT;
	MTCreadRAOF <MTCdictRecord> *dictRecRAOF;
	MTCoutputBitStream *outputBitStream;
	FILE *inputStream;
	BOOL okay;
	BOOL smallMode;
	BOOL verboseMode;

protected:
	void setOkay(BOOL aBOOL) { okay=(aBOOL!=FALSE); };
	void setSmallMode(BOOL aBOOL) { smallMode=aBOOL; };

public:
	// constructors and destructors
	MTCtextToBits();
	~MTCtextToBits();

	// open data structures methods
	BOOL openDictName(const MTCstring &, BOOL useSmallMode=FALSE);
	BOOL openTypeName(const MTCstring &);
	BOOL openOutputName(const MTCstring &);
	BOOL openInputName(const MTCstring &);

	// attribute access methods
	BOOL getOkay() const 			{ return okay; };
	const MTCstring & getDictName() const	{ return dictName; };
	const MTCstring & getTypeName() const	{ return typeName; };
	const MTCstring & getOutputName() const	{ return outputName; };
	const MTCstring & getInputName() const	{ return inputName; };
	BOOL getSmallMode() const		{ return smallMode; };
	void setVerboseMode(BOOL aBOOL)		{ verboseMode=aBOOL; };
	BOOL getVerboseMode() const		{ return verboseMode; };

	// do it
	BOOL isReady(); 			// am I ready to doIt?
	BOOL doIt(BOOL checkIfReady=TRUE);	// babble away!
};

#endif // __TXT2BITS_H_
