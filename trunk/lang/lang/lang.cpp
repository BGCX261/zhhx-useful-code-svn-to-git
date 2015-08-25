// lang.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "lang.h"
#include "langDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ClangApp

BEGIN_MESSAGE_MAP(ClangApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// ClangApp ����

ClangApp::ClangApp()
{
	EnableHtmlHelp();

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� ClangApp ����

ClangApp theApp;

extern HINSTANCE g_hLangDllHandler;
// ClangApp ��ʼ��

HHOOK hCBT;

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch( nCode )
	{
	case HCBT_CREATEWND:
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
		else
		{
			AfxMessageBox(_T("no messagebox"));
		}
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




BOOL ClangApp::InitInstance()
{
	HINSTANCE hLangDll = NULL;
	LANGID langID ;
	//langID = GetSystemDefaultLangID();	
	langID = GetUserDefaultLangID();	
	switch( langID )
	{
		case 0x0409://English (United States)
			hLangDll = LoadLibrary(_T("lang_english.dll"));
			break;
		case 0x0804://Chinese (PRC)		
		default:
			hLangDll = GetModuleHandle(NULL);
	}
	
	g_hLangDllHandler = hLangDll;
	AfxSetResourceHandle(hLangDll);


	

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	installHook(m_hInstance);


	ClangDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	uninstallHook();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
