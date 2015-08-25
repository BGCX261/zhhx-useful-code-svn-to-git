// langDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "lang.h"
#include "langDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


HINSTANCE g_hLangDllHandler = NULL;
bool LoadLangDLL(LPCTSTR strLangDir, LANGID nLangID)
{
	HINSTANCE hLangDll = LoadLibrary(strLangDir);
	if( hLangDll == NULL )
	{
		g_hLangDllHandler = GetModuleHandle(NULL);
		return false;
	}
	g_hLangDllHandler = hLangDll;
	return true;
}

CString GetResString(UINT uResID, WORD wLangID)
{
	CString str;
	str.LoadString(g_hLangDllHandler, uResID, wLangID);
	return str;
}

CString GetResString(UINT uResID)
{
// 	static TCHAR str[256] = {0};
//	int nNum = 255;
//	memset(str, 0, sizeof(str));
//	LoadString(g_hLangDllHandler, uResID, str, nNum);
//	return str;
//
	CString str;
	str.LoadString(g_hLangDllHandler, uResID);
	return str;
}



/*
HHOOK hCBT;

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch( nCode )
	{
	//case HCBT_CREATEWND:
	case HCBT_ACTIVATE:
		TCHAR strTitle[256] = {0};
		GetWindowText((HWND)wParam, strTitle, 255);
		if( _tcscmp(strTitle, _T("hint")) == 0 )
		{
			HWND hWnd = FindWindowEx((HWND)wParam, NULL, NULL, _T("确定"));
			if( hWnd )
			{
				SetWindowText(hWnd, _T("OK"));
			}
			HWND hWnd1 = FindWindowEx((HWND)wParam, NULL, NULL, _T("取消"));
			if( hWnd1 )
			{
				SetWindowText(hWnd1, _T("Cancel"));
			}
			return 0;
		}
// 		else
// 		{
// 			AfxMessageBox(_T("no messagebox"));
// 		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


void installHook(HINSTANCE h)
{
	hCBT = SetWindowsHookEx(WH_CBT, (HOOKPROC)CBTProc, h, 0);
}


void uninstallHook()
{
	UnhookWindowsHookEx(hCBT);
}


*/

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// ClangDlg 对话框




ClangDlg::ClangDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ClangDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ClangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClangDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_STARTMONITOR, &ClangDlg::OnBnClickedButtonStartmonitor)
END_MESSAGE_MAP()


// ClangDlg 消息处理程序

BOOL ClangDlg::OnInitDialog()
{
// 	HINSTANCE hLangDll = LoadLibrary(_T("lang_english.dll"));
// 	g_hLangDllHandler = hLangDll;
// 	AfxSetResourceHandle(hLangDll);

	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//g_hLangDllHandler = GetModuleHandle(NULL);
	//LoadLangDLL(_T("D:\\test\\lang\\debug\\langdll.dll"), 0);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void ClangDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void ClangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR ClangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ClangDlg::OnBnClickedButtonStartmonitor()
{
	// TODO: Add your control notification handler code here
	//::MessageBox(NULL, GetResString(IDS_START_HINT), GetResString(IDS_START_HINT_CAPTION), MB_YESNO );

	HINSTANCE h = GetModuleHandle(NULL);
//	installHook(h);

	MessageBox(GetResString(IDS_START_HINT), GetResString(IDS_START_HINT_CAPTION), MB_YESNO );
	
/*
	MSGBOXPARAMS mbp;
	mbp.cbSize = sizeof(MSGBOXPARAMS);
	mbp.hwndOwner = NULL;
	mbp.hInstance = g_hLangDllHandler;//GetModuleHandle(NULL);
	mbp.lpszText = GetResString(IDS_START_HINT);
	mbp.lpszCaption = GetResString(IDS_START_HINT_CAPTION);
	mbp.dwStyle = MB_YESNO;
	mbp.lpszIcon = NULL;
	mbp.dwContextHelpId = 0;
	mbp.lpfnMsgBoxCallback = 0;
	mbp.dwLanguageId = GetUserDefaultLangID();

	MessageBoxIndirect(&mbp);

*/
//	uninstallHook();

//	CDialog dlg(_T("aaaa"), NULL);
//	dlg.DoModal();
}
