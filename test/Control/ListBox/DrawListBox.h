#ifndef __DRAWLISTBOX_H__
#define __DRAWLISTBOX_H__


#include <afxtempl.h>

//CBaseListBox
class CBaseListBox : public CListBox
{
	DECLARE_DYNAMIC(CBaseListBox)
public:
	CBaseListBox();
	virtual ~CBaseListBox();

public:
	DECLARE_MESSAGE_MAP()

public:
	virtual void				MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void				DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
protected:
	afx_msg void				OnDestroy();

public:	
	int							AddString(LPCTSTR lpszItem, int nImage);
	int							InsertString(int nIndex, LPCTSTR lpszString, int nImage);
	int							DeleteString(int nIndex);

	void						ResetContent();

	int							SetItemData(int nIndex, DWORD dwItemData);
	DWORD						GetItemData(int nIndex);
	int							SetItemDataPtr(int nIndex, void* pData);
	void*						GetItemDataPtr(int nIndex);

	void						SetItemDisable(int nIndex, BOOL bEnable=FALSE, BOOL bRepaint=TRUE);
	void						SetImageList(CImageList* pImageList);

protected:
	int							SetListData(int nIndex, DWORD dwValue, LPVOID lpData, int nImage, DWORD dwMask, DWORD dwFlags);

	void						FreeResources();
	void						DeleteItemData(int nIndex);
	
protected:	
	CImageList*					m_pImageList;
	SIZE						m_szImageSize;

	typedef struct _LISTBOX_DATA_
	{
		DWORD					dwValue;
		LPVOID					lpData;
		int						nImage;
		UINT					nFormat;
		DWORD					dwMask;

		_LISTBOX_DATA_::_LISTBOX_DATA_()
		{
			::ZeroMemory(this, sizeof(_LISTBOX_DATA_));
		}
	}LISTBOX_DATA;
};


//////////////////////////////////////////////////////////////////////////
//CDrawListBox

class CDrawListBox : public CBaseListBox
{
	DECLARE_DYNAMIC(CDrawListBox)

public:
	CDrawListBox();
	virtual ~CDrawListBox();

public:
	DECLARE_MESSAGE_MAP()

public:
	virtual void				MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void				DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual	void 				PreSubclassWindow();	
	afx_msg HBRUSH 				CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL 				OnEraseBkgnd(CDC* pDC);
	afx_msg void 				OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 				OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL 				OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void 				OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	BOOL						ItemFromPointX(CPoint pt, BOOL& bOutside, int& nIndex);

	void						DrawBkgnd(CDC* pDC, CRect rect, COLORREF crFill, COLORREF crBorder);
	void						DrawIcon(CDC* pDC, HICON hIcon, CRect rcIcon, CSize szIcon);
	void						DrawCurvLine(CDC *pDC, CPoint ptFrom, CPoint ptTo, COLORREF clrCenter, COLORREF clrEnds);

public:	
	BOOL						Create(const RECT &rect, CWnd *pParentWnd, UINT nID);

	void						SetFont(int nSize, LPCTSTR lpFaceName);
	void						SetItemHeight(int nItemHeight);

	void 						SetWndBorder(BOOL bWndBorder, COLORREF color = NULL);
	void						SetWndBkColor(COLORREF color);

	void						SetBkBitmap(UINT nIDBitmap);
	void						SetBkBitmap(LPCTSTR lpBitmapName);

	void 						SetCursor(UINT nIDCursor);
	void 						SetCursor(LPCTSTR lpCursorName);

	void						SetCurLineColor(COLORREF crCenter, COLORREF crEnd);
	void						SetTextColor(COLORREF crNormalText, COLORREF crSelText);
	void						SetSelColor(COLORREF crSelBorder, COLORREF crSelFill);
	void						SetHoverColor(COLORREF crHoverBorder, COLORREF crHoverFill);

protected:
	UINT						m_nCtrlID;

	CDC 						m_BkMemDC;
	CBitmap*					m_pBkBitmap;

	CFont 						m_cFont;

private:
	BOOL 						m_bWndBorder;
	BOOL						m_bBkBitmap;	
	BOOL						m_bArrowCursor;
	BOOL						m_bHoverItem;

	int							m_nItemHeight;
	int							m_nPrevHover;
	int 						m_nCurHover;

	CRect 						m_rcFrame;
	HCURSOR						m_hCursor;

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