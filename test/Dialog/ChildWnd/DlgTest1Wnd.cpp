#include "stdafx.h"
#include "DlgTest1Wnd.h"

extern BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

IMPLEMENT_DYNAMIC(CDlgTest1Wnd, CDialog)
CDlgTest1Wnd::CDlgTest1Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest1Wnd::IDD, pParent)
{
	m_hEditWnd = NULL;

	m_strAppPath = _T("");
	m_strShellPath = _T("");
	m_strDefaultPath = _T("");
	
	m_OldEditProc = NULL;
	m_pfCallRingFunc = NULL;

	//m_emEvalType = EVAL_EMPTYTYPE;
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
	DWORD dwDataPtr = 0;

#if 0
	nIndex = m_hComboEval.GetCurSel();
	if (nIndex == -1)
	{
		return;
	}

	dwDataPtr = m_hComboEval.GetItemData(nIndex);
	if (dwDataPtr == CB_ERR)
	{
		return;
	}

	switch(dwDataPtr)
	{
	case EVAL_EMPTYTYPE:
		{
			GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_FINDNAME)->SetWindowText(_T(""));
			GetDlgItem(IDC_EDIT_SUBNAME)->SetWindowText(_T(""));

			m_emEvalType = EVAL_EMPTYTYPE;
		}
		break;

	case EVAL_ALLFILENAME:
		{
			GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_SUBNAME)->SetWindowText(_T(""));

			m_emEvalType = EVAL_ALLFILENAME;
		}
		break;

	case EVAL_SPECIFYNAME:
		{
			GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);

			m_emEvalType = EVAL_SPECIFYNAME;
		}
		break;

	case EVAL_SPECIFYNUMINDEX:
		{
			GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);

			m_emEvalType = EVAL_SPECIFYNUMINDEX;
		}
		break;
	}
#endif
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