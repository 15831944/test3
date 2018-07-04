#pragma once

typedef enum tagCameraPTZOrg{
	CAMERA_PTZ_EMPTY = 0,
	CAMERA_PTZ_NORMAL,
	CAMERA_PTZ_LEFT,
	CAMERA_PTZ_LEFT_BOTTOM,
	CAMERA_PTZ_BOTTOM,
	CAMERA_PTZ_RIGHT_BOTTOM,
	CAMERA_PTZ_RIGHT,
	CAMERA_PTZ_RIGHT_TOP,
	CAMERA_PTZ_TOP,
	CAMERA_PTZ_LEFT_TOP
}CAMERA_PTZ_ORG;

using namespace Gdiplus;
class CDlgPTZControlWnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPTZControlWnd)
public:
	CDlgPTZControlWnd(CWnd* pParent = NULL);
	virtual ~CDlgPTZControlWnd();
	
public:
	enum { IDD = IDD_PTZ_DIALOG };
	
protected:
	virtual void					DoDataExchange(CDataExchange* pDX);   
	virtual BOOL					PreTranslateMessage(MSG* pMsg);
	virtual BOOL					OnInitDialog();

	afx_msg void					OnPaint();

	afx_msg void					OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void					OnMouseLeave();

	afx_msg void					OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void					OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL							InitCtrl();
	BOOL							InitInfo();

	BOOL							InitWndSkin();
	BOOL							CreateChildWnd();
	
	BOOL							SetPTZWndImage(CAMERA_PTZ_ORG emPTZOrg, LPCTSTR lpszStylePath);
	BOOL							DrawPTZWndImage(Bitmap *pPTZImage, POINT ptMousePos);

	BOOL							SetLayeredWnd(int iTransparent, CDC *pDC, Bitmap *pPTZImage);
	BOOL							UpdateLayeredWnd(CDC *pDC, int iWidth, int iHeight, BLENDFUNCTION stcBlend);

private:
	BOOL							m_bRefreshSkin;
	BOOL							m_bTracking;

	CString							m_strAppPath;
	Bitmap*							m_pPTZImage;
};