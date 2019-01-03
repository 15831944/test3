#include "stdafx.h"
#include "ShellCtrlClass.h"

//////////////////////////////////////////////////////////////////////////
//文件树结构
CShellTreeCtrl::CShellTreeCtrl()
{
	m_pShellListCtrl = NULL;
	m_pSelectedItem = NULL;
}

CShellTreeCtrl::~CShellTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CShellTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING,	OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING,		OnSelchanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
void CShellTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if(PopulateTree(pNMHDR))
	{
		*pResult = 0;
	}
	else
	{
		*pResult = 1;
	}
	
	*pResult = 0;
}

void CShellTreeCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	BOOL bRet = FALSE;
	HTREEITEM hItem = NULL;
	LPTVITEMDATA* lptvid = NULL;
	NM_TREEVIEW* pNMTreeView = NULL;

	do 
	{
		pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		if (pNMTreeView == NULL)
		{
			bRet = FALSE;
			break;
		}

		if(!(pNMTreeView->action))
		{
			bRet = FALSE;
			break;
		}

		hItem = pNMTreeView->itemNew.hItem;
		m_pSelectedItem = hItem;

		lptvid = (LPTVITEMDATA*)m_pMalloc->Alloc(sizeof(LPTVITEMDATA));
		if (!lptvid)
		{
			bRet = FALSE;
			break;
		}

		lptvid = (LPTVITEMDATA*)pNMTreeView->itemNew.lParam;
		if(lptvid == NULL)
		{
			bRet = FALSE;
			break;
		}

		if(pNMTreeView->action != 4096)
		{
			if(m_pShellListCtrl != NULL)
			{
				m_pShellListCtrl->LVPopulateFiles(lptvid);
			}
			else
			{
				ULONG uAttr = SFGAO_FOLDER;
				lptvid->lpsfParent->GetAttributesOf(1, (LPCITEMIDLIST *)&lptvid->lpi, &uAttr);
				if(!(uAttr & SFGAO_FOLDER))
				{
					CShellContextMenuClass cmc;
					cmc.ShowMenu(lptvid, FALSE, m_hWnd);
				}
			}
		}

		bRet = TRUE;
	} while (FALSE);
	
	*pResult = 0;
}


