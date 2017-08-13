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
}

CShellClass::~CShellClass()
{
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
	lptvitem->iImage = GetIcon (lpifq, 
								SHGFI_PIDL | 
								CSIDL_DESKTOP |
								SHGFI_ICON |
								SHGFI_SYSICONINDEX | 
								SHGFI_SMALLICON);

	lptvitem->iSelectedImage = GetIcon (lpifq, 
										SHGFI_PIDL | 
										SHGFI_SYSICONINDEX | 
										SHGFI_SMALLICON | 
										SHGFI_OPENICON);

	return;
}

//bRoot :		是否根目录
//pszBufName :	返回的文件夹名称
//tvins :		返回的Tree结构体信息	
//hParent :		Tree父节点信息
//hPrev :		Tree前一个节点信息
//lpsf  :		IShellFolder接口
//lpifq :		LPITEMIDLIST指针
//lpi   :		LPITEMIDLIST指针
//bChildValid :
BOOL CShellClass::InsertTreeItem(BOOL bRoot, char* pszBufName, TVINSERTSTRUCT* tvins, HTREEITEM hParent, HTREEITEM hPrev, LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, LPITEMIDLIST lpi, BOOL bChildValid)
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
	ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_FILESYSTEM | SFGAO_GHOSTED | SFGAO_LINK | SFGAO_SHARE ;

	if(!(lpi) || (lpi->mkid.cb >= MAX_PATH))
	{
		return FALSE;
	}

	CoInitialize(NULL);
	
	hr = lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
	if (FAILED(hr) || ulAttrs == 0)
	{
		return FALSE;
	}
	else
	{
		SetTvMask(&tvi, ulAttrs, bChildValid);
	}

	hr = SHGetMalloc(&lpMalloc);
	if(FAILED(hr))
	{
		return FALSE;
	}

	if (ulAttrs & SFGAO_FOLDER | SFGAO_FILESYSTEM)
	{
		lptvid = (LPTVITEMDATA*) lpMalloc->Alloc(sizeof (LPTVITEMDATA));
		if (!lptvid)
		{
			bRet = FALSE;
			goto part1; 
		}

		if (!GetName(lpsf, lpi, SHGDN_NORMAL, pszBufName))
		{
			bRet = FALSE;
			goto part1;
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
	}
	
part1:
	lpMalloc->Release();
//	lpsf->Release();
    CoUninitialize();
	
	return bRet;
}

HIMAGELIST CShellClass::GetImageList(BOOL bSmall)
{
	SHFILEINFO sfi;
	HIMAGELIST himl;
	
	if(bSmall)
	{
		himl = (HIMAGELIST)SHGetFileInfo( "", 
                                       0,
                                       &sfi, 
                                       sizeof(SHFILEINFO), 
                                       SHGFI_SYSICONINDEX |
									   SHGFI_SMALLICON);
	}
	else
	{
		himl = (HIMAGELIST)SHGetFileInfo( "", 
                                       0,
                                       &sfi, 
                                       sizeof(SHFILEINFO), 
                                       SHGFI_SYSICONINDEX |
									   SHGFI_LARGEICON);
	}
	
	return himl;
}

int CShellClass::GetIcon (LPITEMIDLIST lpi, UINT uFlags)
{
	SHFILEINFO sfi;
	SHGetFileInfo ((LPCSTR)lpi, 0, &sfi, sizeof (SHFILEINFO), uFlags);
	return sfi.iIcon;
}

int CShellClass::GetNormalIcon(LPITEMIDLIST lpifq)
{
	int nIconIndex;
	nIconIndex = GetIcon (lpifq, 
				 		SHGFI_PIDL | 
						CSIDL_DESKTOP |
						SHGFI_ICON |
						SHGFI_SYSICONINDEX | 
						SHGFI_SMALLICON);

	return nIconIndex;
}

