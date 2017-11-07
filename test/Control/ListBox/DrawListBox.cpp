#include "stdafx.h"
#include "DrawListBox.h"


#define LIST_FLAG_VALUE_ITEM	0x01
#define LIST_FLAG_DATA_ITEM		0x02
#define LIST_FLAG_IMAGE_ITEM	0x04
#define LIST_FLAG_MASK_ITEM		0x08

#define LIST_FLAG_ALL_ITEM		0xFF
#define LIST_FLAG_DISABLE_ITEM	0x00000001

#define	CX_BORDER				3
#define CY_BORDER				2

//////////////////////////////////////////////////////////////////////////
//CBaseListBox
IMPLEMENT_DYNAMIC(CBaseListBox, CListBox)
CBaseListBox::CBaseListBox()
{
	m_pImageList = NULL;
	::ZeroMemory(&m_szImageSize, sizeof(m_szImageSize));	
}

CBaseListBox::~CBaseListBox()
{
}

BEGIN_MESSAGE_MAP(CBaseListBox, CListBox)
END_MESSAGE_MAP()

void CBaseListBox::OnDestroy()
{
	FreeResources();
	CListBox::OnDestroy();
}

void CBaseListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);

	CString	strText;
	CListBox::GetText(lpMeasureItemStruct->itemID, strText);

	CDC* pDC = GetDC();

	CRect rcText(0, 0, lpMeasureItemStruct->itemWidth, lpMeasureItemStruct->itemHeight);
	int nHeight = pDC->DrawText(strText, -1, rcText, DT_WORDBREAK | DT_EXPANDTABS | DT_CALCRECT);

	if (m_pImageList)
	{
		lpMeasureItemStruct->itemHeight = max(nHeight, m_szImageSize.cy + CY_BORDER*2);
	}
	else
	{
		lpMeasureItemStruct->itemHeight = nHeight;
	}
	
	lpMeasureItemStruct->itemHeight += CY_BORDER*2;
}

void CBaseListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CString strText;

	COLORREF crColor;

	COLORREF crText     = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crNormal   = GetSysColor(COLOR_WINDOW);
	COLORREF crSelected = GetSysColor(COLOR_HIGHLIGHT);

	LISTBOX_DATA* lpLBData = (LISTBOX_DATA*)CListBox::GetItemDataPtr(lpDrawItemStruct->itemID);
	if (lpLBData == NULL || lpLBData == (LPVOID)-1L)
	{
		return;
	}

	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) ? TRUE : FALSE;
	BOOL bFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS) ? TRUE : FALSE;
	BOOL bDisabled = ((lpDrawItemStruct->itemState & ODS_DISABLED) || (lpLBData->dwMask & LIST_FLAG_DISABLE_ITEM)==LIST_FLAG_DISABLE_ITEM);

	CRect rcItem = lpDrawItemStruct->rcItem;
	CRect rcIcon = lpDrawItemStruct->rcItem;
	CRect rcText = lpDrawItemStruct->rcItem;
	
	pDC->SetBkMode(TRANSPARENT);

	//Icon Rect
	if (m_pImageList)
	{
		rcIcon.right  = rcItem.left + m_szImageSize.cx + CX_BORDER * 2;
		rcIcon.bottom = rcIcon.top  + m_szImageSize.cy + CX_BORDER * 2;

		if (bSelected && !bDisabled)
		{
			crColor = crSelected;
		}
		else
		{
			crColor = crNormal;
		}

		pDC->SetBkColor(crColor);
		pDC->FillSolidRect(rcIcon.left, rcIcon.top, rcIcon.Width(), rcIcon.Height(), crColor);

		HICON hIcon = m_pImageList->ExtractIcon(lpLBData->nImage);
		if (hIcon)
		{
			DrawIconEx(pDC->m_hDC, rcIcon.left, rcIcon.top, hIcon, m_szImageSize.cx, m_szImageSize.cy, NULL, NULL,  DI_NORMAL);
		}
	}
	else
	{
		rcIcon.SetRectEmpty();
	}

	//Text Rect
	rcText.left  = rcIcon.right;

	//Draw Text Color
	if (bDisabled)
	{
	}
	else
	{
		if (bSelected)
		{
			pDC->SetTextColor(RGB(255,255,0));
		}
		else
		{
			pDC->SetTextColor(RGB(255,0,0));
		}
	}
	
	//Draw Background 
	pDC->SetBkColor(crColor);
	pDC->FillSolidRect(rcText, crColor);

	CListBox::GetText(lpDrawItemStruct->itemID, strText);
	pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
}

