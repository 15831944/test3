#pragma once
#include "afxwin.h"
#include <atlconv.h>

#include "../inc/opengl_texture_draw_video.h"

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
	afx_msg void					OnBnClickedBtnDrawtest();

protected:
	opengl_texture_draw_video		m_openglDrawVideo1;
	opengl_texture_draw_video		m_openglDrawVideo2;

private:
	CString							m_strDefaultPath;
	CString							m_strAppPath;

	BOOL							m_bCameraOpen;	
};
