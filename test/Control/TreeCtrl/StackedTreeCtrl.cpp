#include "stdafx.h"
#include "StackedTreeCtrl.h"

CStackedTreeCtrl::CStackedTreeCtrl()
{
}

CStackedTreeCtrl::~CStackedTreeCtrl()
{
}

BOOL CStackedTreeCtrl::Create(DWORD dwStyle, const CRect &pWndRect, CWnd* pParent, UINT nWndId)
{
	if (!m_hWndGroupBox.Create(_T(""), dwStyle, pWndRect, pParent, nWndId))
	{
		return FALSE;
	}
	
	m_hWndGroupBox.ShowWindow(SW_SHOW);
	return TRUE;
}

BOOL CStackedTreeCtrl::InitializeCtrl()
{
	CTreeCtrl
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
	
	strBtnItem = _T("ROOT Item");
	pbtnItem->Create(strBtnItem, WS_CHILD|BS_LEFT, rcClient, NULL, dwCtrlId);

	//m_hStackedWncCtrl.AddPane(pbtnItem, this);
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