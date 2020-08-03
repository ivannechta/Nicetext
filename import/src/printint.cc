
static char rcsid []  = "@(#)$Id: printint.cc,v 1.3 1997/03/05 17:53:41 markc Exp $";

#include <iostream.h>

int main()
{
	cerr << "counting to stdout..." << endl;

	for(int i=0; i<16384+2048; i++)
		cout << i << endl;

	return 0;
}
