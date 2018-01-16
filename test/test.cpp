#include "stdafx.h"
#include "test.h"
#include "testDlg.h"

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CTestApp::CTestApp()
{
}

CTestApp theApp;
BOOL CTestApp::InitInstance()
{
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CTestDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	
	GdiplusShutdown(gdiplusToken);
	return FALSE;
}

int CTestApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt) 
{
	return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
}

int CTestApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}