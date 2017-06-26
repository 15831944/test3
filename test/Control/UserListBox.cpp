#include "stdafx.h"
#include "UserListBox.h"


IMPLEMENT_DYNAMIC(CUserListBox, CListBox)
CUserListBox::CUserListBox()
			:m_bDown(FALSE)
{
}

CUserListBox::~CUserListBox()
{
}

BEGIN_MESSAGE_MAP(CUserListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()			

void CUserListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(GetCount() == 0)
	{
		return;
	}
	
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	
	LISTBOX_DATA *pData = (LISTBOX_DATA*)GetItemDataPtr(lpDrawItemStruct->itemID);
	
	CString strItem;
	GetText(lpDrawItemStruct->itemID, strItem);
	
	//
	if(lpDrawItemStruct->itemAction | ODA_SELECT && lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		CBrush brush;
	}
	else
	{
		CBrush brush;
	}
	
	//
	pData->rcRect.left   = 
	pData->rcRect.top    = 
	pData->rcRect.right  =
	pData->rcRect.bottom =
	
	CBitmap bitmap;
	bitmap.LoadBitmap(MAKEINTRESOURCE());
	
	//
	HICON hIcon;
	hIcon = (HICON)LoadImage();
	DrawIconEx();
	
	//
	
}

void CUserListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
}

BOOL CUserListBox::OnEraseBkgnd(CDC* pDC)
{
	return CListBox::OnEraseBkgnd(pDC);
}

void CUserListBox::OnMouseMove(UINT nFlags, CPoint point)
{
}

void CUserListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void CUserListBox::OnDestroy()
{
}

void CUserListBox::InsertItem(int nIndex, LPCTSTR lpszItem, LPCTSTR lpszAppend, UNIT uIcon, BOOL state)
{
}