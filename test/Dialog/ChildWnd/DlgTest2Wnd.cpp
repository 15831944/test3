#include "stdafx.h"
#include "DlgTest2Wnd.h"

extern BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

IMPLEMENT_DYNAMIC(CDlgTest2Wnd, CDialog)
CDlgTest2Wnd::CDlgTest2Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest2Wnd::IDD, pParent)
{
	m_bInited = FALSE;
	m_bShowing = FALSE;

	m_nPrePage = -1;
	m_nDefaultSel = -1;
	m_nCurSelIndex = -1;

	m_strAppPath = _T("");
	m_strShellPath = _T("");
	m_strDefaultPath = _T("");
	
	m_hEditWnd = NULL;
	m_OldEditProc = NULL;
}

CDlgTest2Wnd::~CDlgTest2Wnd()
{
}

void CDlgTest2Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_SYSDIR,			m_hSysDirTree);
	DDX_Control(pDX, IDC_LIST_SYSDIR,			m_hSysDirList);
	DDX_Control(pDX, IDC_COMBO_EVALNAME,		m_hComboEval);
}

BEGIN_MESSAGE_MAP(CDlgTest2Wnd, CDialog)
	ON_WM_CREATE()

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_BTN_RUNMODIFY,			OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BTN_OPENFLODER,			OnBnClickedBtnOpenFloder)

	ON_CBN_SELCHANGE(IDC_COMBO_EVALNAME,		OnCbnSelchangeComboEvalname)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SYSDIR,	OnTvnSelchangedTree)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
int CDlgTest2Wnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (!CreateChildWnd())
	{
		return -1;
	}

	return 0;
}

BOOL CDlgTest2Wnd::OnInitDialog()
{
	BOOL bRet = FALSE;
	CDialog::OnInitDialog();

	do 
	{
		if (!Init())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);
	
	if (!bRet)
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
	}

	return bRet;  
}

BOOL CDlgTest2Wnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTest2Wnd::OnPaint()
{
	CPaintDC dc(this);

	if (!m_bInited)
	{
		return;
	}

	DrawWndImage(&dc);
}

BOOL CDlgTest2Wnd::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgTest2Wnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInited)
	{
		return;
	}

	SetWndControlLayout();
}

void CDlgTest2Wnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_bShowing = bShow;
	CDialog::OnShowWindow(bShow, nStatus);
}

//////////////////////////////////////////////////////////////////////////
//
void CDlgTest2Wnd::OnBnClickedBtnOpenFloder()
{
}

void CDlgTest2Wnd::OnBnClickedButtonRun()
{
	BOOL bRet = FALSE;

	CString strPrompt;
	update_file_name filenameFunc;

	do 
	{
		if (m_vecFileInfo.size() == 0)
		{
			bRet = FALSE;
			break;
		}

		if (!update_file_data::Instance().SetUpdateFileData(m_vecFileInfo, GetUpdateFileData, this))
		{
			bRet = FALSE;
			break;
		}

		if (!filenameFunc.CreateUpdateProc())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		if (strPrompt != _T(""))
		{
			::MessageBox(NULL, strPrompt, _T("����!"), MB_ICONWARNING|MB_OK);
		}
	}
}

void CDlgTest2Wnd::OnCbnSelchangeComboEvalname()
{
	int nIndex = 0;
	int nCurSel = 0;
	DWORD dwDataPtr = 0;

	nIndex = m_hComboEval.GetCurSel();
	if (nIndex == -1)
	{
		return;
	}

	nCurSel = nIndex+1;
	m_nCurSelIndex = nIndex;

	if (nCurSel != m_nPrePage)
	{
		((CDialog*)m_pArPage[m_nPrePage])->ShowWindow(SW_HIDE);
		((CDialog*)m_pArPage[nCurSel])->ShowWindow(SW_SHOW);

		m_nPrePage = nCurSel;
	}
}

