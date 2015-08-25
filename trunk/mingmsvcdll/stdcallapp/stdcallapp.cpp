// stdcallapp.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "../mingmsvcdll/teststdcall.h"


int main(int argc, char* argv[])
{
	int b = TestStdcallFunc( 20 );

	fprintf(stderr, "TestStdcallFunc=%d\n", b);

	return 0;
}
