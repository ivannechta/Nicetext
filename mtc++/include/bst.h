
/*
 * MTC++ Binary Search Tree Template
 * Copyright (c) Mark T. Chapman 1995
 *
 * class MTCBST -- Binary Search Tree -- allows one instance of a nodekey 
 * class MTCBSTFreq - BST ++ a freq count of nodekey
 *
 * note: forEach() works with BST and BSTFreq
 *       forEachFreq() adds the count as an extra parm to function ptr
 *
 * $Id: bst.h,v 1.11 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: bst.h,v $
 * Revision 1.11  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.10  1996/01/14 19:23:54  markc
 * removed all const & -- it is not very important to have const here,
 * it did not prevent manipulation via find() -- which should not be done!
 *
 * Revision 1.9  1995/11/12  22:04:48  chapman
 * Added getCurrent()
 * Optimized "add" and "find" to see if the current node is the search value
 *
 * Revision 1.8  1995/10/12  15:53:44  markc
 * Added getLowest() method
 *
 * Revision 1.7  1995/08/31  02:57:55  markc
 * Added BOOL overwrite to BST::add().
 *
 * Revision 1.6  1995/07/29  16:45:14  markc
 * Added forEachFreq for MTCBSTFreq -- this allows a function to get passed the
 * frequency of each node.
 *
 * Revision 1.5  1995/07/26  13:45:45  markc
 * Added #ifdef DEBUG
 *
 * Revision 1.4  1995/07/06  03:31:18  markc
 * Check In
 *
 * Revision 1.3  1995/06/30  19:06:53  markc
 * Added forEach() method
 *
 * Revision 1.2  1995/06/30  18:41:07  markc
 * Added Frequency Count Inheritance
 *
 * Revision 1.1  1995/06/07  01:49:27  markc
 * Initial revision
 *
 */

#ifndef  __BST_H_
#define  __BST_H_

static char rcsid__BST_H_ []  = "@(#)$Id: bst.h,v 1.11 1997/03/05 15:58:28 markc Exp $";

// binary search tree node template

template <class T> 
class MTCBSTNode {
public:
	T value;		// the value of this BSTnode	
        MTCBSTNode <T> * left;  // BST < value
	MTCBSTNode <T> * right;	// BST > value
public:
	MTCBSTNode() { left=right=NULL; };
        MTCBSTNode(T & aT) { left=right=NULL; value=aT; }; 
	virtual ~MTCBSTNode() 
	   { if (left) delete left; if (right) delete right; left=right=NULL; };
};

// binary search tree with frequency count node template

template <class T>
class MTCBSTNodeFreq : public MTCBSTNode<T> {	       
public:
        long count;             // the magnitude of this BSTnode
public:
	MTCBSTNodeFreq() { count=1; };
	MTCBSTNodeFreq(T & aT) : MTCBSTNode <T> (aT) { count=1; };
	virtual ~MTCBSTNodeFreq() {}; 
};

// binary search tree template

template <class T> 
class MTCBST  {
protected:
	MTCBSTNode <T> * root;
	MTCBSTNode <T> * current;
	long count;
	virtual MTCBSTNode <T> * createNewNode(T &); 
	void forEachNode(MTCBSTNode <T> *, void (*doIt)(T *));
	
public:
	MTCBST () { root=current=NULL; count=0; };
	virtual ~MTCBST() { if (root) delete root; root=NULL; };
	virtual int add(T &, BOOL overwrite=FALSE);
	int remove(T &);
	T * find(T &); 
	T * getLowest();
	T * getCurrent() { if (current!=NULL) return &current->value; else return NULL; };
	long getCount() const { return count; };
	void clear() { if (root) delete root; root=NULL; count=0; };;
	void forEach(void (*doIt)(T *));
#ifdef DEBUG
	virtual void dump();
	virtual void dumpNode(MTCBSTNode <T> * );
#endif // DEBUG
};

// binary search tree with freq count node template
 
template <class T>
class MTCBSTFreq : public MTCBST <T> {
  protected:
	virtual MTCBSTNode <T> * createNewNode(T &); 
	void forEachNodeFreq(MTCBSTNodeFreq <T> *, void (*doIt)(T *, long));

  public:
        virtual int add(T &, BOOL overwrite=FALSE);
	void forEachFreq(void (*doIt)(T *, long));
#ifdef DEBUG
	virtual void dump();
	virtual void dumpNode(MTCBSTNode <T> * );
#endif // DEBUG
};

// create a new node

template <class T>
inline MTCBSTNode <T> * MTCBST <T>::createNewNode(T &aT)
{
	MTCBSTNode <T> * newNode = new MTCBSTNode <T> (aT);

	if (newNode==NULL)
	{
		perror("MTCBST::createNewNode()");
		abort();
	}

	count++;

	return newNode;
}

// create a new FREQ node

template <class T>
inline MTCBSTNode <T> * MTCBSTFreq <T>:: createNewNode(T &aT)
{
	MTCBSTNode <T> * newNode = new MTCBSTNodeFreq <T> (aT);

	if (newNode==NULL)
	{
		perror("MTCBSTFreq::createNewNode()");
		abort();
	}

	this->count++;


	return newNode;
}


// add a node to the BST

template <class T> 
inline int MTCBST <T>::add(T &aT, BOOL overwrite)
{
	// is this the first node?

	if (root==NULL)
	{
		count=0;

		root=createNewNode(aT);

		return 0;
	}

	// is it already the current node?

	if ((current!=NULL)&&(aT==current->value))
	{
		if (overwrite==TRUE)
		{
			current->value=aT;
		}
		return 1;
	}

	// otherwise, dive deeper!

	current=root;

	for(;;) 
	{
		if (aT==current->value)
		{
			if (overwrite==TRUE)
			{
				current->value=aT;
			}
			return 1;
		}

		if (aT<current->value)
		{
			if (current->left==NULL)
			{
				current->left=createNewNode(aT);
				return 0;
			}
			else
			{
				current=current->left;
			}
		}
		else
		{
			if (current->right==NULL)
			{
				current->right=createNewNode(aT);
				return 0;
			}
			else
			{
				current=current->right;
			}
		}
	}
}

