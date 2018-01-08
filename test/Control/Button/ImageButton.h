#pragma once

using namespace Gdiplus;
class CImageButton : public CButton
{
public:
	CImageButton();
	virtual ~CImageButton();
	
protected:
	virtual void 	PreSubclassWindow();
	virtual BOOL 	PreTranslateMessage(MSG* pMsg);
	virtual void 	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	afx_msg BOOL 	OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH 	CtlColor(CDC* pDC, UINT nCtlColor);
	
	afx_msg void 	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

protected:
	void			DrawWndBk(CDC *pDC);
	void			DrawWndBtn(CDC *pDC);
	
public:
	BOOL			SetImage(LPCTSTR lpszImageNormal, LPCTSTR lpszImageHover, LPCTSTR lpszImageDown, LPCTSTR lpszImageDisable);	
	
protected:
	CDC				m_dcWndBkDC;
	
	CDC				m_dcStandDC;
	CDC				m_dcStandPressDC;
	CDC				m_dcStandHotLightDC;
	
	CDC				m_dcDisableDC;
	CDC*			m_pCurBtnDC;
	
private:
	BOOL			m_bInitedBitmap;
	BOOL			m_bIsTracking;
	BOOL			m_bIsHovering;
	BOOL			m_bIsDisabled;
	
	Bitmap*			m_pImageNormal;
	Bitmap*			m_pImageHover;
	Bitmap*			m_pImageDown;
	Bitmap*			m_pImageDisable;
	
	CString			m_strImageNormal;
	CString			m_strImageHover;
	CString			m_strImageDown;
	CString			m_strImageDisable;
};