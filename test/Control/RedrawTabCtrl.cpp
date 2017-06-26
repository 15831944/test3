// RedrawTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "RedrawTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRedrawTabCtrl

CRedrawTabCtrl::CRedrawTabCtrl()
{
}

CRedrawTabCtrl::~CRedrawTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CRedrawTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CRedrawTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRedrawTabCtrl message handlers

void CRedrawTabCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTabCtrl::PreSubclassWindow();
}

BOOL CRedrawTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CTabCtrl::OnEraseBkgnd(pDC);
}

void CRedrawTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CTabCtrl::OnPaint() for painting messages
}

void CRedrawTabCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
