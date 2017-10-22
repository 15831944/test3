#include "stdafx.h"
#include "ScrollWndMsg.h"

CScrollWndMsg::CScrollWndMsg()
{
	m_crWndBk = RGB(50,150,200);
}

CScrollWndMsg::~CScrollWndMsg()
{
}

//////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CScrollWndMsg, CWnd)
	//{{AFX_MSG_MAP(CScrollWndMsg)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CScrollWndMsg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
//	return CWnd::OnEraseBkgnd(pDC);
}

void CScrollWndMsg::OnPaint() 
{
	CPaintDC dc(this); 
	CRect rc;
	GetClientRect(&rc);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap *pOldBitmap = MemDC.SelectObject(&MemBitmap);

	CBrush brBkGnd;
	brBkGnd.CreateSolidBrush(m_crWndBk);
	MemDC.FillRect(&rc ,&brBkGnd);
	MemDC.SetBkMode(TRANSPARENT);

	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

void CScrollWndMsg::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
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

void CScrollWndMsg::SetFont(int nSize, LPCTSTR lpszFaceName)
{
}

void CScrollWndMsg::SetWndText(LPCTSTR lpszWndText, COLORREF color)
{
	m_strWndText = lpszWndText;
	m_crNormalText = color;
}

void CScrollWndMsg::SetWndTipText(LPCTSTR lpszWndTipText, COLORREF color)
{
	m_strWndTipText = lpszWndTipText;
	m_crWndTipText =  color;
}

void CScrollWndMsg::SetWndBorder(BOOL bWndBorder, COLORREF color)
{
	m_bWndBorder = bWndBorder;
	m_crWndBorder =  color;
}

void CScrollWndMsg::SetWndBkColor(COLORREF color)
{
	m_crWndBk =  color;
}

void CScrollWndMsg::SetTextColor(COLORREF crNormalText, COLORREF crSelText)
{
	m_crNormalText =  crNormalText;
	m_crSelText = crSelText;
}

void CScrollWndMsg::SetSelColor(COLORREF crSelBorder, COLORREF crSelFill)
{
	m_crSelBorder = crSelBorder;
	m_crSelFill = crSelFill;
}

void CScrollWndMsg::SetHoverColor(COLORREF crHoverBorder, COLORREF crHoverFill)
{
	m_crHoverBorder = crHoverBorder;
	m_crHoverFill = crHoverFill;
}

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