int	CBaseListBox::AddString(LPCTSTR lpszItem, int nImage)
{
	int	nIndex   = LB_ERR;

	nIndex = CListBox::AddString(lpszItem);
	if (nIndex != LB_ERR && nIndex != LB_ERRSPACE)
	{
		SetListData(nIndex, 0, NULL, nImage, 0, LIST_FLAG_ALL_ITEM);
	}

	return nIndex;
}

int	CBaseListBox::InsertString(int nIndex, LPCTSTR lpszString, int nImage)
{
	int	nNewIndex   = LB_ERR;

	nNewIndex = CListBox::InsertString(nIndex, lpszString);
	if (nNewIndex != LB_ERR && nNewIndex != LB_ERRSPACE)
	{
		SetListData(nNewIndex, 0, NULL, nImage, 0, LIST_FLAG_ALL_ITEM);
	}

	return nNewIndex;
}

int	CBaseListBox::DeleteString(int nIndex)
{
	int	nRetValue = LB_ERR;

	DeleteItemData(nIndex);
	nRetValue = CListBox::DeleteString(nIndex);

	return nRetValue;
}

void CBaseListBox::ResetContent()
{
	FreeResources();
	CListBox::ResetContent();
}

int	CBaseListBox::SetItemData(int nIndex, DWORD dwItemData)
{
	return SetListData(nIndex, dwItemData, NULL, 0, 0, LIST_FLAG_VALUE_ITEM);
}

DWORD CBaseListBox::GetItemData(int nIndex)
{
	LISTBOX_DATA* lpLBData = NULL;

	lpLBData = (LISTBOX_DATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != (LPVOID)-1L)
	{
		return lpLBData->dwValue;
	}

	return LB_ERR;
}

int	CBaseListBox::SetItemDataPtr(int nIndex, void* pData)
{
	return SetListData(nIndex, 0, pData, 0, 0, LIST_FLAG_DATA_ITEM);
}

void* CBaseListBox::GetItemDataPtr(int nIndex)
{
	LISTBOX_DATA* lpLBData = NULL;

	lpLBData = (LISTBOX_DATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != (LPVOID)-1L)
	{
		return lpLBData->lpData;
	}

	return (LPVOID)-1L;
}

void CBaseListBox::SetItemDisable(int nIndex, BOOL bEnable, BOOL bRepaint/* =TRUE */)
{
	LISTBOX_DATA*	lpLBData = NULL;

	lpLBData = (LISTBOX_DATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != NULL && lpLBData != (LPVOID)-1L)
	{
		if (bEnable)
		{
			SetListData(nIndex, 0, NULL, 0, (lpLBData->dwMask & ~LIST_FLAG_DISABLE_ITEM), LIST_FLAG_MASK_ITEM);
		}
		else
		{
			SetListData(nIndex, 0, NULL, 0, (lpLBData->dwMask | LIST_FLAG_DISABLE_ITEM), LIST_FLAG_MASK_ITEM);
		}

		if (bRepaint)
		{
			Invalidate();
		}
	}
}

void CBaseListBox::SetImageList(CImageList* pImageList)
{
	m_pImageList = pImageList;

	if (m_pImageList)
	{
		ImageList_GetIconSize(m_pImageList->m_hImageList, (LPINT)&m_szImageSize.cx, ((LPINT)&m_szImageSize.cy));
	}
	else
	{
		::ZeroMemory(&m_szImageSize, sizeof(m_szImageSize));
	}

	Invalidate();
}

