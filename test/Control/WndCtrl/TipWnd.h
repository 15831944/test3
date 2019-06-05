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

	void			ShowWnd(LPCTSTR lpszTipInfo);
	void			ShowWnd(CPoint *pt, LPCTSTR lpszTipInfo);
	
protected:
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnPaint();
	
	afx_msg void 	OnDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL			DrawWndBk(CDC *pDC);
	BOOL			DrawWndImage(CDC *pDC);

	void			SetWndSize(CSize *pSize);
	
protected:
	CFont			*m_pFont;
	CPoint			m_ptOrigin;
	
private:
	CString			m_strWndText;
};