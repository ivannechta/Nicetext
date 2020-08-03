
/*
 * 
 * MTC++ Init File Access Class
 * Copyright (c) Mark T. Chapman 1993, 1994, 1995
 * 
 * $Id: initfile.cc,v 1.3 1997/02/28 16:55:36 markc Exp $
 * 
 * $Log: initfile.cc,v $
 * Revision 1.3  1997/02/28 16:55:36  markc
 * Added getOkay()
 *
 * Revision 1.2  1995/06/30 23:32:20  markc
 * Update for general MTC++.h
 *
// Revision 1.1  1995/06/07  01:21:53  markc
// Initial revision
//
 * 
 */

static char rcsid []  = "@(#)$Id: initfile.cc,v 1.3 1997/02/28 16:55:36 markc Exp $";

#include "../include/MTC++.h"
#include "../include/mstring.h"
#include "../include/errormsg.h"
#include "../include/initfile.h"

// #define INITDEBUG 1

// MTCinitFile Constructor

MTCinitFile::MTCinitFile(const char *aFileName, int create)
	: okay(TRUE)
{
	if (strlen(aFileName)==0)
	{
		(void)strcpy(inFileName, INITFILE_FILE_DEFAULT);
	}
	else
	{
		(void)strcpy(inFileName, aFileName);
	}

	(void)strcpy(backupFileName, INITFILE_BACKUPFILE_DEFAULT);
	(void)strcpy(tmpFileName, INITFILE_TEMPFILE_DEFAULT);

	initSection();
	initKey();
	initValue();

	inFilePtr = NULL;
	tmpFilePtr = NULL;
	sectionSPos = 0;
	keySPos = 0;

	if (create)
	{
		if ((tmpFilePtr = new fstream(aFileName, ios::out))==NULL)
		{
			errorMsg(EMT_ERROR, "Count not create init file", "MTCinitFile::MTCinitFile()");
			setOkay(FALSE);
		}
		else
		{
			delete tmpFilePtr;
			tmpFilePtr = NULL;
		}
	}

	if (aFileName!=NULL)
	{
		openInFile();
	}
}

// MTCinitFile Destructor

MTCinitFile::~MTCinitFile()
{
	(void)closeInFile();

	if (tmpFilePtr!=NULL)
	{
		(void)closeTmpFile();
	}
}

// MTCinitFile - Go to first section header in the init file

int MTCinitFile::firstSection()
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	// rewind the file

	rewindInFile();

	initSection();
	initKey();
	initValue();

	// read the first section line in the file

	return readLine();
}

// MTCinitFile - go to the next section header

int MTCinitFile::nextSection()
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	int r;

#ifdef INITDEBUG
	cout << "** NEXT SECTION CALLED **" << endl;
#endif 
	char saveSection[INITFILE_MAXSECTION];

	(void)strcpy(saveSection, section);

	// read lines until we get a new section or eof()

	while (((r=readLine())==0)&&(strcmp(saveSection, section)==0));

#ifdef INITDEBUG
	cout << "** EXIT NEXT SECTION CALLED **" << endl;
#endif 
	return r;
}


// MTCinitFile -- Go to the first key in the current section

int MTCinitFile::firstKey()
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	int r;
	streampos saveSPos=sectionSPos;

	// rewind to start of the section

	inFilePtr->seekg(sectionSPos);

	// read until we get the first key

	if ((r=readLine())==0)
	{
		char saveSect[INITFILE_MAXSECTION];
		strcpy(saveSect, section);
		if ((r=readLine())==0)
		{
			if ((strcmp(section, saveSect)!=0)||(strlen(value)==0))
			{
				sectionSPos=saveSPos;
				strcpy(section, saveSect);
				findSection(section);
				r=1;
			}	
		}
	}

	return r;
}

// MTCinitFile -- Go to the next key in the current section

int MTCinitFile::nextKey()
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	char saveSection[INITFILE_MAXSECTION+1];         // temp section buffer
	streampos saveSPos=sectionSPos;

	strcpy(saveSection, section);

	// read the next line, if we are in same section all is well

#ifdef INITDEBUG
	cout << "NextKey Called:" << endl;