int CBaseListBox::SetListData(int nIndex, DWORD dwValue, LPVOID lpData, int nImage, DWORD dwMask, DWORD dwFlags)
{
	LISTBOX_DATA* lpLBData = NULL; 
	int				nRetValue = LB_ERR;

	lpLBData = (LISTBOX_DATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData == NULL)
	{
		lpLBData = new LISTBOX_DATA;
		if (lpLBData)
		{
			::ZeroMemory(lpLBData, sizeof(LISTBOX_DATA));
		}
	}

	if (lpLBData)
	{
		if ((dwFlags & LIST_FLAG_VALUE_ITEM) == LIST_FLAG_VALUE_ITEM)
		{
			lpLBData->dwValue = dwValue;
		}

		if ((dwFlags & LIST_FLAG_DATA_ITEM) == LIST_FLAG_DATA_ITEM)
		{
			lpLBData->lpData = lpData;
		}

		if ((dwFlags & LIST_FLAG_IMAGE_ITEM) == LIST_FLAG_IMAGE_ITEM)
		{
			lpLBData->nImage = nImage;
		}

		if ((dwFlags & LIST_FLAG_MASK_ITEM) == LIST_FLAG_MASK_ITEM)
		{
			lpLBData->dwMask = dwMask;
		}

		nRetValue = CListBox::SetItemDataPtr(nIndex, lpLBData);
	}

	return nRetValue;
}

void CBaseListBox::DeleteItemData(int nIndex)
{
	LISTBOX_DATA*	lpLBData = NULL;

	lpLBData = (LISTBOX_DATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != (LPVOID)-1L)
	{
		delete lpLBData;
		lpLBData = NULL;
	}

	CListBox::SetItemData(nIndex, NULL);
}

