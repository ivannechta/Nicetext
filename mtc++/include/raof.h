/*
 * MTC++ Random Access Objects in Files Template
 * Copyright (c) Mark T. Chapman 1995
 *
 ********************************************************************
 * The concept is simple.  When you write out a bunch of VARIABLE length 
 * objects to a file, you can no longer fseek to a particular object boundary.
 *
 * The RAOF templates automatically store fseek information in a fixed length
 * hash file that works along with the object stream.
 *
 * This allows easy random access to variable length objects in a file.
 *
 * THINK OF RAOF's AS A FANCY HASH-TABLE.  RAOF's ARE NOT "DATABASE" TABLES!
 *
 * First you create a RAOF through MTCwriteRAOF.  Next you delete the 
 * the MTCwriteRAOF. Then you may access the objects through 
 * MTCreadRAOF::read(index).  Also, MTCreadRAOF has a find() method that does
 * a binary search based on the T.operator<() and T.operator==(). 
 *
 * Notes:
 *
 * 1. MTCwriteRAOF: the corresponding .dat file is really no different than if
 * you just inserted the objects into the ofstream with << instead of append.
 *
 * Therefore, your stream EXTRCTOR must know when it is the end of an object
 * or else the MTCreadRAOF will misbehave (as would ifstream access with >>). 
 * MTCreadRAOF does nothing to prevent this but the NEEDS_EOL parameter.
 *
 * 2. MTCreadRAOF::find() can only do a binary search if the .jmp file points
 * to objects that are sorted with respect to T.operator<() and T.operator==().
 * It is important that the SORT ORDER MUST BE UNIQUE FOR EACH OBJECT or
 * else THE BINARY SEARCH USED IN FIND() WILL RETURN ONE OF MANY OBJECTS that
 * is ==. (very bad).
 *
 * Sorted .jmp files are created two ways:
 *    - the objects were MTCwriteRAOF::appended in order.
 *    - the .jmp file was created later with a MTCcreateAltJmpRAOF object
 *      (see raofalt.h)
 *
 * Remember, sort order is only important if you need to use MTCreadRAOF::find()
 *
 ********************************************************************
 * Note: After you delete the write object for a file pair you may create 
 *       a read object in the same program.
 *
 *                       <same file name>  <different file names>
 * concurrent read              YES               YES    
 * concurrent write              NO               YES 
 * concurrent read/write         NO               YES 
 ********************************************************************
 *
 * class MTCwriteRAOF -- used to create a readable RAOF pair
 * class MTCreadRAOF  -- used to read objects from a RAOF pair
 *
 ********************************************************************
 * $Id: raof.h,v 1.12 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: raof.h,v $
 * Revision 1.12  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.11  1996/04/10 20:54:44  markc
 * removed #warning
 *
 * Revision 1.10  1996/04/10 20:39:44  markc
 * fixed sizeof() for the raofJumpTableType to be unsigned int.
 * this allows different machines to use the same .jmp and .alt files
 *
 * Revision 1.9  1996/02/07 03:09:37  markc
 * Added setOkay(FALSE) to read::init() when stat fails.
 *
 * Revision 1.8  1996/01/14  19:46:21  markc
 * relaxed const in find()
 *
 * Revision 1.7  1996/01/14  19:38:56  markc
 * relaxed const
 *
 * Revision 1.6  1995/11/09  20:51:16  markc
 * Moved #ifdef APPEND_LENGTH_TO_JMP in writeRAOF destructor so that
 * it actually delete's jumpFile.
 *
 * Revision 1.5  1995/11/08  18:02:52  chapman
 * Updated for raofalt support
 *
 * Revision 1.4  1995/07/27  12:19:01  markc
 * Added getIndex() method
 *
 * Revision 1.3  1995/07/26  16:13:59  markc
 * Added NEED_EOL parm to constructors for writeRAOF
 *
 * Revision 1.2  1995/07/26  13:44:52  markc
 * Added #ifdef DEBUG
 *
 * Revision 1.1  1995/07/06  11:53:11  markc
 * Initial revision
 *
 *
 */

#ifndef  __RAOF_H_
#define  __RAOF_H_

