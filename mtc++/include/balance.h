
/*
 * MTC++ Balanced Search Tree Function Templates
 * Copyright (c) Mark T. Chapman 1995
 *
 * Note: this really belongs as part of bst.h or raof.h -- but I will decide
 *       later where it best fits in the object model.
 *
 * $Id: balance.h,v 1.2 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: balance.h,v $
 * Revision 1.2  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.1  1995/08/24 16:15:52  markc
 * Initial revision
 *
 *
 */

#ifndef  __BALANCE_H_
#define  __BALANCE_H_

static char rcsid__BALANCE_H_ []  = "@(#)$Id: balance.h,v 1.2 1997/03/05 15:58:28 markc Exp $";

// create a balanced MTCBST from a sorted readRAOF

template <class T>
class MTCbalanceBSTfromRAOF {
private:
	MTCBST <T> *bst;
	MTCreadRAOF <T> *raof;
	void readRange(long, long);
public:
	MTCbalancedBSTfromRAOF(MTCBST <T> &aBST, MTCreadRAOF <T> &aRAOF)
		: bst(&aBST), raof(&aRAOF) {}; 
	int doIt();
};

template <class T>
inline int MTCbalanceBSTfromRAOF<T>::doIt()
{
	bst->clear();

	readRange(0, raof->getCount());

	cout << "raof count: " << raof->getCount() << endl;
	cout << " bst count: " << bst->getCount() << endl;

	return 0;
}

template <class T>
inline void MTCbalancedBSTfromRAOF<T>::readRange(long low, long high)
{
	if (low==high)
	{
		bst->add(raof->read(low));
	}
	else
	{
		if (low<high)
		{
			long med=(low+high)/2;
			readElem(low, med);
			readElem(++med, high);
		}
	}
}

#endif __BALANCE_H_
