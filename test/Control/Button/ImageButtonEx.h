#pragma once

//////////////////////////////////////////////////////////////////////////
//
using namespace Gdiplus;
class CGdiPlusBitmap
{
public:
	Gdiplus::Bitmap* m_pBitmap;
	
public:
	CGdiPlusBitmap()				{ m_pBitmap = NULL; }
	CGdiPlusBitmap(LPCWSTR pFile)	
	{ 
		Load(pFile);
	}
	
	virtual ~CGdiPlusBitmap()		{ Empty(); }
	operator Gdiplus::Bitmap*() const { return m_pBitmap; }
	
public:
	void Empty()					
	{
		if ( m_pBitmap != NULL )
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
	}
	
	bool Load(LPCWSTR pFile)
	{
		Empty();
		m_pBitmap = Gdiplus::Bitmap::FromFile(pFile);
		return m_pBitmap->GetLastStatus() == Gdiplus::Ok;
	}
};

//////////////////////////////////////////////////////////////////////////
//
class CImageButtonEx : public CButton
{
public:
	CImageButtonEx();
	virtual ~CImageButtonEx();

public:
	void SetBkGnd(CDC* pDC);
	
	void SetImageEx(LPCTSTR lpszStdImage, LPCTSTR lpszStdPImage, LPCTSTR lpszStdDImage);
	
	void SetTextEx(UINT uiFontSize, UINT uiWeight, LPCTSTR lpszBtnText, LPCTSTR lpszFaceName, COLORREF crTextColor, CRect rcText = CRect(0,0,0,0));

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
	void			SetImageWndBk(CDC *pDC);
	void			DrawImageWndBk(CDC *pDC, CDC *pCurBtnDC);

	BOOL			LoadStdImage(LPCTSTR lpszImage);
	BOOL			LoadStdPImage(LPCTSTR lpszImage);
	BOOL			LoadStdDImage(LPCTSTR lpszImage);
	
	void			DrawStdWndBk(CDC *pDC, CDC *pWndBkDC);
	void			DrawStdPWndBk(CDC *pDC, CDC *pWndBkDC);
	void			DrawStdDWndBk(CDC *pDC, CDC *pWndBkDC);
	
	int				GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

protected:
	CFont			m_Font;
	LOGFONT			m_lfFont;
	
	CRect			m_rcText;
	CDC*			m_pCurBtnDC;

	CGdiPlusBitmap*	m_pStdImage;
	CGdiPlusBitmap* m_pStdPImage;
	CGdiPlusBitmap*	m_pStdDImage;
	
private:
	BOOL			m_bIsTracking;
	BOOL			m_bIsHovering;
	BOOL			m_bHaveBitmaps;
	
	CString			m_strBtnText;
	COLORREF		m_crTextColor;
	
	CDC				m_dcWndBkDC;	// button background
	
	CDC				m_dcStand;		// standard button
	CDC				m_dcStandP;		// standard button pressed
	CDC				m_dcStandD;		// grayscale button
};