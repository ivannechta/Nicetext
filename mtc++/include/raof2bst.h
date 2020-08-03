
/*
 * Optimally Load a Sorted RAOF into a BST
 * Copyright (c) Mark T. Chapman 1995
 *
 * Note: this class template should really be a function template
 *
 * $Id: raof2bst.h,v 1.5 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: raof2bst.h,v $
 * Revision 1.5  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.4  1997/03/02 03:30:08  markc
 * Added return FALSE; if the counts are not okay
 *
 * Revision 1.3  1996/01/14 19:38:56  markc
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

#ifndef  __RAOF2BST_H_
#define  __RAOF2BST_H_

static char rcsid__RAOF2BST_H_ []  = "@(#)$Id: raof2bst.h,v 1.5 1997/03/05 15:58:28 markc Exp $";

// class template to load a sorted raof into a bst

template <class T>
class MTCraofToBST {
private:
	MTCreadRAOF <T> * raof;
	MTCBST <T> 	* bst;
	void load(long low, long high)
	{
		if (high<low)
		{
			return;
		}

		long middle=(low+high)/2;

		// the following is done to work around const :(
		bst->add(*raof->read(middle));

		load(low, middle-1);
		load(middle+1, high);
	}
protected:
public:
	BOOL doIt(MTCreadRAOF <T> &aRAOF, MTCBST <T> &aBST)
		{
			raof=&aRAOF;
			bst=&aBST;
			load(0,raof->getCount()-1);

			if (raof->getCount()!=bst->getCount())
			{
				errorMsg(EMT_ERROR, "I do not have as many nodes in the BST as were in the raof.", "MTCraofToBST::doIt()"); 
				return FALSE;

			}
			
			return TRUE;
		}

};

#endif __ROAFALT_H_
