#include "stdafx.h"
#include "ScrollWndMsg.h"

CScrollWndMsg::CScrollWndMsg()
{
	m_nWndId   = 0;
	m_dwStyple = 0;
	m_nTextStartX = 0;
	m_nTextStartY = 0;

	m_bInited = FALSE;
	m_bRefreshSkin = FALSE;
	m_bRefreshText = TRUE;

	m_bWndSel    = FALSE;
	m_bWndBorder = FALSE;
	m_bWndHover  = FALSE;
	m_bBkBitmap  = FALSE;

	m_strWndText = _T("");
	m_strWndTipText = _T("");

	m_pBkBitmap = NULL;

	m_clrWndBk = RGB(50,150,200);
	m_clrWndBorder = RGB(50,150,200);

	m_clrNormalText = RGB(50,150,200);
	m_clrWndTipText = RGB(50,150,200);
	
	m_clrSelText    = RGB(50,150,200);
	m_clrHoverText  = RGB(50,150,200);
	m_clrDisableText = RGB(50,150,200);

	m_clrSelBorder  = RGB(50,150,200);
	m_clrSelFill    = RGB(50,150,200);

	m_clrHoverBorder = RGB(50,150,200);
	m_clrHoverFill  = RGB(50,150,200);
}

CScrollWndMsg::~CScrollWndMsg()
{
}

//////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CScrollWndMsg, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()

	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CScrollWndMsg::PreTranslateMessage(MSG* pMsg)
{
	return CWnd::PreTranslateMessage(pMsg);
}

void CScrollWndMsg::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

void CScrollWndMsg::OnPaint() 
{
	CPaintDC dc(this); 

	CPen   BorderPen, *pOldPen(NULL);
	CBrush FillBrush, *pOldBrush(NULL);

	CRect rcWnd;
	GetClientRect(&rcWnd);
	rcWnd.DeflateRect(0, 1, 5, 6);

	if (m_bWndSel)
	{
		FillBrush.CreateSolidBrush(m_clrSelFill);
		pOldBrush = dc.SelectObject(&FillBrush);

		BorderPen.CreatePen(PS_SOLID, 1, m_clrSelBorder);
		pOldPen = dc.SelectObject(&BorderPen);

		dc.RoundRect(&rcWnd, CPoint(5,5));

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);

		BorderPen.DeleteObject();
		FillBrush.DeleteObject();
	}

	if (m_bWndHover)
	{
		FillBrush.CreateSolidBrush(m_bWndSel ? m_clrSelFill : m_clrHoverFill);
		pOldBrush = dc.SelectObject(&FillBrush);

		BorderPen.CreatePen(PS_SOLID, 1, m_clrHoverBorder);
		pOldPen = dc.SelectObject(&BorderPen);

		dc.RoundRect(&rcWnd, CPoint(5,5));

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);

		BorderPen.DeleteObject();
		FillBrush.DeleteObject();
	}

	if (m_bRefreshText)
	{
		DrawEdge1(&dc, &rcWnd, m_strWndText);
	}
}

BOOL CScrollWndMsg::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcWnd;
	GetClientRect(&rcWnd);
	rcWnd.DeflateRect(0, 1, 5, 6);

	if (m_bWndBorder)
	{
		CPen pen, *pOldPen(NULL);

		pen.CreatePen(PS_SOLID, 1, m_clrWndBorder);
		pOldPen = pDC->SelectObject(&pen);
 
		pDC->Rectangle(&rcWnd);

		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}

	if (m_bBkBitmap && m_pBkBitmap != NULL)
	{
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);

		CBitmap bitmap;
		CBitmap *pOldBitmap = m_pBkBitmap;

		BITMAP bmp;
		pOldBitmap->GetBitmap(&bmp);

		pDC->StretchBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

		if (MemDC.GetSafeHdc())
		{
			MemDC.SelectObject(pOldBitmap);
			MemDC.DeleteDC();
		}
	}
	else
	{
		CBrush BkBrush, *pOldBrush(NULL);
		BkBrush.CreateSolidBrush(m_clrWndBk);

		pOldBrush = pDC->SelectObject(&BkBrush);
		pDC->PatBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), PATCOPY);

		pDC->SelectObject(pOldBrush);
		BkBrush.DeleteObject();
	}
	
	return TRUE;	
