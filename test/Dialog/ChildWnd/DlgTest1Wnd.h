#pragma once
#include "afxwin.h"

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

	afx_msg void			OnDestroy();
	afx_msg void			OnLbnSelchangeListTest();
	afx_msg LRESULT			OnTest1WndMessage(WPARAM wParam, LPARAM lParam);
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