// add a node to the FREQ BST

template <class T>
inline int MTCBSTFreq <T>::add(T &aT, BOOL overwrite)
{
    int retval;

    if ((retval=MTCBST <T>::add(aT, overwrite))==1)
    {		
	((MTCBSTNodeFreq <T>*)this->current)->count++;
    }

    return retval;
}

// remove aNode from the BST

template <class T>
inline int MTCBST <T>::remove(T &aT)
{
	// is the tree empty?

	if (root==NULL)
	{
		return -1;
	}

	// if not, find the node with the value 

	current=root;
	MTCBSTNode <T> **parentBranch=NULL;
	MTCBSTNode <T> *tempPtr=NULL;

	while (current!=NULL)
	{
		if (aT==current->value)
		{
			break;
		}

		if (aT<current->value)
		{
			parentBranch=&current->left;
			current=current->left;
		}
		else
		{
			parentBranch=&current->right;
			current=current->right;
		}

	}

	// if we didn't find it, we can't delete it!

	if (current==NULL)
	{
		return -1;
	}

	// if we found it, then we will delete it!

	if ((current->left==NULL)&&(current->right==NULL))
	{
		if (parentBranch==NULL)
		{
			delete current;
			root=NULL;
		}
		else
		{
			delete current;
			*parentBranch=NULL;
		}
	}
	else if (current->left==NULL)
	{
		if (parentBranch==NULL)
		{
			tempPtr=root;
			root=current->right;
			delete tempPtr;
		}
		else
		{
			*parentBranch=current->right;
			delete current;
		}
	}
	else if (current->right==NULL)
	{
		if (parentBranch==NULL)
		{
			tempPtr=root;
			root=current->left;
			delete tempPtr;
		}
		else
		{
			*parentBranch=current->left;
			delete current;
		}
	}
	else
	{
		cout << "did not delete!" << endl;
		count++;	
	}

	current=NULL;
	count--;

	return 0;
}

// find aNode in the BST 

template <class T>
inline T * MTCBST <T>::find(T &aT) 
{
	// is it the current node?

	if ((current!=NULL)&&(current->value==aT))
	{
		return &current->value;
	}

	// otherwise dive deeper

	current=root;

	while (current!=NULL)
	{
		if (aT==current->value)
			break;

		if (aT<current->value)
		{
			current=current->left;
		}
		else
		{
			current=current->right;
		}
	}

	if (current==NULL)
	{
		return NULL;
	}

	return &current->value;
}

#ifdef DEBUG
// dump the values in the tree to stdout

template <class T>  
inline void MTCBST <T>::dump()
{
	dumpNode(root);
}

template <class T> 
inline void MTCBST <T>::dumpNode(MTCBSTNode <T> *aNodePtr)
{
	if (aNodePtr==NULL)
		return;

	dumpNode(aNodePtr->left);

	cout << aNodePtr->value << endl;

//	cout << aNodePtr->count << '\t' << aNodePtr->value << endl;

	dumpNode(aNodePtr->right);
}

// dump the values in the tree to stdout

template <class T>  
inline void MTCBSTFreq <T>::dump()
{
	dumpNode(root);
}

template <class T> 
inline void MTCBSTFreq <T>::dumpNode(MTCBSTNode <T> *aNodePtr)
{
	if (aNodePtr==NULL)
		return;

	dumpNode(aNodePtr->left);

	cout << ((MTCBSTNodeFreq<T> *)aNodePtr)->count << '\t' << aNodePtr->value << endl;

	dumpNode(aNodePtr->right);
}

#endif // DEBUG

// get the lowest value node in the BST -- kinda like a heap...

template <class T>
inline T * MTCBST <T>::getLowest() 
{
	MTCBSTNode<T> *tempNodePtr=root;
	T * answer=NULL;

	while (tempNodePtr!=NULL)
	{
		current=tempNodePtr;
		answer=&current->value;
		tempNodePtr=tempNodePtr->left;
	}

	return answer;
}

// do something to each of the values in the tree in order 

template <class T>
inline void MTCBST <T>::forEach(void (*doIt)(T *))
{
        forEachNode(root, doIt);
}

template <class T>
inline void MTCBST <T>::forEachNode(MTCBSTNode <T> *aNodePtr, void (*doIt)(T *))
{
        if (aNodePtr==NULL)
                return;

        forEachNode(aNodePtr->left, doIt);

	doIt(&aNodePtr->value);

        forEachNode(aNodePtr->right, doIt);
}

// do something to each of the values in the tree in order 

template <class T>
inline void MTCBSTFreq <T>::forEachFreq(void (*doIt)(T *, long))
{
        forEachNodeFreq((MTCBSTNodeFreq <T>*)this->root, doIt);
}

template <class T>
inline void MTCBSTFreq <T>::forEachNodeFreq(MTCBSTNodeFreq <T> *aNodePtr, void (*doIt)(T *, long))
{
        if (aNodePtr==NULL)
                return;

        forEachNodeFreq((MTCBSTNodeFreq <T> *)aNodePtr->left, doIt);

	doIt(&aNodePtr->value, aNodePtr->count);

        forEachNodeFreq((MTCBSTNodeFreq <T> *)aNodePtr->right, doIt);
}

#endif __BST_H_
