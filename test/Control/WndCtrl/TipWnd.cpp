#include "stdafx.h"
#include "TipWnd.h"

CTipWnd::CTipWnd()
{
	m_strShowText = _T("12345");
	
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

/////////////////////////////////////////////////////////////////////
//
BOOL CTipWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	pDC->GetClipBox(rc);
	
	CBrush br(GetSysColor(COLOR_INFOBK));
	
	CBrush* pOldBrush = pDC->SelectObject(&br);
	pDC->PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),PATCOPY);

	pDC->SelectObject(pOldBrush);
	return TRUE;
}

void CTipWnd::OnPaint() 
{
	CPaintDC dc(this); 
	
	CRect rc;
	GetClientRect(&rc);

	CFont *pFont = (CFont *)dc.SelectObject(m_pFont);

	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strShowText, rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

	dc.SelectObject(pFont);
}

void CTipWnd::OnDestroy()
{
	CWnd::OnDestroy();
}

/////////////////////////////////////////////////////////////////////
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

void CTipWnd::ShowWnd(CPoint *pt, LPCTSTR lpszTipInfo)
{
	if (pt != NULL)
	{
		m_ptOrigin = *pt;
	}
	else
	{
		GetCursorPos(&m_ptOrigin);
	}
	
	m_strShowText = lpszTipInfo;
}