// langDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lang.h"
#include "langDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���


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
			HWND hWnd = FindWindowEx((HWND)wParam, NULL, NULL, _T("ȷ��"));
			if( hWnd )
			{
				SetWindowText(hWnd, _T("OK"));
			}
			HWND hWnd1 = FindWindowEx((HWND)wParam, NULL, NULL, _T("ȡ��"));
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

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// ClangDlg �Ի���




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


// ClangDlg ��Ϣ�������

BOOL ClangDlg::OnInitDialog()
{
// 	HINSTANCE hLangDll = LoadLibrary(_T("lang_english.dll"));
// 	g_hLangDllHandler = hLangDll;
// 	AfxSetResourceHandle(hLangDll);

	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//g_hLangDllHandler = GetModuleHandle(NULL);
	//LoadLangDLL(_T("D:\\test\\lang\\debug\\langdll.dll"), 0);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ClangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
