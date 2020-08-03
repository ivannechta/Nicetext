/*
 * MTC++ Error Message Processing Routines for stdout
 * Copyright (c) Mark T. Chapman 1993,1995
 *
 * $Id: stdermsg.cc,v 1.5 1997/02/23 20:27:15 markc Exp $
 *
 * $Log: stdermsg.cc,v $
 * Revision 1.5  1997/02/23 20:27:15  markc
 * Added strerror() output to EMT_ERROR types
 * Added ERRORMSG_OUTPUT #define to redirect output
 *
 * Revision 1.4  1996/01/08 19:48:00  markc
 * Added space between critical error and message
 *
// Revision 1.3  1995/11/04  19:11:44  chapman
// added const to char * parms
//
// Revision 1.2  1995/06/30  23:31:21  markc
// Update for general MTC++.h
//
// Revision 1.1  1995/06/07  01:44:32  markc
// Initial revision
//
 *
 */

static char rcsid []  = "@(#)$Id: stdermsg.cc,v 1.5 1997/02/23 20:27:15 markc Exp $";

#include "../include/MTC++.h"
#include "../include/errormsg.h"
#include <sys/errno.h>

#ifndef ERRORMSG_OUTPUT
#define ERRORMSG_OUTPUT cerr
#endif

int errorMsg(short EMT, const char *Message, const char *FunctionName)
{
	switch (EMT) {
		case EMT_MESSAGE: 	ERRORMSG_OUTPUT << "* Message: ";
					break;
		case EMT_WARNING: 	ERRORMSG_OUTPUT << "** Warning: ";
					break;
		case EMT_ERROR:		ERRORMSG_OUTPUT << "*** \aERROR (";
					ERRORMSG_OUTPUT << strerror(errno);
					ERRORMSG_OUTPUT << "):";
					break;
		case EMT_DEBUG:		ERRORMSG_OUTPUT << "DEBUG MESSAGE: ";
					break;
		default:        	ERRORMSG_OUTPUT << "**** Critical Error -- Program ending:\t ";
					ERRORMSG_OUTPUT << FunctionName << " " << Message << endl;
					exit(0);
	}

	ERRORMSG_OUTPUT << FunctionName << " " << Message << endl;

	return 0;
}
	
