/*
 * rinfo - show information about RCS in nested dirtories
 * 
 * 1. Which files are locked and by whom?
 * 2. Which repositories do not have a member check-out (locked or unlocked).
 *
 * Mark T. Chapman
 * Feb, 1995
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef  UNIX
#include <sys\stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif UNIX
#include <dirent.h>

#ifndef MAXPATH
#define MAXPATH 80
#endif  /* MAXPATH */

#define MAXLOGNAME 	64		/* MAX LOGNAME size in rcs header */
#define SCREENWIDTH 	78 	/* used for non-scrolling status messages */

#define LOCKSTRING   "locks"
#define HEADSTRING   "head\t"
#define HEADCOUNT    20     /* used for head count */

#define MAXSNAPSHOTCOMMENT 512

short takeSnapShot;
char  snapShotComment[MAXSNAPSHOTCOMMENT+1];

#define SNAPSHOTFILE "SNAPSHOT.RNF"

/* function prototypes */
void showUsage(void);
char *cleanStr(char *);
int processDir(char *);
int processRCSdir(char *);
int processRCSfile(char *, char *, FILE *snapshot);
void printProgress(char *);

/* process a directory and all subdir's */

int processDir(char *dirname)
{
	DIR *dir;
	struct dirent *ent;
	char slash[2];

	/* is there an extra '/' at the end of the dirname? */

	if (strlen(dirname)>1)
	{
		if ((dirname[strlen(dirname)-1]=='/')||(dirname[strlen(dirname)-1]=='\\'))
		{
			char newDirName[MAXPATH+1];
			strcpy(newDirName, dirname);
			newDirName[strlen(newDirName)-1]=NULL;
			return processDir(newDirName);
		}
	}
	else
	{
		if (strlen(dirname)==0)
		{
			return -1;
		}
	}

	/* is the current dirname a directory? */

	if ((dir = opendir(dirname)) == NULL)
	{
		return -1;
	}

	printProgress(dirname);

	/* if this is a root directory then adjust slash picture */

	if ((strcmp(dirname, "/")==0)||(strcmp(dirname, "\\")==0))
	{
		strcpy(slash, "");
	}
	else
	{
		strcpy(slash, "/");
	}

	/* loop through directory entries */

	while ((ent = readdir(dir)) != NULL)
	{
		/* process subdirectories */

		if ((strcmp(ent->d_name, ".")!=0)&&(strcmp(ent->d_name, "..")!=0))
		{
			char subDir[MAXPATH+1];
			sprintf(subDir, "%s%s%s", dirname, slash, ent->d_name);

			/* if this is an RCS subdirectory then process */
			if (strcmp(ent->d_name, "RCS")==0)
			{
				processRCSdir(subDir);
			}
			else  /* otherwise recurse through subdirs */
			{
				processDir(subDir);
			}
		}
	}

	/* clean up and get out! */

	if (closedir(dir) != 0)
	{
		perror("Unable to close directory");
		exit(1);
	}

	return 0;
}

/* process an RCS subdirectory */

int processRCSdir(char *dirname)
{
	DIR *dir;
	struct dirent *ent;
	FILE *fp;
	char snapshotFile[MAXPATH+1];

	/* is the current dirname a directory? */

	if ((dir = opendir(dirname)) == NULL)
	{
		return -1;
	}

	/* open snapshot file */

	if (takeSnapShot)
	{
		sprintf(snapshotFile, "%s/"SNAPSHOTFILE, dirname);

		if ((fp=fopen(snapshotFile, "a"))==NULL)
		{
			perror(snapshotFile);
		}
		else
		{
			fprintf(fp, "\nSNAPSHOT into %s\n%s\n", snapshotFile, snapShotComment);
		}
	}
	else
	{
		fp=NULL;
	}

	/* loop through directory entries */

	while ((ent = readdir(dir)) != NULL)
	{
		/* process files */

		if ((strcmp(ent->d_name, ".")!=0)&&(strcmp(ent->d_name, "..")!=0)
			&&(strcmp(ent->d_name, SNAPSHOTFILE)!=0))
		{
			processRCSfile(dirname, ent->d_name, fp);
		}
	}

	/* clean up and get out! */

	if ((fp!=NULL)&&(fclose(fp)==EOF))
	{
		perror(snapshotFile);
	}

	if (closedir(dir) != 0)
	{
		perror("Unable to close directory");
	}

	return 0;
}