BOOL CShellTreeCtrl::PopulateTree(NMHDR* pNMHDR)
{
	HRESULT hr;
	BOOL bRet = FALSE;

	ULONG celtFetched = 0;
	ULONG uAttr = 0;
	char szBufName[MAX_PATH] = {0};

	HTREEITEM hItem = NULL;
	HTREEITEM hParent = NULL;

	LPENUMIDLIST lpe = NULL;	
	LPITEMIDLIST pidlItems = NULL;
	LPENUMIDLIST ppenum = NULL;

	LPTVITEMDATA *lptvid = NULL;
	IShellFolder *psfProgFiles = NULL;
	NM_TREEVIEW  *pNMTreeView = NULL;

	CWaitCursor wait;
	TVINSERTSTRUCT tvins;
	
	do 
	{
		pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		if (pNMTreeView == NULL)
		{
			bRet = FALSE;
			break;
		}

		hItem = pNMTreeView->itemNew.hItem;
		lptvid = (LPTVITEMDATA*)m_pMalloc->Alloc(sizeof(LPTVITEMDATA));
		if (lptvid == NULL)
		{
			bRet = FALSE;
			break;
		}

		lptvid = (LPTVITEMDATA*)pNMTreeView->itemNew.lParam;
		if(lptvid == NULL)
		{
			bRet = FALSE;
			break;
		}

		if(lptvid->bRoot)
		{
			psfProgFiles = lptvid->lpsfParent;
		}
		else
		{
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, NULL, IID_IShellFolder, (LPVOID *) &psfProgFiles);
			if (FAILED(hr))
			{
				bRet = FALSE;
				break;
			}
		}

		if (psfProgFiles == NULL)
		{
			bRet = FALSE;
			break;
		}

		hr = psfProgFiles->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN, &ppenum);
		if(FAILED(hr) || ppenum == NULL)
		{
			bRet = FALSE;
			break;
		}

		DeleteChildren(hItem);
		if(m_pShellListCtrl != NULL)
		{
			m_pShellListCtrl->MyDeleteAllItems();
		}

		while(((hr = ppenum->Next(1, &pidlItems, &celtFetched)) == S_OK) && ((celtFetched) == 1))
		{
			memset(&tvins, 0x0, sizeof(TVINSERTSTRUCT));

			if (pidlItems == NULL)
			{
				continue;
			}

			uAttr = SFGAO_FOLDER;
			hr =psfProgFiles->GetAttributesOf(1, (LPCITEMIDLIST *)&pidlItems, &uAttr);
			if (FAILED(hr) || uAttr == 0)
			{
				continue;
			}

			if(m_pShellListCtrl == NULL)
			{
				if (!CShellClass::Instance().InsertTreeItem(FALSE, TRUE, hItem, NULL, psfProgFiles, lptvid->lpifq, pidlItems, szBufName, &tvins))
				{
					continue;
				}
				else
				{
					HTREEITEM hPrev = InsertItem(&tvins);
				}
			}
			else 
			{
				if(uAttr & SFGAO_FOLDER)
				{
					CShellClass::Instance().InsertTreeItem(FALSE, TRUE, hItem, NULL, psfProgFiles, lptvid->lpifq, pidlItems, szBufName, &tvins);
					HTREEITEM hPrev = InsertItem(&tvins);
				}
			}
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CShellTreeCtrl::SetTreeImages()
{
	HIMAGELIST himagelist;
	himagelist = CShellClass::Instance().GetImageList();

	SetImageList(m_pImageList.FromHandle(himagelist), TVSIL_NORMAL); 
	SetImageList(m_pImageList.FromHandle(himagelist), TVSIL_STATE); 
}

UINT CShellTreeCtrl::DeleteChildren(HTREEITEM hItem)
{
	UINT nCount = 0;

	HTREEITEM hChild = GetChildItem (hItem);
	while (hChild != NULL) 
	{
		HTREEITEM hNextItem = GetNextSiblingItem (hChild);
		DeleteItem (hChild);
	
		hChild = hNextItem;
		nCount++;
	}

	return nCount;
}

HTREEITEM CShellTreeCtrl::InsertDesktopItem(LPSHELLFOLDER lpsf)
{
	BOOL bRet = FALSE;
	
	LPITEMIDLIST lpi = NULL;
	HTREEITEM hParent = NULL;	

	TVINSERTSTRUCT tvins = {0};
	char szBufName[MAX_PATH] = {0};

	do 
	{
		SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &lpi);
		if (!CShellClass::Instance().InsertTreeItem(TRUE, TRUE, NULL, NULL , lpsf , NULL, lpi, szBufName, &tvins))
		{
			bRet = FALSE;
			break;
		}

		hParent = InsertItem(&tvins);
		Expand(hParent, TVE_EXPAND);

		bRet = TRUE;
	} while (FALSE);

	return hParent;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CShellTreeCtrl::InitializeCtrl()
{
	BOOL bRet = FALSE;

	HRESULT hr;
	HTREEITEM hParent = NULL;
	LPSHELLFOLDER lpsf = NULL;

	do 
	{
		hr = SHGetMalloc(&m_pMalloc);
		if (FAILED(hr))
		{
			bRet = FALSE;
			break;
		}

		SetTreeImages();
		ModifyStyle(NULL, TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT, 0);

		hr = SHGetDesktopFolder(&lpsf);
		if (FAILED(hr))
		{
			bRet = FALSE;
			break;
		}

		hParent = InsertDesktopItem(lpsf);
		lpsf->Release();

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CShellTreeCtrl::SubclassDlgItem(UINT nID, CWnd* pParent)
{
	return CTreeCtrl::SubclassDlgItem(nID, pParent);
}

void CShellTreeCtrl::SetSelectList(CShellListCtrl& hListCtrl)
{
	m_pShellListCtrl = &hListCtrl;
}

/////////////////////////////////////////////////////////////////////////////
//文件列表
CShellListCtrl::CShellListCtrl()
{
	m_giCtr = 0;

	m_pTvid = NULL;

	m_pParam = NULL;
	m_pShellTreeCtrl = NULL;
	m_pCallBackShellPath = NULL;

	HRESULT hr = SHGetMalloc(&m_pMalloc);
	if(FAILED(hr))
	{
		AfxMessageBox(IDS_MEMORY_ERROR);
	}
}

CShellListCtrl::~CShellListCtrl()
{
}

BEGIN_MESSAGE_MAP(CShellListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO,	OnGetdispinfo)
	ON_NOTIFY_REFLECT(NM_DBLCLK,		OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK,		OnRclick)
	ON_MESSAGE(WM_UPDATECTRLDIR_MSG,	OnUpdateCtrlItem)
END_MESSAGE_MAP()

void CShellListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	BOOL bRet = FALSE;
	char refTime[32] = {0};
	char sNumBuff[32] = {0};
	char szBuff[MAX_PATH] = {0};

	SHFILEINFO sfi;
	SYSTEMTIME st;
	SHFILEINFO fileInfo;

	WIN32_FIND_DATA fd;
	LPTVITEMDATA* lptvid = NULL;
	
	DWORD dwStyles = SHGFI_PIDL|SHGFI_TYPENAME;
	ULONG uAttr = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_FILESYSTEM | SFGAO_GHOSTED | SFGAO_LINK | SFGAO_SHARE;

	do 
	{
		LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
		if (pDispInfo == NULL)
		{
			bRet = FALSE;
			break;
		}

		lptvid = (LPTVITEMDATA*)m_pMalloc->Alloc(sizeof(LPTVITEMDATA));
		if (lptvid == NULL)
		{
			bRet = FALSE;
			break;
		}
		lptvid = (LPTVITEMDATA*)pDispInfo->item.lParam;

		SHGetFileInfo ((LPCSTR)lptvid->lpi, 0, &sfi, sizeof(SHFILEINFO), dwStyles);
		SHGetDataFromIDList(lptvid->lpsfParent, lptvid->lpi, SHGDFIL_FINDDATA, (WIN32_FIND_DATA*)&fd, sizeof(fd));

		if (pDispInfo->item.mask & LVIF_IMAGE)
		{
			pDispInfo->item.iImage = CShellClass::Instance().GetNormalIcon(lptvid->lpifq);

			lptvid->lpsfParent->GetAttributesOf(1, (LPCITEMIDLIST *) &lptvid->lpi, &uAttr);
			if (uAttr & SFGAO_GHOSTED)
			{
				pDispInfo->item.mask |= LVIF_STATE;
				pDispInfo->item.stateMask = LVIS_CUT;
				pDispInfo->item.state = LVIS_CUT;
			}

			if (uAttr & SFGAO_LINK)
			{
				pDispInfo->item.mask |= LVIF_STATE;
				pDispInfo->item.stateMask = LVIS_OVERLAYMASK;
				pDispInfo->item.state = INDEXTOOVERLAYMASK(2);
			}

			if (uAttr & SFGAO_SHARE)
			{
				pDispInfo->item.mask |= LVIF_STATE;
				pDispInfo->item.stateMask = LVIS_OVERLAYMASK;
				pDispInfo->item.state = INDEXTOOVERLAYMASK(1);
			}
		}

		if (pDispInfo->item.mask & LVIF_TEXT)
		{
			switch(pDispInfo->item.iSubItem)
			{
			case ID_COL_NAME:
				{
					CShellClass::Instance().GetName(lptvid->lpsfParent, lptvid->lpi, SHGDN_NORMAL, szBuff);
					_tcscpy(pDispInfo->item.pszText, szBuff);
				}
				break;
			case ID_COL_TYPE:
				{
					SHGetFileInfo((LPCTSTR)lptvid->lpi, NULL, &fileInfo, sizeof(fileInfo), SHGFI_PIDL|SHGFI_TYPENAME);
					_tcscpy(pDispInfo->item.pszText,fileInfo.szTypeName);
				}
				break;
			case ID_COL_SIZE:
				{
					if(fd.dwFileAttributes != 3435973836)
					{
						if(fd.nFileSizeLow)
						{
							if(fd.nFileSizeLow > 1024)
							{
								ltoa((long)fd.nFileSizeLow/1024,sNumBuff,10);
								strcat(sNumBuff, " KB");
							}
							else
							{
								ltoa((long)fd.nFileSizeLow,sNumBuff,10);
							}
						}
						else
						{
							strcpy(sNumBuff,"");
						}

						_tcscpy(pDispInfo->item.pszText,sNumBuff);
					}
				}
				break;
			case ID_COL_DATE:
				{
					if(fd.dwFileAttributes != 3435973836)
					{
						FileTimeToSystemTime(&fd.ftLastWriteTime, &st);

						wsprintf(refTime, _T("%02u-%02u-%04u"), st.wMonth, st.wDay, st.wYear); 
						_tcscpy(pDispInfo->item.pszText, refTime);
					}
				}
				break;
			}
		}

		bRet = TRUE;
	} while (FALSE);

	*pResult = 0;
}

void CShellListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	BOOL bRet = FALSE;
	LVHITTESTINFO lvhInf = {0};

	do 
	{
		GetCursorPos(&lvhInf.pt);
		ScreenToClient(&lvhInf.pt);

		int item = ListView_HitTest(m_hWnd, &lvhInf);
		if((LVHT_ONITEMLABEL & lvhInf.flags ) || (LVHT_ONITEMICON & lvhInf.flags))
		{
			LPTVITEMDATA* lptvid = NULL;
			lptvid = (LPTVITEMDATA*) m_pMalloc->Alloc (sizeof (LPTVITEMDATA));

			LVITEM lvi;
			lvi.mask = LVIF_PARAM;
			lvi.iItem = lvhInf.iItem;
			ListView_GetItem(m_hWnd, &lvi);
			lptvid = (LPTVITEMDATA*)lvi.lParam;

			ULONG uAttr = SFGAO_FOLDER;
			lptvid->lpsfParent->GetAttributesOf(1, (LPCITEMIDLIST *) &lptvid->lpi, &uAttr);

			if(uAttr & SFGAO_FOLDER)
			{
				char szBuff[MAX_PATH];
				CShellClass csc;

				csc.GetName(lptvid->lpsfParent , lptvid->lpi , SHGDN_NORMAL, szBuff);
				//this->m_pShellTreeCtrl->SelectThisItem(szBuff);
			}
			else
			{
				ShowStdMenu(FALSE, lptvid);
			}
		}

		bRet = TRUE;
	} while (FALSE);
	
	*pResult = 0;
}

void CShellListCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		CShellContextMenuClass cmc;
		cmc.ShowViewStyleMenu(*this);
	}

	*pResult = 0;
}

