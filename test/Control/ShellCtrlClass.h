#ifndef __SHELLCTRLCLASS_H__
#define __SHELLCTRLCLASS_H__

#include "../inc/common/ShellClass.h"

#define IDS_MEMORY_ERROR "Error Allocating Shell Memory"

#define ID_COL_NAME   0
#define ID_COL_TYPE   1
#define ID_COL_SIZE   2
#define ID_COL_DATE   3

typedef BOOL (*GETSHELLTREE_PATH_CALLBACK_FUNC)(char* pszShellPath);

class CShellListCtrl;
class CShellTreeCtrl;
/////////////////////////////////////////////////////////////////////////////
//
class CShellTreeCtrl : public CTreeCtrl
{
public:
	CShellTreeCtrl();
	virtual ~CShellTreeCtrl();

protected:
	afx_msg void 	OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void 	OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	
public:
	BOOL 			SubclassDlgItem(UINT nID, CWnd* pParent);
	BOOL 			PopulateTree(NMHDR* pNMHDR);
	
	void 			InitializeCtrl();
	void			SetSelectList(CShellListCtrl& hListCtrl);
	
	HTREEITEM 		InsertDesktopItem(LPSHELLFOLDER lpsf);
	UINT 			DeleteChildren(HTREEITEM hItem);

protected:
	void 			SelectThisItem(const char *szBuff);
	void 			SetupImages();

protected:
	CImageList		m_pImageList;
	HTREEITEM       m_pSelectedItem;

	CShellListCtrl* m_pShellListCtrl;

private:
	LPMALLOC        m_pMalloc;		
};

/////////////////////////////////////////////////////////////////////////////
//
class CShellListCtrl : public CListCtrl
{
public:
	CShellListCtrl();
	virtual ~CShellListCtrl();
	
protected:
	afx_msg void 	OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void 	OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void 	OnRclick(NMHDR* pNMHDR, LRESULT* pResult);	
	DECLARE_MESSAGE_MAP()
	
public:
	BOOL 			SubclassDlgItem(UINT nID, CWnd* pParent);
	BOOL 			InsertListViewItem(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPITEMIDLIST lpifq);
	
	int 			InitilizeCtrl(GETSHELLTREE_PATH_CALLBACK_FUNC pCallBackPath = NULL);
	
	void 			SetupImageLists();
	void 			LVPopulateFiles(LPTVITEMDATA* lptvid);
	void 			MyDeleteAllItems();
	void 			ShowStdMenu(BOOL bShowMenu ,  LPTVITEMDATA* lptvid);	
	
protected:
	CImageList      m_pImageListL;
	CImageList      m_pImageListS;
	
	CShellTreeCtrl* m_pShellTreeCtrl;
	GETSHELLTREE_PATH_CALLBACK_FUNC	m_pCallBackShellPath;

private:	
	int             giCtr;
	LPMALLOC        m_pMalloc;
};

/////////////////////////////////////////////////////////////////////////////
//
class CShellContextMenuClass
{
public:
	CShellContextMenuClass(); 
	virtual ~CShellContextMenuClass();

public:
	void ShowMenu(LPTVITEMDATA *lptvid, BOOL bShowMenu, HWND m_hWnd);
	void ShowViewStyleMenu(CListCtrl& mListCtrl);
};

#endif