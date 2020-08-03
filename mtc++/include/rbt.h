
/*
 * MTC++ Red Black Tree -- Balance Binary Tree Template
 * Copyright (c) Mark T. Chapman 1995
 *
 * class MTCRBT -- Red Black Tree -- allows one instance of a nodev 
 *
 * $Id: rbt.h,v 1.9 1998/07/13 18:30:19 markc Exp $
 *
 * $Log: rbt.h,v $
 * Revision 1.9  1998/07/13 18:30:19  markc
 * fixed copyright
 *
 * Revision 1.8  1997/03/05 19:50:40  markc
 * Added destructor for MTCRBTFREQ
 *
 * Revision 1.7  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id: rbt.h,v 1.9 1998/07/13 18:30:19 markc Exp $";
 *
 * Revision 1.6  1997/02/16 06:03:43  markc
 * Added MTCRBTFreq() class
 *
 * Revision 1.5  1997/02/16 02:56:17  markc
 * added findClosestSmaller() method
 *
 * Revision 1.4  1996/01/20 14:24:38  markc
 * it seems to work -- used different approach
 *
 * Revision 1.3  1996/01/20  12:47:16  markc
 * skip this version
 *
 * Revision 1.2  1996/01/20  10:56:26  markc
 * not working yet.
 *
 * Revision 1.1  1996/01/14  13:49:53  markc
 * Initial revision
 *
 *
 */

#ifndef  __RBT_H_
#define  __RBT_H_

static char rcsid__RBT_H_ []  = "@(#)$Id: rbt.h,v 1.9 1998/07/13 18:30:19 markc Exp $";

#define RBT_RED		0	
#define RBT_BLACK	1

// red-black binary search tree node template

template <class T> 
class MTCRBTNode {
public: 			// (should be protected)	
	short color;		// is this a red or black node?
	T v;			// the v of this RBTnode	
        MTCRBTNode <T> * l;  	// RBTnode < v
	MTCRBTNode <T> * r;	// RBTnode > v
	MTCRBTNode <T> * p;	// parent
public:
	MTCRBTNode() : l(NULL), r(NULL), p(NULL), color(RBT_BLACK) {};
        MTCRBTNode(T & v) : l(NULL), r(NULL), p(NULL), color(RBT_BLACK), v(v) 
		{};
	virtual ~MTCRBTNode() 
		{ if (l) delete l; if (r) delete r; r=l=NULL; };
};

// red-black binary search tree with frequency count node template

template <class T>
class MTCRBTNodeFreq : public MTCRBTNode<T> {	       
public:
        long count;             // the magnitude of this RBTnode
public:
	MTCRBTNodeFreq() { count=1; };
	MTCRBTNodeFreq(T & aT) : MTCRBTNode <T> (aT) { count=1; };
	virtual ~MTCRBTNodeFreq() {}; 
};

// red black tree template

template <class T> 
class MTCRBT  {
protected:
	MTCRBTNode <T> * root;
	MTCRBTNode <T> * current;  
	virtual MTCRBTNode <T> * createNewNode(T &); 

	void rotateLeft(MTCRBTNode <T> *x);
	void rotateRight(MTCRBTNode <T> *y);

	void forEachNode(MTCRBTNode <T> *, void (*doIt)(T *));

	long count;
	
public:
	MTCRBT() : count(0), root(NULL) {};
	virtual ~MTCRBT() { if (root) delete root; root=NULL; };
	virtual int add(T &, BOOL overwrite=FALSE);
	T * find(T &); 
	T * findClosestSmaller(T &); 
	T * getLowest();
	long getCount() const { return count; };
	void clear() { if (root) delete root; root=NULL; count=0; };;
	void forEach(void (*doIt)(T *));
#ifdef DEBUG
	virtual void dump();
	virtual void dumpNode(MTCRBTNode <T> * );
#endif // DEBUG
};

// red-black binary search tree with freq count node template
 
template <class T>
class MTCRBTFreq : public MTCRBT <T> {
  protected:
	virtual MTCRBTNode <T> * createNewNode(T &); 
	void forEachNodeFreq(MTCRBTNodeFreq <T> *, void (*doIt)(T *, long));

  public:
	virtual ~MTCRBTFreq() { return; };
        virtual int add(T &, BOOL overwrite=FALSE);
	void forEachFreq(void (*doIt)(T *, long));
#ifdef DEBUG
	virtual void dump();
	virtual void dumpNode(MTCRBTNodeFreq <T> * );
#endif // DEBUG
};

// create a new node

