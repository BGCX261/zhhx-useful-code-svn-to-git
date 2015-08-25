// testwave.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "testwave.h"
#include "wave-test.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		os_sound_init();

		os_sound_start();

		char c;
		scanf("%c",&c);

		os_sound_close();
	}

	return nRetCode;
}
