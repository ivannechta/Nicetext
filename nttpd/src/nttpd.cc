/*
 * HTTP Server Daemon for NICETEXT System
 * Copyright (c) Mark T. Chapman 1996-1998
 *
 * WARNING: THIS IS A TERRIBLE HACK OF A PROGRAM 
 *          IT PROBABLY IS A SECURITY RISK AND A RESOURCE HOG
 * 
 * $Id: nttpd.cc,v 1.21 1998/08/25 20:44:08 markc Exp $
 *
 * $Log: nttpd.cc,v $
 * Revision 1.21  1998/08/25 20:44:08  markc
 * Moved "Official Home Page"
 *
 * Revision 1.20  1998/08/25 18:10:32  markc
 * added "truncateLength()" to clean up the dataStr
 * fixed problem in recvStrBuf to NULL terminate the buffer - THIS FIXED A BIG BUG
 *
 * Revision 1.19  1998/08/24 02:37:52  markc
 * Getting closer - DEBUGMSG(), DEBUG(),
 * an optimized MTCstring class in mtc++ library (cache length of string)
 *
 * Revision 1.18  1998/08/21 19:23:15  markc
 * fixed noCaseStrStr() to perform a bit better
 * added some debug messages
 *
 * Revision 1.17  1998/08/20 13:26:23  markc
 * removed MSG_EOR for Digital UNIX
 * added setStatFreq(0) for MTCbitsToText - this disables status updates to stderr
 *
 * Revision 1.16  1998/08/19 21:58:38  markc
 * cast bzero parameters properly
 * also, the 1.15 version needed #ifdef's for MSG_EOF, MSG_EOR
 * These are being done to port to gcc 2.8.1 on Digital UNIX
 *
 * Revision 1.15  1998/08/19 21:50:05  markc
 * *** empty log message ***
 *
 * Revision 1.14  1998/08/19 21:30:50  markc
 * fixed blank messages
 * made it look even cleaner
 *
 * Revision 1.13  1998/08/19 20:02:49  markc
 * made it look better
 * added exit if I get confused
 *
 * Revision 1.12  1998/08/19 18:18:15  markc
 * append '/' to workDir
 *
 * Revision 1.11  1998/08/19 15:26:57  markc
 * added command-line parameters
 *
 * Revision 1.10  1998/08/17 15:23:14  markc
 * *** empty log message ***
 *
 * Revision 1.9  1998/07/13 18:23:11  markc
 * It's working again - using stringLists instead of strings
 * this is much more efficient
 *
 *
 * Revision 1.8  1997/08/28 05:05:01  markc
 * NOT WORKING
 *
 * Revision 1.7  1997/08/28 02:10:11  markc
 * Working - but inefficient!
 *
 * Revision 1.6  1997/08/23 02:59:32  markc
 * Fixed it!
 *
 * Revision 1.5  1997/08/22 12:33:07  markc
 * Switched to MTCstring from strstream
 *
 * Revision 1.4  1997/08/22 12:21:01  markc
 * *** empty log message ***
 *
 * Revision 1.3  1997/08/20 01:41:18  markc
 * Added word-wrap
 * Added strstream for Content-length:
 *
 * Revision 1.2  1997/08/18 19:40:17  markc
 * Added scramble
 *
 * Revision 1.1  1997/08/18 03:44:49  markc
 * Initial revision
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream.h>
#include <fstream.h>
#include <strstream>
#include <signal.h>
#include "../../mtc++/include/MTC++.h"
#include "../../mtc++/include/mstring.h"
#include "../../mtc++/include/errormsg.h"
#include "../../mtc++/include/rbt.h"
#include "../../mtc++/include/list.h"
#include "../../mtc++/include/raof.h"
#include "../../mtc++/include/raof2rbt.h"
#include "../../gendict/include/dictrec.h"
#include "../../gendict/include/dictarec.h"
#include "../../gendict/include/typerec.h"
#include "../../gendict/include/typearec.h"
#include "../../gendict/include/sentnode.h"
#include "../../gendict/include/sentmdl.h"
#include "../../gendict/include/dconst.h"
#include "../../babble/include/bitstrm.h"
#include "../../babble/include/bitinput.h"
#include "../../babble/include/bitoutput.h"
#include "../../babble/include/rule.h"
#include "../../babble/include/grammar.h"
#include "../../babble/include/bits2txt.h"
#include "../../babble/include/txt2bits.h"
#include "../include/postdata.h"
#include "../../mtc++/include/strlst.h"

#define MYPORT 	2119	// the port for the server
#define BACKLOG 100	// # of pending connections in the queue 
#define TMPDIR  "/tmp"  // where to place the temporary files...
#define WORKDIR  "./"  // location of dictionary and style source files
#define MAX_COLUMN 70   // when to wrap words
#define MAX_SEND_BLOCK 128    // maximum length of send() buffers

//#define DEBUG 
#ifdef DEBUG
#define DEBUGMSG(x) cout << getpid() << ": "; x 
#else
#define DEBUGMSG(x)
#endif

#define DICTPARM "dict"
#define TYPEPARM "type"
#define MODELPARM "model"

#define MAX_BUFFER 655350 
#define CONTENT_LENGTH_STR "Content-length: "
#define CLS_LEN 16 

BOOL sendInitial(MTCstringList &os);
BOOL sendFile(MTCstringList &os, const char *file, BOOL wordWrap=FALSE);
BOOL sendNiceText(MTCstringList &os, MTCRBT<MTCpostData> &pdSet);
BOOL sendScramText(MTCstringList &os, MTCRBT<MTCpostData> &pdSet);
BOOL sendStop(int fd);
BOOL parseParms(const MTCstring &aStr, MTCRBT<MTCpostData> &);
void dumpPostData(MTCpostData *aPD);
void randomize();
void closeAndExit(int fd);
BOOL postResponse(MTCstringList &os, int fd);
MTCstring & recvStrBuf(int fd, MTCstring &);
const char * noCaseStrStr(const char *big, const char *little, BOOL prefix=FALSE);
void usage();

int hexdigit(char c);

void killZombies(int);

MTCstring workDir(WORKDIR);
MTCstring tmpDir(TMPDIR);
int myPort=MYPORT;
BOOL localHostOnly=TRUE;

int main(int argc, char *argv[])
{
	// read parameters 

        extern char *optarg;
	extern int optind;
	int ch;
	while ((ch = getopt(argc, argv, "b:hp:wt:")) != EOF)
	{
		switch(ch) 
		{
			case 'b':
				workDir=optarg;
				if (workDir.c_str()[workDir.length()] != '/')
				{
					workDir += "/";
				}
                                break;
			case 'p':
				myPort=atoi(optarg);
				break;
			case 't':
				tmpDir=optarg;
				break;
			case 'w':
				localHostOnly=FALSE;
				break;
			default:
				usage();	
				return(-1);
		}
	}

	argc -= optind;
	argv += optind;

	if (workDir == WORKDIR)
	{
		char buffer[512];
		workDir=getwd(buffer);
	}

	// check some constants

#ifdef DEBUG
	if (strlen(CONTENT_LENGTH_STR)!=CLS_LEN)
	{
		DEBUGMSG(cout << "OH NO! - YOU MUST SET CLS_LEN to " << strlen(CONTENT_LENGTH_STR) << " instead of " << CLS_LEN <<" in nttpd.cc " << endl;)
	}
#endif DEBUG 

	// start processing

	cerr << "NTTPD 0.1 process started" << endl;
	cerr << "\tbase directory: " << workDir << endl;
	cerr << "\ttemp directory: " << tmpDir << endl;
	cerr << "\tlistening on port: " << myPort << endl;
 
	if (localHostOnly==TRUE)
	{
		cerr << "\tI will only accept connections from localhost" << endl; 
	}
	else
	{
		cerr << "\t-w selected - I will accept connections from ANY MACHINE!" << endl;
	}

	cerr << endl;
	cerr << "* * * * * * * * * *" << endl;
	cerr << "To connect to this server, ";
	cerr << "go to http://localhost:" << myPort << '/' << endl;
	cerr << "from your favorite browser" << endl; 
	cerr << "* * * * * * * * * *" << endl;
	cerr << endl;
	cerr << "To kill this server, hit control-c or use the operating systems kill command" << endl; 
	cerr << endl;
	cerr << "WARNING: " << endl;
	cerr << "\tAlthough the rest of the NICETEXT system is quite stable" << endl;
	cerr << "\tthis program is a terrible hack." << endl;
	cerr << "\tIt is most likely a resource hog and a security risk..." << endl; 
	cerr << endl;
 
	// start processing

	int sockfd, newfd;	
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	socklen_t sin_size=sizeof(struct sockaddr_in);

	signal(SIGCHLD, killZombies);
	signal(SIGKILL, killZombies);
	signal(SIGSTOP, killZombies);
	signal(SIGQUIT, killZombies);

	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(myPort);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(((char *)&(my_addr.sin_zero)), sizeof(my_addr.sin_zero));
	
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
	{
		perror("bind");
		exit(2);
	}

	if (listen(sockfd, BACKLOG)==-1)
	{
		perror("listen");
		exit(3);
	}

	for(;;) 
	{
		if ((newfd=accept(sockfd, (struct sockaddr *)&their_addr, &sin_size))==-1)
		{
			perror("accept");
			continue;
		}

		time_t now(time(NULL));

		cout << ctime(&now) << "Server (" << getpid() << ") got a connection from " << inet_ntoa(their_addr.sin_addr) << endl;

                // spawn child process

                if (!fork())
                {
			randomize();

			BOOL keepAlive=FALSE;

#ifdef DEBUG
			for(int reqLoop=1; ; reqLoop++)	
#else
			for(;;)
#endif
			{
				DEBUGMSG(cout << "START OF REQUEST LOOP: " << reqLoop << endl;)

				MTCstringList response;

                        	if ((localHostOnly==TRUE)&&(strcmp(inet_ntoa(their_addr.
sin_addr),"127.0.0.1")!=0))
                        	{
                                	response.add("I cannot accept a connection from ");   
									response.add(inet_ntoa(their_addr.sin_addr));
									response.add("\nbecause the -w parameter is not set for nttpd\n");
                                	postResponse(response, newfd);
                                	closeAndExit(newfd);
                        	}

				MTCstring recvBuf;

				recvStrBuf(newfd, recvBuf);

				if (recvBuf.length()==0)
				{
					DEBUGMSG(cout << "EMPTY RECEIVE BUFFER - ";)

					if (keepAlive==TRUE)
					{
						DEBUGMSG(cout << "KEEPALIVE ON" << endl;)
						sendStop(newfd);
						closeAndExit(newfd);
					}
					else
					{
						DEBUGMSG(cout << "NO KEEPALIVE" << endl;)
						perror("receive");

						response.add("ERROR - I AM CONFUSED BY THIS EMPTY REQUEST!\n");
						postResponse(response, newfd);
						closeAndExit(newfd);
					}
				}

				DEBUGMSG(cout << "CHECKING FOR KEEP-ALIVE..." << endl;)

				if ((keepAlive==FALSE)&&(noCaseStrStr(recvBuf.c_str(), "Connection: Keep-Alive")!=NULL))
				{
					keepAlive=TRUE;
					DEBUGMSG(cout << "KEEPALIVE SET TO TRUE!" << endl;)
				}

				// is this a "GET" request?

				if (noCaseStrStr(recvBuf.c_str(), "GET /", TRUE)!=NULL)
				{
					DEBUGMSG(cout << "GET REQUEST" << endl;)

					// is this an initial request?

					if ((recvBuf.length()>4)&&(recvBuf.c_str()[5]==' '))
					{
						sendInitial(response);
					}
					else    // no, then treat as file req
					{
						// TREAT ALL AS INITIAL...

						sendInitial(response);
/*
 * FILE REQUESTS ARE DISABLED FOR SECURITY REASONS
 * OTHERWISE, SOMEONE COULD REQUEST COPIES OF ANY FILE(S) 
 * THAT THE EFFECTIVE-USER-ID OF THIS PROCESS CAN READ
 * 
						 sendFile(response, strtok(&buffer[5], " "));

						*response << "<HTML><HEAD><TITLE><ERROR - URL NOT AVAILABLE></TITLE></HEAD><BODY>THE URL REQUESTED IS NOT AVAILABLE AT THIS TIME<BR></BODY></HTML>");
						postResponse(response, newfd);
						closeAndExit(newfd);
*/
					}
				}
				else
				{
					MTCstringList dataStr;
					BOOL postOp=(noCaseStrStr(recvBuf.c_str(), "POST /", TRUE)!=NULL);

					if (postOp)
					{
						DEBUGMSG(cout << "POST OPERATION" << endl;)
						unsigned requestLength=0;

						BOOL readingContentLength=TRUE; 

#ifdef DEBUG
						int clLoop=0; 
#endif
						do	
						{
							DEBUGMSG(cout << "Reading Content Length: " << ++clLoop << endl;) 

							const char *cl=noCaseStrStr(recvBuf.c_str(), CONTENT_LENGTH_STR); 
							if (cl!=NULL)
							{
								int clLen=strlen(cl);

								if (clLen>CLS_LEN)
								{
									int i;
			
									for(i=CLS_LEN; i<clLen&&cl[i]>='0'&&cl[i]<='9'; i++)
									{
										DEBUGMSG(cout << "\"" << cl[i] << "\"" << endl;)
									}

									DEBUGMSG(cout << "i=" << i << ",cl[i]=\"" << cl[i] << "\"" << endl;)

									if ((cl[i]=='\n')||(cl[i]=='\r'))
									{
										requestLength=atoi(&cl[CLS_LEN]);
										readingContentLength=FALSE;
									}
									else
									{
										DEBUGMSG(cout << "Hmmm..we don't seem to have read the entire value of the Content-Length: (" << cl << ")" << endl;)
										if (cl[i]!='\0')
										{
											DEBUGMSG(cout << "the expected value of 'NULL' was not the last character - somethings wrong with this logic" << endl;)
										}
									}
								}
								else
								{
									DEBUGMSG(cout << "clLen<=CLS_LEN" << endl;)
								}
							}
							else
							{
								DEBUGMSG(cout << "cl==NULL!" << endl;)
							}

							if (readingContentLength==TRUE)
							{
								MTCstring readMoreStr;
								recvStrBuf(newfd, readMoreStr);
								recvBuf+=readMoreStr;

							}
						} while (readingContentLength==TRUE);

						DEBUGMSG(cout << "Content-Length: " << requestLength << endl;)

						dataStr.add(noCaseStrStr(recvBuf.c_str(), "NTTPDATA"));

						DEBUGMSG(cout << "INITIAL DATASTR LENGTH: " << dataStr.length() << endl;)

						if ((dataStr.length()<requestLength))
						{
							DEBUGMSG(cout << "READING MORE DATA..." << endl;)

							do
							{
								MTCstring tempBuf;
 								recvStrBuf(newfd, tempBuf);
 								dataStr.add(tempBuf);
								DEBUGMSG(cout << "INTERMEDIATE DATASTR LENGTH: " << dataStr.length() << endl;)
							}  while (dataStr.length() < requestLength);
							DEBUGMSG(cout << "DONE ADDING TO DATASTR" << endl;)
						}

						if ((dataStr.length()>requestLength))
						{
							DEBUGMSG(cout << "TRUNCATING dataStr.Length() from " << dataStr.length() << " to match content-length " << requestLength << endl;)
							dataStr.truncateLength(requestLength);
						}

						DEBUGMSG(cout << "FINAL DATASTR LENGTH: " << dataStr.length() << endl;)

						DEBUGMSG(cout << "DONE READING" << endl;)

						MTCRBT<MTCpostData> pdSet;

						parseParms(dataStr.getValue(), pdSet);

						MTCpostData pd;

						pd.setParam("NTTPDATA");

						MTCpostData *pdPtr;

						if ((pdPtr=pdSet.find(pd))==NULL)
						{
							if (keepAlive==TRUE)
							{
								DEBUGMSG(cout << "INVALID POST REQUEST - There is no NTTPDATA" << endl;)
								/*
								postResponse(response, newfd);
								sendStop(newfd);
								closeAndExit(newfd);
								*/
								continue;
							}
							else
							{
								response.add("ERROR: INVALID NTTPD POST REQUEST");
							}
						}
						else
						{
							if (pdPtr->getValue()=="INITIAL")
							{
								DEBUGMSG(cout << "SENDING NICETEXT" << endl;)
								sendNiceText(response, pdSet);
								if (keepAlive==TRUE)
								{
									
									postResponse(response, newfd);
									sendStop(newfd);
									closeAndExit(newfd);
								}
							}
							else if (pdPtr->getValue()=="NICETEXT")
							{
								DEBUGMSG(cout << "Sending Scrambled Text" << endl;)

								sendScramText(response, pdSet);
								DEBUGMSG(cout << "DONE Sending Scrambled Text" << endl;)

								if (keepAlive==TRUE)
								{
									postResponse(response, newfd);
									sendStop(newfd);

									closeAndExit(newfd);
								}
							}
							else
							{
								DEBUGMSG(cout << "BAD POST - SENDING INITIAL" << endl;)
								response.add("POST REQUEST ACCEPTED<BR>Post type:");
								response.add(pdPtr->getValue());
								response.add("<BR>");

								sendInitial(response);
							}	
						}
					}
					else
					{
						// any other requests are errors...

						DEBUGMSG(cout << "Confused...!!!" << endl;)
						// exit!!!

						sendStop(newfd);
						closeAndExit(newfd);

						if (keepAlive==TRUE)
						{
							DEBUGMSG(cout << "CONTINUING..." << endl;)
							goto continue_loop;
						}
						else
						{
							response.add("ERROR - I AM CONFUSED ABOUT THIS REQUEST!");
						}
					}
				}

				if (keepAlive==TRUE)
				{
					postResponse(response, newfd);
					sendStop(newfd);
				}
				else
				{
					postResponse(response, newfd);
					closeAndExit(newfd);
				}

