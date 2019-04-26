#ifndef __SHELLCLASS_H__
#define __SHELLCLASS_H__

//http://blog.csdn.net/sam695781276/article/details/47003163
//http://blog.csdn.net/akof1314/article/details/5495956
//http://www.cnblogs.com/jsxyhelu/p/6440910.html
//http://www.freebuf.com/column/134192.html

typedef struct {
	BOOL	bRoot;
	LPITEMIDLIST	lpi;
	LPSHELLFOLDER	lpsfParent;
	LPITEMIDLIST 	lpifq;
}LPTVITEMDATA;	


//CShellManager		//CMFCShellTreeCtrl
class CShellClass
{
public:
	CShellClass();
	~CShellClass();
	
public:
	static CShellClass&	Instance();
		
public:
	void 				SetTvMask(TVITEM *tvi, ULONG ulAttrs, BOOL bChildValid);
	void 				GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem);
	
	BOOL	 			InsertTreeItem(BOOL bRoot, BOOL bChildValid, HTREEITEM hParent, HTREEITEM hPrev, LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, LPITEMIDLIST lpi, char *pszBufName, TVINSERTSTRUCT *tvins);
	
public:	
	HIMAGELIST			GetImageList(BOOL bSmall = TRUE);
	
	int 				GetIcon (LPITEMIDLIST lpi, UINT uFlags);
	int					GetNormalIcon(LPITEMIDLIST lpifq);
	
	BOOL 				GetParentID(LPITEMIDLIST pidl);
	BOOL				GetName (LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags, LPSTR lpFriendlyName);
	
	UINT 				GetSize(LPCITEMIDLIST pidl); 
	LPITEMIDLIST 		GetNextItemID(LPCITEMIDLIST pidl); 
	LPITEMIDLIST 		GetNextItem(LPCITEMIDLIST pidl);
	LPITEMIDLIST 		Copy(LPMALLOC lpMalloc , LPCITEMIDLIST pidlSource);
	LPITEMIDLIST 		CopyItemID(LPMALLOC g_pMalloc , LPITEMIDLIST pidl); 
	LPITEMIDLIST 		Concatenate(LPMALLOC lpMalloc ,LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
};

#endif