

#include "teststdcall.h"




int __stdcall TestStdcallFunc(int b)
{
	int a = 1;

	int c = 2;

	c = a + b;

	return c;
}