continue_loop:
				response.clear();
			}

			closeAndExit(newfd);

		}

		// parent process again...

		close(newfd);
	}

	exit(0);
}

// send the initial page

BOOL sendInitial(MTCstringList &os)
{
	DEBUGMSG(cout << "INITIAL REQUEST." << endl;)
 	os.add("<HTML> \
<HEAD> \
<TITLE>NICETEXT Primary Input Screen</TITLE> \
</HEAD> \
<BODY BGCOLOR=\"#AAAAAA\"> \
<H2>NICETEXT Web-Sampler &copy; 1998  -  Primary Input Screen</H2>\
<P>The purpose of this demo is to convert your message into something \
completely different. The same message can be changed into many\
different texts - even with the same style and dictionary.  What's more, \
you will have the opportunity to convert the <I>nicetext</I> back into\
your original message!</P> \
<FORM METHOD=POST ACTION=\"nicetext?\"> \
<INPUT TYPE=HIDDEN NAME=NTTPDATA VALUE=\"INITIAL\">\
<H3>Enter a Message:</H3>\
<P>\
<TEXTAREA NAME=PLAINTEXT COLS=80 ROWS=12></TEXTAREA> \
</P> \
<H3>Select the <B>NICETEXT</B> Style:\
<SELECT NAME=STYLE> \
<OPTION VALUE=\"Shakespeare\"> Shakespeare \
<OPTION VALUE=\"Asoep's Fables\"> Asoep's Fables \
<OPTION VALUE=\"Wizard of OZ\"> Wizard of OZ \
</SELECT> \
<INPUT TYPE=CHECKBOX NAME=BIGDICT VALUE=\"YES\"> Use a larger dictionary\
</H3>\
</P>\
<INPUT TYPE=SUBMIT VALUE=\" Convert my message \"> \
<INPUT TYPE=RESET  VALUE=\" Clear \"> \
</FORM> \
<P>For more information please see the <A HREF=\"http://www.ctgi.net/nicetext/\">Official NICETEXT Home Page</A></P> \
</BODY> \
</HTML>");

	return TRUE;
}