void CBaseListBox::FreeResources()
{
	int nCount = GetCount();
	if (nCount != LB_ERR)
	{
		while(nCount > 0)
		{
			DeleteItemData(nCount);

			nCount--;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//CDrawListBox
IMPLEMENT_DYNAMIC(CDrawListBox, CBaseListBox)

CDrawListBox::CDrawListBox()
{
	m_pBkBitmap   = NULL;
	m_hCursor	  = NULL;
	m_pImageList  = NULL;

	m_bWndBorder  = TRUE;
	m_bBkBitmap   = FALSE;
	m_bArrowCursor= FALSE;
	m_bHoverItem  = FALSE;

	m_nItemHeight = 30;
	m_nPrevHover  = -1;
	m_nCurHover	  = -1;

	m_nCtrlID	  = 0;

	m_crWndBk		 = RGB(255, 255, 255);
	m_crWndBorder    = RGB(141, 141, 141);

	m_crCenter		 = RGB(0, 0, 0);
	m_crEnd			 = RGB(255, 255, 255);

	m_crNormalText	 = RGB(255, 0, 0);
	m_crSelText      = RGB(255,255, 255);
	m_crHoverText	 = RGB(255, 0, 0);

	m_crHoverBorder	 = RGB(169, 216, 236);
	m_crHoverFill	 = RGB(255,130,0);//RGB(229, 238, 245);

	m_crSelBorder    = RGB(169, 216, 236);
	m_crSelFill		 = RGB(0, 0, 180);

	m_rcFrame.SetRectEmpty();
}

CDrawListBox::~CDrawListBox()
{
	if (m_bBkBitmap && m_pBkBitmap)
	{
		delete m_pBkBitmap;
		m_pBkBitmap = NULL;
	}
}

BEGIN_MESSAGE_MAP(CDrawListBox, CBaseListBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CDrawListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{//
	lpMeasureItemStruct->itemHeight = m_nItemHeight;

	if(m_rcFrame.IsRectEmpty())
	{
		GetClientRect(&m_rcFrame);
	}
}

void CDrawListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct);

	LISTBOX_DATA* lpLBData = (LISTBOX_DATA*)CListBox::GetItemDataPtr(lpDrawItemStruct->itemID);
	if (lpLBData == NULL || lpLBData == (LPVOID)-1L)
	{
		return;
	}

	CString strText;

	CRect rcIcon;
	CRect rcText;
	CRect rcHover;
	
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	
//	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	HICON hIcon = ImageList_GetIcon(m_pImageList->m_hImageList, lpLBData->nImage, ILD_NORMAL);

	CRect rcItem = lpDrawItemStruct->rcItem;
	rcItem.DeflateRect(1, 1);

	//ICON RECT
	(m_pImageList != NULL) ? rcIcon.SetRect((rcItem.left+3), ( rcItem.top+(rcItem.Height()-m_szImageSize.cy)/2), (rcItem.left + m_szImageSize.cx+10), (rcItem.bottom)) : rcIcon.SetRectEmpty();
	//TEXT RECT
	rcText.SetRect((rcIcon.right+1), rcItem.top, rcItem.right, rcItem.bottom);

	CListBox::GetText(lpDrawItemStruct->itemID, strText);
	dc.SetBkMode(TRANSPARENT);
	
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) ? TRUE : FALSE;
	BOOL bFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS) ? TRUE : FALSE;
	BOOL bDisabled = ((lpDrawItemStruct->itemState & ODS_DISABLED));

	//Draw Text Color
	if (bDisabled)
	{
		dc.SetTextColor(m_crDisableText);
	}
	else
	{  
		if (bSelected)
		{//选中
			DrawBkgnd(&dc, rcItem, m_crSelFill, RGB(255,0,0));	
			dc.SetTextColor(m_crSelText);
		}
		else if (m_bHoverItem && !bSelected)
		{//滑过
			GetItemRect(m_nCurHover, &rcHover);
			rcHover.DeflateRect(1,1);
			
			if(::EqualRect(&rcItem, &rcHover))
			{
				DrawBkgnd(&dc, rcItem, m_crHoverFill, m_crHoverBorder);
			}
		
			dc.SetTextColor(m_crHoverText);
		}
		else
		{//普通
			dc.SetTextColor(m_crNormalText);
		}
	}
	
	//
	DrawCurvLine(&dc, CPoint(rcItem.left+6, rcItem.bottom), CPoint(rcItem.right-6, rcItem.bottom), m_crCenter, m_crEnd);
	
	//
	((m_pImageList != NULL)&&hIcon != NULL ) ? DrawIcon(&dc, hIcon, rcIcon, m_szImageSize) :  NULL;

	dc.SelectObject(&m_cFont);
	dc.DrawText(strText, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	
	DestroyIcon(hIcon);
	dc.Detach();
}

void CDrawListBox::PreSubclassWindow()
{
	if(!m_cFont.GetSafeHandle())
	{
		m_cFont.CreatePointFont(100, _T("宋体"));
	}

	CListBox::PreSubclassWindow();
}

HBRUSH CDrawListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

BOOL CDrawListBox::OnEraseBkgnd(CDC* pDC) 
{
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

	pDC->StretchBlt(0, 0, m_rcFrame.Width(), m_rcFrame.Height(), &m_BkMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	if(m_bWndBorder)
	{
		CBrush  brush(m_crWndBorder);
		pDC->FrameRect(&m_rcFrame, &brush);
	}

	return CListBox::OnEraseBkgnd(pDC);
}

BOOL CDrawListBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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

	return CListBox::OnSetCursor(pWnd, nHitTest, message);
}

void CDrawListBox::OnMouseMove(UINT nFlags, CPoint point) 
{	
	BOOL bMouseOver = FALSE;
	BOOL bOutside = FALSE;
	BOOL bInRect = FALSE;
	
	CRect rcItem;
	CRect rcPrev;
	
	if(m_rcFrame.PtInRect(point))
	{
		bMouseOver = FALSE;
		SetCapture();
	}
	else
	{
		bMouseOver = TRUE;
		ReleaseCapture();
	}
	
	int nCulSel = GetCurSel();

	if(!bMouseOver)
	{//鼠标移进
		m_nCurHover = ItemFromPoint(point, bOutside);
		
		GetItemRect(m_nCurHover, rcItem);
		bInRect = rcItem.PtInRect(point);
		
		if(m_nPrevHover != m_nCurHover)
		{//
			if(m_nPrevHover != -1 && m_nPrevHover != nCulSel)
			{
				GetItemRect(m_nPrevHover, &rcPrev);
				InvalidateRect(&rcPrev);
				
				m_bHoverItem = FALSE;
			}
			
			if(bInRect)
			{								
				if(m_nCurHover != nCulSel)
				{
					InvalidateRect(&rcItem);
				}
			
				m_bHoverItem = TRUE;	
				m_nPrevHover = m_nCurHover;
				
				m_bArrowCursor = FALSE;
				SendMessage(WM_SETCURSOR);
			}
		}
		else if(!bInRect && (m_nCurHover==GetCount()-1))
		{//
			if(m_nPrevHover != nCulSel)
			{
				GetItemRect(m_nPrevHover, &rcPrev);
				InvalidateRect(&rcPrev);
			}
			
			m_bHoverItem = FALSE;
			m_nPrevHover = -1;
			
			m_bArrowCursor = TRUE;
			SendMessage(WM_SETCURSOR);
		}
	}
	else
	{//鼠标移出
		if(m_nPrevHover != nCulSel)
		{
			GetItemRect(m_nCurHover, &rcItem);
			InvalidateRect(&rcItem);
		}

		m_bHoverItem = FALSE;		
		m_nPrevHover = -1;
		
		m_bArrowCursor = TRUE;
		SendMessage(WM_SETCURSOR);
	}
	
	CListBox::OnMouseMove(nFlags, point);
}

void CDrawListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRect rcItem;
	CRect rcPrev;
	
	int nPrevSel = GetCurSel();
	
	if(nChar == VK_UP || nChar == VK_DOWN)
	{
		if(nPrevSel != -1)
		{
			GetItemRect(nPrevSel, &rcPrev);
			InvalidateRect(&rcPrev);
		}	
		
		if(nPrevSel == (GetCount()-1) && (nChar == VK_DOWN))
		{
			SetCurSel(0);
			return;
		}
		else if(!nPrevSel && (nChar == VK_UP))
		{
			SetCurSel(GetCount()-1);
			return;
		}
	}
	else
	{
		return;
	}

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDrawListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcItem;
	CRect rcPrev;

	int nPrevSel = GetCurSel();
	
	if(nPrevSel != -1)
	{
		GetItemRect(nPrevSel, &rcPrev);
		InvalidateRect(&rcPrev);
	}

	if(m_nCurHover == (GetCount()-1))
	{
		GetItemRect((GetCount()-1), rcItem);
		if(!rcItem.PtInRect(point))
		{
			return;
		}
	}

	SetCurSel(m_nCurHover);
	
	CListBox::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CDrawListBox::Create(const RECT &rect, CWnd *pParentWnd, UINT nID)
{//LBS_OWNERDRAWVARIABLE  //LBS_OWNERDRAWFIXED
	m_nCtrlID = nID;

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_OWNERDRAWVARIABLE | LBS_NOTIFY | LBS_HASSTRINGS;

	return CListBox::Create(dwStyle, rect, pParentWnd, nID);
}

void CDrawListBox::SetFont(int nSize, LPCTSTR lpFaceName)
{
	if(!m_cFont.GetSafeHandle())
	{
		m_cFont.CreatePointFont(nSize, lpFaceName);
	}
}

void CDrawListBox::SetItemHeight(int nItemHeight)
{
	m_nItemHeight = nItemHeight;
}

void CDrawListBox::SetWndBorder(BOOL bWndBorder, COLORREF color)
{
	m_bWndBorder  = bWndBorder;
	m_crWndBorder = color;
}

void CDrawListBox::SetWndBkColor(COLORREF color)
{
	m_crWndBk = color;
}

void CDrawListBox::SetBkBitmap(UINT nIDBitmap)
{
	m_pBkBitmap->LoadBitmap(nIDBitmap);

	m_bBkBitmap = TRUE;
}

void CDrawListBox::SetBkBitmap(LPCTSTR lpBitmapName)
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL,
		lpBitmapName,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE);

	m_pBkBitmap = CBitmap::FromHandle(hBitmap);	

	m_bBkBitmap = TRUE;
}

