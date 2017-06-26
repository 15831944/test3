#ifndef __DRAWBUTTON_H__
#define __DRAWBUTTON_H__

class CDrawButton : public CButton
{
	DECLARE_DYNAMIC(CDrawButton)
public:
	CDrawButton();
	virtual ~CDrawButton();

public:
	DECLARE_MESSAGE_MAP()	
	
public:
	virtual void				DrawItem(LPDRAWITEMSTRUCT lpDIS);

protected:
	virtual	void 				PreSubclassWindow();
	afx_msg HBRUSH 				CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL 				OnEraseBkgnd(CDC* pDC);
	afx_msg void 				OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 				OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL 				OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void 				OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	BOOL						Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
	
	void 						SetFont(int nSize, LPCTSTR lpFaceName);
	void						SetImageList(CImageList* pImageList);
	
	void						SetButtonText(LPCTSTR lpszString);
	void						SetTooltipText(LPCTSTR lpszTipString);
	
	void 						SetWndBorder(BOOL bWndBorder, COLORREF color = NULL);
	void						SetWndBkColor(COLORREF color);
	
//	void						SetBkBitmap();
//	void						SetBkBitmap();
	
	void						SetIcon(LPCTSTR lpszIconName);
	void						SetIcon(UINT nIDIconRes);

	void						SetCursor(UINT nIDCursor);
	void						SetCursor(LPCTSTR lpCursorName);
	
	void						SetTextColor(COLORREF crNormalText, COLORREF crSelText);
	void						SetSelColor(COLORREF crSelBorder, COLORREF crSelFill);
	void						SetHoverColor(COLORREF crHoverBorder, COLORREF crHoverFill);
	
protected:
	UINT						m_nButtonID;
	UINT						m_nCheck;

	DWORD						m_dwStyle;

	CDC 						m_BkMemDC;
	CBitmap*					m_pBkBitmap;

	CFont 						m_cFont;
	HCURSOR						m_hCursor;

#pragma pack(1)
	typedef struct _STRUCT_ICONS
	{
		HICON		hIcon;
		DWORD		dwWidth;
		DWORD		dwHeight;
	}STRUCT_ICONS;
#pragma pack()

#pragma pack(1)
	typedef struct _STRUCT_BITMAPS
	{
		HBITMAP		hBitmap;
		DWORD		dwWidth;
		DWORD		dwHeight;
		HBITMAP		hMask;
		COLORREF	crTransparent;
	}STRUCT_BITMAPS;
#pragma pack()

#pragma pack(1)
	typedef struct _STRUCT_CALLBACK
	{
		HWND		hWnd;
		UINT		nMessage;
		WPARAM		wParam;
		LPARAM		lParam;
	}STRUCT_CALLBACK;
#pragma pack()

	STRUCT_ICONS				m_csIcons[2];
	STRUCT_BITMAPS				m_csBitmaps[4];	//0:NormalBitmap 1:SelectBitmap 2:OverBitmap 3:DisableBitmap
	STRUCT_CALLBACK				m_csCallbacks;

	CBitmap						m_NormalBitmap;
	CBitmap						m_SelectBitmap;
	CBitmap						m_OverBitmap;
	CBitmap						m_DisableBitmap;
	CBitmap						m_FocusBitmap;
	CBitmap						m_MaskBitmap;
	
private:
	BOOL						m_bCheckBox;
	BOOL						m_bBorder;
	BOOL						m_bButtonDown;

	BOOL						m_bBkBitmap;	
	BOOL 						m_bWndBorder;	
	BOOL						m_bArrowCursor;
	
	COLORREF					m_crWndBk;
	COLORREF					m_crWndBorder;	

	COLORREF					m_crCenter;
	COLORREF					m_crEnd;
	
	COLORREF					m_crNormalText;	
	COLORREF					m_crSelText;
	COLORREF					m_crHoverText;	
	COLORREF					m_crDisableText;
	
	COLORREF					m_crSelBorder;
	COLORREF					m_crSelFill;
	
	COLORREF					m_crHoverBorder;
	COLORREF					m_crHoverFill;
};

#endif