// send a file

BOOL sendFile(MTCstringList &os, const char *file, BOOL wordWrap)
{
	if (strncmp(file, tmpDir.c_str(), tmpDir.length())!=0)
	{
		os.add("ERROR:  I cannot allow you to view this file.");
		os.add("\n(");
		os.add(file);
		os.add(")\n");
		cerr << "User attempted to read: " << file << endl;
	}

	if (file==NULL)
	{
		return FALSE;
	}
	else
	{
		ifstream readFile(file);

		if (readFile.bad())
		{
			os.add("ERROR - I COULD NOT FIND THAT URL");
			return FALSE;
		}

		char c;
		MTCstring lineBuffer;

		while (!readFile.bad()&&!readFile.eof())
		{
			readFile.get(c);

			if ((c==' ')&&(wordWrap==TRUE)&&(lineBuffer.length()>MAX_COLUMN))
			{
				lineBuffer+=" \n";
				os.add(lineBuffer);
				lineBuffer="";
			}
			else
			{
				lineBuffer += c;

				if (c=='\n')
				{
					os.add(lineBuffer);
					lineBuffer="";
				}
			}
		}

		os.add(lineBuffer);
	}

	return TRUE;
}

// parse the parameters from a POST operation

BOOL parseParms(const MTCstring & aStr, MTCRBT<MTCpostData> &pd)
{
	DEBUGMSG(cout << "Parsing Parameters...for {" << aStr << "}[" << aStr.length() << "]" << endl;)

	pd.clear();

	const char *h=aStr.c_str();

	if ((h==NULL)||(*h==(char)NULL))
	{
		DEBUGMSG(cout << "DONE Parsing Parameters...the string length is zero" << endl;)
		return FALSE;
	}

	MTCpostData newData;
	const char *t;
	char value[aStr.length()+1];
	BOOL done=FALSE;

	do
	{
		// read parameter name

		for(t=h; ((*t!='\0')&&(*t!='=')&&(*t!='\n')&&(*t!='\r')); t++);

		// did we find one?

		if ((t-h)>0)
		{
			// copy the parameter name

			char parm[(t-h)];
	 
			strncpy(parm, h, (t-h));

			parm[(t-h)] = '\0';

			newData.setParam(parm);

			DEBUGMSG(cout << "parm:" << newData.getParam() << endl;)

			// is this the end of the data?
		
			if ((*t=='\0')||(*t=='\n')||(*t=='\r'))
			{
				pd.add(newData);
				done=TRUE;
			}
			else
			{
				// is this a parameter we care about?
			
				if ((newData.getParam()!="STYLE")
				&&  (newData.getParam()!="NTTPDATA")
				&&  (newData.getParam()!="PLAINTEXT")
				&&  (newData.getParam()!="BIGDICT"))
				{
					DEBUGMSG(cout << "WE DON'T CARE ABOUT:" << newData.getParam() << endl;)
					// if not then skip the value!

					for(t=h; ((*t!='\0')&&(*t!='\n')&&(*t!='\r')&&(*t!='&')); t++);
				}
				else
				{
					// read parameter value 

					char *v=value;

					int len=0;

					h=(t+1);

					for(t=h; ((*t!='\0')&&(*t!='\n')&&(*t!='\r')&&(*t!='&')); t++)
					{
						len++;
						switch (*t) {
						case '+':
							*v++=' ';
							break;
						case '%':
							if ((*(++t)!='\0')&&(*(++t)!='\0'))
							{
								*v++=(char)hexdigit(*(t-1))*16+hexdigit(*t);
								len+=2;
							}
							else
							{
								cout << getpid() << " : ERROR: I COULD NOT READ THE REST OF THE HEX DIGITS!" << endl;
								done=TRUE;
							}
							break;
						default:
							*v++=*t;
							break;
						}
					}
	
					*v='\0';
	
					newData.setValue(value);
	
					DEBUGMSG(cout << "value(" << newData.getValue().length() << ", " << len << "):" << newData.getValue() << endl;)

					pd.add(newData);
				}
	
				if ((*t=='\0')||(*t=='\n')||(*t=='\r'))
				{
					done=TRUE;
				}
	
				h=(t+1);
	
				if ((*h=='\0')||(*t=='\n')||(*t=='\r'))
				{
					done=TRUE;
				}
			}
		}
		else 
		{
			// we are all done!
			done=TRUE;
		}
	} while (done==FALSE);

#ifdef DEBUG
	DEBUGMSG(cout << "DONE Parsing Parameters..." << endl;)

	pd.forEach(dumpPostData);
#endif

	return TRUE;
}