void CDrawListBox::SetCursor(UINT nIDCursor)
{
	ASSERT(!m_hCursor);

	m_hCursor = AfxGetApp()->LoadCursor(nIDCursor);

	SendMessage(WM_SETCURSOR);
}

void CDrawListBox::SetCursor(LPCTSTR lpCursorName)
{
	ASSERT(!m_hCursor);

	m_hCursor = ::LoadCursorFromFile(lpCursorName);

	SendMessage(WM_SETCURSOR);
}

void CDrawListBox::SetCurLineColor(COLORREF crCenter, COLORREF crEnd)
{
	m_crCenter = crCenter;
	m_crEnd = crEnd;
}

void CDrawListBox::SetTextColor(COLORREF crNormalText, COLORREF crSelText)
{
	m_crNormalText = crNormalText;
	m_crSelText = crSelText;
}

void CDrawListBox::SetSelColor(COLORREF crSelBorder, COLORREF crSelFill)
{
	m_crSelBorder = crSelBorder;
	m_crSelFill = crSelFill;
}

void CDrawListBox::SetHoverColor(COLORREF crHoverBorder, COLORREF crHoverFill)
{
	m_crHoverBorder = crHoverBorder;
	m_crHoverFill = crHoverFill;
}

void CDrawListBox::DrawCurvLine(CDC *pDC, CPoint ptFrom, CPoint ptTo, COLORREF clrCenter, COLORREF clrEnds)
{
	int nCenterR = GetRValue(clrCenter),
		nCenterG = GetGValue(clrCenter),
		nCenterB = GetBValue(clrCenter),
		nEndsR   = GetRValue(clrEnds),
		nEndsG   = GetGValue(clrEnds),
		nEndsB   = GetBValue(clrEnds);

	int nIncWidth = (ptTo.x - ptFrom.x) / 2;
	ASSERT(nIncWidth > 0);

	double IncR = double(abs(nCenterR - nEndsR)) / double(nIncWidth),
		IncG = double(abs(nCenterG - nEndsG)) / double(nIncWidth),
		IncB = double(abs(nCenterB - nEndsB)) / double(nIncWidth);

	CPoint ptCur = ptFrom;
	int nCurR = nEndsR,
		nCurG = nEndsG,
		nCurB = nEndsB;
	//正负符号，用于确定是加还是减
	int nSignR = (nCenterR - nEndsR) > 0 ? 1 : (-1),
		nSignG = (nCenterG - nEndsG) > 0 ? 1 : (-1),
		nSignB = (nCenterB - nEndsB) > 0 ? 1 : (-1);

	for(; ptCur.x <= ptFrom.x + nIncWidth; ptCur.x ++)
	{
		pDC->SetPixel(ptCur, RGB(nCurR, nCurG, nCurB));
		nCurR = int(nEndsR + nSignR * IncR * (ptCur.x - ptFrom.x));
		nCurG = int(nEndsG + nSignG * IncG * (ptCur.x - ptFrom.x));
		nCurB = int(nEndsB + nSignB * IncB * (ptCur.x - ptFrom.x));
		//TRACE("nCurR = %d\n", nCurR);
	}

	for(int nMidPos = (ptFrom.x + ptTo.x) / 2; ptCur.x <= ptTo.x; ptCur.x ++)
	{
		pDC->SetPixel(ptCur, RGB(nCurR, nCurG, nCurB));
		nCurR = int(nCenterR - nSignR * IncR * (ptCur.x - nMidPos));
		nCurG = int(nCenterG - nSignG * IncG * (ptCur.x - nMidPos));
		nCurB = int(nCenterB - nSignB * IncB * (ptCur.x - nMidPos));
		//TRACE("nCurR = %d\n", nCurR);
	}
}

void CDrawListBox::DrawIcon(CDC* pDC, HICON hIcon, CRect rcIcon, CSize szIcon)
{
	::DrawIconEx(pDC->m_hDC, rcIcon.left, rcIcon.top, hIcon, szIcon.cx, szIcon.cy, NULL, NULL, DI_NORMAL);
}

void CDrawListBox::DrawBkgnd(CDC* pDC, CRect rect, COLORREF crFill, COLORREF crBorder)
{
	CPen penBorder;
	CBrush brushFill;

	brushFill.CreateSolidBrush(crFill); //单色画刷  //GetStockObject 特定颜色画刷  //CreatePatternBrush 位图画刷  //CreateHatchBrush  带阴影画刷  //CreateBrushIndirect  LOGBRUSH结构画刷
	penBorder.CreatePen(PS_SOLID, 1, crBorder);

	pDC->SelectObject(&brushFill);
	pDC->SelectObject(&penBorder);

	pDC->RoundRect(&rect, CPoint(5,5));

	DeleteObject(brushFill);
	DeleteObject(penBorder);
}
