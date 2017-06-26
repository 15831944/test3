#pragma once
#include "afxwin.h"
#include <atlconv.h>


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

	afx_msg void					OnCbnSelchangeComboEnumdevice();
	afx_msg void					OnBnClickedBtnCaptureimage();

protected:
	CCaptureWndVideo				m_CameraVideo;

private:
	CString							m_strDefaultPath;
	CString							m_strAppPath;

	BOOL							m_bCameraOpen;
	
};
