// chart.h : main header file for the chart application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CchartApp:
// See chart.cpp for the implementation of this class
//

class CchartApp : public CWinApp
{
public:
	CchartApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CchartApp theApp;