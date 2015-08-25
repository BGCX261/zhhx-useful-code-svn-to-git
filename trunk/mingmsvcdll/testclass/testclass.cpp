// testclass.cpp : Defines the entry point for the DLL application.
//
/*
#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
*/

#include "testclass.h"


CTestClass::CTestClass()
{
	m_a = 0;
	m_b = 0;
}


CTestClass::CTestClass(int a, int b)
{
	m_a = a;
	m_b = b;
}

CTestClass::~CTestClass()
{	
}


int CTestClass::SetValue(int a, int b)
{
	m_a = a;
	m_b = b;

	return 0;
}


void CTestClass::GetValue(int& a, int& b)
{
	a = m_a;
	b = m_b;
}



