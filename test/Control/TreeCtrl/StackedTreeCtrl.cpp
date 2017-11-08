#include "stdafx.h"
#include "StackedTreeCtrl.h"

CStackedTreeCtrl::CStackedTreeCtrl()
{
	m_dwWndId = 0;
	m_pParentWnd = NULL;
}

CStackedTreeCtrl::~CStackedTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CStackedTreeCtrl, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL CStackedTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	return CWnd::PreTranslateMessage(pMsg);
}

void CStackedTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

void CStackedTreeCtrl::OnPaint()
{
	CPaintDC dc(this); 
}

BOOL CStackedTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	CPen pen, *pOldPen(NULL);

	CRect rcWnd;
	GetClientRect(&rcWnd);
	rcWnd.DeflateRect(1,1);

	pen.CreatePen(PS_SOLID, 1, RGB(141, 0, 141));
	pOldPen = pDC->SelectObject(&pen);

	pDC->Rectangle(&rcWnd);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	return TRUE;
}

void CStackedTreeCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
}

BOOL CStackedTreeCtrl::Create(DWORD dwStyle, const CRect &pWndRect, CWnd* pParent, UINT nWndId)
{
	if (!CWnd::CreateEx(NULL, NULL, NULL, dwStyle, pWndRect, pParent, nWndId))
	{
		return FALSE;
	}

	m_dwWndId = nWndId;
	m_pParentWnd = pParent;
	return TRUE;
}

BOOL CStackedTreeCtrl::InitializeCtrl()
{
	return TRUE;
}

BOOL CStackedTreeCtrl::InsertRootItem()
{
	DWORD dwCtrlId;
	CString strBtnItem;
	
	CRect rcClient;
	CButton* pbtnItem = NULL;
	
	pbtnItem = new CButton;
	if (pbtnItem == NULL)
	{
		return FALSE;
	}
	
	dwCtrlId = 0;
	strBtnItem = _T("ROOT Item");

	pbtnItem->Create(strBtnItem, WS_CHILD|BS_LEFT, rcClient, NULL, dwCtrlId);
	return TRUE;
}

BOOL CStackedTreeCtrl::InsertChildItem()
{
	return TRUE;
}

BOOL CStackedTreeCtrl::DeleteChildItem()
{
	return TRUE;
}

BOOL CStackedTreeCtrl::ExpandingItem()
{
	return TRUE;
}

BOOL CStackedTreeCtrl::SelchangingItem()
{
	return TRUE;
}

BOOL CStackedTreeCtrl::GetRootNodeData()
{
	return TRUE;
}

BOOL CStackedTreeCtrl::GetChildNodeData()
{
	return TRUE;
}

BOOL CStackedTreeCtrl::PopulateItem()
{
	return TRUE;
}