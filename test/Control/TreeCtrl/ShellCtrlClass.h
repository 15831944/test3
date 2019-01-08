#ifndef __SHELLCTRLCLASS_H__
#define __SHELLCTRLCLASS_H__

#include "../../inc/common/ShellClass.h"

#define IDS_MEMORY_ERROR "Error Allocating Shell Memory"
#define WM_UPDATECTRLDIR_MSG				WM_USER + 0x1002

#define ID_COL_NAME   0
#define ID_COL_TYPE   1
#define ID_COL_SIZE   2
#define ID_COL_DATE   3

typedef BOOL (*GETSHELLTREE_PATH_CALLBACK_FUNC)(char* pszShellPath, void *pParentObject);

class CShellListCtrl;
class CShellTreeCtrl;
/////////////////////////////////////////////////////////////////////////////
//
class CShellTreeCtrl : public CTreeCtrl
{
public:
	CShellTreeCtrl();
	virtual ~CShellTreeCtrl();

public:
	BOOL 			InitializeCtrl();
	BOOL 			SubclassDlgItem(UINT nID, CWnd* pParent);

	void			SetSelectList(CShellListCtrl& hListCtrl);

protected:
	afx_msg void 	OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void 	OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL 			PopulateTree(NMHDR* pNMHDR);
	
	void 			SetTreeImages();
	UINT 			DeleteChildren(HTREEITEM hItem);
	HTREEITEM 		InsertDesktopItem(LPSHELLFOLDER lpsf);
	
protected:
	CImageList		m_pImageList;
	HTREEITEM       m_pSelectedItem;

private:
	LPMALLOC        m_pMalloc;
	CShellListCtrl* m_pShellListCtrl;
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

	afx_msg LRESULT	OnUpdateCtrlItem(WPARAM wParam, LPARAM lParam);
	
public:
	BOOL 			SubclassDlgItem(UINT nID, CWnd* pParent);
	BOOL 			InsertListViewItem(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPITEMIDLIST lpifq);
	
	int 			InitilizeCtrl(void *pParentObject, GETSHELLTREE_PATH_CALLBACK_FUNC pCallBackPath = NULL);
	
	void 			SetupImageLists();
	void 			LVPopulateFiles(LPTVITEMDATA* lptvid);
	void 			MyDeleteAllItems();
	void 			ShowStdMenu(BOOL bShowMenu ,  LPTVITEMDATA* lptvid);	
	
protected:
	CImageList      m_pImageListL;
	CImageList      m_pImageListS;

	LPTVITEMDATA*	m_pTvid;
	
	void*			m_pParam;
	CShellTreeCtrl* m_pShellTreeCtrl;
	GETSHELLTREE_PATH_CALLBACK_FUNC	m_pCallBackShellPath;

private:	
	int             m_giCtr;
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