void CDlgTest2Wnd::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	BOOL bRet = FALSE;

	CString strShellPath;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	do 
	{
		if (!m_hSysDirTree.GetItemPath(strShellPath, pNMTreeView->itemNew.hItem))
		{
			bRet = FALSE;
			break;
		}

		ShowShellPathFile(strShellPath);

		bRet = TRUE;
	} while (FALSE);
	
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest2Wnd::GetUpdateFileData(void *pUpdateData, void *pParentObject)
{
	BOOL bRet = FALSE;

	UPDATE_FILEDATA stcFileData;

	CDlgTest2Wnd* pWndInfo = NULL;
	UPDATE_FILEDATA* pFileData = NULL;
	
	do 
	{
		pWndInfo = (CDlgTest2Wnd*)pParentObject;
		if (pWndInfo == NULL)
		{
			bRet = FALSE;
			break;
		}

		pFileData = (UPDATE_FILEDATA*)pUpdateData;
		if (pFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		memset(&stcFileData, 0x0, sizeof(UPDATE_FILEDATA));
		if (pFileData->emUpdateStatus == STATE_UPDATEINPUTE_TYPE)
		{
			if (!pWndInfo->GetCurConfigData(&stcFileData))
			{
				bRet = FALSE;
				break;
			}

			memcpy(pUpdateData, &stcFileData, sizeof(UPDATE_FILEDATA));
		}
		else if (pFileData->emUpdateStatus == STATE_UPDATESUCCED_TYPE
				 || pFileData->emUpdateStatus == STATE_UPDATEFAILED_TYPE)
		{
			pWndInfo->ShowShellPathFile(pWndInfo->m_strShellPath);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::GetShellTreePath(char* pszShellPath, void *pParentObject)
{
	BOOL bRet = FALSE;
	CDlgTest2Wnd* pWndInfo = NULL;

	do 
	{
		pWndInfo = (CDlgTest2Wnd*)pParentObject;
		if (pWndInfo == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (pszShellPath == NULL || strcmp(pszShellPath, _T("")) == 0)
		{
			bRet = FALSE;
			break;
		}

		pWndInfo->m_strShellPath = pszShellPath;
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

LRESULT CDlgTest2Wnd::EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	TCHAR	szPath[_MAX_PATH] = {0};
	CString strFilePath;

	CDlgTest2Wnd *pDlg = (CDlgTest2Wnd*)GetWindowLong(hwnd, GWL_USERDATA);
	if (pDlg->m_hEditWnd == hwnd)
	{
		switch(uMsg)
		{
		case WM_CONTEXTMENU:
			{
			}
			break;

		case WM_LBUTTONDOWN:
			{
				BROWSEINFO bi;
				ZeroMemory(&bi,sizeof(BROWSEINFO));

				bi.hwndOwner = pDlg->GetSafeHwnd();
				bi.pidlRoot  = NULL;
				bi.lpszTitle = _T("ѡ���ļ�·��:");
				bi.pszDisplayName  = szPath;
				bi.ulFlags   = BIF_RETURNONLYFSDIRS;
				bi.lpfn      = NULL;
				bi.lParam    = NULL;

				LPITEMIDLIST pItemIDList=SHBrowseForFolder(&bi);
				if (pItemIDList)
				{
					if (SHGetPathFromIDList(pItemIDList, szPath))
					{
						strFilePath = szPath;
					}
					else
					{
						strFilePath = _T("");
					}

					if(pItemIDList != NULL)  
					{  
						::CoTaskMemFree(pItemIDList); 
						pItemIDList = NULL;
					}
				}
				else
				{
					strFilePath = _T("");	
				}

				if (strFilePath == _T(""))
				{
					return 0;
				}

				::SetWindowText(hwnd, strFilePath);
			}
			break;
		}

		return CallWindowProc(pDlg->m_OldEditProc, hwnd, uMsg, wParam, lParam);
	}

	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest2Wnd::Init()
{
	BOOL bRet = FALSE;

	do 
	{
		if (!InitCtrl())
		{
			bRet = FALSE;
			break;
		}

		if (!InitInfo())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

BOOL CDlgTest2Wnd::InitCtrl()
{
	BOOL bRet = FALSE;

	do 
	{
		if (!InitWndSkin())
		{
			bRet = FALSE;
			break;
		}

		if (!InitWndInfo())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::InitInfo()
{
	BOOL bRet = FALSE;

	do 
	{
/*
		TCHAR lpszDesktopPath[_MAX_PATH] ={0};
		if (::SHGetSpecialFolderPath(this->GetSafeHwnd(), lpszDesktopPath, CSIDL_DESKTOP, NULL))
		{
			m_strDefaultPath = lpszDesktopPath;
		}
		else
		{
			m_strDefaultPath = _T("C:\\");
		}

		m_strAppPath = pGlobal->GetAppPath();

		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
		if (pEdit)
		{
			pEdit->SetReadOnly(TRUE);
			m_hEditWnd = pEdit->GetSafeHwnd();

			SetWindowLong(m_hEditWnd, GWL_USERDATA, (DWORD)this);
			m_OldEditProc = (WNDPROC)GetWindowLong(m_hEditWnd, GWL_WNDPROC);
			SetWindowLong(m_hEditWnd, GWL_WNDPROC, (DWORD)EditWndProc);
		}
*/

#if 0
		//this->GetDlgItem(IDC_EDIT_FINDNAME)->SendMessage(EM_SETCUEBANNER, 0, (LPARAM)(LPCWSTR)L"������ҵ�����...");
		//this->GetDlgItem(IDC_EDIT_SUBNAME)->SendMessage(EM_SETCUEBANNER, 0, (LPARAM)(LPCWSTR)L"�����滻������...");
#endif

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::CreateChildWnd()
{
	BOOL bRet = FALSE;

	do 
	{
		m_dlgShowLogoWnd.Create(CDlgShowLogoWnd::IDD, this);
		m_dlgFileNameAdd.Create(CDlgFileNameAdd::IDD, this);
		m_dlgFileNameDate.Create(CDlgFileNameDate::IDD, this);
		m_dlgFileNameDel.Create(CDlgFileNameDel::IDD, this);
		m_dlgFileNameExt.Create(CDlgFileNameExt::IDD, this);
		m_dlgFileNameIndex.Create(CDlgFileNameIndex::IDD, this);
		m_dlgFileNameReplace.Create(CDlgFileNameReplace::IDD, this);

		m_pArPage.Add(&m_dlgShowLogoWnd);
		m_pArPage.Add(&m_dlgFileNameAdd);
		m_pArPage.Add(&m_dlgFileNameDel);
		m_pArPage.Add(&m_dlgFileNameExt);
		m_pArPage.Add(&m_dlgFileNameReplace);
		m_pArPage.Add(&m_dlgFileNameDate);
		m_pArPage.Add(&m_dlgFileNameIndex);

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::InitWndSkin()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::InitWndInfo()
{
	BOOL bRet = FALSE;

	CString strPrompt;
	CString strListHeader;

	do 
	{
		if (!SetChildWnd(TRUE))
		{
			bRet = FALSE;
			break;
		}

		//TreeCtrl:
		//m_hSysDirTree.SetFlags(SHCONTF_FOLDERS |SHCONTF_STORAGE);
		m_hSysDirTree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
		m_hSysDirTree.Expand(m_hSysDirTree.GetRootItem(), TVE_EXPAND);

		//ListCtrl:
		m_hSysDirList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
		m_hSysDirList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		//m_hSysDirList.SetCheckboxeStyle(RC_CHKBOX_NORMAL);

		m_hSysDirList.InsertColumn(1, _T("����"), LVCFMT_LEFT, 100);
		m_hSysDirList.InsertColumn(2, _T("��չ��"), LVCFMT_CENTER, 80);
		m_hSysDirList.InsertColumn(3, _T("״̬"), LVCFMT_CENTER, 80);

		//m_hSysDirList.SetItemTextColor(-1, 2, RGB(0, 0, 0));
		//m_hSysDirList.SetItemBkColor(-1, 2, RGB(255, 255, 0));
		 
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::UpdateWndCtrl()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::UpdateWndInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CDlgTest2Wnd::SetWndControlLayout()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);
}

BOOL CDlgTest2Wnd::DrawWndImage(CDC *pDC)
{
	BOOL bRet = FALSE;

	do 
	{
		if (pDC == NULL)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest2Wnd::SetChildWnd(BOOL bFlag)
{
	BOOL bRet = FALSE;

	int nIndex = 0;
	CRect rcSubWnd;

	do 
	{
		if (bFlag)
		{
			//1
			m_hComboEval.InsertString(nIndex, _T("�ļ������..."));
			m_hComboEval.SetItemData(nIndex++, CONFIG_ADDFILENAME_TYPE);

			m_hComboEval.InsertString(nIndex, _T("�ļ���ɾ��..."));
			m_hComboEval.SetItemData(nIndex++, CONFIG_DELFILENAME_TYPE);

			m_hComboEval.InsertString(nIndex, _T("�ļ���չ��..."));
			m_hComboEval.SetItemData(nIndex++, CONFIG_EXTFILENAME_TYPE);

			m_hComboEval.InsertString(nIndex, _T("�ļ����滻..."));
			m_hComboEval.SetItemData(nIndex++, CONFIG_REPLACEFILENAME_TYPE);

			m_hComboEval.InsertString(nIndex, _T("�ļ�������..."));
			m_hComboEval.SetItemData(nIndex++, CONFIG_DATEFILENAME_TYPE);

			m_hComboEval.InsertString(nIndex, _T("�ļ������..."));
			m_hComboEval.SetItemData(nIndex++, CONFIG_INDEXFILENAME_TYPE);

			GetDlgItem(IDC_STATIC_RECT)->GetWindowRect(&rcSubWnd);
			GetDlgItem(IDC_STATIC_RECT)->ShowWindow(SW_HIDE);
			ScreenToClient(&rcSubWnd);

			for (nIndex=0; nIndex<m_pArPage.GetSize(); nIndex++)
			{
				((CDialog*)m_pArPage[nIndex])->ShowWindow(SW_HIDE);
				((CDialog*)m_pArPage[nIndex])->MoveWindow(&rcSubWnd);
			}

			m_nDefaultSel = 0;
			((CDialog*)m_pArPage[m_nDefaultSel])->ShowWindow(TRUE);
			((CDialog*)m_pArPage[m_nDefaultSel])->SetFocus();
			m_nPrePage = m_nDefaultSel;
		}

		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

void CDlgTest2Wnd::ShowShellPathFile(LPCTSTR lpszShellPath)
{
	BOOL bRet = FALSE;

	int nPos = -1;
	int nCount = -1;

	CString strFileExt;
	CString strFileName;
	CString strFilePath;

	SHFILEINFO shFileInfo;
	std::vector<UPDATE_FILEINFO *>::iterator iterFileInfo;

	do 
	{
		if (lpszShellPath == NULL || *lpszShellPath == '\0')
		{
			bRet = FALSE;
			break;
		}

		update_file_data::Instance().ClearFileInfo(m_vecFileInfo);
		if (!update_file_data::Instance().EnumFileInfo(lpszShellPath, m_vecFileInfo))
		{
			bRet = FALSE;
			break;
		}

		m_hSysDirList.DeleteAllItems();
		for (iterFileInfo=m_vecFileInfo.begin(); iterFileInfo!=m_vecFileInfo.end(); ++iterFileInfo)
		{
			memset(&shFileInfo, 0x0, sizeof(SHFILEINFO));
			if ((*iterFileInfo) == NULL)
			{
				continue;
			}

			strFileExt  = (*iterFileInfo)->szFileExt;
			strFileName = (*iterFileInfo)->szFileName;
			strFilePath = (*iterFileInfo)->szFilePath;

			SHGetFileInfo(strFilePath, 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME|SHGFI_ATTRIBUTES);
			if(shFileInfo.dwAttributes&(SFGAO_LINK|SFGAO_HIDDEN))
			{
				continue;
			}

			nPos = strFileName.Find(strFileExt);
			if (nPos != -1)
			{
				strFileName = strFileName.Left(nPos);
			}

			nCount = m_hSysDirList.GetItemCount();
			m_hSysDirList.InsertItem(nCount, strFileName);
			m_hSysDirList.SetItemText(nCount, 1, strFileExt);
		}

		m_strShellPath = lpszShellPath;

		bRet = TRUE;
	} while (FALSE);
}

void CDlgTest2Wnd::SetButtonCtrl(BOOL bFlag)
{
	if (bFlag)
	{
	}
	else
	{

	}
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest2Wnd::UpdateConfigInfo(BOOL bFlag)
{
	BOOL bRet = FALSE;
	CString strPrompt;

	do 
	{
		if (bFlag)
		{
			if (!UpdateWndCtrl())
			{
				bRet = FALSE;
				break;
			}

			if (!UpdateWndInfo())
			{
				bRet = FALSE;
				break;
			}
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgTest2Wnd::GetCurConfigData(UPDATE_FILEDATA *pUpdateData)
{
	BOOL bRet = FALSE;

	UINT uiCurWndConfig = 0;
	DWORD_PTR pdwWndConfig = NULL;

	do 
	{
		if (pUpdateData == NULL || m_nCurSelIndex == -1)
		{
			bRet = FALSE;
			break;
		}

		pdwWndConfig = m_hComboEval.GetItemData(m_nCurSelIndex);
		if (pdwWndConfig == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (pdwWndConfig == CONFIG_ADDFILENAME_TYPE)
		{
			if (!m_dlgFileNameAdd.GetWndAddData(pUpdateData))
			{
				bRet = FALSE;
				break;
			}
		}
		else if (pdwWndConfig == CONFIG_DELFILENAME_TYPE)
		{
			if (!m_dlgFileNameDel.GetWndAddData(pUpdateData))
			{
				bRet = FALSE;
				break;
			}
		}
		else if (pdwWndConfig == CONFIG_EXTFILENAME_TYPE)
		{
			if (!m_dlgFileNameExt.GetWndAddData(pUpdateData))
			{
				bRet = FALSE;
				break;
			}
		}
		else if (pdwWndConfig == CONFIG_REPLACEFILENAME_TYPE)
		{
			if (!m_dlgFileNameReplace.GetWndAddData(pUpdateData))
			{
				bRet = FALSE;
				break;
			}
		}
		else if (pdwWndConfig == CONFIG_DATEFILENAME_TYPE)
		{
			if (!m_dlgFileNameDate.GetWndAddData(pUpdateData))
			{
				bRet = FALSE;
				break;
			}
		}
		else if (pdwWndConfig == CONFIG_INDEXFILENAME_TYPE)
		{
			if (!m_dlgFileNameIndex.GetWndAddData(pUpdateData))
			{
				bRet = FALSE;
				break;
			}
		}

		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}