static char rcsid__RAOF_H_ []  = "@(#)$Id: raof.h,v 1.12 1997/03/05 15:58:28 markc Exp $";

#define NEEDS_EOL TRUE			// use this to clarify constructor calls

// DO NOT USE WITH #include "raofalt.h" //  #define APPEND_LENGTH_TO_JMP 

// make raofJumpTableType large enough to count all objects.
// BE CAREFUL -- MTC*RAOF's don't know about overflow!
// if you make it smaller, then you could save space.

typedef unsigned int raofJumpTableType; 

// raof base class 

class MTCbaseRAOF {
private:
	BOOL okay;
protected:
	MTCstring dataFileName;
	MTCstring jumpFileName;
	void setOkay(BOOL aBool) { okay=(aBool!=FALSE); };
	long count;
public:
	// constructors and desstructors
	MTCbaseRAOF(const MTCstring &prefix) 
		: okay(TRUE), count(0)
		{ dataFileName=prefix+".dat"; jumpFileName=prefix+".jmp"; }; 
        MTCbaseRAOF(const MTCstring &df, const MTCstring &jf) 
		: okay(TRUE), count(0) 
		{ dataFileName=df; jumpFileName=jf; }; 
	virtual ~MTCbaseRAOF() {};

	// member access methods
	BOOL getOkay() const { return okay; };
	long getCount() const { return count; };
};

// raof write class

template <class T>
class MTCwriteRAOF : public MTCbaseRAOF {
private:
	raofJumpTableType pos;
	void init();			// shared constructor code
	ofstream *dataFile;
	ofstream *jumpFile;
	BOOL needEOL;			// do we need EOL between objects?

protected:
public:
	// constructors and destructors

	MTCwriteRAOF(const MTCstring &prefix, BOOL eolBool=FALSE)
		: MTCbaseRAOF(prefix), needEOL(eolBool) { init(); };
		
	MTCwriteRAOF(const MTCstring &data, const MTCstring &jump, 
		BOOL eolBool=FALSE)
		: MTCbaseRAOF(data, jump), needEOL(eolBool) { init(); };

	virtual ~MTCwriteRAOF();

	// methods
	int append(T *);
	int append(T &aT) { append(&aT); };
	void setNeedEOL(BOOL aBool) { needEOL=(aBool!=FALSE); };
	BOOL getNeedEOL() const     { return needEOL; };
};

// raof read class

template <class T>
class MTCreadRAOF : public MTCbaseRAOF {
private:
	raofJumpTableType pos;
	raofJumpTableType offset;
	ifstream *dataFile;
	ifstream *jumpFile;
	T value;
	long index;
	void init();			// shared constructor code

protected:
public:
	// constructors and destructors

	MTCreadRAOF(const MTCstring &prefix)
		: MTCbaseRAOF(prefix) { init(); };

	MTCreadRAOF(const MTCstring &data, const MTCstring &jump) 
		: MTCbaseRAOF(data, jump) { init(); };

	virtual ~MTCreadRAOF();

	// methods
	T * read(long anIndex=0);	// should be const
	const T * find(T &);
	long getIndex() const { return index; };

	// used only by MTCcreateAltJmpRAOF -- friend is too ugly here...
	raofJumpTableType getPos() const { return pos; };
};

// write shared constructor code

template <class T>
inline void MTCwriteRAOF <T>::init()
{
	if ((dataFile=new ofstream(dataFileName.c_str()))==NULL)
	{
		setOkay(FALSE);
		perror(dataFileName.c_str());
	}

	if ((jumpFile=new ofstream(jumpFileName.c_str()))==NULL)
	{
		setOkay(FALSE);
		perror(jumpFileName.c_str());
	}
}

// write destructor

template <class T>
inline MTCwriteRAOF <T>::~MTCwriteRAOF()
{
	if (dataFile!=NULL)
	{
		delete dataFile;
		dataFile=NULL;
	}

	if (jumpFile!=NULL)
	{
#ifdef APPEND_LENGTH_TO_JMP
#ifdef DEBUG
		cout << "Output Count: " << count << endl;
#endif DEBUG
		jumpFile->write((void *)&count, sizeof(long));
#endif // APPEND_LENGTH_TO_JMP
		delete jumpFile;
		jumpFile=NULL;
	}
}

