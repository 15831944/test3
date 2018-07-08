#include "stdafx.h"
#include "DlgTest1Wnd.h"

extern BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

IMPLEMENT_DYNAMIC(CDlgTest1Wnd, CDialog)
CDlgTest1Wnd::CDlgTest1Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest1Wnd::IDD, pParent)
{
	m_bInited = FALSE;
	m_bShowing = FALSE;

	m_nIndex = 0;
	m_nPrePage = 0;
	m_nDefaultSel = 0;

	m_strAppPath = _T("");
	m_strShellPath = _T("");
	m_strDefaultPath = _T("");
	
	m_hEditWnd = NULL;
	m_OldEditProc = NULL;
	m_pfCallRingFunc = NULL;
}

CDlgTest1Wnd::~CDlgTest1Wnd()
{
}

void CDlgTest1Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1_SYSDIR,			m_hSysDirTree);
	DDX_Control(pDX, IDC_LIST1_SYSDIR,			m_hSysDirList);
	DDX_Control(pDX, IDC_COMBO1_EVALNAME,		m_hComboEval);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_BTN1_OPENFLODER,			OnBnClickedBtnOpenFloder)
	ON_CBN_SELCHANGE(IDC_COMBO1_EVALNAME,		OnCbnSelchangeComboEvalname)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!Init())
	{
		return FALSE;
	}

	return TRUE;  
}

void CDlgTest1Wnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_bInited)
	{
		return;
	}

	SetWndControlLayout();
}

void CDlgTest1Wnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_bShowing = bShow;
	CDialog::OnShowWindow(bShow, nStatus);
}

void CDlgTest1Wnd::OnBnClickedBtnOpenFloder()
{

}

void CDlgTest1Wnd::OnBnClickedBtnRunModify()
{
	BOOL bRet = FALSE;

	int nIndex = -1;
	DWORD dwDataPtr = 0;

	CString strFindName;
	CString strSubName;
	CString strPrompt;

	do 
	{
		if (m_strShellPath == _T(""))
		{
			bRet = FALSE;
			strPrompt = _T("请选择正确的文件路径!");
			break;
		}

		if (strSubName == _T(""))
		{
			bRet = FALSE;
			strPrompt = _T("新文件名称不能为空, 请检查!");
			break;
		}

/*
		if (!m_hUpdateFile.CreateUpdateProc(this->GetSafeHwnd(), m_strShellPath, strFindName, strSubName, m_emEvalType))
		{
			bRet = FALSE;
			strPrompt = _T("文件名称修改失败, 请检查!");
			break;
		}
*/

		bRet = TRUE;
	} while (FALSE);

	if (!bRet && strPrompt != _T(""))
	{
		::MessageBox(NULL, strPrompt, _T("警告!"), MB_ICONWARNING | MB_OK);
	}
}

void CDlgTest1Wnd::OnCbnSelchangeComboEvalname()
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
	if (nCurSel != m_nPrePage)
	{
		((CDialog*)m_pArPage[m_nPrePage])->ShowWindow(SW_HIDE);
		((CDialog*)m_pArPage[nCurSel])->ShowWindow(SW_SHOW);
		m_nPrePage = nCurSel;
	}
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::GetShellTreePath(char* pszShellPath, void* pParam)
{
	CDlgTest1Wnd* pTest1Wnd = (CDlgTest1Wnd*)pParam;
	if (pTest1Wnd == NULL)
	{
		return FALSE;
	}

	if (pszShellPath == NULL || strcmp(pszShellPath, _T("")) == 0)
	{
		return FALSE;
	}

	pTest1Wnd->m_strShellPath = pszShellPath;
	return TRUE;
}