template <class T>
inline MTCRBTNode <T> * MTCRBT <T>::createNewNode(T &v)
{
	MTCRBTNode <T> * newNode = new MTCRBTNode <T> (v);

	if (newNode==NULL)
	{
		perror("MTCRBT::createNewNode()");
		abort();
	}

	count++;

	return newNode;
}

// create a new FREQ node

template <class T>
inline MTCRBTNode <T> * MTCRBTFreq <T>:: createNewNode(T &aT)
{
	MTCRBTNode <T> * newNode = new MTCRBTNodeFreq <T> (aT);

	if (newNode==NULL)
	{
		perror("MTCRBTFreq::createNewNode()");
		abort();
	}

	this->count++;

	return newNode;
}

// rotateLeft method

template <class T> 
inline void MTCRBT <T>::rotateLeft(MTCRBTNode <T> *x)
{
	MTCRBTNode <T> *y=x->r;	// set y
	
	if (y==NULL) 
	{
		errorMsg(EMT_ERROR, "I cannot rotate l a node with a NULL r subtree", "MTCRBT<T>::rotateLeft()"); 
		return;
	}

	x->r=y->l;		// turn y's l subtree into x's r subtree

	if (y->l!=NULL) y->l->p=x; 	// change y.l's parent to x	

	y->p=x->p;		// link x's parent to y 

	if (x->p==NULL)
	{
		root=y;
	}
	else
	{
		if (x==x->p->l)	x->p->l=y;
		else 		x->p->r=y;
	}

	y->l=x;			// put x on y's l
	x->p=y;

	return;
}

// rotateRight method

template <class T> 
inline void MTCRBT <T>::rotateRight(MTCRBTNode <T> *y)
{
	MTCRBTNode <T> *x=y->l;		// set x
	
	if (x==NULL) 
	{
		errorMsg(EMT_ERROR, "I cannot rotate r a node with a NULL l subtree", "MTCRBT<T>::rotateRight()"); 
		return;
	}

	y->l=x->r;		// turn x's r subtree into y's l subtree

	if (x->r!=NULL) x->r->p=y; 	// change x.r's parent to y 	

	x->p=y->p;		// link y's parent to x

	if (y->p==NULL)
	{
		root=x;
	}
	else
	{
		if (y==y->p->r)	y->p->r=x;
		else 		y->p->l=x;
	}

	x->r=y;			// put y on x's r
	y->p=x;

	return;
}

// add a node to the RBT

template <class T> 
inline int MTCRBT <T>::add(T &v, BOOL overwrite)
{
	// is this the first node?

	if (root==NULL)
	{
		count=0;
		root=createNewNode(v);
		// root->color=RBT_BLACK;	// (implied)
		// root->p=NULL;		// (implied)
		return 0;
	}

	// otherwise, dive deeper!

	current=root;
	MTCRBTNode <T> *x=NULL;

	for(;;) 
	{
		if (v==current->v)
		{
			if (overwrite==TRUE)
			{
				current->v=v;
			}
			return 1;
		}

		if (v<current->v)
		{
			if (current->l==NULL)
			{
				x=current->l=createNewNode(v);
				x->p=current;
				break;
			}
			else
			{
				current=current->l;
			}
		}
		else
		{
			if (current->r==NULL)
			{
				x=current->r=createNewNode(v);
				x->p=current;
				break;
			}
			else
			{
				current=current->r;
			}
		}
	}

	// rb insert

	MTCRBTNode <T> *y=NULL;

	x->color=RBT_RED;

	while ((x!=root)&&(x->p->color==RBT_RED))
	{
		if (x->p==x->p->p->l)
		{
			y=x->p->p->r;

			if ((y!=NULL)&&(y->color==RBT_RED))
			{
				x->p->color=RBT_BLACK;
				y->color=RBT_BLACK;
				x->p->p->color=RBT_RED;
				x=x->p->p;
			}
			else
			{
				if (x==x->p->r)
				{
					x=x->p;
					rotateLeft(x);
				}
				x->p->color=RBT_BLACK;
				x->p->p->color=RBT_RED;
				rotateRight(x->p->p);
			}
		}
		else
		{
			y=x->p->p->l;

			if ((y!=NULL)&&(y->color==RBT_RED))
			{
				x->p->color=RBT_BLACK;
				y->color=RBT_BLACK;
				x->p->p->color=RBT_RED;
				x=x->p->p;
			}
			else
			{
				if (x==x->p->l)
				{
					x=x->p;
					rotateRight(x);
				}
				x->p->color=RBT_BLACK;
				x->p->p->color=RBT_RED;
				rotateLeft(x->p->p);
			}
		}
	}

	root->color=RBT_BLACK;

	return 0;
}

// add a node to the FREQ RBT

