#include "stdafx.h"
#include "test.h"
#include "./Dialog/testDlg.h"

BEGIN_MESSAGE_MAP(CTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, CWinAppEx::OnHelp)
END_MESSAGE_MAP()

CTestApp::CTestApp()
{
	m_ulGdiPlusToken = NULL;
}

CTestApp theApp;
BOOL CTestApp::InitInstance()
{
	InitShellManager();
	GdiplusStartup(&m_ulGdiPlusToken, &m_GdiPlusStartupInput, NULL);

	CTestDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	
	return FALSE;
}

int CTestApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt) 
{
	return CWinAppEx::DoMessageBox(lpszPrompt, nType, nIDPrompt);
}

int CTestApp::ExitInstance()
{
	GdiplusShutdown(m_ulGdiPlusToken);
	return CWinAppEx::ExitInstance();
}