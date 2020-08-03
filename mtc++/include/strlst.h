/* 
 * String list container
 * Copyright (c) Mark T. Chapman 1998
 *
 * MTC++ Class library
 * 
 * The purpose of this container is to more efficiently create MTCstring
 * objects by appending many multiple MTCstrings.
 *
 *
 * $Id: strlst.h,v 1.4 1998/08/25 18:11:32 markc Exp $
 *
 * $Log: strlst.h,v $
 * Revision 1.4  1998/08/25 18:11:32  markc
 * added truncateLength()
 *
 * Revision 1.3  1998/08/19 21:54:15  markc
 * cast NULL to (char)NULL
 *
 * Revision 1.2  1998/07/13 18:40:14  markc
 * fixed {}'s
 *
 * Revision 1.1  1998/07/13 18:34:21  markc
 * Initial revision
 *
 *
 */

#ifndef __STRLST_H__
#define __STRLST_H__

// Class for making one big string from a list of strings

class MTCstringList {
private:
	MTCstring value;
	MTClist <MTCstring> list;
	long listLength;
	BOOL isCompressed;

public:
	MTCstringList() { listLength=0; isCompressed=TRUE; };

	virtual ~MTCstringList() {};

	BOOL add(const char * aStr)
	{
		if ((aStr!=NULL)&&(aStr[0]!=(char)NULL))
		{
			MTCstring t(aStr);

			return(add(t));
		}

		return FALSE;
	};

	BOOL add(const MTCstring &aString)
	{
		if (aString.length()>0)
		{
			listLength+=aString.length();
			isCompressed = FALSE;
			return (list.add(aString));
		}
		
		return TRUE;
	};

	void clear()
	{
		value="";
		isCompressed=TRUE;
		list.clear();
		listLength=0;
	};

	long length() const 
	{ 
		return (listLength+value.length());
	};

	void compress()
	{
		if (isCompressed==FALSE)
		{
			isCompressed=TRUE;

			char buffer[length()+1];

			long index=0;

			if (value.length()!=0)
			{
				strcpy(&buffer[index], value.c_str());
				index+=value.length();
				buffer[index]=(char)NULL;
			}
		
			MTCstring *tempStr;
	
			for(tempStr=list.getFirst(); tempStr!=NULL; tempStr=list.getNext())
			{
				if (tempStr->length()!=0)
				{
					strcpy(&buffer[index], tempStr->c_str());
					index+=tempStr->length();
					buffer[index]=(char)NULL;
				}
			}

			if (index!=length())
			{
				errorMsg(EMT_ABORT, "I somehow miscalculated the length of the list of strings", "MTCstringList::compress()");
			}

			list.clear();
			listLength=0;

			value=buffer;
		}
	};

	MTCstring & getValue() { compress(); return value; };

	unsigned truncateLength(unsigned aNum)
	{
		compress();

		return(value.truncateLength(aNum));
	}
};

#endif __STRLST_H__
