#include "stdafx.h"
#include "test.h"
#include "./Dialog/testDlg.h"

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CTestApp::CTestApp()
{
	m_ulGdiPlusToken = NULL;
}

CTestApp theApp;
BOOL CTestApp::InitInstance()
{
	CTestDlg dlg;
	m_pMainWnd = &dlg;

	GdiplusStartup(&m_ulGdiPlusToken, &m_GdiPlusStartupInput, NULL);

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
	return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
}

int CTestApp::ExitInstance()
{
	GdiplusShutdown(m_ulGdiPlusToken);
	return CWinApp::ExitInstance();
}