#pragma once

#include "afxwin.h"
#include <atlconv.h>

#include "../Inc/audio/waveplayer.h"
#include "../Control/Button/ImageButton.h"
#include "../Control/ListCtrl/ReportCtrl.h"
#include "../Control/TreeCtrl/StackedTreeCtrl.h"

using namespace std;
using namespace Gdiplus;
class CDlgTest4Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest4Wnd)

public:
	CDlgTest4Wnd(CWnd* pParent = NULL); 
	virtual ~CDlgTest4Wnd();

public:
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);    
	virtual BOOL					OnInitDialog();

	afx_msg void					OnPaint();

	afx_msg void					OnBnClickedButton1();
	afx_msg void					OnBnClickedButton2();
	afx_msg void					OnBnClickedButton3();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL							InitCtrl();
	BOOL							InitInfo();

	BOOL							InitList();
	BOOL							InitWndRect();

	BOOL							DrawRectFrameLine(CDC *pDC);
	
protected:
	CButton							m_btnTest3;
	CReportCtrl						m_ListTest;

protected:
	WavePlayer						m_hWavPlayer;
	CStackedTreeCtrl				m_hTreeCtrl;
};
