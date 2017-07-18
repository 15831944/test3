#pragma once
#include "afxwin.h"
#include <atlconv.h>

#include "../Inc/audio/waveplayer.h"

using namespace std;
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
	DECLARE_MESSAGE_MAP()
	afx_msg void					OnBnClickedButton1();
	afx_msg void					OnBnClickedButton2();

protected:
	WavePlayer						m_hWavPlayer;
};
