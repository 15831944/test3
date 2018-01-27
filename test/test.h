#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		

class CTestApp : public CWinApp
{
public:
	CTestApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	DECLARE_MESSAGE_MAP()
};