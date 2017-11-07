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
	m_bRefreshText = FALSE;

	m_bBorder = FALSE;
	m_bBkBitmap  = FALSE;
	m_bWndBorder = FALSE;

	m_strWndText = _T("this is a test!");
	m_strWndTipText = _T("");

	m_pBkBitmap = NULL;

	m_crWndBk = RGB(50,150,200);
	m_crWndBorder = RGB(50,150,200);

	m_crNormalText = RGB(150,250,200);
	m_crWndTipText = RGB(50,150,200);
	
	m_crSelText    = RGB(50,150,200);
	m_crHoverText  = RGB(50,150,200);
	m_crDisableText = RGB(50,150,200);

	m_crSelBorder  = RGB(50,150,200);
	m_crSelFill    = RGB(50,150,200);

	m_crHoverBorder = RGB(50,150,200);
	m_crHoverFill  = RGB(50,150,200);
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

	int nHeight = 0;

	CDC MemDC;
	CBrush brBkGnd;
	CBitmap MemBitmap;

	
	CRect rcClient;
	
	GetClientRect(&rcClient);

	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap *pOldBitmap = MemDC.SelectObject(&MemBitmap);

	if (m_bRefreshSkin=TRUE)
	{
		brBkGnd.CreateSolidBrush(m_crWndBk);
		MemDC.FillRect(&rcClient ,&brBkGnd);
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.SetTextColor(m_crNormalText);

		if (m_bRefreshText=TRUE)
		{
			
//			MemDC.TextOut(0, 0, "this is a test!");
		}

		dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);
	}

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

void CScrollWndMsg::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
}
//////////////////////////////////////////////////////////////////////////
//
void CScrollWndMsg::SetRectPostion1()
{
	CRect rcClient, rcText;
	GetClientRect(&rcClient);

//	int height = dc.DrawText( m_szText, &rectTmp, DT_CALCRECT|DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK );
//	rect.top += (rect.Height() - height)/2;
//	dc.DrawText( m_szText, &rect, DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK );

// 	rcText = rcClient;
// 	nHeight = MemDC.DrawText(m_strWndText, rcText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);
// 	rcText.OffsetRect(0, (rcClient.Height()-nHeight)/2);
// 	MemDC.DrawText(m_strWndText, rcClient, DT_CENTER | DT_WORDBREAK);
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