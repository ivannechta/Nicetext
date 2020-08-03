/*
 * Bitstream to Text Enging Class
 * Copyright (c) Mark T. Chapman 1995
 *
 * "The engine for the babble process"
 *
 * $Id: bits2txt.h,v 1.14 1998/02/19 19:32:59 markc Exp $
 *
 * $Log: bits2txt.h,v $
 * Revision 1.14  1998/02/19 19:32:59  markc
 * *** empty log message ***
 *
 * Revision 1.13  1997/03/05 17:44:52  markc
 * Added static char rcsid []  = "@(#)$Id: bits2txt.h,v 1.14 1998/02/19 19:32:59 markc Exp $";
 *
 * Revision 1.12  1997/02/27 17:52:37  markc
 * Added statFreq
 *
 * Revision 1.11  1997/02/27 13:35:54  chapman
 * Enabled -s parameter for smaller memory usage
 *
 * Revision 1.10  1997/02/24 04:28:29  markc
 *  Added bit-input and output statistics
 *
 * Revision 1.9  1997/02/24 00:12:42  markc
 * Added maxModelLength() and printStats
 *
 * Revision 1.8  1997/02/16 06:58:07  markc
 * BST replaced with RBT
 *
 * Revision 1.7  1996/02/14 15:07:11  markc
 * added BOOL space
 *
 * Revision 1.6  1996/02/08  01:39:59  markc
 * added freshModel -- keep generating until end of current sentence
 *
 * Revision 1.5  1996/02/08  00:53:21  markc
 * added {low} {cap} {shift} to models
 *
 * Revision 1.4  1996/01/20  18:40:39  markc
 * added eolAfterModel
 *
 * Revision 1.3  1996/01/13  13:34:11  markc
 * -m option for babble added -- both models and grammars work!
 * ,
 *
 * Revision 1.2  1996/01/13  13:02:53  markc
 * cap first letter of first word of each sentence
 *
 * Revision 1.1  1996/01/12  23:51:25  markc
 * Initial revision
 *
 *
 */

#ifndef __BITS2TXT_H_
#define __BITS2TXT_H_

static char rcsid__BITS2TXT_H_ []  = "@(#)$Id: bits2txt.h,v 1.14 1998/02/19 19:32:59 markc Exp $";

#define DEFAULT_PRINTSTAT_FREQ  100L	// printStat every x models 

// forward reference
class MTCbitsToTextBase;
class MTCbitsToTextGrammar;
class MTCbitsToTextModel;

// abstract class for conversion objects

class MTCbitsToTextBase
{
private:
	MTCstring dictName;
	MTCstring typeName;
	MTCstring outputName;
	MTCstring inputName;
	MTCRBT <MTCdictAltRecord> dictAltRecRBT;
	MTCRBT <MTCtypeAltRecord> typeAltRecRBT;
	MTCreadRAOF <MTCdictAltRecord> *dictAltRecRAOF;
	MTCinputBitStream *inputBitStream;
	ostream *outputStream;
	BOOL okay;
	BOOL capFirstWord;
	BOOL firstWord;
	BOOL eolAfterModel;
	BOOL space;
	BOOL cap;
	BOOL shift;
	int maxModelLength;
	long modelsSkipped;
	long modelsUsed;
	long totalLengthUsed;
	long inputBits;
	long extraBits;
	long outputBytes;
	long statFreq;
	BOOL smallMode;

