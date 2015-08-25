// cdeclapp.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "../testcdecl/testcdecl.h"

int main(int argc, char* argv[])
{
	int a = TestCdeclFunc( 10 );


	fprintf(stderr, "TestCdeclFunc=%d\n", a);

	return 0;
}

