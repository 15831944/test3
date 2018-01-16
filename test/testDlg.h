#pragma once

#include "Dialog/DlgTest1Wnd.h"
#include "Dialog/DlgTest2Wnd.h"
#include "Dialog/DlgTest3Wnd.h"
#include "Dialog/DlgTest4Wnd.h"
/////////////////////////////////////////////////////////////////////////////
// 
using namespace std;
class CAboutDlg : public CSkinDialog
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
	
	afx_msg void		OnPaint();
	afx_msg HCURSOR		OnQueryDragIcon();
	afx_msg void		OnSysCommand(UINT nID, LPARAM lParam);
	
	afx_msg void		OnTcnSelchangeTabWndctrl(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
	
protected:	
	void				Init();

	void				InitCtrl();
	void				InitInfo();

protected:
	CTabCtrl			m_hTabCtrl;

	CDlgTest1Wnd		m_hDlgTest1Wnd;
	CDlgTest2Wnd		m_hDlgTest2Wnd;
	CDlgTest3Wnd		m_hDlgTest3Wnd;
	CDlgTest4Wnd		m_hDlgTest4Wnd;

private:
	CPtrArray			m_pArPage;
	int					m_nPrePage;	

	int					m_nIndex;
};