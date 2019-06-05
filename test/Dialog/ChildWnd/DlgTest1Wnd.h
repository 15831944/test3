#pragma once
#include "afxwin.h"

#include "../../Control/WndCtrl/TipWnd.h"

using namespace std;
class CDlgTest1Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest1Wnd)
public:
	CDlgTest1Wnd(CWnd* pParent = NULL);
	virtual ~CDlgTest1Wnd();

public:
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual BOOL			OnInitDialog();
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

	afx_msg void  			OnSize(UINT nType, int cx, int cy);
	afx_msg void  			OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void			OnBnClickedButton1();
	afx_msg void			OnBnClickedButton2();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL					InitCtrl();
	BOOL					InitInfo();

	void					SetWndControlLayout();

protected:
	BOOL					m_bInited;
	CTipWnd					m_TipWnd;
};