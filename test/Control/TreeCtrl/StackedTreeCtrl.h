#pragma once

#include "StackedClass.h"

class CStackedTreeCtrl : public CWnd
{
public:
	CStackedTreeCtrl();
	virtual ~CStackedTreeCtrl();

protected:
	virtual BOOL				PreTranslateMessage(MSG* pMsg);
	
	afx_msg void				OnSize(UINT nType, int cx, int cy);
	afx_msg void				OnPaint();
	afx_msg BOOL				OnEraseBkgnd(CDC* pDC);
	afx_msg void				OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

public:
	BOOL						Create(DWORD dwStyle, const CRect &pWndRect, CWnd* pParent, UINT nWndId);

	BOOL						InitializeCtrl();
	BOOL						InsertRootItem();
	BOOL						InsertChildItem();
	BOOL						DeleteChildItem();
	
	BOOL						ExpandingItem();
	BOOL						SelchangingItem();
	
protected:
	BOOL						GetRootNodeData();
	BOOL						GetChildNodeData();
		
	BOOL						PopulateItem();
	
protected:
	CStackedClass				m_StackedClass;

private:
	DWORD						m_dwWndId;
	CWnd*						m_pParentWnd;
};