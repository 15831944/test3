#ifndef __SCROLLWNDMSG_H__
#define __SCROLLWNDMSG_H__

typedef struct _STRUCT_ICONS
{
	HICON		hIcon;
	DWORD		dwWidth;
	DWORD		dwHeight;
}STRUCT_ICONS;

class CScrollWndMsg : public CWnd
{
public:
	CScrollWndMsg();
	~CScrollWndMsg();

protected:
	virtual BOOL 				PreTranslateMessage(MSG* pMsg);

	afx_msg void				OnSize(UINT nType, int cx, int cy);
	afx_msg void 				OnPaint();
	afx_msg BOOL				OnEraseBkgnd(CDC* pDC);
	afx_msg void				OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void 				OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	//static BOOL				hasclass;
	//static BOOL				RegisterWindowClass();

public:
	BOOL						Create(DWORD dwStyle, const CRect &pWndRect, CWnd* pParent, UINT nWndId);

	void						SetWndBkColor(COLORREF color = NULL);
	void						SetWndBkBitmap(UINT nIDBitmap);
	void						SetWndBkBitmap(LPCTSTR lpBitmapName);
	void						SetWndBorder(BOOL bWndBorder, COLORREF color = NULL);

	void						SetFont(int nHeight, LPCTSTR lpszFaceName);
	void						SetWndText(LPCTSTR lpszWndText, COLORREF color = NULL);
	void						SetTextColor(COLORREF clrNormalText, COLORREF clrSelText);
	void						SetWndTipText(LPCTSTR lpszWndTipText, COLORREF color = NULL);

	void						SetSelColor(COLORREF clrSelBorder, COLORREF clrSelFill);
	void						SetHoverColor(COLORREF clrHoverBorder, COLORREF clrHoverFill);
	
	void						SetScrollPause();
	void						SetScrollSpeed(DWORD dwSpan);
	void						SetScrollShowPos(DWORD dx, DWORD dy);
	void						ClearScrollText();
	
protected:
	void						DrawEdge1(CDC* pDC, CRect* pWndRect, LPCTSTR lpszText);

protected:
	UINT						m_nWndId;
	DWORD						m_dwStyple;
	
	CBitmap*					m_pBkBitmap;
	CFont*						m_pFont;

	STRUCT_ICONS				m_csIcons[2];
	
private:
	BOOL						m_bInited;
	BOOL						m_bRefreshSkin;
	BOOL						m_bRefreshText;
		
	BOOL						m_bWndSel;
	BOOL 						m_bWndBorder;
	BOOL						m_bWndHover;
	BOOL						m_bBkBitmap;

	int							m_nTextStartX;
	int							m_nTextStartY;
	
	CString						m_strWndText;
	CString						m_strWndTipText;
	
	COLORREF					m_clrWndBk;
	COLORREF					m_clrWndBorder;

	COLORREF					m_clrNormalText;	
	COLORREF					m_clrWndTipText;

	COLORREF					m_clrSelText;
	COLORREF					m_clrHoverText;
	COLORREF					m_clrDisableText;
	
	COLORREF					m_clrSelBorder;
	COLORREF					m_clrSelFill;
	
	COLORREF					m_clrHoverBorder;
	COLORREF					m_clrHoverFill;
};

#endif