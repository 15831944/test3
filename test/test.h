#if !defined(AFX_TEST_H__AD32478F_2060_4857_AC5D_C96B8440FE5E__INCLUDED_)
#define AFX_TEST_H__AD32478F_2060_4857_AC5D_C96B8440FE5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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


#endif 
