#pragma once

using namespace Gdiplus;
class CTipWnd : public CWnd
{
public:
	CTipWnd();
	~CTipWnd();

public:
	BOOL			Create(CWnd *pParentWnd);
	void			RelayEvent(LPMSG lpMsg);

	void			SetWndFont(UINT uiFontHeight, UINT uiFontWidth, LPCTSTR lpszFaceName, COLORREF crTextColor);

	void			ShowWnd(LPCTSTR lpszTipInfo);
	void			ShowWnd(CPoint *pt, LPCTSTR lpszTipInfo);
	
protected:
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnPaint();
	
	afx_msg void 	OnDestroy();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL			DrawWndBk(CDC *pDC);
	BOOL			DrawWndImage(CDC *pDC);

	void			SetWndSize(CSize *pSize);
	
protected:
	CFont			m_Font;
	CPoint			m_ptOrigin;
	
private:
	CString			m_strWndText;
	COLORREF		m_crTextColor;
};