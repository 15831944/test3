#pragma once
#include "afxwin.h"

using namespace std;
class CDlgTest2Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest2Wnd)
public:
	CDlgTest2Wnd(CWnd* pParent = NULL);
	virtual ~CDlgTest2Wnd();

public:
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual BOOL			OnInitDialog();
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

	afx_msg void			OnDestroy();
	afx_msg void			OnLbnSelchangeListTest();
	afx_msg LRESULT			OnTest2WndMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL					InitCtrl();
	BOOL					InitInfo();

	void					CreateImageList();

protected:
	CFont					m_font;
	CListBox				m_ListBox;		//CDragListBox
	CImageList				m_ImageList;

	CScrollWndMsg			m_dlgScrollWnd;
};