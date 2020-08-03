
/*
 * MTC++ Create Alternate Jumpfiles for Random Access Objects in Files Template
 * Copyright (c) Mark T. Chapman 1995
 *
 ********************************************************************
 * (see raof.h)
 *
 * Now that you have a MTCreadRAOF<T> how do you create alternate "keys" for
 * the find() method?
 *
 * 1.  Somehow you need to define a class Y that has the operator<() and 
 *     operator==() overloaded to "sort" the T objects in a different order.
 *     Note that Y must use a T compatible stream extractor (at least for the
 *     new key fields).
 *    
 *     Suggestions...
 *     A. define class Y : public T {}
 *     B. Or manually create a minimal class Y with a T compatible extractor
 *
 * 2.  Remember -- these keys MUST BE UNIQUE.  One easy way is to use the
 *     primary key as a tie-breaker.  
 *
 ********************************************************************
 *
 * class MTCcreateAltJmpRAOF -- used to create an alternat .jmp for a readRAOF
 * class MTCAltJmpNode - used within a MTCcreateAltJmpRAOF to sort
 *
 ********************************************************************
 * $Id: raofalt.h,v 1.6 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: raofalt.h,v $
 * Revision 1.6  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.5  1996/04/10 20:39:44  markc
 * fixed sizeof() for the raofJumpTableType to be unsigned int.
 * this allows different machines to use the same .jmp and .alt files
 *
 * Revision 1.4  1996/01/20 17:20:02  markc
 * updated to use rbt instead of bst
 *
 * Revision 1.3  1995/11/10  21:09:43  chapman
 * Added load() method for doIt() so that
 * it can better handle partially sorted raof.
 *
 * Revision 1.2  1995/11/08  18:03:04  chapman
 * Now it works!
 *
 * Revision 1.1  1995/11/08  17:25:54  chapman
 * Initial revision
 *
 *
 */

#ifndef  __RAOFALT_H_
#define  __RAOFALT_H_

static char rcsid__RAOFALT_H_ []  = "@(#)$Id: raofalt.h,v 1.6 1997/03/05 15:58:28 markc Exp $";

// class to sort the indexes in a MTCreadRAOF in a MTCcreateAltJmpRAOF

template <class T>
class MTCaltJmpNode {
private:
	T value;
	raofJumpTableType jumpPos;
protected:
public:
	// default constructor
	MTCaltJmpNode()
		: jumpPos(0) {};

	// copy constructor
	MTCaltJmpNode(MTCaltJmpNode &aSrc)
		: value(aSrc.value), jumpPos(aSrc.jumpPos) {};

	// parameterized constructor
	MTCaltJmpNode(T aT, raofJumpTableType aPos)
		: value(aT), jumpPos(aPos) {};

	// set attribute
	void setValue(const T &aT) { value=aT; };
	void setJumpPos(raofJumpTableType aPos) { jumpPos=aPos; };

	// get attribute
	const T & getValue() const { return value; };
	raofJumpTableType getJumpPos() const { return jumpPos; };

	// overloaded assignment operator
	MTCaltJmpNode & operator=(const MTCaltJmpNode <T> &aSrc)
		{ value=aSrc.getValue(); jumpPos=aSrc.getJumpPos(); };

	// minimal set of comparison operators
	BOOL operator<(const MTCaltJmpNode <T> &aSrc) const
		{ return getValue()<aSrc.getValue(); };

	BOOL operator==(const MTCaltJmpNode <T> &aSrc) const
		{ return getValue()==aSrc.getValue(); };
};


// class to create alternate jump file for a RAOF
// WARNING -- DO NOT MAKE TWO CONCURRENT INSTANCES OF THE FOLLOWING TEMPLATE 
// GLOBAL DATA IS USED BY THE STATIC DUMP METHOD
// (dump must be static to work with the rbt.forEach() method)

ofstream *altJmpFile=NULL;	// fix this some day...

template <class T>
class MTCcreateAltJmpRAOF {
private:
	MTCreadRAOF <T> readRAOF;
	MTCaltJmpNode <T> tempJmpNode; 
	MTCRBT <MTCaltJmpNode <T> > rbt;
protected:
public:
	// constructor
	MTCcreateAltJmpRAOF(const MTCstring &prefix)
		: readRAOF(prefix)
		{
			MTCstring temp(prefix);
			temp+=".alt";

			if ((altJmpFile=new ofstream(temp.c_str()))==NULL) 
			{
				cerr << "could not open output file" << temp << endl;
			}
		};

	MTCcreateAltJmpRAOF(const MTCstring &datName, 
		const MTCstring &jmpName, const MTCstring &altName) 
		: readRAOF(datName, jmpName)
		{
			if ((altJmpFile=new ofstream(altName.c_str()))==NULL) 
			{
				cerr << "could not open output file" << altName << endl;
			}
		};

	// destructor
	virtual ~MTCcreateAltJmpRAOF()
		{ 
			if (altJmpFile!=NULL)
			{
				delete altJmpFile;
				altJmpFile=NULL;
			}
		}

	// dump an jump index to .alt file

	static void dump(MTCaltJmpNode <T> *anAltJmp)
	{
		if (altJmpFile!=NULL)
		{
			long count=anAltJmp->getJumpPos();

			altJmpFile->write((char *)&count, sizeof(raofJumpTableType));
		}
	}

	// doIt() -- usually called only once...

	BOOL doIt()
		{
			for(long i=0; i<readRAOF.getCount(); i++)
			{
				tempJmpNode.setValue(*readRAOF.read(i));
				tempJmpNode.setJumpPos(readRAOF.getPos());
				rbt.add(tempJmpNode);
			}

			if (rbt.getCount()!=readRAOF.getCount())
			{
				char buffer[400];

				sprintf(buffer, "I had read %d objects from the readRAOF, but I only had %d objects in the rbt.", readRAOF.getCount(), rbt.getCount()); 
				errorMsg(EMT_WARNING, buffer, "MTCcreateAltJmpRAOF::doIt()");
			}
			
			rbt.forEach(dump);
		}

};

#endif __ROAFALT_H_