LRESULT CDlgTest1Wnd::EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	TCHAR	szPath[_MAX_PATH] = {0};
	CString strFilePath;

	CDlgTest1Wnd *pDlg = (CDlgTest1Wnd*)GetWindowLong(hwnd, GWL_USERDATA);

	if (pDlg->m_hEditWnd == hwnd)
	{
		switch(uMsg)
		{
		case WM_CONTEXTMENU:
			{
				return 0;
			}
			break;

		case WM_LBUTTONDOWN:
			{
				BROWSEINFO bi;
				ZeroMemory(&bi,sizeof(BROWSEINFO));

				bi.hwndOwner = pDlg->GetSafeHwnd();
				bi.pidlRoot  = NULL;
				bi.lpszTitle = _T("选择文件路径:");
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
BOOL CDlgTest1Wnd::Init()
{
	if (!InitCtrl())
	{
		return FALSE;
	}

	if (!InitInfo())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgTest1Wnd::InitCtrl()
{
	if (!CreateChildWnd())
	{
		return FALSE;
	}

	if (!SetWndCtrlInfo())
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDlgTest1Wnd::InitInfo()
{
	int nIndex = 0;
	int nCount = 0;

#if 0
	CString strItemString;
	EVAL_ITEMDATA hEvalItem[4];

	memset(&hEvalItem, 0x0, sizeof(hEvalItem));

	hEvalItem[0].emEvalType = EVAL_EMPTYTYPE;
	strcpy(hEvalItem[0].szItemName, _T("请选择..."));

	hEvalItem[1].emEvalType = EVAL_ALLFILENAME;
	strcpy(hEvalItem[1].szItemName, _T("全部名称"));

	hEvalItem[2].emEvalType = EVAL_SPECIFYNAME;
	strcpy(hEvalItem[2].szItemName, _T("指定名称"));

	hEvalItem[3].emEvalType = EVAL_SPECIFYNUMINDEX;
	strcpy(hEvalItem[3].szItemName, _T("数字索引"));

	for (nIndex=0; nIndex<sizeof(hEvalItem)/sizeof(EVAL_ITEMDATA); nIndex++)
	{
		m_hComboEval.InsertString(nIndex, hEvalItem[nIndex].szItemName);
		m_hComboEval.SetItemData(nIndex, hEvalItem[nIndex].emEvalType);
	}

	m_hComboEval.SetCurSel(0);
#endif
	return TRUE;
}

BOOL CDlgTest1Wnd::CreateChildWnd()
{
	int nIndex = 0;
	CRect rcSubWnd;

	m_dlgShowLogoWnd.Create(CDlgShowLogoWnd::IDD, this);
	m_dlgFileNameAdd.Create(CDlgFileNameAdd::IDD, this);
	m_dlgFileNameDate.Create(CDlgFileNameDate::IDD, this);
	m_dlgFileNameDel.Create(CDlgFileNameDel::IDD, this);
	m_dlgFileNameExt.Create(CDlgFileNameExt::IDD, this);
	m_dlgFileNameIndex.Create(CDlgFileNameIndex::IDD, this);
	m_dlgFileNameReplace.Create(CDlgFileNameReplace::IDD, this);

	m_pArPage.Add(&m_dlgShowLogoWnd);
	m_pArPage.Add(&m_dlgFileNameAdd);
	m_pArPage.Add(&m_dlgFileNameDate);
	m_pArPage.Add(&m_dlgFileNameDel);
	m_pArPage.Add(&m_dlgFileNameExt);
	m_pArPage.Add(&m_dlgFileNameIndex);
	m_pArPage.Add(&m_dlgFileNameReplace);

	m_hComboEval.AddString(_T("1"));
	m_hComboEval.AddString(_T("2"));
	m_hComboEval.AddString(_T("3"));
	m_hComboEval.AddString(_T("4"));
	m_hComboEval.AddString(_T("5"));
	m_hComboEval.AddString(_T("6"));

	GetDlgItem(IDC_STATIC1_RECT)->GetWindowRect(&rcSubWnd);
	GetDlgItem(IDC_STATIC1_RECT)->ShowWindow(SW_HIDE);
	ScreenToClient(&rcSubWnd);

	for (nIndex=0; nIndex<m_pArPage.GetSize(); nIndex++)
	{
		((CDialog*)m_pArPage[nIndex])->ShowWindow(SW_HIDE);
		((CDialog*)m_pArPage[nIndex])->MoveWindow(&rcSubWnd);
	}

	((CDialog*)m_pArPage[m_nDefaultSel])->ShowWindow(TRUE);
	((CDialog*)m_pArPage[m_nDefaultSel])->SetFocus();

	m_nPrePage = m_nDefaultSel;
	return TRUE;
}

BOOL CDlgTest1Wnd::SetWndCtrlInfo()
{
	m_hSysDirTree.InitializeCtrl();
	m_hSysDirList.InitilizeCtrl(this, (GETSHELLTREE_PATH_CALLBACK_FUNC)GetShellTreePath);

	m_hSysDirTree.SetSelectList(m_hSysDirList);
//	m_hButton1.SubclassDlgItem(IDC_BUTTON1, this);

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
	//this->GetDlgItem(IDC_EDIT_FINDNAME)->SendMessage(EM_SETCUEBANNER, 0, (LPARAM)(LPCWSTR)L"输入查找的名称...");
	//this->GetDlgItem(IDC_EDIT_SUBNAME)->SendMessage(EM_SETCUEBANNER, 0, (LPARAM)(LPCWSTR)L"输入替换的名称...");

	return TRUE;
}

void CDlgTest1Wnd::SetWndControlLayout()
{
}