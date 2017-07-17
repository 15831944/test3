#if !defined(AFX_TESTDLG_H__F4D8A3C3_2AF6_45E0_B4FD_0A81D067D979__INCLUDED_)
#define AFX_TESTDLG_H__F4D8A3C3_2AF6_45E0_B4FD_0A81D067D979__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Inc/ETSLayout.h"

#include "Dialog/DlgTest1Wnd.h"
#include "Dialog/DlgTest2Wnd.h"
#include "Dialog/DlgTest3Wnd.h"
#include "Dialog/DlgTest4Wnd.h"
/////////////////////////////////////////////////////////////////////////////
// 
using namespace std;
using namespace ETSLayout;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	DECLARE_LAYOUT();
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);     
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
class CTestDlg : public CDialog
{
public:
	CTestDlg(CWnd* pParent = NULL);	

	DECLARE_LAYOUT();
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CTestDlg)
protected:
	virtual void		DoDataExchange(CDataExchange* pDX);	
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CTestDlg)
	virtual BOOL		OnInitDialog();
	afx_msg void		OnPaint();
	afx_msg HCURSOR		OnQueryDragIcon();
	afx_msg void		OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void		OnTcnSelchangeTabWndctrl(NMHDR *pNMHDR, LRESULT *pResult);

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

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_TESTDLG_H__F4D8A3C3_2AF6_45E0_B4FD_0A81D067D979__INCLUDED_)