template <class T>
inline int MTCRBTFreq <T>::add(T &aT, BOOL overwrite)
{
    int retval;

    if ((retval=MTCRBT <T>::add(aT, overwrite))==1)
    {		
//	((MTCRBTNodeFreq <T>*)current)->count++;
	((MTCRBTNodeFreq <T>*)(this->current))->count++;

    }

    return retval;
}

// find aNode in the RBT 

template <class T>
inline T * MTCRBT <T>::find(T &v) 
{
	MTCRBTNode <T> *current;

	current=root;

	while (current!=NULL)
	{
		if (v==current->v)
			break;

		if (v<current->v)
		{
			current=current->l;
		}
		else
		{
			current=current->r;
		}
	}

	if (current==NULL)
	{
		return NULL;
	}

	return &current->v;
}

// find in the RBT or find the closest node smaller than v 

template <class T>
inline T * MTCRBT <T>::findClosestSmaller(T &v) 
{
	MTCRBTNode <T> *current;
	MTCRBTNode <T> *closest=NULL;

	current=root;

	while (current!=NULL)
	{
		if (v==current->v)
		{
			return &current->v;  // found an exact match! 
		}

		if (v<current->v)
		{
			current=current->l;
		}
		else
		{
			if (closest==NULL)
			{
				closest=current;
			}
			else
			{
				if (closest->v<current->v)
				{
					closest=current;
				}
			}

			current=current->r;
		}
	}

	if (closest==NULL)
	{
		return NULL;
	}

	return &closest->v;
}

#ifdef DEBUG
// dump the nodes in the tree to stdout

template <class T>  
inline void MTCRBT <T>::dump()
{
	dumpNode(root);
}

// dump the freqnodes to stdout

template <class T>
inline void MTCRBTFreq <T>::dump()
{
	dumpNode(root);
}

template <class T> 
inline void MTCRBT <T>::dumpNode(MTCRBTNode <T> *aNodePtr)
{
	if (aNodePtr==NULL)
	{
		return;
	}

	cout << "left : " << endl;

	dumpNode(aNodePtr->l);

	cout << "node : " << aNodePtr->v;

	if (aNodePtr->color==RBT_RED)
	{
		cout << "\tRED" << endl;
	}
	else
	{
		cout << "\tBLACK" << endl;
	}

	cout << "right: " << endl;

	dumpNode(aNodePtr->r);
}


template <class T> 
inline void MTCRBTFreq <T>::dumpNode(MTCRBTNode <T> *aNodePtr)
{
	if (aNodePtr==NULL)
		return;

	dumpNode(aNodePtr->left);

	cout << ((MTCRBTNodeFreq<T> *)aNodePtr)->count << '\t' << aNodePtr->value << endl;

	dumpNode(aNodePtr->right);
}
#endif // DEBUG

// get the lowest v node in the RBT -- kinda like a heap...

template <class T>
inline T * MTCRBT <T>::getLowest() 
{
	MTCRBTNode<T> *current;
	MTCRBTNode<T> *tempNodePtr=root;

	T * answer=NULL;

	while (tempNodePtr!=NULL)
	{
		current=tempNodePtr;
		answer=&current->v;
		tempNodePtr=tempNodePtr->l;
	}

	return answer;
}

// do something to each of the vs in the tree in order 

template <class T>
inline void MTCRBT <T>::forEach(void (*doIt)(T *))
{
        forEachNode(root, doIt);
}

template <class T>
inline void MTCRBT <T>::forEachNode(MTCRBTNode <T> *aNodePtr, void (*doIt)(T *))
{
        if (aNodePtr==NULL)
	{
                return;
	}

        forEachNode(aNodePtr->l, doIt);

	doIt(&aNodePtr->v);

        forEachNode(aNodePtr->r, doIt);
}

// do something to each of the values in the tree in order 

template <class T>
inline void MTCRBTFreq <T>::forEachFreq(void (*doIt)(T *, long))
{
//        forEachNodeFreq((MTCRBTNodeFreq <T>*)root, doIt);
        forEachNodeFreq((MTCRBTNodeFreq <T>*)this->root, doIt);
}

template <class T>
inline void MTCRBTFreq <T>::forEachNodeFreq(MTCRBTNodeFreq <T> *aNodePtr, void (*doIt)(T *, long))
{
        if (aNodePtr==NULL)
                return;

        forEachNodeFreq((MTCRBTNodeFreq <T> *)aNodePtr->l, doIt);

	doIt(&aNodePtr->v, aNodePtr->count);

        forEachNodeFreq((MTCRBTNodeFreq <T> *)aNodePtr->r, doIt);
}

#endif __RBT_H_
