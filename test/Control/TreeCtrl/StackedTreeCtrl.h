#pragma once

#include "StackedClass.h"

class CStackedTreeCtrl : public CWnd
{
public:
	CStackedTreeCtrl();
	~CStackedTreeCtrl();

public:
	BOOL					Create(DWORD dwStyle, const CRect &pWndRect, CWnd* pParent, UINT nWndId);

	BOOL					InitializeCtrl();
	BOOL					InsertRootItem();
	BOOL					InsertChildItem();
	BOOL					DeleteChildItem();
	
	BOOL					ExpandingItem();
	BOOL					SelchangingItem();
	
protected:
	BOOL					GetRootNodeData();
	BOOL					GetChildNodeData();
	
	BOOL					PopulateItem();
	
protected:
	CStackedClass			m_StackedClass;
};