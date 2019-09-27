#pragma once

using namespace Gdiplus;
class CImageButtonEx : public CButton
{
public:
	CImageButtonEx();
	virtual ~CImageButtonEx();

public:
	void			SetImage(LPCTSTR lpszStdImage, LPCTSTR lpszStdHImage, LPCTSTR lpszStdPImage, LPCTSTR lpszStdDImage);
	void			SetText(UINT uiFontSize, UINT uiWeight, LPCTSTR lpszBtnText, LPCTSTR lpszFaceName, COLORREF crTextColor, CRect rcText = CRect(0,0,0,0));

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
	void			GetParentWndBk();
	void			DrawParentWndBk(CDC *pDC);

	void			SetImageWndBk(CDC *pDC);
	void			DrawImageWndBk(CDC *pDC, CDC *pCurBtnDC);

protected:
	CDC				m_dcWndBkDC;
	
	CDC				m_dcStand;
	CDC				m_dcStandP;
	CDC				m_dcStandH;
	CDC				m_dcStandD;

	CFont			m_Font;
	
private:
	BOOL			m_bIsTracking;
	BOOL			m_bIsHovering;
	BOOL			m_bHaveBitmaps;
	
	CRect			m_rcText;

	CString			m_strBtnText;
	COLORREF		m_crTextColor;

	Bitmap*			m_pStdImage;
	Bitmap*			m_pStdHImage;
	Bitmap*			m_pStdPImage;
	Bitmap*			m_pStdDImage;

	CDC*			m_pPreBtnDC;
};