
/*
 * Optimally Load a Sorted RAOF into a RBT
 * Copyright (c) Mark T. Chapman 1995
 *
 * Note: this class template should really be a function template
 *
 * $Id: raof2rbt.h,v 1.4 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: raof2rbt.h,v $
 * Revision 1.4  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1997/03/02 03:30:08  markc
 * Added return FALSE; if the counts are not okay
 *
 * Revision 1.2  1997/02/16 06:03:30  markc
 * fixed redundant load()'s
 *
 * Revision 1.1  1996/01/20 17:13:24  markc
 * Initial revision
 *
 * Revision 1.3  1996/01/14  19:38:56  markc
 * relaxed const
 *
 * Revision 1.2  1995/11/12  18:17:05  chapman
 * Removed verbose output
 * Fixed (.,->) problems.
 *
 * Revision 1.1  1995/11/12  18:02:54  chapman
 * Initial revision
 *
 *
 */

#ifndef  __RAOF2RBT_H_
#define  __RAOF2RBT_H_

static char rcsid__RAOF2RBT_H_ []  = "@(#)$Id: raof2rbt.h,v 1.4 1997/03/05 15:58:28 markc Exp $";

// class template to load a sorted raof into a rbt

template <class T>
class MTCraofToRBT {
private:
	MTCreadRAOF <T> * raof;
	MTCRBT <T> 	* rbt;
	void load(long low, long high)
	{
		for(long i=low; i<=high; i++)
		{
			rbt->add(*raof->read(i));
		}
	}
protected:
public:
	BOOL doIt(MTCreadRAOF <T> &aRAOF, MTCRBT <T> &aRBT)
		{
			raof=&aRAOF;
			rbt=&aRBT;
			load(0,raof->getCount()-1);

			if (raof->getCount()!=rbt->getCount())
			{
				errorMsg(EMT_ERROR, "I do not have as many nodes in the RBT as were in the raof.", "MTCraofToRBT::doIt()"); 
				return FALSE;

			}
			
			return TRUE;
		}

};

#endif __ROAFALT_H_
