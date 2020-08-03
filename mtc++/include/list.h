
/*
 * MTC++ Linked List Template
 * Copyright (c) Mark T. Chapman 1995
 *
 * class MTClist --  linked list
 *
 * $Id: list.h,v 1.7 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: list.h,v $
 * Revision 1.7  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.6  1995/12/16 13:01:23  markc
 * Added operator=()
 *
 * Revision 1.5  1995/12/15  15:38:59  markc
 * removed default ctor for MTClistNode
 *
 * Revision 1.4  1995/12/05  21:12:52  chapman
 * Added copy ctor
 * Updated forEach() to be more efficient (non-recursive)
 * Added current to initial add method
 *
 * Revision 1.3  1995/10/12  20:45:40  markc
 * Added getCurrent();
 *
 * Revision 1.2  1995/08/24  20:15:17  markc
 * added getFirst() getNext()
 *
 * Revision 1.1  1995/08/24  16:15:22  markc
 * Initial revision
 *
 *
 */

#ifndef  __LIST_H_
#define  __LIST_H_

static char rcsid__LIST_H_ []  = "@(#)$Id: list.h,v 1.7 1997/03/05 15:58:28 markc Exp $";

// linked list node template

template <class T> 
class MTClistNode {
public:
	T value;		// the value of this listnode	
        MTClistNode <T> * next; // list < value
public:
        MTClistNode(const T & aT) : next(NULL), value(aT) {};

	virtual ~MTClistNode() 
	   { if (next) delete next; next=NULL; };
};

// linked list template

template <class T> 
class MTClist  {
protected:
	MTClistNode <T> * root;
	MTClistNode <T> * current;
	MTClistNode <T> * last;
	long count;
	virtual MTClistNode <T> * createNewNode(const T &); 
#ifdef DEBUG
	virtual void dumpNode(MTClistNode <T> * );
#endif // DEBUG
	
public:
	MTClist () : root(NULL), current(NULL), last(NULL), count(0) {};
	MTClist (MTClist <T> &aList);
	virtual ~MTClist() { if (root) delete root; root=NULL; };

	MTClist <T> & operator=(MTClist <T> &aList);	// assignment operator

	virtual int add(const T &);
	T * find(const T &); 
	T * getFirst();
	T * getNext();
	T * getCurrent();
	long getCount() const { return count; };
	virtual void clear() { if (root) delete root; root=current=last=NULL; count=0; };;
	virtual void forEach(void (*doIt)(T *));
#ifdef DEBUG
	virtual void dump();
#endif // DEBUG
};

// copy ctor 

template <class T>
inline MTClist<T>::MTClist(MTClist <T> &aT) 
	: root(NULL), current(NULL), last(NULL), count(0) 
{
	T *t;

	for(t=aT.getFirst(); t!=NULL; t=aT.getNext())
	{
		add(*t);
	}
}

// assignment operator

template <class T>
inline MTClist<T> & MTClist<T>::operator=(MTClist <T> &aT)
{
	clear();

	T *t;

	for(t=aT.getFirst(); t!=NULL; t=aT.getNext())
	{
		add(*t);
	}

	return *this;
}

// create a new node

template <class T>
inline MTClistNode <T> * MTClist <T>:: createNewNode(const T &aT)
{
	MTClistNode <T> * newNode = new MTClistNode <T> (aT);

	if (newNode==NULL)
	{
		perror("MTClist::createNewNode()");
		abort();
	}

	count++;

	return newNode;
}

// add a node to the list

template <class T> 
inline int MTClist <T>::add(const T &aT)
{
	// is this the first node?

	if (root==NULL)
	{
		count=0;

		current=root=last=createNewNode(aT);

		return 0;
	}

	// otherwise, dive deeper!

	last->next=createNewNode(aT);
	last=last->next;
}

// find aNode in the list 

template <class T>
inline T * MTClist <T>::find(const T &aT) 
{
	current=root;

	while (current!=NULL)
	{
		if (aT==current->value)
		{
			break;
		}

		current=current->next;

	}

	if (current==NULL)
	{
		return NULL;
	}

	return &current->value;
}

// get the first item in the list

template <class T>
inline T * MTClist <T>::getFirst()
{
	T * retVal;

	current=root;

	if (current==NULL)
	{
		return NULL;
	}

	retVal=&current->value;

	current=current->next;

	return retVal;
}

// get the next item in the list

template <class T>
inline T * MTClist <T>::getNext()
{
	T * retVal;

	if (current==NULL)
	{
		return NULL;
	}

	retVal=&current->value;

	current=current->next;

	return retVal;
}

// get the current item in the list

template <class T>
inline T * MTClist <T>::getCurrent()
{
	T * retVal;

	if (current==NULL)
		retVal=NULL;
	else
		retVal=&current->value;

	return retVal;
}

#ifdef DEBUG
// dump the values in the tree to stdout

template <class T>  
inline void MTClist <T>::dump()
{
	dumpNode(root);
}

template <class T> 
inline void MTClist <T>::dumpNode(MTClistNode <T> *aNodePtr)
{
	if (aNodePtr==NULL)
		return;

	dumpNode(aNodePtr->next);

	cout << aNodePtr->value << endl;
}

#endif // DEBUG

// do something to each of the values in the tree in order 

template <class T>
inline void MTClist <T>::forEach(void (*doIt)(T *))
{
	for(current=root; current!=NULL; current=current->next)
	{
		doIt(&current->value);
	}
}

#endif __LIST_H_