#endif
	if (readLine()==0)
	{
#ifdef INITDEBUG
		cout << "section: " << section << " save: " << saveSection << endl;
#endif 
		if (strcmp(section, saveSection)==0)
		{
#ifdef INITDEBUG
			cout << "LEAVING nextKEY" << endl;
#endif
			return 0;
		}
	}

#ifdef INITDEBUG
	errorMsg(EMT_MESSAGE, "WE left the section");
#endif
	// otherwise clean up and bail out!

	if (inFilePtr->eof()!=0)
	{
		initSection();
	}
	else
	{
#ifdef INITDEBUG
		cout << "nextKey calling findsection" << endl;
#endif
		sectionSPos=saveSPos;
		strcpy(section, saveSection);
		findSection(saveSection);
#ifdef INITDEBUG
		cout << "exit nextKey calling findsection" << endl;
#endif
	}

	initKey();
	initValue();

#ifdef INITDEBUG
	cout << "LEAVING nextKEY" << endl;
#endif

	return -1;
}


// MTCinitFile -- Find the section heading within the init file

int MTCinitFile::findSection(char *aSection)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	int r;          // return value

	if (aSection==NULL)
		return 1;

	// are we already in the section?

	if (strncmp(aSection, section, INITFILE_MAXSECTION)==0)
	{
		inFilePtr->seekg(sectionSPos);
		return 0;
	}
		
	// otherwise go to first section

	if ((r=firstSection())!=0)
		return r;

	// read sections until we find it or eof()

	while ((strncmp(aSection, section, INITFILE_MAXSECTION)!=0)&&(r==0))
		r=nextSection();

	return r;
}

// MTCinitFile -- Find a specific key within current or specified section

int MTCinitFile::findKey(char *aKey, char *aSection)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	int r;          // return value

	if (aKey==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "Key parameter cannot be NULL", "MTCinitFile::findKey()");
#endif // ENABLE_EMT_DEBUG
		return 1;
	}

	// if specified, find aSection -- otherwise go to start of current section

	if (aSection!=NULL)
	{
		r=findSection(aSection);
		if (r!=0)
		{
			return r;
		}
	}
	else
	{
		inFilePtr->seekg(sectionSPos);
	}

	// find the key in the section

	while ((strncmp(key, aKey, INITFILE_MAXKEY)!=0)&&((r=nextKey())==0));

	return r;
}

// MTCinitFile -- Add a Section to the init file (or select existing...)

int MTCinitFile::addSection(char *aSection)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	char tmpSection[INITFILE_MAXSECTION];

	if (aSection==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "aSection cannot be NULL", "MTCinitFile::addSection()");
#endif // ENABLE_EMT_DEBUG
		return 1;
	}

	(void)strncpy(tmpSection, aSection, INITFILE_MAXSECTION);

	tmpSection[INITFILE_MAXSECTION-1]=0;

	if (findSection(tmpSection)==0)
		return 0;

	if (openTmpFile()!=0)
	{
		errorMsg(EMT_ERROR, "Could not add section to init file", "MTCinitFile::addSection()");
		return -1;
	}

	tmpFilePtr->seekg(0);

	rewindInFile();

	(void)copyRest();

	*tmpFilePtr << endl << '[' << tmpSection << ']' << endl;

	(void)replaceInFile();

	findSection(tmpSection);

	return 0;
}

// MTCinitFile -- Delete entire Section from the init file

int MTCinitFile::deleteSection(char *aSection)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	char tmpSection[INITFILE_MAXSECTION];   // temporary section

	if (aSection==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "aSection cannot be NULL", "MTCinitFile::deleteSection()");
#endif // ENABLE_EMT_DEBUG
		return 1;
	}

	// if the section isn't in the file, don't bother "deleting" it!

	if (findSection(aSection)!=0)
	{
		return -2;
	}

	nextSection();

	(void)strcpy(tmpSection, section);

	// do the deletion

	if (openTmpFile()!=0)           // open temporary init file
	{
		errorMsg(EMT_ERROR, "Could not delete section from init file", "MTCinitFile::deleteSection"); 
		return -3;
	}

	copyPrev(aSection);             // copy all previous sections and keys

					// Note: this line is what "deletes" the key

	if (tmpSection!=NULL)
	{
		copyRest(tmpSection);   // copy the rest of the keys and sections
	}

	if (replaceInFile()!=0)         // replace the infile with the temporary file
	{
		errorMsg(EMT_ERROR, "Could not delete section from init file", "MTCinitFile::deleteSection"); 
		return -5;
	}

	return 0;
}

