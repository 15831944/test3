#include "stdafx.h"
#include "CustomCombox.h"
#include <io.h>

#define MAX_HISTORY_ITEMS	100
#define MAX_DROPDOWN_ITEMS	10

CCustomCombox::CCustomCombox(BOOL bAllowSortStyle)
{
	TRACE(_T("in CCustomCombox::CCustomCombox\n"));
	
	m_nMaxHistoryItems	= MAX_HISTORY_ITEMS;
	m_nDropSize			= MAX_DROPDOWN_ITEMS;
	m_bAllowSortStyle 	= bAllowSortStyle;
	m_bCheckAccess		= FALSE;
	m_bSaveRestoreLastCurrent = TRUE;
}


CCustomCombox::~CCustomCombox()
{
}


BEGIN_MESSAGE_MAP(CCustomCombox, CComboBox)
	//{{AFX_MSG_MAP(CCustomCombox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCustomCombox::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!m_bAllowSortStyle)
	{
		cs.style &= ~CBS_SORT;
	}
	
	return CComboBox::PreCreateWindow(cs);
}


void CCustomCombox::PreSubclassWindow() 
{
	if (! m_bAllowSortStyle && GetStyle() & CBS_SORT)
	{
		TRACE(_T("WARNING: Creating History combo with CBS_SORT style\n"));
	}
	
	CComboBox::PreSubclassWindow();
}


int CCustomCombox::AddString(LPCTSTR lpszString)
{
	if(m_strSection.IsEmpty() || m_strKeyPrefix.IsEmpty())
	{
		return CComboBox::AddString(lpszString);
	}
	
	int nRet   = -1;
	int nIndex = -1;
	int nCount = -1;
	
	CString strItem(lpszString);
	
	strItem.TrimLeft();
	strItem.TrimRight();
	
	if(strItem.IsEmpty())
	{
		TRACE(_T("ERROR  string is empty\n"));
		return CB_ERR;
	}
	
	nRet = CComboBox::InsertString(0, strItem);
	nIndex = FindStringExact(0, strItem);
	if(nIndex != -1 && nIndex != 0)
	{
		DeleteString(nIndex);
	}
	
	nCount = GetCount();
	for(nIndex=m_nMaxHistoryItems; nIndex<nCount; nIndex++)
	{
		DeleteString(m_nMaxHistoryItems);
	}
	
	SetCurSel(nRet);
	return nRet;
}


void CCustomCombox::SaveHistory(BOOL bAddCurrentItemToHistory/*=TRUE*/)
{
	TRACE(_T("in CCustomCombox::SaveHistory\n"));
	if(m_strSection.IsEmpty())
	{
		return;
	}
	
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp);
	
	if(bAddCurrentItemToHistory)
	{
		CString strCurItem;
		GetWindowText(strCurItem);
		
		strCurItem.TrimLeft();
		strCurItem.TrimRight();
		
		if(!strCurItem.IsEmpty())
		{
			AddString(strCurItem);
		}
	}
	
	int nIndex = -1;
	int nMax = min(GetCount(), m_nMaxHistoryItems+1);
	CString strKey, strText;
	
	for(nIndex=0; nIndex<nMax; nIndex++)
	{
		strKey = strText = _T("");
		strKey.Format(_T("%s%04d"), m_strKeyPrefix, nIndex);
		GetLBText(nIndex, strText);
		pApp->WriteProfileString(m_strSection, strKey, strText);
	}
	
	nIndex = nMax;
	do{
		strKey = strText = _T("");
		strKey.Format(_T("%s%04d"), m_strKeyPrefix, nIndex++);
		strText = pApp->GetProfileString(m_strSection, strKey);
		if(!strText.IsEmpty())
		{
			pApp->WriteProfileString(m_strSection, strKey, NULL);
		}
	}while(!strText.IsEmpty() && nIndex < m_nMaxHistoryItems);
	
	if(m_bSaveRestoreLastCurrent)
	{
		GetWindowText(strText);
		if(!m_strKeyCurItem.IsEmpty())
		{
			strKey = m_strKeyCurItem;
		}
		else if(!m_strKeyPrefix.IsEmpty())
		{
			strKey = m_strKeyPrefix;
		}
		else
		{
			strKey = _T("Last");
		}
		
		pApp->WriteProfileString(m_strSection, strKey, strText);
	}
}


