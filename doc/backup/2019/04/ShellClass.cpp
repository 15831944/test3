#include "stdafx.h"
#include "ShellClass.h"

/************************************************************************/
/*	author : wl
 *	email  : lysgwl@163.com
 *  date   : 2017.08.10 09:38
*/
/************************************************************************/

CShellClass::CShellClass()
{
	CoInitialize(NULL);
}

CShellClass::~CShellClass()
{
	CoUninitialize();
}

CShellClass& CShellClass::Instance()
{
	static CShellClass inst;
	return inst;
}

void CShellClass::SetTvMask(TVITEM *tvi, ULONG ulAttrs, BOOL bChildValid)
{
	tvi->mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	if (ulAttrs & SFGAO_HASSUBFOLDER)
	{
		if(bChildValid)
		{
			tvi->cChildren = 1;
			tvi->mask |= TVIF_CHILDREN;
		}
	}

	if (ulAttrs & SFGAO_GHOSTED)
	{
		tvi->mask |= LVIF_STATE;
		tvi->stateMask = LVIS_CUT;
		tvi->state = LVIS_CUT;
	}

	if (ulAttrs & SFGAO_LINK)
	{
		tvi->mask |= LVIF_STATE;
		tvi->stateMask = LVIS_OVERLAYMASK;
		tvi->state = INDEXTOOVERLAYMASK(2);
	}

	if (ulAttrs & SFGAO_SHARE)
	{
		tvi->mask |= LVIF_STATE;
		tvi->stateMask = LVIS_OVERLAYMASK;
		tvi->state = INDEXTOOVERLAYMASK(1);
	}
}

void CShellClass::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	lptvitem->iImage = GetIcon(lpifq, SHGFI_PIDL|CSIDL_DESKTOP|SHGFI_ICON|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	lptvitem->iSelectedImage = GetIcon(lpifq, SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON);
}

//bRoot :		是否根目录
//hParent :		Tree父节点信息
//hPrev :		Tree前一个节点信息
//lpsf  :		IShellFolder接口
//lpifq :		LPITEMIDLIST指针
//lpi   :		LPITEMIDLIST指针
//pszBufName :	返回的文件夹名称
//tvins :		返回的Tree结构体信息
BOOL CShellClass::InsertTreeItem(BOOL bRoot, BOOL bChildValid, HTREEITEM hParent, HTREEITEM hPrev, LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, LPITEMIDLIST lpi, char* pszBufName, TVINSERTSTRUCT* tvins)
{
	BOOL bRet = FALSE;

	HRESULT hr;
	UINT uCount = 0;
	
	TV_ITEM tvi;
	LPMALLOC lpMalloc;

	LPENUMIDLIST lpe = NULL;
	LPITEMIDLIST lpiTemp = NULL;
	LPITEMIDLIST lpifqThisItem = NULL;

	LPTVITEMDATA* lptvid = NULL;
	
	char szShellName[MAX_PATH] = {0};
	ULONG ulAttrs = SFGAO_FOLDER | SFGAO_HASSUBFOLDER | SFGAO_FILESYSTEM | SFGAO_GHOSTED | SFGAO_SHARE | SFGAO_LINK;

	do 
	{
		if(!lpi || (lpi->mkid.cb >= MAX_PATH))
		{
			bRet = FALSE;
			break;
		}

		hr = lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
		if (FAILED(hr) || ulAttrs == 0)
		{
			bRet = FALSE;
			break;
		}

		SetTvMask(&tvi, ulAttrs, bChildValid);

		hr = SHGetMalloc(&lpMalloc);
		if(FAILED(hr))
		{
			bRet = FALSE;
			break;
		}

		if (!(ulAttrs&(SFGAO_FOLDER|SFGAO_FILESYSTEM)))
		{
			bRet = FALSE;
			break;
		}

		lptvid = (LPTVITEMDATA*)lpMalloc->Alloc(sizeof (LPTVITEMDATA));
		if (!lptvid)
		{
			bRet = FALSE;
			break;
		}

		if (!GetName(lpsf, lpi, SHGDN_NORMAL, pszBufName))
		{
			bRet = FALSE;
			break;
		}

		tvi.pszText = pszBufName;
		tvi.cchTextMax = MAX_PATH;

		lptvid->lpi = CopyItemID(lpMalloc, lpi);

		lptvid->lpsfParent = lpsf;
		lpsf->AddRef();

		lptvid->bRoot = bRoot;
		lptvid->lpifq = Concatenate(lpMalloc, lpifq, lpi);

		GetNormalAndSelectedIcons(lptvid->lpifq, &tvi);

		tvi.lParam = (LPARAM)lptvid;

		tvins->item = tvi;
		tvins->hInsertAfter = TVI_LAST;//hPrev;
		tvins->hParent = hParent;

		bRet = TRUE;
	} while (FALSE);
	
	if (!bRet)
	{
		lpMalloc->Release();
		//lpsf->Release();
	}

	return bRet;
}

