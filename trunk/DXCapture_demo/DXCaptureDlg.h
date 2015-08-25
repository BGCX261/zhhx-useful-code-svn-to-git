// DXCaptureDlg.h : header file
//

#if !defined(AFX_DXCAPTUREDLG_H__FDFA0C6E_202A_4638_9D62_AA46A058EBCD__INCLUDED_)
#define AFX_DXCAPTUREDLG_H__FDFA0C6E_202A_4638_9D62_AA46A058EBCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VMR_Capture.h"
#include "Showpic.h"
/////////////////////////////////////////////////////////////////////////////
// CDXCaptureDlg dialog

class CDXCaptureDlg : public CDialog
{
// Construction
public:
	CDXCaptureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDXCaptureDlg)
	enum { IDD = IDD_DXCAPTURE_DIALOG };
	CComboBox	m_ctrlDevice;
	CShowpic	m_ctrlCaptureIMG;
	CStatic	m_ctrlLiveVideo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDXCaptureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CVMR_Capture m_VMRCap;
	

	// Generated message map functions
	//{{AFX_MSG(CDXCaptureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStartcap();
	afx_msg void OnInitcam();
	afx_msg void OnPause();
	afx_msg void OnRestart();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXCAPTUREDLG_H__FDFA0C6E_202A_4638_9D62_AA46A058EBCD__INCLUDED_)
