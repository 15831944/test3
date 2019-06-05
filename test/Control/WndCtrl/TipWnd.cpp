#include "stdafx.h"
#include "TipWnd.h"

CTipWnd::CTipWnd()
{
	m_strWndText = _T("");

	m_ptOrigin = CPoint(0, 0);
	m_crTextColor = RGB(255, 0, 0);
}

CTipWnd::~CTipWnd()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CTipWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CTipWnd::OnEraseBkgnd(CDC* pDC) 
{
	DrawWndBk(pDC);

	return CWnd::OnEraseBkgnd(pDC);
}

void CTipWnd::OnPaint() 
{
	CPaintDC dc(this); 

	DrawWndImage(&dc);
}

void CTipWnd::OnDestroy()
{
	CWnd::OnDestroy();
}

void CTipWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));  
	CWnd::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CTipWnd::DrawWndBk(CDC *pDC)
{
	CBrush brush;
	CBrush *pOldBrush = NULL;
	
	if (pDC == NULL)
	{
		return FALSE;
	}

	CRect rcWnd;
	pDC->GetClipBox(rcWnd);

	brush.CreateSolidBrush(GetSysColor(COLOR_INFOBK));
	pOldBrush = pDC->SelectObject(&brush);

	pDC->PatBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);
	return TRUE;
}

BOOL CTipWnd::DrawWndImage(CDC *pDC)
{
	CRect rcWndText;
	CFont *pOldFont = NULL;

	int iTextHeight = -1;

	if (pDC == NULL)
	{
		return FALSE;
	}

	CRect rcWnd;
	GetClientRect(&rcWnd);

	pOldFont = pDC->SelectObject(&m_Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crTextColor);

	rcWndText = rcWnd;
	iTextHeight = pDC->DrawText(m_strWndText, rcWndText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);

	rcWndText = rcWnd;
	if (rcWndText.Height() > iTextHeight)
	{
		//rcWndText.top += (rcWndText.Height() - iTextHeight)/2;
		rcWndText.DeflateRect(0, (rcWndText.Height()-iTextHeight)/2);
	}

	pDC->DrawText(m_strWndText, rcWndText, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	pDC->SelectObject(pOldFont);
	return TRUE;
}

void CTipWnd::SetWndSize(CSize *pSize)
{
	CRect rcWnd;
	CFont *pFont = NULL;

	CDC	*pDC = GetDC();
	if (pSize == NULL || pDC == NULL)
	{
		return;
	}

	pFont = (CFont *)pDC->SelectObject(&m_Font);
	pDC->DrawText(m_strWndText, &rcWnd, DT_CALCRECT);
	pDC->SelectObject(pFont);

	pSize->cx = rcWnd.Width() + 25;
	pSize->cy = rcWnd.Height() + 10;

	ReleaseDC(pDC);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CTipWnd::Create(CWnd *pParentWnd)
{
	CFont *pFont = NULL;
	LOGFONT	lfFont = {0};

	if (pParentWnd == NULL)
	{
		return FALSE;
	}
	
	pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	if (pFont != NULL)
	{
		pFont->GetLogFont(&lfFont);
		if (m_Font.m_hObject == NULL)
		{
			m_Font.CreateFontIndirect(&lfFont);
		}
	}
	
	return CWnd::CreateEx(NULL, 
						AfxRegisterWndClass(0), 
						NULL, 
						WS_BORDER|WS_POPUP, 
						0, 0, 0, 0, 
						pParentWnd->GetSafeHwnd(),
						NULL, NULL);
}

void CTipWnd::RelayEvent(LPMSG lpMsg)
{
	switch (lpMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		ShowWindow(SW_HIDE);
		break;
	}
}

void CTipWnd::SetWndFont(UINT uiFontHeight, UINT uiFontWidth, LPCTSTR lpszFaceName, COLORREF crTextColor)
{
	LOGFONT	lfFont = {0};

	lfFont.lfHeight = uiFontHeight;
	lfFont.lfWeight = uiFontWidth;

	lfFont.lfCharSet=GB2312_CHARSET;
	_tcscpy_s(lfFont.lfFaceName, lpszFaceName);

	if (m_Font.m_hObject != NULL)
	{
		m_Font.DeleteObject();
	}

	m_Font.CreateFontIndirect(&lfFont);
	m_crTextColor = crTextColor;
}

void CTipWnd::ShowWnd(LPCTSTR lpszTipInfo)
{
	m_strWndText = lpszTipInfo;
	ShowWindow(SW_SHOW);
}

void CTipWnd::ShowWnd(CPoint *pt, LPCTSTR lpszTipInfo)
{
	CSize size;
	if (pt != NULL)
	{
		m_ptOrigin = *pt;
	}
	else
	{
		GetCursorPos(&m_ptOrigin);
	}

	m_strWndText = lpszTipInfo;
	SetWndSize(&size);

	SetWindowPos(&wndTop, m_ptOrigin.x, m_ptOrigin.y, size.cx, size.cy, SWP_NOACTIVATE | SWP_SHOWWINDOW);
}