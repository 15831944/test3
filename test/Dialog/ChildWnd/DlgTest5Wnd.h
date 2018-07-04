#pragma once
#include "afxwin.h"

using namespace std;
using namespace Gdiplus;
class CDlgTest5Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest5Wnd)
public:
	CDlgTest5Wnd(CWnd* pParent = NULL); 
	virtual ~CDlgTest5Wnd();

public:
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);    
	virtual BOOL					OnInitDialog();

	afx_msg void					OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL							InitCtrl();
	BOOL							InitInfo();

protected:
	CUserListBox					m_UserListbox;
};