// dump post data to stdout

void dumpPostData(MTCpostData *aPD)
{
	if (aPD!=NULL)
	{
		DEBUGMSG(cout << aPD->getParam() << "=" << aPD->getValue() << endl;)
	}
	else
	{
		DEBUGMSG(cout << "* * * * * ERROR: NULL VALUE FOR POST DATA PTR" << endl;)
	}
}

// return the integer value for a hex digit

int hexdigit(char c)
{
	if ((c>='0')&&(c<='9'))
	{
		return (9-('9'-c));
	}
	if ((c>='A')&&(c<='F'))
	{
		return (15-('F'-c));
	}
	if ((c>='a')&&(c<='f'))
	{
		return (15-('f'-c));
	}

	return 0;
}

// send nice text from the plaintext

BOOL sendNiceText(MTCstringList &os, MTCRBT<MTCpostData> &pdSet)
{
	os.add("<HTML> \
<HEAD> \
<TITLE>NICETEXT Output Screen</TITLE> \
</HEAD> \
<BODY BGCOLOR=\"#AAAAAA\"> \
<H2>NICETEXT Web-Sampler &copy; 1998  -  <I>Nicetext</I> Output Screen</H2>\
<P>This is the result of the <I>nicetext</I> operation on the message\
you selected.  If you <I>scramble</I> the unmodified text below then  \
the software will recover your original message on the next page.\
But, if you modify the text - who knows what you will end up with?  Also,\
if you are not satisfied with the quality of the <I>nicetext</I> then \
use the reload button on your browser for another try!</P>\
<FORM METHOD=POST ACTION=\"scramble?\"> \
<INPUT TYPE=HIDDEN NAME=NTTPDATA VALUE=\"NICETEXT\">\
<P>\
<H3>Here is the <I>NICETEXT</I>:</H3>\
<TEXTAREA NAME=PLAINTEXT COLS=80 ROWS=12>\
");

	MTCstring plainTextFileName(tempnam(tmpDir.c_str(), "nttp-pt"));

	ofstream plainTextFile(plainTextFileName.c_str());

	MTCpostData pd;
	pd.setParam("STYLE");
	MTCpostData *ptPtr=pdSet.find(pd);

	MTCstring formStyle;

	char c='S';

	if (ptPtr!=NULL)
	{
		c=ptPtr->getValue().c_str()[0];
		formStyle=ptPtr->getValue();
	}

	MTCstring dictParm(workDir), typeParm(workDir), modelParm(workDir);

	switch (c) {
	case 'A':
		dictParm+="fabl";
		dictParm+=DICTPARM;
		typeParm+="fabl";
		typeParm+=TYPEPARM;
		modelParm+="fabl";
		modelParm+=MODELPARM;
		break;
	case 'S':
		dictParm+="shak";
		dictParm+=DICTPARM;
		typeParm+="shak";
		typeParm+=TYPEPARM;
		modelParm+="shak";
		modelParm+=MODELPARM;
		break;
	default:
		dictParm+="wizoz";
		dictParm+=DICTPARM;
		typeParm+="wizoz";
		typeParm+=TYPEPARM;
		modelParm+="wizoz";
		modelParm+=MODELPARM;
		break;
	};

	MTCstring formDict;
	pd.setParam("BIGDICT");
	ptPtr=pdSet.find(pd);

	if (ptPtr!=NULL)
	{
		dictParm=workDir;
		dictParm+="mstr";
		dictParm+=DICTPARM;
		formDict=" checked ";
	}

	pd.setParam("PLAINTEXT");
	ptPtr=pdSet.find(pd);

	if (ptPtr==NULL)
	{
		os.add("SORRY, I COULD NOT SEND THE PLAINTEXT TO THE NICETEXT SYSTEM<BR>");
	}
	else
	{
		if (ptPtr->getValue().length()==0)
		{
			plainTextFile << " " << endl;
		}
		else
		{
			plainTextFile << ptPtr->getValue() << endl;
		}

		plainTextFile.flush();
		plainTextFile.close();

		MTCbitsToTextModel *converter= new MTCbitsToTextModel;

		if (converter==NULL)
		{
			os.add("SORRY, I DID NOT HAVE ENOUGH MEMORY TO CREATE THE CONVERTER OBJECT");
		}
		else 
		if (converter->openInputName(plainTextFileName.c_str())==FALSE)
		{
			os.add("SORRY, I COULD NOT RECEIVE THE PLAINTEXT FROM THE NICETEXT SYSTEM<BR>");
		}
		else 
		{
			converter->setStatFreq(0);	// do not show stats

			MTCstring niceTextFileName(tempnam(tmpDir.c_str(), "nttp-nt"));

			if (converter->openOutputName(niceTextFileName)==FALSE)
			{
				os.add("SORRY, I COULD NOT OPEN THE PLAINTEXT DATABASE<BR>");
			}
			else 
			if (converter->openTypeName(typeParm)==FALSE)
			{
				os.add("SORRY, I COULD NOT OPEN THE TYPE DATABASE<BR>");
				os.add(typeParm);
			}
			else if (converter->openModelName(modelParm)==FALSE)
			{
				os.add("SORRY, I COULD NOT OPEN THE MODEL DATABASE<BR>");
			}
			else if (converter->openDictName(dictParm, TRUE)==FALSE)
			{
				os.add("SORRY, I COULD NOT OPEN THE DICTIONARY DATABASE<BR>");
			}
			else 
			{
				converter->doIt();
				delete converter;
				converter=NULL;
				sendFile(os, niceTextFileName.c_str(), TRUE);
				unlink(niceTextFileName.c_str());
				unlink(plainTextFileName.c_str());
			}
		}
	}

	os.add("</TEXTAREA> \
</P> \
<H3>\
<INPUT TYPE=HIDDEN NAME=STYLE VALUE=\"");

	os.add(formStyle.c_str());

	os.add("\">Style:");

	os.add(formStyle.c_str());

	os.add("\n");

	if (formDict.length()!=0)
	{
		os.add("... with a larger dictionary\
<INPUT TYPE=HIDDEN NAME=BIGDICT VALUE=\"YES\">");
	}

	os.add("</H3>\
<INPUT TYPE=SUBMIT VALUE=\" Scramble this message \"> \
<INPUT TYPE=RESET  VALUE=\" Reset \"> \
</FORM> \
</BODY>\
</HTML>\
");
}