	virtual void getNextModel()=0;// pure virtual used by getNextType()
	long getNextType();
	void printPunctuation(MTCstring &);

protected:
	BOOL freshModel;
	MTCsentenceModel *model;
	void setOkay(BOOL aBOOL) 		{ okay=(aBOOL!=FALSE); };
	void setModelsSkipped(long aLong) 	{ modelsSkipped=aLong; };
	void setModelsUsed(long aLong) 		{ modelsUsed=aLong; };
	void setTotalLengthUsed(long aLong) 	{ totalLengthUsed=aLong; };
	void incModelsSkipped()			{ modelsSkipped++; };
	void incModelsUsed()			{ modelsUsed++; };
	void incTotalLengthUsed(long aLong)	{ totalLengthUsed+=aLong; };
	void incInputBits(int anInt)		{ inputBits+=anInt; };
	void incExtraBits(int anInt)		{ extraBits+=anInt; };
	void incOutputBytes(int anInt)		{ outputBytes+=anInt; };
	void incOutputBytes()			{ outputBytes++; };
	void setSmallMode(BOOL aBOOL)		{ smallMode=aBOOL; };
	const MTCreadRAOF <MTCdictAltRecord> *getDictAltRecRAOF() const 
		{ return dictAltRecRAOF; };

public:
	// constructors and destructors
	MTCbitsToTextBase();
	~MTCbitsToTextBase();

	// open data structures methods
	BOOL openDictName(const MTCstring &, BOOL smallMode=FALSE);
	BOOL openTypeName(const MTCstring &);
	BOOL openOutputName(const MTCstring &);
	BOOL openInputName(const MTCstring &);

	// attribute access methods
	BOOL getOkay() const 			{ return okay; };
	const MTCstring & getDictName() const	{ return dictName; };
	const MTCstring & getTypeName() const	{ return typeName; };
	const MTCstring & getOutputName() const	{ return outputName; };
	const MTCstring & getInputName() const	{ return inputName; };
	BOOL getCapFirstWord() const		{ return capFirstWord; };
	void setCapFirstWord(BOOL aBool) 	{ capFirstWord=aBool; };
	BOOL getEOLAfterModel() const		{ return eolAfterModel; };
	void setEOLAfterModel(BOOL aBool)	{ eolAfterModel=aBool; };
	void setMaxModelLength(int anInt)	
	{ if (anInt>0) maxModelLength=anInt; };
	int getMaxModelLength() const 		{ return maxModelLength; };
	long getModelsSkipped() const 		{ return modelsSkipped; };
	long getModelsUsed() const		{ return modelsUsed; };
	long getTotalLengthUsed() const		{ return totalLengthUsed; };
	long getInputBits() const		{ return inputBits; };
	long getExtraBits() const		{ return extraBits; };
	long getOutputBytes() const		{ return outputBytes; };
	BOOL getSmallMode() const		{ return smallMode; };
	void setStatFreq(long aLong)		
	{ statFreq = (((statFreq==1)||(statFreq==-1)) ? 2 : aLong); };
	long getStatFreq() const		{ return statFreq; };
		
	// do it
	virtual BOOL isReady(); 		// am I ready to doIt?
	BOOL doIt(BOOL checkIfReady=TRUE);	// babble away!
	void printStats();			// print statistics
};

// concrete class for converting using a probabilistic grammar file

class MTCbitsToTextGrammar : public MTCbitsToTextBase
{
private:
	virtual void getNextModel();	// used by getNextType()
	MTCstring gramName;
	MTCgrammar *grammar;		// the grammar
protected:
public:
	// constructors and destructors
	MTCbitsToTextGrammar();
	~MTCbitsToTextGrammar();

	// open data structures methods
	BOOL openGrammarName(const MTCstring &aFileName);

	// get attribute methods
	const MTCstring & getGramName() const { return gramName; };

	// am I ready to doIt?
	virtual BOOL isReady();
};

// concreate class for converting using a model.dat raof

class MTCbitsToTextModel : public MTCbitsToTextBase
{
private:
	MTCstring modelName;
	MTCreadRAOF <MTCsentenceModel> *modelRAOF;

	virtual void getNextModel();	// used by getNextType()
protected:
public:
	// constructors and destructors
	MTCbitsToTextModel();
	~MTCbitsToTextModel();

	// open data structures methods
	BOOL openModelName(const MTCstring &aFileName);

	// get attribute methods
	const MTCstring & getModelName() const { return modelName; };

	// am I ready to doIt?
	virtual BOOL isReady();
};

#endif // __BITS2TXT_H_