void CCustomCombox::ClearHistory(BOOL bDeleteRegistryEntries/*=TRUE*/)
{
	ResetContent();
	
	if(!m_strSection.IsEmpty() && bDeleteRegistryEntries)
	{
		CWinApp* pApp = AfxGetApp();
		ASSERT(pApp);
		
		int nIndex = -1;
		CString strKey, strText;
		
		do{
			strKey.Format(_T("%s%04d"), m_strKeyPrefix, nIndex++);
			strText = pApp->GetProfileString(m_strSection, strKey);
			if(!strText.IsEmpty())
			{
				pApp->WriteProfileString(m_strSection, strKey, NULL);
			}
		}while(!strText.IsEmpty() && nIndex < m_nMaxHistoryItems);
		
		if(!m_strKeyCurItem.IsEmpty())
		{
			strKey = m_strKeyCurItem;
		}
		else if(!m_strKeyPrefix.IsEmpty())
		{
			strKey = m_strKeyPrefix;
		}
		else
		{
			strKey = _T("Last");
		}
		
		pApp->WriteProfileString(m_strSection, strKey, NULL);
	}
}


CString CCustomCombox::LoadHistory(CRecentFileList* pListMRU, BOOL bSelectMostRecent/*=TRUE*/)
{
	if(pListMRU == NULL)
	{
		return _T("");
	}
	
	int nIndex = -1;
	int nCount = pListMRU->GetSize();
	CString strText;
	
	for(nIndex=0; nIndex<nCount; nIndex++)
	{
		CComboBox::AddString((*pListMRU)[nIndex]);
	}
	
	if(bSelectMostRecent)
	{
		SetCurSel(0);
	}
	
	GetWindowText(strText);
	return strText;
}


CString CCustomCombox::LoadHistory(LPCTSTR lpszSection, 
								   LPCTSTR lpszKeyPrefix,    
								   BOOL bSaveRestoreLastCurrent/*=TRUE*/,    
								   LPCTSTR lpszKeyCurItem/*=NULL*/)
{
	if(lpszSection == NULL || lpszKeyPrefix == NULL || *lpszSection == _T('\0'))
	{
		return _T("");
	}
	
	TRACE(_T("lpszSection=%s  lpszKeyPrefix=%s\n"), lpszSection, lpszKeyPrefix);
	
	m_strSection = lpszSection;
	m_strKeyPrefix = lpszKeyPrefix;
	m_strKeyCurItem = (lpszKeyCurItem == NULL) ? _T("") : lpszKeyCurItem;
	m_bSaveRestoreLastCurrent = bSaveRestoreLastCurrent;
	
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp);
	
	int nIndex = 0, nMax = 0;
	CString strText, strKey;
	
	nMax = m_nMaxHistoryItems;
	do{
		strKey.Format(_T("%s%04d"), m_strKeyPrefix, nIndex++);
		strText = pApp->GetProfileString(m_strSection, strKey);
		
		if(m_bCheckAccess)
		{
			if(_taccess(strText, 00) != -1)
			{
				if(!strText.IsEmpty())
				{
					CComboBox::AddString(strText);
				}
			}
		}
		else
		{
			if(!strText.IsEmpty())
			{
				CComboBox::AddString(strText);
			}
		}
	}while(!strText.IsEmpty() && nIndex < nMax);
	
	if(m_bSaveRestoreLastCurrent)
	{
		if(!m_strKeyCurItem)
		{
			strKey = m_strKeyCurItem;
		}
		else if(m_strKeyPrefix.IsEmpty())
		{
			strKey = _T("Last");
		}
		else
		{
			strKey = m_strKeyPrefix;
		}
		
		strText = pApp->GetProfileString(m_strSection, strKey);
		if(!strText.IsEmpty())
		{
			nIndex = FindStringExact(-1, strText);
			if(nIndex != -1)
			{
				SetCurSel(nIndex);
			}
			else if(GetStyle() & CBS_DROPDOWN)
			{
				SetWindowText(strText);
			}
		}
	}
	
	return strText;
}		


void CCustomCombox::SetMaxHistoryItems(int nMaxItems)
{
	int nCount = 0, nIndex = 0;
	m_nMaxHistoryItems = nMaxItems;
	
	nCount = GetCount();
	for(nIndex = m_nMaxHistoryItems; nIndex < nCount; nIndex++)
	{
		DeleteString(m_nMaxHistoryItems);
	}
}


void CCustomCombox::StoreValue(BOOL bIgnoreIfEmpty/*=TRUE*/)
{
	CString strValue;
	GetWindowText(strValue);
	
	if(bIgnoreIfEmpty && strValue.IsEmpty())
	{
		return;
	}
	
	AddString(strValue);
}


void CCustomCombox::OnDropdown() 
{
	CRect rect;
	GetWindowRect(&rect);
	
	int nHeight = rect.Height() + (2 * ::GetSystemMetrics(SM_CXBORDER));
	int nItemHeight = GetItemHeight(0);
	
	nHeight += (m_nDropSize * nItemHeight);
	SetWindowPos(NULL, 0, 0, rect.Width(), nHeight, SWP_NOMOVE | SWP_NOZORDER);
}
						   