// send a stop request for keep-alive functions

BOOL sendStop(int fd)
{
	DEBUGMSG(cout << "SENDING STOP" << endl;)

#ifdef MSG_EOF
	if (send(fd, "\n", 1, MSG_EOF)==-1)
#else
	if (send(fd, "\n", 1, 0)==-1)
#endif
	{
		return FALSE;
	}

	return TRUE;
}

// send scrambled text from the nicetext

BOOL sendScramText(MTCstringList &os, MTCRBT<MTCpostData> &pdSet)
{
	os.add("<HTML> \
<HEAD> \
<TITLE>SCRAMBLED Output Screen</TITLE> \
</HEAD> \
<BODY BGCOLOR=\"#AAAAAA\"> \
<H2>NICETEXT Web-Sampler &copy; 1998  -  <I>Scrambled</I> Output Screen</H2>\
<P>This page contains the <I>scrambled</I> version of the <I>nicetext</I>\
on the previous page.  If you did not modify the <I>nicetext</I> then this\
should be your original message! (If it does NOT match your original message\
then rest assured that it is because of the poor implementation of HTTP/1.0\
rather than a problem with the basic transformation software.)</P>\
<H3>Here is the SCRAMBLED Text:</H3> \
<FORM METHOD=POST ACTION=\"index.html\"> \
<INPUT TYPE=HIDDEN NAME=NTTPDATA VALUE=\"INITIAL\">\
<P>\
<TEXTAREA NAME=PLAINTEXT COLS=80 ROWS=12 READONLY=\"YES\">\
");

	MTCstring plainTextFileName(tempnam(tmpDir.c_str(), "nttp-pt"));

	ofstream plainTextFile(plainTextFileName.c_str());

	MTCpostData pd;
	pd.setParam("STYLE");
	MTCpostData *ptPtr=pdSet.find(pd);

	MTCstring formStyle;

	char c='S';

	if (ptPtr!=NULL)
	{
		c=ptPtr->getValue().c_str()[0];
		formStyle=ptPtr->getValue();
	}

	MTCstring dictParm(workDir), typeParm(workDir);

	switch (c) {
	case 'A':
		dictParm+="fabl";
		dictParm+=DICTPARM;
		typeParm+="fabl";
		typeParm+=TYPEPARM;
		break;
	case 'S':
		dictParm+="shak";
		dictParm+=DICTPARM;
		typeParm+="shak";
		typeParm+=TYPEPARM;
		break;
	default:
		dictParm+="wizoz";
		dictParm+=DICTPARM;
		typeParm+="wizoz";
		typeParm+=TYPEPARM;
		break;
	};

	pd.setParam("BIGDICT");
	ptPtr=pdSet.find(pd);

	if (ptPtr!=NULL)
	{
		dictParm=workDir;
		dictParm+="mstr";
		dictParm+=DICTPARM;
	}

	pd.setParam("PLAINTEXT");
	ptPtr=pdSet.find(pd);

	if (ptPtr==NULL)
	{
		os.add("SORRY, I COULD NOT SEND THE PLAINTEXT TO THE SCRAMBLE SYSTEM");
	}
	else
	{
		if (ptPtr->getValue().length()==0)
		{
			plainTextFile << " " << endl;
			DEBUGMSG(cout << "BLANK SUBMITTED!" << endl;)
		}
		else
		{
			plainTextFile << ptPtr->getValue() << endl;
		}

		plainTextFile.flush();
		plainTextFile.close();
	
		MTCtextToBits *converter= new MTCtextToBits;

		if (converter==NULL)
		{
			os.add("SORRY, I DID NOT HAVE ENOUGH MEMORY TO CREATE THE CONVERTER OBJECT");
		}
		else 
		if (converter->openInputName(plainTextFileName.c_str())==FALSE)
		{
			os.add("SORRY, I COULD NOT RECEIVE THE PLAINTEXT FROM THE NICETEXT SYSTEM<BR>");
		}
		else 
		{
			converter->setVerboseMode(FALSE);

			MTCstring niceTextFileName(tempnam(tmpDir.c_str(), "nttp-nt"));

			if (converter->openOutputName(niceTextFileName)==FALSE)
			{
				os.add("SORRY, I COULD NOT OPEN THE PLAINTEXT DATABASE<BR>");
			}
			else 
			if (converter->openTypeName(typeParm)==FALSE)
			{
				os.add("SORRY, I COULD NOT OPEN THE TYPE DATABASE<BR>");
			}
			else if (converter->openDictName(dictParm, TRUE)==FALSE)
			{
				os.add("SORRY, I COULD NOT OPEN THE DICTIONARY DATABASE<BR>");
			}
			else 
			{
				converter->doIt();
				delete converter;
				converter=NULL;
				sendFile(os, niceTextFileName.c_str());
				unlink(niceTextFileName.c_str());
				unlink(plainTextFileName.c_str());
			}
		}
	}

	os.add("\
</TEXTAREA> \
</P>\
</FORM> \
<P>\
<A HREF=\"start.html\">GO BACK TO START PAGE...</A>\
</P>\
</BODY>\
</HTML>\
");
}

