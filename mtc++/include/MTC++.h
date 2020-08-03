/*
 *
 * MTC++ Library Header File
 * Copyright (c) Mark T. Chapman 1995
 *
 * $Id: MTC++.h,v 1.4 1997/03/05 15:58:28 markc Exp $
 * 
 * $Log: MTC++.h,v $
 * Revision 1.4  1997/03/05 15:58:28  markc
 * Added static char rcsid []  = "@(#)$Id$";
 *
 * Revision 1.3  1995/11/08 17:09:56  chapman
 * Added #include's for stat call
 *
 * Revision 1.2  1995/06/30  20:20:55  markc
 * removed #includes for library headers
 *
 * Revision 1.1  1995/06/07  01:51:00  markc
 * Initial revision
 *
 *
 */

#ifndef __MTCPP_H_
#define __MTCPP_H_

static char rcsid__MTCPP_H_ []  = "@(#)$Id: MTC++.h,v 1.4 1997/03/05 15:58:28 markc Exp $";

#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef BOOL
#define BOOL short
#endif  BOOL

#ifndef TRUE
#define TRUE 1
#endif  TRUE

#ifndef FALSE
#define FALSE 0
#endif  FALSE

#ifndef MAXPATH
#define MAXPATH 80
#endif  MAXPATH

/*
 * #include "../include/mstring.h"
 * #include "../include/errormsg.h"
 * #include "../include/initfile.h"
 */

#endif __MTCPP_H_
