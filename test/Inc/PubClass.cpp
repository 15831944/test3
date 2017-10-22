#include "stdafx.h"
#include "PubClass.h"

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwFlag = 0;
	DWORD dwProcessID = 0;
	HWND hTargetWnd = NULL;

	HT_ENUM_WNDINFO* pWndInfo = (HT_ENUM_WNDINFO*)lParam;
	if (pWndInfo == NULL)
	{
		return FALSE;
	}

	::GetWindowThreadProcessId(hwnd, &dwProcessID);
	if (dwProcessID != pWndInfo->dwProcessID)
	{
		return TRUE;
	}

	CString s1;
	::GetWindowText(hwnd, s1.GetBuffer(MAX_PATH), MAX_PATH);

	if (s1 != _T("ÈÕÀú´°¿Ú²âÊÔ"))
	{
		return TRUE;
	}

	CWnd* pWnd = CWnd::FromHandle(hwnd);
	if (pWnd == NULL)
	{
		return FALSE;
	}

	dwFlag = pWnd->m_nFlags & WF_MODALLOOP;
	return TRUE;
}

int CALLBACK EnumFontProc(ENUMLOGFONTEX *lpelf,NEWTEXTMETRICEX *lpntm,DWORD nFontType,long lParam)
{
	CString strFontName;

	CWnd* pWnd = NULL;
	HT_TESTWND_MSG* pWndMsg = NULL;

	pWnd = (CWnd*)lParam;
	if (pWnd == NULL)
	{
		return 0;
	}

	strFontName = lpelf->elfLogFont.lfFaceName;
	if (strFontName == _T("") || strFontName.GetLength() <= 0)
	{
		return 0;
	}

	pWndMsg = new HT_TESTWND_MSG;
	if (pWndMsg == NULL)
	{
		return 0;
	}

	pWndMsg->bFlag = TRUE;
	pWndMsg->hWnd  = pWnd->GetSafeHwnd();
	pWndMsg->dwParam = GETSYSTEM_FONT;
	strcpy(pWndMsg->szValue, strFontName.GetBuffer(0));

	::PostMessage(pWnd->GetSafeHwnd(), WM_TEST2WND_CTRL, GETSYSTEM_FONT, (LPARAM)pWndMsg);
	return 1;
}