HIMAGELIST CShellClass::GetImageList(BOOL bSmall)
{
	SHFILEINFO sfi = {0};
	HIMAGELIST himl = NULL;
	
	if(!bSmall)
	{
		himl = (HIMAGELIST)SHGetFileInfo(_T(""), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_LARGEICON);
	}
	else
	{
		himl = (HIMAGELIST)SHGetFileInfo(_T(""), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	}
	
	return himl;
}

int CShellClass::GetIcon(LPITEMIDLIST lpi, UINT uFlags)
{
	SHFILEINFO sfi = {0};
	SHGetFileInfo ((LPCSTR)lpi, 0, &sfi, sizeof (SHFILEINFO), uFlags);

	return sfi.iIcon;
}

int CShellClass::GetNormalIcon(LPITEMIDLIST lpifq)
{
	int nIconIndex;
	nIconIndex = GetIcon (lpifq, SHGFI_PIDL|CSIDL_DESKTOP|SHGFI_ICON|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);

	return nIconIndex;
}

BOOL CShellClass::GetParentID(LPITEMIDLIST pidl)
{
	BOOL bRet = FALSE;
	LPITEMIDLIST pidlNext = NULL;
	
	do 
	{
		if (pidl == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (!pidl->mkid.cb)
		{
			bRet = FALSE;
			break;
		}

		pidlNext = pidl;
		while (pidlNext)
		{
			pidl = pidlNext;
			pidlNext = GetNextItemID(pidl);
		}

		pidl->mkid.cb = 0; 
		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

BOOL CShellClass::GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags, LPSTR lpFriendlyName)
{
	BOOL bRet = FALSE;
	STRRET str;
	
	do 
	{
		if (lpsf->GetDisplayNameOf(lpi, dwFlags, &str) != NOERROR)
		{
			bRet = FALSE;
			break;
		}

		switch (str.uType)
		{
		case STRRET_WSTR:
			{
				WideCharToMultiByte (CP_ACP, 0, str.pOleStr, -1,lpFriendlyName, sizeof(str), NULL, NULL);
			}
			break;

		case STRRET_OFFSET:
			{
				lstrcpy (lpFriendlyName, (LPSTR)lpi + str.uOffset);
			}
			break;

		case STRRET_CSTR:
			{
				lstrcpy (lpFriendlyName, (LPSTR) str.cStr);
			}
			break;
		}

		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

UINT CShellClass::GetSize(LPCITEMIDLIST pidl)
{
	BOOL bRet = FALSE;

	UINT cbTotal = 0; 
	LPITEMIDLIST pidlTemp = NULL;
	
	do 
	{
		pidlTemp = (LPITEMIDLIST)pidl;
		if (pidlTemp == NULL)
		{
			bRet = FALSE;
			break;
		}

		while(pidlTemp->mkid.cb)       
		{       
			cbTotal += pidlTemp->mkid.cb;       
			pidlTemp = GetNextItem(pidlTemp);       
		}  

		cbTotal += sizeof(ITEMIDLIST);
		bRet = TRUE;
	} while (FALSE);

	return cbTotal; 
}

LPITEMIDLIST CShellClass::GetNextItemID(LPCITEMIDLIST pidl)
{
	BOOL bRet = FALSE;

	int cb = -1;
	LPITEMIDLIST pidlTemp = NULL;

	do 
	{
		if(pidl == NULL)
		{
			bRet = FALSE;
			break;
		}

		cb = pidl->mkid.cb;
		if (cb == 0)
		{
			bRet = FALSE;
			break;
		}

		pidlTemp = (LPITEMIDLIST)(((LPBYTE) pidl) + cb); 
	} while (FALSE);
	
	return (pidlTemp->mkid.cb == 0)?NULL:(LPITEMIDLIST)pidlTemp;
}

LPITEMIDLIST CShellClass::GetNextItem(LPCITEMIDLIST pidl)
{
	BOOL bRet = FALSE;
	
	LPITEMIDLIST pidlTemp = NULL;

	do 
	{
		if(pidl == NULL)  
		{		
			return NULL;
		}		

		pidlTemp = (LPITEMIDLIST)(LPBYTE)(((LPBYTE)pidl) + pidl->mkid.cb);
		bRet = TRUE;
	} while (FALSE);
	
	return pidlTemp; 
}

LPITEMIDLIST CShellClass::Copy(LPMALLOC lpMalloc , LPCITEMIDLIST pidlSource)
{
	BOOL bRet = FALSE;

	UINT cbSource = 0;
	LPITEMIDLIST pidlTarget = NULL; 
	
	do 
	{
		if(NULL == pidlSource)
		{
			bRet = FALSE;
			break;
		}

		cbSource = GetSize(pidlSource);

		pidlTarget = (LPITEMIDLIST)lpMalloc->Alloc(cbSource); 
		if(!pidlTarget)
		{		
			bRet = FALSE;
			break;
		}
		
		CopyMemory(pidlTarget, pidlSource, cbSource);
		bRet = TRUE;
	} while (FALSE);
	  
	return pidlTarget; 
}

LPITEMIDLIST CShellClass::CopyItemID(LPMALLOC g_pMalloc , LPITEMIDLIST pidl)
{
	BOOL bRet = FALSE;

	int cb = -1;
	LPITEMIDLIST pidlNew = NULL;

	do 
	{
		cb = pidl->mkid.cb;
		pidlNew = (LPITEMIDLIST)g_pMalloc->Alloc(cb + sizeof(USHORT)); 
		if (pidlNew == NULL)
		{
			bRet = FALSE;
			break;
		}

		CopyMemory(pidlNew, pidl, cb); 
		*((USHORT *) (((LPBYTE) pidlNew) + cb)) = 0; 

		bRet = TRUE;
	} while (FALSE);

	return pidlNew; 
}

LPITEMIDLIST CShellClass::Concatenate(LPMALLOC lpMalloc ,LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	BOOL bRet = FALSE;

	UINT cb1 = 0;
	UINT cb2 = 0;
	LPITEMIDLIST pidlNew = NULL; 
	
	do 
	{
		if(!pidl1 && !pidl2)
		{		
			bRet = FALSE;
			break;
		}

		if(!pidl1)    
		{    
			pidlNew = Copy(lpMalloc , pidl2);     
			return pidlNew;    
		}

		if(!pidl2)    
		{    
			pidlNew = Copy(lpMalloc, pidl1);     
			return pidlNew;    
		}

		cb1 = GetSize(pidl1) - sizeof(ITEMIDLIST);  
		cb2 = GetSize(pidl2);

		pidlNew = (LPITEMIDLIST)lpMalloc->Alloc(cb1 + cb2);  
		if(pidlNew)    
		{ 
			CopyMemory(pidlNew, pidl1, cb1);         
			CopyMemory(((LPBYTE)pidlNew) + cb1, pidl2, cb2);    
		} 

		bRet = TRUE;
	} while (FALSE);

	return pidlNew;
}