// dllapp.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "../mingmsvcdll/teststdcall.h"
#include "../testcdecl/testcdecl.h"
#include "../testclass/testclass.h"
#include "../testdefault/testdefault.h"

int main(int argc, char* argv[])
{
	int a = TestCdeclFunc( 10 );

	int b = TestStdcallFunc( 20 );

	int c = TestDefaultFunc( 30 );

	CTestClass aTest;
	aTest.SetValue(10, 20);

	int e =40, f=50;
	aTest.GetValue(e,f);

	fprintf(stderr, "TestCdeclFunc=%d, TestStdcallFunc=%d, TestDefaultFunc=%d, CTestClass:(%d, %d)\n", a, b, c, e, f);

	return 0;
}

