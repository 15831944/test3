#include "stdafx.h"
#include "UserListBox.h"

#define MASK_DWDATA		0x01
#define MASK_LPDATA		0x02
#define MASK_IMAGE		0x04
#define MASK_FLAGS		0x08
#define MASK_ALL		0xff

CUserListBox::CUserListBox()
{
}

CUserListBox::~CUserListBox()
{
}

BEGIN_MESSAGE_MAP(CUserListBox, CListBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
void CUserListBox::OnDestroy()
{
}

void CUserListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	int nHeight = 0;

	CRect rcRect;
	CString strText;
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);

	CDC* pDC = GetDC();
	if (pDC == NULL)
	{
		return;
	}

//	CListBox::GetText(lpMeasureItemStruct->itemID, strText);
//	rcRect.SetRect(0, 0, lpMeasureItemStruct->itemWidth, lpMeasureItemStruct->itemHeight);
//	nHeight = pDC->DrawText(strText, -1, rcRect, DT_WORDBREAK | DT_EXPANDTABS | DT_CALCRECT);

	lpMeasureItemStruct->itemWidth  = 650;
	lpMeasureItemStruct->itemHeight = 82;
}

void CUserListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	BOOL bIsFocused  = FALSE;
	BOOL bIsSelected = FALSE;
	BOOL bIsDisabled = FALSE;

	CRect rcItem;
	CRect rcText;
	CRect rcCenter;

	CString strText;
	COLORREF crColor = RGB(0, 0, 0);
	COLORREF crNormal = GetSysColor(COLOR_WINDOW);
	COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crSelected = GetSysColor(COLOR_HIGHLIGHT);

	if (GetCount() <= 0)
	{
		return;
	}

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (pDC == NULL)
	{
		return;
	}

	LISTBOX_DATA *pData = (LISTBOX_DATA*)GetItemDataPtr(lpDrawItemStruct->itemID);
	if (pData == NULL || pData == (LPVOID)-1)
	{
		return;
	}

	bIsFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS);
	bIsSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);
	bIsDisabled = ((lpDrawItemStruct->itemState & ODS_DISABLED));

	rcItem = lpDrawItemStruct->rcItem;
	rcText = lpDrawItemStruct->rcItem;
	rcCenter = lpDrawItemStruct->rcItem;

	pDC->SetBkMode(TRANSPARENT);
	CListBox::GetText(lpDrawItemStruct->itemID, strText);

	rcCenter = rcText;
	pDC->DrawText(strText, -1, rcCenter, DT_WORDBREAK|DT_EXPANDTABS|DT_CALCRECT|pData->nFormat);
	rcCenter.OffsetRect(0, (rcText.Height() - rcCenter.Height())/2);

	if (bIsDisabled)
	{
		crColor = crNormal;
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	else 
	{
		if (bIsSelected)
		{
			crColor = crSelected;
			pDC->SetTextColor(0x00FFFFFF & ~crText);
		}
		else
		{
			crColor = crNormal;
			pDC->SetTextColor(crText);
		}
	}

	pDC->SetBkColor(crColor);
	pDC->FillSolidRect(rcText, crColor);
	pDC->DrawText(strText, -1, rcCenter, DT_WORDBREAK|DT_EXPANDTABS);

	if (bIsFocused && !bIsDisabled)
	{
		pDC->DrawFocusRect(&rcItem);
	}
}

int CUserListBox::SetListItemData(int nIndex, DWORD dwItemData, LPVOID lpData, int nImage, DWORD dwFlag, BYTE byMask)
{
	LISTBOX_DATA *lpLBData = NULL;
	int	nRetValue = LB_ERR;

	lpLBData = (LISTBOX_DATA *)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData == NULL)
	{
		lpLBData = new LISTBOX_DATA;
		if (lpLBData == NULL)
		{
			return -1;
		}
		::ZeroMemory(lpLBData, sizeof(LISTBOX_DATA));
	}

	if (lpLBData)
	{
		if ((byMask&MASK_DWDATA) == MASK_DWDATA)
		{
			lpLBData->dwItemData = dwItemData;
		}

		if ((byMask&MASK_LPDATA) == MASK_LPDATA)
		{
			lpLBData->pData = lpData;
		}
		
		if ((byMask&MASK_IMAGE) == MASK_IMAGE)
		{
			lpLBData->nImage = nImage;
		}
		
		if ((byMask&MASK_FLAGS) == MASK_FLAGS)
		{
			lpLBData->dwFlags = dwFlag;
		}

		nRetValue = CListBox::SetItemDataPtr(nIndex, lpLBData);
	}

	return nRetValue;
}

//////////////////////////////////////////////////////////////////////////
int CUserListBox::AddString(LPCTSTR lpstString)
{
	int	nIndex = LB_ERR;
	nIndex = CListBox::AddString(lpstString);

	if (nIndex != LB_ERR && nIndex != LB_ERRSPACE)
	{
		if (SetListItemData(nIndex, 0, NULL, -1, 0, MASK_ALL) == -1)
		{
			return -1;
		}
	}

	return nIndex;
}

int CUserListBox::InsertItem(int nIndex, LPCTSTR lpszString)
{
	int nIndex1 = LB_ERR;
	nIndex1 = CListBox::InsertString(nIndex, lpszString);

	if (nIndex1 != LB_ERR && nIndex1 != LB_ERRSPACE)
	{
		if (SetListItemData(nIndex1, 0, NULL, -1, 0, MASK_ALL) == -1)
		{
			return -1;
		}
	}

	return nIndex1;
}