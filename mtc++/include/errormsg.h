/*
 * Error Message Processing Routines 
 * Copyright (c) Mark T. Chapman 1993, 1994, 1995
 * 
 * $Id: errormsg.h,v 1.3 1997/03/05 15:58:28 markc Exp $
 *
 * $Log: errormsg.h,v $
 * Revision 1.3  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.2  1995/11/04 19:10:52  chapman
 * added const to char * parms
 *
 * Revision 1.1  1995/06/07  01:49:36  markc
 * Initial revision
 *
 *
 */

#ifndef __ERRORMSG_H_
#define __ERRORMSG_H_

static char rcsid__ERRORMSG_H_ []  = "@(#)$Id: errormsg.h,v 1.3 1997/03/05 15:58:28 markc Exp $";

#define EMT_DEBUG  -1	// Debug Messages Only 
#define EMT_MESSAGE 0 	// Just Display the message and continue
#define EMT_WARNING 1	// The message is a warning -- Option to Cancel
#define EMT_ERROR   2 	// An error occurred -- recommend canclation 
#define EMT_ABORT   3	// Immediately Abort the program -- critical error
 
int errorMsg(short EMT, const char *Message, const char *FunctionName=NULL);

#endif // __ERRORMSG_H_
