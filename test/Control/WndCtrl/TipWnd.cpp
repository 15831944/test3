#include "stdafx.h"
#include "TipWnd.h"

CTipWnd::CTipWnd()
{
	m_strWndText = _T("");
	
	m_pFont = NULL;
	m_ptOrigin = CPoint(0, 0);	
}

CTipWnd::~CTipWnd()
{
}

BEGIN_MESSAGE_MAP(CTipWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	
	ON_WM_DESTROY()
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
	CFont *pOldFont = NULL;
	if (pDC == NULL)
	{
		return FALSE;
	}

	CRect rcWnd;
	GetClientRect(&rcWnd);

	pOldFont = pDC->SelectObject(m_pFont);
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(m_strWndText, &rcWnd, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

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

	pFont = (CFont *)pDC->SelectObject(m_pFont);
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
	if (pParentWnd == NULL)
	{
		return FALSE;
	}
	
	m_pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	
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