#pragma once

#include "../Control/DrawListBox.h"
#include "afxwin.h"

using namespace std;
class CDlgTest2Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest2Wnd)
public:
	CDlgTest2Wnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest2Wnd();

public:
	void					Init();
	void					InitCtrl();
	void					InitInfo();

	void					SetListWndInfo(LPCTSTR lpszText);

public:
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	virtual BOOL			OnInitDialog();
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

	afx_msg void			OnLbnSelchangeListTest();

	void					CreateImageList();

protected:
	CFont					m_font;
	CListBox				m_ListBox;		//CDragListBox
	CImageList				m_ImageList;
};