// MTCinitFile -- Add a key=value to the current or specified section

int MTCinitFile::addKey(char *aValue, char *aKey, char *aSection)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	int noKeyFound;                         // return code save
	char tmpSection[INITFILE_MAXSECTION];           // save current section

	if ((aValue==NULL)||(aKey==NULL))
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "aValue or aKey should not be NULL", "MTCinitFile::addKey()"); 
#endif // ENABLE_EMT_DEBUG
		return 1;
	}

	if ((strlen(aValue)>INITFILE_MAXVALUE)||(strlen(aKey)>INITFILE_MAXKEY))
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "String length of aValue or aKey is too long", "MTCinitFile::addKey()");
#endif // ENABLE_EMT_DEBUG
		return 2;
	}

	// get correct section

	if (aSection==NULL)
	{
		(void)strcpy(tmpSection, section);
	}
	else
	{
		(void)strcpy(tmpSection, aSection);
	}

	if (strlen(tmpSection)==0)
	{
		return 2;
	}

	// if the section does not exist, add it!

	if (findSection(tmpSection)!=0)
	{
		(void)addSection(tmpSection);
	}

	// make sure the value has changed -- save some time!

	if ((findKey(aKey)==0)&&(strcmp(value, aValue)==0))
	{
		return 0;
	}
	
	// open temporary init file

	if (openTmpFile()!=0)           
	{
		errorMsg(EMT_ERROR, "Could not add key to init file", "MTCinitFile::addKey()");
		return -3;
	}

	// copy all previous sections and keys

	if ((noKeyFound=findKey(aKey))==0)
	{
		copyPrev(tmpSection, aKey);
	}
	else
	{
		copyPrev(tmpSection);
		*tmpFilePtr << '[' << section << ']' << endl;
	}

	// insert the key=value line

	*tmpFilePtr << aKey << '=' << aValue << endl;

	// copy the rest of the init file

	if (noKeyFound==0)
	{
		inFilePtr->seekg(strlen(key)+strlen(value)+2, ios::cur);
	}
	else
	{
		inFilePtr->seekg(strlen(section)+3, ios::cur);
	}

	copyRest();

	// clean up

	if (replaceInFile()!=0)                 // replace the infile with the temporary file
	{
		errorMsg(EMT_ERROR, "Could not add key to init file", "MTCinitFile::addKey()");
		return -5;
	}

	(void)findKey(aKey, tmpSection);        // set current position

	return 0;
}

// MTCinitFile -- Delete a key=value line from the current or specified section

int MTCinitFile::deleteKey(char *aKey, char *aSection)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	char tmpSection[INITFILE_MAXSECTION];   // temporary section pointer

	if (aKey==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "aKey cannot be NULL", "MTCinitFile::deleteKey()");
#endif // ENABLE_EMT_DEBUG
		return -1;
	}

	if (aSection==NULL)
	{
		(void)strncpy(tmpSection, section, (INITFILE_MAXSECTION-1));
	}
	else
	{
		(void)strncpy(tmpSection, aSection, (INITFILE_MAXSECTION-1));
	}

	// if the key isn't in the file, don't bother "deleting" it!

	if (findKey(aKey, tmpSection)!=0)
	{
		return -2;
	}

	// do the deletion

	if (openTmpFile()!=0)           // open temporary init file
	{
		errorMsg(EMT_ERROR, "Could not delete key from init file", "MTCinitFile::deleteKey()"); 
		return -3;
	}

	copyPrev(tmpSection, aKey);     // copy all previous sections and keys

					// Note: this line is what "deletes" the key

	copyRest(tmpSection, aKey);     // copy the rest of the keys and sections

	if (replaceInFile()!=0)         // replace the infile with the temporary file
	{
		errorMsg(EMT_ERROR, "Could not delete key from init file", "MTCinitFile::deleteKey()"); 
		return -5;

	}

	return 0;
}

