#pragma once

#include "./ChildWnd/DlgTest1Wnd.h"
#include "./ChildWnd/DlgTest2Wnd.h"
#include "./ChildWnd/DlgTest3Wnd.h"
#include "./ChildWnd/DlgTest4Wnd.h"
#include "./ChildWnd/DlgTest5Wnd.h"

using namespace std;
/////////////////////////////////////////////////////////////////////////////
//
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

public:
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void		DoDataExchange(CDataExchange* pDX);     
	virtual BOOL		OnInitDialog();

	afx_msg void		OnPaint();
	afx_msg void		OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR		OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL				m_bInited;
	HICON				m_hIcon;
};

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog	ShowCaret
class CTestDlg : public CDialog
{
public:
	CTestDlg(CWnd* pParent = NULL);	
	virtual ~CTestDlg();

public:
	enum { IDD = IDD_TEST_DIALOG };
	
protected:
	HICON m_hIcon;
	virtual void		DoDataExchange(CDataExchange* pDX);	
	virtual BOOL		OnInitDialog();

	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnDestroy();
	
	afx_msg void		OnPaint();
	afx_msg HCURSOR		OnQueryDragIcon();
	afx_msg void		OnSysCommand(UINT nID, LPARAM lParam);
	
	afx_msg void		OnTcnSelchangeTabWndCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
	
protected:	
	BOOL				InitCtrl();
	BOOL				InitInfo();

	BOOL				InitWndCtrl();
	BOOL				InitWndInfo();

	BOOL				CreateChildWnd();
	void				DestroyChildWnd();

	void				Hook();
	void				UnHook();

protected:
	CTabCtrl			m_hTabCtrl;

 	CDlgTest1Wnd		m_hDlgTest1Wnd;
 	CDlgTest2Wnd		m_hDlgTest2Wnd;
	CDlgTest3Wnd		m_hDlgTest3Wnd;
	CDlgTest4Wnd		m_hDlgTest4Wnd;
	CDlgTest5Wnd		m_hDlgTest5Wnd;

private:
	CPtrArray			m_pArPage;
	int					m_nPrePage;	

	int					m_nIndex;
	int					m_nDefaultSel;
};