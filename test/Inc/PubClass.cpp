#include "stdafx.h"
#include "PubClass.h"

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwFlag = 0;
	DWORD dwProcessID = 0;
	HWND hTargetWnd = NULL;

	ENUM_WNDINFO* pWndInfo = (ENUM_WNDINFO*)lParam;
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