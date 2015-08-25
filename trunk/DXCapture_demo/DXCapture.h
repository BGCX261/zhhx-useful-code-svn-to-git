// DXCapture.h : main header file for the DXCAPTURE application
//

#if !defined(AFX_DXCAPTURE_H__FB509783_01FB_45BE_B806_2211D6A6DB06__INCLUDED_)
#define AFX_DXCAPTURE_H__FB509783_01FB_45BE_B806_2211D6A6DB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define CAM_WIDTH  320
#define CAM_HEIGHT 240
#define CAM_BITS   24
/////////////////////////////////////////////////////////////////////////////
// CDXCaptureApp:
// See DXCapture.cpp for the implementation of this class
//

class CDXCaptureApp : public CWinApp
{
public:
	CDXCaptureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDXCaptureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDXCaptureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXCAPTURE_H__FB509783_01FB_45BE_B806_2211D6A6DB06__INCLUDED_)
