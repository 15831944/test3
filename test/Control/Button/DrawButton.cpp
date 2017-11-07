#include "stdafx.h"
#include "DrawButton.h"

IMPLEMENT_DYNAMIC(CDrawButton, CButton)
CDrawButton::CDrawButton()
{
	m_pBkBitmap   = NULL;
	m_hCursor	  = NULL;

	m_bCheckBox   = FALSE;
	m_bBorder     = FALSE;
	m_bButtonDown = FALSE;

	m_bBkBitmap   = FALSE;
	m_bWndBorder  = TRUE;
	m_bArrowCursor= FALSE; 

	m_nButtonID	  = 0;
	m_nCheck      = 0;

	m_crWndBk	  = RGB(255, 255, 255);
	m_crWndBorder = RGB(141, 141, 141);

	m_crNormalText = RGB(255, 0, 0);
	m_crSelText    = RGB(255,255, 255);
	m_crHoverText  = RGB(255, 0, 0);

	memset(&m_csIcons, 0x0, sizeof(STRUCT_ICONS));
}

CDrawButton::~CDrawButton()
{
	
}

BEGIN_MESSAGE_MAP(CDrawButton, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDrawButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS);
	
	CString strText;
	
	CRect rcIcon;
	CRect rcText;
	CRect rcHover;
	
	BOOL bSelected;
	BOOL bFocused;
	BOOL bDisabled;
	
	CDC dc;
	dc.Attach(lpDIS->hDC);
	
	CRect rcItem = lpDIS->rcItem;
	rcItem.DeflateRect(1, 1);

	//ICON RECT
//	(m_csIcons[0].hIcon != NULL) ?  rcIcon.SetRect((rcItem.left+3), (rcItem.top+(rcItem.Height()-m_csIcons[0].dwHeight)/2), (rcItem.left+m_csIcons[0].dwWidth+10), (rcItem.bottom)) : rcIcon.SetRectEmpty();
	if (m_csIcons[0].hIcon != NULL)
	{
		 rcIcon.SetRect((rcItem.left+3), (rcItem.top+(rcItem.Height()-m_csIcons[0].dwHeight)/2), (rcItem.left+m_csIcons[0].dwWidth+10), (rcItem.bottom));
	}
	else
	{
		rcIcon.SetRectEmpty();
	}
	//TEXT RECT
	rcText.SetRect((rcIcon.right+1), rcItem.top, rcItem.right, rcItem.bottom);
	
	GetWindowText(strText);
	dc.SetBkMode(TRANSPARENT);
	
	if(m_bCheckBox)
	{
		bSelected = (lpDIS->itemState & ODS_SELECTED) || (m_nCheck != 0);
	}
	else
	{
		bSelected = (lpDIS->itemState & ODS_SELECTED);
	}
	
	bFocused  = (lpDIS->itemState & ODS_FOCUS);
	bDisabled = ((lpDIS->itemState & ODS_DISABLED));

	if (bDisabled)
	{
		if (m_DisableBitmap.m_hObject == NULL)
		{
			dc.FillSolidRect(&rcItem, GetSysColor(COLOR_BTNFACE));
		}
		else
		{

		}

		dc.SetTextColor(m_crDisableText);
	}
	else
	{
		if (bSelected)
		{
		}
		else
		{
			dc.SetTextColor(m_crNormalText);
		}
	}

	dc.SelectObject(&m_cFont);
	dc.DrawText(strText, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	dc.Detach();
}