// MTCinitFile -- open a temporary output file

int MTCinitFile::openTmpFile()
{
	if (tmpFilePtr!=NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "Cannot open same file twice!", "MTCinitFile::openTmpFile()");
#endif // ENABLE_EMT_DEBUG
		return -1;
	}

	unlink(tmpFileName);

	if ((tmpFilePtr=new fstream(tmpFileName, ios::out))==NULL)
	{
		errorMsg(EMT_ERROR, "Could not open temporary init file", "MTCinitFile::openTmpFile()");
		setOkay(FALSE);
		return -5;
	}

	return 0;
}

// MTCinitFile -- close the temporary output file

int MTCinitFile::closeTmpFile()
{
	if (tmpFilePtr==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "Why close the unopened tempfile?", "MTCinitFile::closeTmpFile()");
#endif // ENABLE_EMT_DEBUG
		return -1;
	}

	tmpFilePtr->flush();
	tmpFilePtr->close();

	delete tmpFilePtr;

	tmpFilePtr=NULL;

	return 0;
}
// MTCinitFile -- Rewind the init input file

int MTCinitFile::rewindInFile()
{
	(void)closeInFile();
	return openInFile();
}

// MTCinitFile -- open a init input file

int MTCinitFile::openInFile()
{
	if (inFilePtr!=NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "Why re-open the infile?", "MTCinitFile::openInFile()");
#endif // ENABLE_EMT_DEBUG
		return -1;
	}

	if ((inFilePtr=new ifstream(inFileName))==NULL)
	{
		errorMsg(EMT_ERROR, "Could NOT open init file", "MTCinitFile::openInFile()");
		setOkay(FALSE);
		return -5;
	}

	if ((!inFilePtr)||(inFilePtr->bad()))
	{
		errorMsg(EMT_ERROR, "Could NOT open init file", "MTCinitFile::openInFile()");
		setOkay(FALSE);
		return -5;
	}

	return 0;
}

// MTCinitFile -- close the init input file

int MTCinitFile::closeInFile()
{
	if (inFilePtr==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "Why close the infile again?", "MTCinitFile::closeInFile()");
#endif // ENABLE_EMT_DEBUG
		return -1;
	}

	inFilePtr->close();

	delete inFilePtr;

	inFilePtr=NULL;

	return 0;
}

// MTCinitFile -- replace the init file with the temporary file

int MTCinitFile::replaceInFile()
{
	unlink(backupFileName);

	(void)closeInFile();
	(void)closeTmpFile();

	if (rename(inFileName, backupFileName)!=0)
	{
		errorMsg(EMT_ERROR, "Rename inFileName to backupFileName", "MTCinitFile::replaceInFile()");
		setOkay(FALSE);
	}

	if (rename(tmpFileName, inFileName)!=0)
	{
		errorMsg(EMT_ERROR, "Rename tmpFileName to inFileName", "MTCinitFile::replaceInFile()");
		setOkay(FALSE);
	}

	(void)openInFile();

	return 0;
}

// MTCinitFile -- Copy everything previous to the section (and key)
// This two-pass approach may be too slow for huge inits -- but who has huge inits ;)

int MTCinitFile::copyPrev(char *aSection, char *aKey)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	int r;                          // return code variable
	streampos sPos;                 // stream position
	char c;                         // input character

	if (tmpFilePtr==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "Cannot use closed temp file", "MTCinitFile::copyPrev()");
#endif // ENABLE_EMT_DEBUG
		return -1;
	}

	// rewind file

	inFilePtr->seekg(0);

	// find the section, key

	if (aKey==NULL)
	{
		r=findSection(aSection);
		sPos=sectionSPos;
	}
	else
	{
		r=findKey(aKey, aSection);
		sPos=keySPos;
	}

	// copy all bytes up to that point!

	inFilePtr->seekg(0);
	inFilePtr->get(c);

	if (r==0)
	{
		while ((inFilePtr->tellg()<=sPos)&&(inFilePtr->eof()==0))
		{
			*tmpFilePtr << c;
			(void)inFilePtr->get(c);
		}
	}
	else
	{
		while (inFilePtr->eof()==0)
		{
			*tmpFilePtr << c;
			(void)inFilePtr->get(c);
		}
	}

	return 0;
}