// append method

template <class T>
inline int MTCwriteRAOF <T>::append(T *aT)
{
	if (getOkay()==FALSE)
	{
		errorMsg(EMT_ABORT, "Attempted to write a non-okay RAOF", "MTCwriteRAOF::append()");
	}

	pos=dataFile->tellp();

	jumpFile->write((char*)&pos, sizeof(raofJumpTableType));

	(*dataFile) << (*aT);

	if (getNeedEOL()==TRUE)
	{
		(*dataFile) << '\n';
	}

	count++;
}

// read shared constructor code

template <class T>
inline void MTCreadRAOF <T>::init()
{
	index=0;

	if ((dataFile=new ifstream(dataFileName.c_str()))==NULL)
	{
		setOkay(FALSE);
		perror(dataFileName.c_str());
	}

	if ((jumpFile=new ifstream(jumpFileName.c_str()))==NULL)
	{
		setOkay(FALSE);
		perror(jumpFileName.c_str());
	}
	else
	{
#ifdef APPEND_LENGTH_TO_JMP
		// read in the count from the end of file
		offset=sizeof(long);

		jumpFile->seekg(-offset, ios::end);
#ifdef DEBUG
		cout << "Jump TO: " << jumpFile->tellg() << endl;
#endif DEBUG

		jumpFile->read((void *)&count, sizeof(raofJumpTableType)); // sizeof(long));
#else  
		// use stat instead of appending info to end of .jmp file

	 	struct stat tempStat;

		if (stat(jumpFileName.c_str(), &tempStat)==0)
		{
			count=tempStat.st_size/sizeof(raofJumpTableType);
		}
		else
		{
			perror(jumpFileName.c_str());
			errorMsg(EMT_ERROR, "I could not stat the jump file", "MTCreadRAOF<T>::init()"); 
			setOkay(FALSE);
		}

#endif // APPEND_LENGTH_TO_JMP
	}
}

// read destructor

template <class T>
inline MTCreadRAOF <T>::~MTCreadRAOF()
{
	if (dataFile!=NULL)
	{
		delete dataFile;
		dataFile=NULL;
	}

	if (jumpFile!=NULL)
	{
		delete jumpFile;
		jumpFile=NULL;
	}

}

// read method -- NOTE: COUNTING STARTS AT ZERO!!!

template <class T>
inline T * MTCreadRAOF <T>::read(long newIndex)
{
	if ((newIndex>=count)||(newIndex<0))
	{
		errorMsg(EMT_WARNING, "Invalid Index for Reading.", "MTCreadRAOF::read()"); 
		return (T *)NULL;
	}

	index=newIndex;

	// compute jump table offset 
	offset=index*sizeof(raofJumpTableType);
	
	// seek to jump table position 
	jumpFile->seekg(offset, ios::beg);
	
	// read the corresponding data file position from the jump file
	jumpFile->read((char*)&pos, sizeof(raofJumpTableType));

	// seek to data file position
	dataFile->seekg(pos, ios::beg);

	// read object from data file
	(*dataFile) >> value;

	return &value;
}

// find method -- use binary search technique -- ASSUMES RAOF IS SORTED!!! 
// for alternate sort orders see raofalt.h

template <class T>
inline const T * MTCreadRAOF <T>::find(T &aT)
{
	long low=0;
	long high=count-1;
	long middle;

	while ((high-low)>=0)
	{
		middle=(high+low)/2;

		read(middle);

#ifdef DEBUG
		cout << "low: " << low << "mid: " << middle << "hi: " << high << endl;
		cout << "find:" << aT;
		cout << "buff:" << value;
#endif DEBUG

		if (aT==value)
		{
#ifdef DEBUG
			cout << "EQUAL" << endl;
#endif DEBUG
			return &value;	// found it!!!
		}

		if (aT>value)
		{
#ifdef DEBUG
			cout << ">" << endl;
#endif DEBUG
			low=middle+1;
		}
		else
		{
#ifdef DEBUG
			cout << "<" << endl;
#endif DEBUG
			high=middle-1;
		}
	}
	
	return NULL;
}

#endif __ROAF_H_
