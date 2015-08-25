#include "test.h"

int test2(int a, int b)
{
	asm("jmp test1");
}

int main(int argc, char* argv[])
{
	printf("call test()\n");
	test();
	
	printf("call test1(2,3)\n");
	int res1 = test1(2,3);
	printf("test1(2,3)=%d\n", res1);
	
	printf("call test2(4,5)\n");
	int res2 = test2(4,5);
	printf("test2(4,5)=%d\n", res2);
	
	return 0;
}