
/* 
 *
 * MTC++ Init File Access Class
 * Copyright (c) Mark T. Chapman 1993, 1994, 1995
 * 
 * $Id: initfile.h,v 1.4 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: initfile.h,v $
 * Revision 1.4  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1997/02/28 16:59:40  markc
 * Added getOkey()
 *
 * Revision 1.2  1995/10/12 15:26:28  markc
 * Added const char get***()'s
 *
 * Revision 1.1  1995/06/07  01:49:48  markc
 * Initial revision
 *
 *
 */

#ifndef __INITFILE_H_
#define __INITFILE_H_

static char rcsid__INITFILE_H_ []  = "@(#)$Id: initfile.h,v 1.4 1997/03/05 15:58:28 markc Exp $";

#define INITFILE_FILE_DEFAULT           "nlp.ini"    // default init file
#define INITFILE_TEMPFILE_DEFAULT       "ini000.tmp"    // default tempfile name
#define INITFILE_BACKUPFILE_DEFAULT     "nlp.bak"   	// default backup file 

#define INITFILE_MAXSECTION     80      // Buffer length for a section name
#define INITFILE_MAXKEY         80      // Buffer length for a key name
#define INITFILE_MAXVALUE       80      // Buffer length for a value
#define INITFILE_MAXLINE        256     // Buffer length for an init file line
#define INITFILE_COMMENTRETURNED        -50     // Return code for readLine(TRUE) if it is a comment line

class MTCinitFile {
private:
	BOOL okay;
	char inFileName[MAXPATH];               // Init File Path
	char tmpFileName[MAXPATH];              // temporary file path
	char backupFileName[MAXPATH];           // name for backup of init file
	char section[INITFILE_MAXSECTION];              // Current Section ID
	char key[INITFILE_MAXKEY];                      // Current Key
	char value[INITFILE_MAXVALUE];                  // Current Value
	char comments[INITFILE_MAXLINE];                        // Last Comment Line
	ifstream *inFilePtr;                    // Pointer to initfile
	fstream *tmpFilePtr;                    // Pointer to temporary output file for init
	streampos sectionSPos;                  // stream position to start of section
	streampos keySPos;                      // stream position to start of key
	int readLine(int saveComments=0);       // input a line from the infile
	void initSection()                      // Init section and sectionSPos
		{ (void)strcpy(section, ""); sectionSPos=0;};
	void initKey()                          // Init key and keySPos
		{ (void)strcpy(key, ""); keySPos=0;};
	void initValue() { (void)strcpy(value, ""); };          // Init value to ""
	int copyPrev(char *aSection, char *aKey=NULL);          // copy init file up to (the key within) the section
	int copyRest(char *aSection=NULL, char *aKey=NULL);     // copy the rest of the init file
	int openTmpFile();                      // open a temporary output file
	int closeTmpFile();                     // close the temporary output file
	int openInFile();                       // open the input init file
	int closeInFile();                      // close the input init file
	int rewindInFile();                     // rewind the input file
	int replaceInFile();                    // replace the infile with the out file
	void setOkay(BOOL aBOOL) 	{ okay=aBOOL; };

public:
	MTCinitFile(const char *aFileName=NULL, int create=FALSE);    // Constructor
	~MTCinitFile();                         // Destructor

	int firstSection();                     // Go to First Section in File
	int nextSection();                      // Go to Next Section in File
	int firstKey();                         // Go to First Key within section
	int nextKey();                          // Go to Next Key within section
	int findSection(char *aSection);        // Go to Specified Section
	int findKey(char *akey, char *aSection=NULL);   // Go to Specified Key

	int addSection(char *aSection);         // Add a new section
	int deleteSection(char *aSection);      // Delete a section
	int addKey(char *aValue, char *aKey, char *aSection=NULL);      // Add a new key=value
	int updateKey(char *aValue, char *aKey, char *aSection=NULL)    // update a key=value
		{ return addKey(aValue, aKey, aSection);};
	int deleteKey(char *aKey, char *aSection=NULL);         // Delete a Key from a section

	char *getSection(char *aSection, int aLen=-1)   // Return current section
		{ if (aLen==-1) return(strncpy(aSection, section, INITFILE_MAXSECTION)); return (strncpy(aSection, section, aLen)); };
	char *getKey(char *aKey, int aLen=-1)           // Return current key
		{ if (aLen==-1) return (strncpy(aKey, key, INITFILE_MAXKEY)); return (strncpy(aKey, key, aLen)); };
	char *getValue(char *aValue, int aLen=-1)               // Return current value
		{ if (aLen==-1) return (strcpy(aValue, value)); return (strncpy(aValue, value, aLen)); };

	const char *getKey() { return key; };
	const char *getSection() { return section; };
	const char *getValue() { return value; };
	BOOL getOkay() const { return okay; };
};

#endif // __INITFILE_H_