// kill any remaining zombie processes

void killZombies(int)
{
	while(waitpid(-1,NULL,WNOHANG)>0);
}


// select a random number seed according to time...

void randomize()
{
        struct timeval t;
        struct timezone tz; 

        if (gettimeofday(&t, &tz)==-1)
        {
                errorMsg(EMT_WARNING, "random seed generation failed...no big de\
al though...", "randomize()");
        }

        srandom(t.tv_sec|t.tv_usec);
}

// close file and exit (child process only...)

void closeAndExit(int fd)
{
	close(fd);
	exit(0);
}

// post the response to the socket

BOOL postResponse(MTCstringList &os, int fd)
{
	int l=os.length();
	
	MTCstring header("HTTP/1.0 200 OK\nServer: Nicetext/0.1.0\nContent-type: text/html\nConnection: close\nContent-length: ");

	char tempStr[20];

	sprintf(tempStr, "%d\n\n", l); 
	header+=tempStr;

	DEBUGMSG(cout << "RESPONSE:" << endl;)
	DEBUGMSG(cout << header;)
	DEBUGMSG(cout << os.getValue().c_str() << endl;)

	DEBUGMSG(cout << "END OF RESPONSE" << endl;)

	send(fd, header.c_str(), header.length(), 0);

	if (l>0)
	{
		int blocks=l/MAX_SEND_BLOCK;
		int remainder=l%MAX_SEND_BLOCK;

		for(int i=0; i<blocks; i++)
		{
			send(fd, &os.getValue().c_str()[i*MAX_SEND_BLOCK], MAX_SEND_BLOCK, 0);
		}

		if (remainder!=0)
		{
			send(fd, &os.getValue().c_str()[l-remainder], remainder, 0);
		}
	}

	return TRUE;
}