/* process and individual RCS repository */

int processRCSfile(char *dirname, char *file, FILE *snapshot)
{
	FILE *fp;
	int lockLetter;
	char lockString[] = LOCKSTRING;
	short done, found, locked;
	char c;
	char buffer[HEADCOUNT+1];
	char parentFile[MAXPATH+1];
	char rcsFile[MAXPATH+1];
	char parentDir[MAXPATH+1];
	int i, j;

	/* print the non-scrolling progress message */
	sprintf(rcsFile, "%s/%s", dirname, file);
	printProgress(rcsFile);

	/* open the file */

	if ((fp=fopen(rcsFile, "r"))==NULL)
	{
		perror(rcsFile);
		return -1;
	}

	/* get the head version */

	if (snapshot)
	{
		if (fread(buffer, HEADCOUNT, 1, fp)!=1)
		{
			perror(rcsFile);
		}
		else
		{
			buffer[HEADCOUNT]=NULL;
			if (strncmp(buffer, HEADSTRING, strlen(HEADSTRING))==0)
			{
				for(i=strlen(HEADSTRING)-1; i<=HEADCOUNT; i++)
				{
					if (buffer[i]==';')
					{
						buffer[i]=NULL;
						break;
					}
				}

				fprintf(snapshot, "%s \t%s\n", &buffer[strlen(HEADSTRING)], file);
			}
			else
			{
				printf("ERROR: %s does not appear to be a RCS-repository.\n", rcsFile);
				fprintf(snapshot, "Bad File: %s\n", rcsFile);
			}
		}
	}

	/* read through file to find keyword "locks" */

	locked=0;
	lockLetter=0;
	done=0;
	found=0;

	while (!done)
	{
		if ((c=fgetc(fp))==EOF)
		{
			fprintf(stderr, "* * * \nERROR: Could not find keyword '%s' in RCS file %s\n* * *\n", lockString, rcsFile);
			done=1;

			if (snapshot)
			{
				fprintf(snapshot, "* * * \nERROR: Could not find keyword '%s' in RCS file %s\n* * *\n", lockString, rcsFile);
			}
		}
		else
		{
			if (lockString[lockLetter]==c)
			{
				if (++lockLetter==strlen(lockString))
				{
					done=1;
					found=(fgetc(fp)!=';');
				}
			}
			else
			{
				lockLetter=0;
			}
		}
	}

	/* did we find the lock keyword? */

	if (found)
	{
		/* try to read the LOGNAME from after the lock string */
		char logName[MAXLOGNAME];
		int logPos;

		/* skip a character */

		fgetc(fp);
		c=fgetc(fp);
		logPos=0;

		while ((c!=EOF)&&(c!=':')&&(logPos<MAXLOGNAME))
		{
			logName[logPos++]=c;
			c=fgetc(fp);
		}

		logName[logPos]=NULL;

		/* we found who has the lock! */

		if (strlen(logName)>0)
		{
			printf("%s has a lock on %s\n", logName, rcsFile);
			locked=1;
			if (snapshot)
			{
				fprintf(snapshot, "%s has a lock on %s\n", logName, rcsFile);
			}
		}
	}

	/* is there an appropriate source in parent directory? */

	if (strlen(dirname)>3)
	{
		struct stat fstat;
		int slen; 
		strncpy(parentDir, dirname, (strlen(dirname)-3)); /* remove "RCS" from dir name */
		parentDir[strlen(dirname)-3]=NULL;
		sprintf(parentFile, "%s%s", parentDir, file);

		/* remove a ",v" extension */

		if ((slen=strlen(parentFile))>2)
		{
			if (strcmp(&parentFile[slen-2], ",v")==0)
				parentFile[slen-2]=NULL;
		}

		if (stat(parentFile, &fstat)==-1)
		{
			if (locked)
			{
				printf("* *\n WARNING: What happened to the locked-version of %s?\n* *\n", parentFile);
				if (snapshot)
				{
					fprintf(snapshot, "* *\n WARNING: What happened to the locked-version of %s?\n* *\n", parentFile);
				}
			}
			else
			{
				printf("%s is not checked out of the repository.\n", parentFile);

				if (snapshot)
				{
					fprintf(snapshot, "%s is not checked out of the repository.\n", parentFile);
				}
			}
		}
	}

	/* close the file */

	if (fclose(fp)==EOF)
	{
		perror(rcsFile);
	}

	return 0;
}