BOOL CShellClass::GetParentID(LPITEMIDLIST pidl)
{
	BOOL fRemoved = FALSE;
	
	if (pidl == NULL)
	{
        return(FALSE);
	}
	
	if (pidl->mkid.cb)
	{
		LPITEMIDLIST pidlNext = pidl;
		while (pidlNext)
        {
           pidl = pidlNext;
		   pidlNext = GetNextItemID(pidl);
        }
		
		pidl->mkid.cb = 0; 
        fRemoved = TRUE;
	}
	
	return fRemoved;
}

BOOL CShellClass::GetName (LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags, LPSTR lpFriendlyName)
{
	BOOL bSuccess = TRUE;
	STRRET str;
	
	if (NOERROR == lpsf->GetDisplayNameOf(lpi, dwFlags, &str))
	{
		switch (str.uType)
		{
		case STRRET_WSTR:
			{
				WideCharToMultiByte (CP_ACP, // code page
									0, // dwFlags
									str.pOleStr, // lpWideCharStr
									-1, // cchWideChar
									lpFriendlyName, // lpMultiByteStr
							//		sizeof (lpFriendlyName), // cchMultiByte
							        sizeof(str), 
									NULL, // lpDefaultChar
									NULL); // lpUsedDefaultChar
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
			
		default:
			{
				bSuccess = FALSE;
			}
			break;
		}	
	}
	else
	{
		bSuccess = FALSE;
	}
	
	return bSuccess;
}

UINT CShellClass::GetSize(LPCITEMIDLIST pidl)
{
	UINT cbTotal = 0; 
	LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl; 
	
	if(pidlTemp)    
	{    
		while(pidlTemp->mkid.cb)       
		{       
			cbTotal += pidlTemp->mkid.cb;       
			pidlTemp = GetNextItem(pidlTemp);       
		}      
		//add the size of the NULL terminating ITEMIDLIST    
		cbTotal += sizeof(ITEMIDLIST);   
	 } 
	 
	return (cbTotal); 
}

LPITEMIDLIST CShellClass::GetNextItemID(LPCITEMIDLIST pidl)
{
	if(pidl == NULL)
	{
		return NULL;
	}
	
	int cb = pidl->mkid.cb; 
	if (cb == 0) 
	{
		return NULL; 
	}
	
	pidl = (LPITEMIDLIST) (((LPBYTE) pidl) + cb); 
	return (pidl->mkid.cb == 0) ? NULL : (LPITEMIDLIST) pidl;
}

LPITEMIDLIST CShellClass::GetNextItem(LPCITEMIDLIST pidl)
{
	if(pidl == NULL)  
	{		
		return NULL;
	}		
	
	return (LPITEMIDLIST)(LPBYTE)(((LPBYTE)pidl) + pidl->mkid.cb); 
}

LPITEMIDLIST CShellClass::Copy(LPMALLOC lpMalloc , LPCITEMIDLIST pidlSource)
{
	LPITEMIDLIST pidlTarget = NULL; 
	UINT cbSource = 0;
	
	if(NULL == pidlSource)
	{
		return (NULL);
	}
	
	cbSource = GetSize(pidlSource);
	
	pidlTarget = (LPITEMIDLIST)lpMalloc->Alloc(cbSource); 
	if(!pidlTarget)
	{		
		return (NULL);
	}
	
	CopyMemory(pidlTarget, pidlSource, cbSource);  
	return pidlTarget; 
}

LPITEMIDLIST CShellClass::CopyItemID(LPMALLOC g_pMalloc , LPITEMIDLIST pidl)
{
	int cb = pidl->mkid.cb; 

	LPITEMIDLIST pidlNew = (LPITEMIDLIST)g_pMalloc->Alloc(cb + sizeof(USHORT)); 
    if (pidlNew == NULL)
	{
		return NULL; 
	}

	CopyMemory(pidlNew, pidl, cb); 

	*((USHORT *) (((LPBYTE) pidlNew) + cb)) = 0; 
	return pidlNew; 
}

LPITEMIDLIST CShellClass::Concatenate(LPMALLOC lpMalloc ,LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	UINT cb1 = 0;
	UINT cb2 = 0;
	
	LPITEMIDLIST pidlNew; 
	
	if(!pidl1 && !pidl2)
	{		
		return NULL;
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

	return pidlNew;
}