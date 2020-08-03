#include <stdio.h>

static char rcsid []  = "@(#)$Id: numsize.cc,v 1.2 1997/03/05 15:51:25 markc Exp $";

int main()
{
   unsigned long s1;
   unsigned int s2;
   unsigned short s3;

   printf("Size of unsigned long: %i\n", sizeof(s1));
   printf("Size of unsigned int: %i\n", sizeof(s2));
   printf("Size of unsigned short: %i\n", sizeof(s3));

   return(0);
}