void CDrawButton::PreSubclassWindow()
{
	m_dwStyle = GetButtonStyle();

	if (!m_cFont.GetSafeHandle())
	{
		m_cFont.CreatePointFont(100, _T("宋体"));
	}

	if ((m_dwStyle&BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX)
	{//复选框
		m_dwStyle = BS_CHECKBOX;
	}
	else if ((m_dwStyle&BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)
	{//单选按钮
		m_dwStyle = BS_RADIOBUTTON;
	}
	else
	{
		m_dwStyle = BS_PUSHBUTTON;
	}

	ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CButton::PreSubclassWindow();
}

HBRUSH CDrawButton::CtlColor(CDC* pDC, UINT nCtlColor)
{
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

BOOL CDrawButton::OnEraseBkgnd(CDC* pDC)
{
	CRect rcItem;
	GetClientRect(&rcItem);

	if(!m_pBkBitmap)
	{
		m_pBkBitmap = new CBitmap();
		m_pBkBitmap->CreateCompatibleBitmap(pDC, 1, 1);
	}

	if(!m_BkMemDC.GetSafeHdc())
	{
		m_BkMemDC.CreateCompatibleDC(pDC);
		m_BkMemDC.SelectObject(m_pBkBitmap);

		if(!m_bBkBitmap)
		{
			m_BkMemDC.FillSolidRect(0, 0, 1, 1, m_crWndBk);
		}
	}	

	BITMAP bmp;
	m_pBkBitmap->GetBitmap(&bmp);
	pDC->StretchBlt(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), &m_BkMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	if (m_bButtonDown)
	{
		if (m_bWndBorder)
		{
		}
		else
		{

		}
	}
	else
	{
		CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT));
		CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));
		CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));
		CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW));

		if (m_bWndBorder)
		{

		}
		else
		{
			CPen* pOldPen = pDC->SelectObject(&penBtnHiLight);
			pDC->MoveTo(rcItem.left, rcItem.bottom-1);
			pDC->LineTo(rcItem.left, rcItem.top);
			pDC->LineTo(rcItem.right, rcItem.top);

			pDC->SelectObject(pen3DLight);
			pDC->MoveTo(rcItem.left+1, rcItem.bottom-1);
			pDC->LineTo(rcItem.left+1, rcItem.top+1);
			pDC->LineTo(rcItem.right, rcItem.top+1);

			pDC->SelectObject(pen3DDKShadow);
			pDC->MoveTo(rcItem.left, rcItem.bottom-1);
			pDC->LineTo(rcItem.right-1, rcItem.bottom-1);
			pDC->LineTo(rcItem.right-1, rcItem.top-1);

			pDC->SelectObject(penBtnShadow);
			pDC->MoveTo(rcItem.left+1, rcItem.bottom-2);
			pDC->LineTo(rcItem.right-2, rcItem.bottom-2);
			pDC->LineTo(rcItem.right-2, rcItem.top);

			pDC->SelectObject(pOldPen);
		}
	}
// 	if(m_bWndBorder)
// 	{
// 		CBrush  brush(m_crWndBorder);
// 		pDC->FrameRect(&rcItem, &brush);
// 	}

	return CButton::OnEraseBkgnd(pDC);
}

BOOL CDrawButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_bArrowCursor)
	{//标准的箭头光标
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}
	else if(!m_hCursor)
	{//手型光标
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		return TRUE;
	}
	else
	{
		::SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)m_hCursor);
	}

	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void CDrawButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CButton::OnMouseMove(nFlags, point);
}

void CDrawButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDrawButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButton::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CDrawButton::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	m_nButtonID = nID;
	
	return CButton::Create(NULL, dwStyle, rect, pParentWnd, nID);
}

void CDrawButton::SetFont(int nSize, LPCTSTR lpFaceName)
{
	if(!m_cFont.GetSafeHandle())
	{
		m_cFont.CreatePointFont(nSize, lpFaceName);
	}
}

void CDrawButton::SetImageList(CImageList* pImageList)
{
// 	m_pImageList = pImageList;
// 
// 	if (m_pImageList)
// 	{
// 		ImageList_GetIconSize(m_pImageList->m_hImageList, (LPINT)&m_szImageSize.cx, ((LPINT)&m_szImageSize.cy));
// 	}
// 	else
// 	{
// 		::ZeroMemory(&m_szImageSize, sizeof(m_szImageSize));
// 	}

	Invalidate();
}

void CDrawButton::SetButtonText(LPCTSTR lpszString)
{
}

void CDrawButton::SetTooltipText(LPCTSTR lpszTipString)
{
}

void CDrawButton::SetWndBorder(BOOL bWndBorder, COLORREF color)
{
	m_bWndBorder  = bWndBorder;
	m_crWndBorder = color;
}

void CDrawButton::SetWndBkColor(COLORREF color)
{
	m_crWndBk = color;
}

void CDrawButton::SetIcon(LPCTSTR lpszIconName)
{
	::LoadImage(NULL, lpszIconName, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

void CDrawButton::SetIcon(UINT nIDIconRes)
{
	HINSTANCE	hInstResource	= NULL;
	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIDIconRes), RT_GROUP_ICON);

	::LoadImage(hInstResource, MAKEINTRESOURCE(nIDIconRes), IMAGE_ICON, 0, 0, 0);
}

void CDrawButton::SetCursor(UINT nIDCursor)
{
	ASSERT(!m_hCursor);

	m_hCursor = AfxGetApp()->LoadCursor(nIDCursor);

	SendMessage(WM_SETCURSOR);
}

void CDrawButton::SetCursor(LPCTSTR lpCursorName)
{
	ASSERT(!m_hCursor);

	m_hCursor = ::LoadCursorFromFile(lpCursorName);

	SendMessage(WM_SETCURSOR);
}

void CDrawButton::SetTextColor(COLORREF crNormalText, COLORREF crSelText)
{
	m_crNormalText = crNormalText;
	m_crSelText = crSelText;
}

void CDrawButton::SetSelColor(COLORREF crSelBorder, COLORREF crSelFill)
{
	m_crSelBorder = crSelBorder;
	m_crSelFill = crSelFill;
}

void CDrawButton::SetHoverColor(COLORREF crHoverBorder, COLORREF crHoverFill)
{
	m_crHoverBorder = crHoverBorder;
	m_crHoverFill = crHoverFill;
}