// DXCaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DXCapture.h"
#include "DXCaptureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define TIMER_DELAY 10
#define TIMER_ID 1
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDXCaptureDlg dialog

CDXCaptureDlg::CDXCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDXCaptureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDXCaptureDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDXCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDXCaptureDlg)
	DDX_Control(pDX, IDC_DEVICE, m_ctrlDevice);
	DDX_Control(pDX, IDC_CAPTURE, m_ctrlCaptureIMG);
	DDX_Control(pDX, IDC_LIVEVIDEO, m_ctrlLiveVideo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDXCaptureDlg, CDialog)
	//{{AFX_MSG_MAP(CDXCaptureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STARTCAP, OnStartcap)
	ON_BN_CLICKED(IDC_INITCAM, OnInitcam)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_BN_CLICKED(IDC_RESTART, OnRestart)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDXCaptureDlg message handlers

BOOL CDXCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	this->GetDlgItem (IDC_INITCAM)->EnableWindow(FALSE);
	this->GetDlgItem (IDC_STARTCAP)->EnableWindow(FALSE);
	
	if(m_VMRCap.EnumDevices(m_ctrlDevice.m_hWnd )>0)
	{
		m_ctrlDevice.SetCurSel (0);
		this->GetDlgItem (IDC_INITCAM)->EnableWindow(TRUE);
	}
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDXCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDXCaptureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDXCaptureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CDXCaptureDlg::OnTimer(UINT nIDEvent) 
{
	
	DWORD dwSize;
	
	dwSize=this->m_VMRCap.GrabFrame ();
	if(dwSize>0)
	{
		BYTE *pImage;
		this->m_VMRCap.GetFrame (&pImage);
		this->m_ctrlCaptureIMG .ShowImage (pImage);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDXCaptureDlg::OnStartcap() 
{
	this->KillTimer (TIMER_ID);
	this->SetTimer (TIMER_ID,TIMER_DELAY,NULL);
	
}

void CDXCaptureDlg::OnInitcam() 
{

	
	int iSel=-1;
	iSel=m_ctrlDevice.GetCurSel ();
	m_VMRCap.Init(iSel,m_ctrlLiveVideo.m_hWnd, CAM_WIDTH, CAM_HEIGHT);
	this->GetDlgItem (IDC_STARTCAP)->EnableWindow(TRUE);
	this->GetDlgItem (IDC_INITCAM)->EnableWindow(FALSE);
}

void CDXCaptureDlg::OnPause() 
{
	
	m_VMRCap.Pause ();

}

void CDXCaptureDlg::OnRestart() 
{
	
	m_VMRCap.Pause ();

	

	
}

void CDXCaptureDlg::OnDestroy() 
{
	this->KillTimer (TIMER_ID);
	CDialog::OnDestroy();
	
	
}
