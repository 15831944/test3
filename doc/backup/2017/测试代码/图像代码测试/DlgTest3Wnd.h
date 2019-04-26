#pragma once
#include "afxwin.h"
#include <atlconv.h>

#include "../inc/CaptureWndVideo.h"
#include "../Inc/OCREngine.h"




using namespace cv;
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
	CComboBox						m_hEnumDeviceList;
	CStatic							m_hVideoWnd;

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);    
	virtual BOOL					OnInitDialog();
	afx_msg void					OnCbnSelchangeComboEnumdevice();
	
	afx_msg void					OnBnClickedButton1();
	afx_msg void					OnBnClickedButton2();

	afx_msg void					OnMenuCapture();
	afx_msg void					OnMenuGlobalthreshold();
	afx_msg void					OnMenuLocalthreshold();

	afx_msg void					OnMenuGrabcutTest();
	afx_msg void					OnMenuContourTest();

	afx_msg void					OnMenuKnnOcr();
	afx_msg void					OnMenuTesseractOcr();
	afx_msg void					OnMenuBarcodeDemo();

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT					OnEnumDevice(WPARAM wParam, LPARAM lParam);

protected:
	BOOL							PopupMenu(UINT nMenuID, UINT nSubMenuPos, CRect *prcRect, CWnd *pParentWnd);

protected:
 	CCaptureWndVideo				m_hcapWndVideo;
 	COcrEngine						m_hOcrEngine;

private:
	std::vector<DeviceName>			m_vecDeviceList;
	CString							m_strDefaultPath;
	CString							m_strAppPath;
};