// MTCinitFile -- Copy everthing past the (key within the) section
// This two-pass approach as about as inefficient as copyPrev() above...

int MTCinitFile::copyRest(char *aSection, char *aKey)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	streampos sPos;                 // stream position
	char c;                         // input character

	if (tmpFilePtr==NULL)
	{
#ifdef ENABLE_EMT_DEBUG
		errorMsg(EMT_DEBUG, "Cannot use closed temp file", "MTCinitFile::copyPrev()");
#endif // ENABLE_EMT_DEBUG
		return -1;
	}

	// find the section, key

	if (aSection!=NULL)
	{
		if (aKey==NULL)
		{
			(void)findSection(aSection);
			(void)nextSection();
			sPos=sectionSPos;
		}
		else
		{
			inFilePtr->seekg(0);
			(void)findKey(aKey, aSection);
			sPos=keySPos+(streampos)strlen(key)+(streampos)strlen(value)+3;
		}

		// copy all bytes past that point!

		inFilePtr->seekg(sPos);
	}

	inFilePtr->get(c);

	while (inFilePtr->eof()==0)
	{
		*tmpFilePtr << c;
		(void)inFilePtr->get(c);
	}

	return 0;
}

// MTCinitFile -- Input a single line from the inFile

int MTCinitFile::readLine(int saveComments)
{
	if (getOkay()==FALSE)
	{
		return -1;
	}

	char buffer[INITFILE_MAXLINE+1];
	int i;
	streampos saveSPos;

	// is the infile open? and are we not at eof()?

	if ((getOkay()==FALSE)||(inFilePtr==NULL)||(inFilePtr->eof()!=0)||(inFilePtr->bad()))
	{
		initSection();
		initKey();
		initValue();
		return -1;
	}

	// read the line in from the init file current position

	saveSPos = inFilePtr->tellg();

	inFilePtr->getline(buffer, INITFILE_MAXLINE);

	// Clean up the newlines and any garbage characters

	for(i=0; i<strlen(buffer); i++)
		if (!isprint(buffer[i]))
		{
			buffer[i]=0;
			break;
		}

	// If the line length is 0, skip to the next line

	if (strlen(buffer)==0)
	{
		initKey();
		initValue();
		if (saveComments==0)
		{
			return readLine();
		}
		else
		{
			(void)strcpy(comments, buffer);
			return INITFILE_COMMENTRETURNED;
		}
	}

#ifdef INITDEBUG
cout << "initfile buffer: " << buffer << endl;
#endif

	// Process the current line

	switch (buffer[0])
	{
		case '[':       // Start a New Section
#ifdef INITDEBUG
				cout << "start a new section" << endl;
#endif
				sectionSPos = saveSPos;
				keySPos = 0;

				(void)strncpy(section, &buffer[1], INITFILE_MAXSECTION);
				initKey();
				initValue();

				for(i=0; (i<strlen(section))&&(i<INITFILE_MAXSECTION); i++)
					if (section[i]==']')
						section[i]=0;

				break;

		case ';':       // Ignore Comment Lines
				initKey();
				initValue();

				if (saveComments==0)
					return readLine();
				else
				{
					(void)strcpy(comments, buffer);
					return INITFILE_COMMENTRETURNED;
				}
				// break;  // unreachable code...

		default:        // Separate KEY=VALUE into key, value

				initKey();
				initValue();

				keySPos = saveSPos;

				int toKey = 0;

				for(i=0; i<strlen(buffer); value[i++]=0)
				{
					if (toKey==0)
					{
						if (i>INITFILE_MAXKEY)
						{
							key[INITFILE_MAXKEY-1]=0;
						}       
						else
						{
							if (buffer[i]=='=')
							{
								key[i]=0;
								toKey=i+1;
							}
							else
								key[i]=buffer[i];
						}
					}
					else
						if (i>INITFILE_MAXVALUE)
							value[INITFILE_MAXVALUE-1]=0;
						else
							value[i-toKey]=buffer[i];
				}

				break;
	}

	return (inFilePtr->eof());
}