//	return CWnd::OnEraseBkgnd(pDC);
}

void CScrollWndMsg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
}

void CScrollWndMsg::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
void CScrollWndMsg::DrawEdge1(CDC* pDC, CRect* pWndRect, LPCTSTR lpszText)
{
	int nTextHeight = 0;
	CRect rcText;

	if (pDC == NULL)
	{
		return;
	}

	if (pWndRect == NULL)
	{
		return;
	}

	if (lpszText == NULL || *lpszText == '\0')
	{
		return;
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_clrNormalText);

	rcText.CopyRect(pWndRect);
	nTextHeight = pDC->DrawText(lpszText, rcText, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	rcText.CopyRect(pWndRect);

	if (pWndRect->Height() > nTextHeight)
	{
		rcText.OffsetRect(0, (pWndRect->Height()-nTextHeight)/2);
		//rcText.top += (pWndRect->Height() - nTextHeight)/2;
	}

	pDC->DrawText(lpszText, rcText, DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK);
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CScrollWndMsg::Create(DWORD dwStyle, const CRect &pWndRect, CWnd* pParent, UINT nWndId)
{
	if (!CWnd::CreateEx(NULL, NULL, NULL, dwStyle, pWndRect, pParent, nWndId))
	{
		return FALSE;
	}

	return TRUE;
}

void CScrollWndMsg::SetWndBkColor(COLORREF color)
{
	m_clrWndBk =  color;
}

void CScrollWndMsg::SetWndBkBitmap(UINT nIDBitmap)
{
	m_pBkBitmap->LoadBitmap(nIDBitmap);
	m_bBkBitmap = TRUE;
}

void CScrollWndMsg::SetWndBkBitmap(LPCTSTR lpBitmapName)
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

void CScrollWndMsg::SetWndBorder(BOOL bWndBorder, COLORREF color)
{
	m_bWndBorder = bWndBorder;
	m_clrWndBorder =  color;
}

void CScrollWndMsg::SetFont(int nHeight, LPCTSTR lpszFaceName)
{
	CFont cfont;

	LOGFONT lf;
	memset(&lf, 0x0, sizeof(LOGFONT));

	lf.lfHeight = nHeight;
	_tcscpy_s(lf.lfFaceName, lpszFaceName);

	VERIFY(cfont.CreateFontIndirect(&lf));
	//m_cFont = (HFONT)cfont.m_hObject;
}

void CScrollWndMsg::SetWndText(LPCTSTR lpszWndText, COLORREF color)
{
	m_strWndText = lpszWndText;
	m_clrNormalText = color;
}

void CScrollWndMsg::SetTextColor(COLORREF clrNormalText, COLORREF clrSelText)
{
	m_clrNormalText =  clrNormalText;
	m_clrSelText = clrSelText;
}

void CScrollWndMsg::SetWndTipText(LPCTSTR lpszWndTipText, COLORREF color)
{
	m_strWndTipText = lpszWndTipText;
	m_clrWndTipText =  color;
}

void CScrollWndMsg::SetSelColor(COLORREF clrSelBorder, COLORREF clrSelFill)
{
	m_clrSelBorder = clrSelBorder;
	m_clrSelFill = clrSelFill;
}

void CScrollWndMsg::SetHoverColor(COLORREF clrHoverBorder, COLORREF clrHoverFill)
{
	m_clrHoverBorder = clrHoverBorder;
	m_clrHoverFill = clrHoverFill;
}

//////////////////////////////////////////////////////////////////////////
//
void CScrollWndMsg::SetScrollPause()
{
}

void CScrollWndMsg::SetScrollSpeed(DWORD dwSpan)
{
}

void CScrollWndMsg::SetScrollShowPos(DWORD dx, DWORD dy)
{
}

void CScrollWndMsg::ClearScrollText()
{
}