#pragma once
#include "afxwin.h"

using namespace std;
class CDlgTest3Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest3Wnd)
public:
	CDlgTest3Wnd(CWnd* pParent = NULL); 
	virtual ~CDlgTest3Wnd();

public:
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);    
	virtual BOOL					OnInitDialog();
	DECLARE_MESSAGE_MAP()

	afx_msg void					OnBnClickedBtnTest1();
	afx_msg void					OnBnClickedBtnTest2();
	afx_msg void					OnBnClickedBtnTest3();

	afx_msg void					OnCbnSelchangeComboEnumdevice();

protected:

private:
	BOOL							m_bCameraOpen;	

	CString							m_strAppPath;
	CString							m_strDefaultPath;	
};