BOOL CShellListCtrl::SubclassDlgItem(UINT nID, CWnd* pParent)
{
	return CListCtrl::SubclassDlgItem(nID, pParent);
}

BOOL CShellListCtrl::InsertListViewItem(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPITEMIDLIST lpifq)
{
	BOOL bRet = FALSE;
	UINT uFlags;
	int iItem = 0;

	LV_ITEM lvi;
	char szBuff[MAX_PATH] = {0};
	LPTVITEMDATA* lptvid = NULL;

	do 
	{
		lptvid = (LPTVITEMDATA*) m_pMalloc->Alloc(sizeof(LPTVITEMDATA));
		if(lptvid == NULL)
		{
			bRet = FALSE;
			break;
		}

		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		uFlags = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON;
		CShellClass::Instance().GetName(lpsf, lpi, SHGDN_NORMAL, szBuff);

		lvi.iItem = m_giCtr++;
		lvi.iSubItem = ID_COL_NAME;
		lvi.pszText =  LPSTR_TEXTCALLBACK; 

		lvi.iImage = I_IMAGECALLBACK; 
		lptvid->lpsfParent = lpsf;
		lptvid->lpi = CShellClass::Instance().CopyItemID(m_pMalloc, lpi);
		lptvid->lpifq = CShellClass::Instance().Concatenate(m_pMalloc, lpifq,lpi); 
		lvi.lParam = (LPARAM)lptvid;

		iItem = InsertItem(&lvi);
		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

int CShellListCtrl::InitilizeCtrl(void* pParam, GETSHELLTREE_PATH_CALLBACK_FUNC pCallBackPath)
{
	BOOL bRet = FALSE;
	int iWidth = -1;

	do 
	{
		CRect rect;
		GetClientRect(&rect);

		ModifyStyle(NULL, LVS_REPORT | LVS_SHAREIMAGELISTS, 0);	//
		SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		if (pParam != NULL)
		{
			m_pParam = pParam;
		}

		if (pCallBackPath != NULL)
		{
			m_pCallBackShellPath = pCallBackPath;
		}

		SetupImageLists();

		iWidth = rect.Width()/3;
		InsertColumn(ID_COL_NAME, "名字", LVCFMT_LEFT,  iWidth, -1);

		iWidth = (rect.Width()-iWidth)/2;
		InsertColumn(ID_COL_SIZE, "大小", LVCFMT_RIGHT, iWidth, -1);
		InsertColumn(ID_COL_DATE, "日期", LVCFMT_LEFT,  iWidth, -1);

		bRet = TRUE;
	} while (FALSE);

	return bRet ? 0 : -1;
}

void CShellListCtrl::SetupImageLists()
{
	BOOL bRet = FALSE;
	HIMAGELIST himlSmall, himlLarge;

	do 
	{
		himlSmall = CShellClass::Instance().GetImageList(TRUE);
		himlLarge = CShellClass::Instance().GetImageList(FALSE);

		SetImageList(m_pImageListL.FromHandle(himlLarge), LVSIL_NORMAL); 
		SetImageList(m_pImageListS.FromHandle(himlSmall), LVSIL_SMALL); 
		SetImageList(m_pImageListS.FromHandle(himlSmall), LVSIL_STATE); 

		bRet = TRUE;
	} while (FALSE);
}

void CShellListCtrl::LVPopulateFiles(LPTVITEMDATA* lptvid)
{
	BOOL bRet = FALSE;

	HRESULT hr;
	ULONG ulAttrs = 0;
	ULONG celtFetched;

	char szShellPath[MAX_PATH] = {0};

	LPITEMIDLIST pidlItems = NULL;
	LPENUMIDLIST ppenum = NULL;
	IShellFolder *psfProgFiles = NULL;

	do 
	{
		m_pTvid = lptvid;
		if(lptvid->bRoot)
		{
			psfProgFiles = lptvid->lpsfParent;
		}
		else
		{
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, NULL, IID_IShellFolder, (LPVOID *)&psfProgFiles);
			if(FAILED(hr) || psfProgFiles == NULL)
			{
				bRet = FALSE;
				break;
			}
		}

		ulAttrs = SFGAO_FILESYSTEM;
		hr = psfProgFiles->GetAttributesOf(1, (const struct _ITEMIDLIST **)&pidlItems, &ulAttrs);
		if (FAILED(hr) || ulAttrs == 0)
		{
			bRet = FALSE;
			break;
		}

		MyDeleteAllItems();
		SetRedraw(FALSE);

		if (ulAttrs&SFGAO_FILESYSTEM)
		{
			if(SHGetPathFromIDList(lptvid->lpifq, szShellPath))
			{
				m_pCallBackShellPath(szShellPath, m_pParam);
			}

			hr = psfProgFiles->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN, &ppenum);
			if(FAILED(hr))
			{
				return;
			}

			while(hr=ppenum->Next(1, &pidlItems, &celtFetched) == S_OK && (celtFetched) == 1)
			{
				InsertListViewItem(psfProgFiles, pidlItems, lptvid->lpifq);
			}
		}

		SetRedraw(TRUE);
		bRet = TRUE;
	} while (FALSE);
}