// read a string buffer from the socket

MTCstring & recvStrBuf(int fd, MTCstring &bufStr)
{
	DEBUGMSG(cout << "recvStrBuf() START" << endl;)

	char readBuf[MAX_BUFFER+1];
	ssize_t size;

	if ((size=recv(fd, readBuf, MAX_BUFFER, 0))==-1)
	{
		perror("recvStrBuf()");
		bufStr="";
	}
	else
	{
		readBuf[size]=(char)NULL;
		bufStr=readBuf;
	}

	DEBUGMSG(cout << "size of recv(): " << size << endl;);

	DEBUGMSG(cout << "bufstr:{" << bufStr << "}[" << bufStr.length() << "]" << endl;)
	DEBUGMSG(cout << "recvStrBuf() END" << endl;)

	return bufStr;
}

// strstr case insensitive

const char * noCaseStrStr(const char *big, const char *little, BOOL prefix)
{
	DEBUGMSG(cout << "start NOCASESTRSTR(" << little << ") " << strlen(big) << "," << strlen(little) << "," << prefix << endl;)

	int bigLen = strlen(big);
	int littleLen = strlen(little);
	MTCstring littleStr(little);
	littleStr.toLower();
	int score;

#ifdef DEBUG
	int loop=0;
#endif DEBUG

	for(int i=0; (((prefix==FALSE)&&(i<bigLen))||((prefix==TRUE)&&(i==0))); i++)
	{
		score=0;

		for(int j=0; ((j<littleLen)&&((i+j)<bigLen)); j++)
		{
#ifdef DEBUG
			loop++;
#endif DEBUG

			if ((tolower(big[i+j])==(littleStr.c_str()[j])))
			{
				score++;
			}
			else
			{
				break;
			}
		}

		if (score==littleLen)
		{
			DEBUGMSG(cout << "end NOCASESTRSTR(" << little << ") FOUND IT IN: " << loop << " loops " << endl;)
			return &big[i];
		}
	}

	DEBUGMSG(cout << "end NOCASESTRSTR(" << little << ") NOTHING FOUND IN: " << loop << " loops " << endl;)

	return NULL;
}

void usage()
{
	cerr << "Usage: nttpd [-b dataBaseDir] [-p port] [-t tempDir] [-w]" << endl;
	cerr << "\t-b\tDirectory where dictionaries & style sources exist." << endl;
	cerr << "\t\t(default is current directory)" << endl;  
	cerr << "\t-t\tTemp directory where work files are written" << endl;
	cerr << "\t\t(default is /tmp )" << endl;  
	cerr << "\t-p\tPort # to listen() for connections (default is 2119)" << endl;
	cerr << "\t-w\tAccept connections machines other than localhost"  << endl;

}
