#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

class CTestApp : public CWinAppEx
{
public:
	CTestApp();

public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	virtual int		DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR		m_ulGdiPlusToken;
	GdiplusStartupInput m_GdiPlusStartupInput;
};