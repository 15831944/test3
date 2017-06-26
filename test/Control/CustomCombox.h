#ifndef __CUSTOMCOMBOX_H__
#define __CUSTOMCOMBOX_H__

#ifndef __AFXADV_H__
#include "afxadv.h"
#endif

class CCustomCombox : public CComboBox
{
public:
	CCustomCombox(BOOL bAllowSortStyle = FALSE);
	virtual ~CCustomCombox();

public:
	int 					AddString(LPCTSTR lpszString);
	void					SaveHistory(BOOL bAddCurrentItemtoHistory = TRUE);
	void					ClearHistory(BOOL bDeleteRegistryEntries = TRUE);
	
	CString					LoadHistory(CRecentFileList* pListMRU, BOOL bSelectMostRecent = TRUE);
	CString					LoadHistory(LPCTSTR lpszSection, LPCTSTR lpszKeyPrefix, BOOL bSaveRestoreLastCurrent = TRUE, LPCTSTR lpszKeyCurItem = NULL);
										
	void					SetMaxHistoryItems(int nMaxItems);
	void					StoreValue(BOOL bIgnoreIfEmpty = TRUE);	
	
	int						GetMaxHistoryItems() 								{ return m_nMaxHistoryItems; }	
	void					SetCheckAccess(BOOL bCheckAccess) 					{ m_bCheckAccess = bCheckAccess; }
	BOOL					GetCheckAccess() 									{ return m_bCheckAccess; }
	void					SetDropSize(int nDropSize) 							{ m_nDropSize = nDropSize; }
	int						GetDropSize() 										{ return m_nDropSize; }
	
protected:
	//{{AFX_MSG(CCustomCombox)
	virtual BOOL 			PreCreateWindow(CREATESTRUCT& cs);
	virtual void 			PreSubclassWindow();
	afx_msg void 			OnDropdown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	int 					m_nMaxHistoryItems;	
	int 					m_nDropSize;
	BOOL 					m_bCheckAccess;
	
	CString 				m_strSection;
	CString 				m_strKeyPrefix;
	CString 				m_strKeyCurItem;
	
	BOOL 					m_bSaveRestoreLastCurrent;
	BOOL 					m_bAllowSortStyle;
};
#endif