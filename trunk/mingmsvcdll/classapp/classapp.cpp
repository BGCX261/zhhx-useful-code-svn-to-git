// classapp.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "../testclass/testclass.h"


int main(int argc, char* argv[])
{
	CTestClass aTest;
	aTest.SetValue(10, 20);

	int e =40, f=50;
	aTest.GetValue(e,f);

	fprintf(stderr, "CTestClass:(%d, %d)\n", e, f);

	return 0;
}

