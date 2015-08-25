// funcobject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"


class CFunc1
{
public:
	template<class T>
	T operator()(T t1, T t2)
	{
		return t1+t2;
	}
};


class CFunc2
{
public:
	template<class T>
	T operator()(T t1, T t2)
	{
		return t1*2+t2*2;
	}
};


class CFunc3
{
public:
	template<class T>
	T operator()(T t1, T t2)
	{
		return t1*3 + t2*3;
	}
};


template<class T, class FUNC1, class FUNC2>
T Calc(T param1, FUNC1 func1, FUNC2 func2, T param2)
{
	T ret = func1(param1,param2) + func2(param1,param2);

	return ret;
}



int _tmain(int argc, _TCHAR* argv[])
{
	int c = 0;

	c = Calc(1, CFunc1(), CFunc2(), 2);

	int e = Calc(1, CFunc1(), CFunc3(), 3);

	int f = Calc(1, CFunc2(), CFunc3(), 4);

	printf("c = %d, e=%d, f=%d\n", c,e,f);
	
	int d = 0;

	scanf("%d", &d);

	return 0;
}