void CShellListCtrl::MyDeleteAllItems()
{
	BOOL bRet = FALSE;

	do 
	{
		m_giCtr = 0;
		DeleteAllItems();

		bRet = TRUE;
	} while (FALSE);
}

void CShellListCtrl::ShowStdMenu(BOOL bShowMenu ,  LPTVITEMDATA* lptvid)
{
	CShellContextMenuClass cmc;
	cmc.ShowMenu(lptvid, bShowMenu, m_hWnd);
}

LRESULT CShellListCtrl::OnUpdateCtrlItem(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;

	do 
	{
		if (m_pTvid != NULL)
		{
			LVPopulateFiles(m_pTvid);
		}

		bRet = TRUE;
	} while (FALSE);
	
	return bRet ? 0 : -1;
}

/////////////////////////////////////////////////////////////////////////////
//
CShellContextMenuClass::CShellContextMenuClass()
{
}

CShellContextMenuClass::~CShellContextMenuClass()
{
}

void CShellContextMenuClass::ShowMenu(LPTVITEMDATA *lptvid, BOOL bShowMenu, HWND m_hWnd)
{
	HRESULT hr;
	HMENU hMenuPopup;
	IContextMenu   *icm;
	IContextMenu2* icm2;
	static POINT point;
	GetCursorPos(&point);

	hr = lptvid->lpsfParent->GetUIObjectOf(m_hWnd, 
		1, 
		(LPCITEMIDLIST*)&lptvid->lpi,
		IID_IContextMenu, 
		NULL, 
		(LPVOID*)&icm);

	if(FAILED(hr))
		return;

	hMenuPopup = CreatePopupMenu();
	if(!hMenuPopup)
		return;

	hr = icm->QueryContextMenu(hMenuPopup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);
	if(FAILED(hr))
		return;

	icm->QueryInterface(IID_IContextMenu2, (LPVOID*)&icm2);
	UINT  id;
	if(bShowMenu)
		id = TrackPopupMenu(hMenuPopup, 
		TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, 
		point.x,point.y,0,m_hWnd,NULL);
	else
		id = hr;

	if(!id)
		return;

	CMINVOKECOMMANDINFO  ici;
	ici.cbSize = sizeof(CMINVOKECOMMANDINFO);
	ici.fMask = 0;
	ici.hwnd = m_hWnd;
	ici.lpVerb = (LPCSTR)(INT_PTR)(id - 1);
	ici.lpParameters = NULL;
	ici.lpDirectory = NULL;
	ici.nShow = SW_SHOWNORMAL;
	ici.dwHotKey = 0;
	ici.hIcon = NULL;
	hr = icm->InvokeCommand(&ici);
}

void CShellContextMenuClass::ShowViewStyleMenu(CListCtrl& mListCtrl)
{
#define ID_LARGEICONS 5001
#define ID_SMALLICONS 5002
#define ID_LIST       5003
#define ID_DETAILS    5004
#define ID_LINES      5005

	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING | MF_ENABLED, ID_LARGEICONS, "&Large Icons");
	menu.AppendMenu(MF_STRING | MF_ENABLED, ID_SMALLICONS, "&Small Icons");
	menu.AppendMenu(MF_STRING | MF_ENABLED, ID_LIST      , "L&ist");
	menu.AppendMenu(MF_STRING | MF_ENABLED, ID_DETAILS   , "&Details");

	POINT pt;
	GetCursorPos(&pt);
	UINT mOptionSelected = menu.TrackPopupMenu(TPM_LEFTALIGN |
		TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, mListCtrl.FromHandle(mListCtrl.m_hWnd));

	switch(mOptionSelected)
	{
	case ID_LARGEICONS:
		mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
		break;
	case ID_SMALLICONS:
		mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
		break;
	case ID_LIST:
		mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
		break;
	case ID_DETAILS:
		mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
		break;
	}
}