/* print a non-scrolling message to stderr */

void printProgress(char *message)
{
	char buffer[SCREENWIDTH+1];
	int i, l;
	l=strlen(message);

	strncpy(buffer, message, SCREENWIDTH);
	buffer[SCREENWIDTH]=NULL;

	if (l<SCREENWIDTH)
	{
		for(i=l; i<SCREENWIDTH; i++)
		{
			buffer[i]=' ';
		}
	}

	fprintf(stderr, "%s\r", buffer);
	fflush(stderr);

}

/* main */

int main(int argc,char *argv[])
{
	char dirName[MAXPATH+1];
	int i;

	printf("rinfo 1.1\n");

	strcpy(dirName, ".");
	strcpy(snapShotComment, "");
	takeSnapShot=0;

	for(i=1; i<argc; i++)
	{
		/* take a snapshot ? */
		if (strcmp(argv[i], "-s")==0)
		{
			#define MAXDATE 64
			#define MAXTIME 64
			#define MAXUSER 64
			#define MAXNOTE MAXSNAPSHOTCOMMENT-MAXDATE-MAXTIME-MAXUSER-1

			char getDate[MAXDATE+1];
			char getTime[MAXTIME+1];
			char getUser[MAXUSER+1];
			char getNote[MAXNOTE+1];

			fprintf(stderr, "Enter information for SNAPSHOT:\n");
			fprintf(stderr, "Today's date: ");
			fgets(getDate, MAXDATE, stdin);
			fprintf(stderr, "Current Time: ");
			fgets(getTime, MAXTIME, stdin);
			fprintf(stderr, "Your Name or User ID: ");
			fgets(getUser, MAXUSER, stdin);
			fprintf(stderr, "Version Notes: ");
			fgets(getNote, MAXNOTE, stdin);
			sprintf(snapShotComment, "\tWhen:\t %s %s\n\tWho:\t %s\n\tNotes:\t %s\n", cleanStr(getDate), cleanStr(getTime), cleanStr(getUser), cleanStr(getNote));

			takeSnapShot=1;
		}

		/* no other "-" parms are defined */
		else if (argv[i][0]=='-')
		{
			showUsage();
			return 0;
		}

		/* use it as a dirName */
		else
		{
			if (strcmp(dirName, ".")==0)
			{
				strcpy(dirName, argv[i]);
			}
			else
			{
				showUsage();
				return -1;
			}
		}
	}

	processDir(dirName);

	printProgress("");

	printf("rinfo Complete!\n");

	return (0);
}

/* print usage */

void showUsage()
{
	fprintf(stderr, "Usage: rinfo [-s] [dirname]\n");
	fprintf(stderr, "     -s: take snapshot\n");
	fprintf(stderr, "dirname: defaults to '.'\n");
}

char *cleanStr(char *aString)
{
	int i;

	if (aString!=NULL)
	{
		for(i=0; i<strlen(aString); i++)
		{
			if (aString[i]=='\n')
			{
				aString[i]=' ';
			}
		}
	}

	return aString;
}

