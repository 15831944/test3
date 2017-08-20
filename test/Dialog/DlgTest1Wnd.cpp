#include "stdafx.h"
#include "../test.h"
#include "DlgTest1Wnd.h"

#include "../inc/GlobalInfo.h"
#include "../contrib/VideoWndThread.h"

IMPLEMENT_DYNAMIC(CDlgTest1Wnd, CDialog)
CDlgTest1Wnd::CDlgTest1Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest1Wnd::IDD, pParent)
{
	m_strDefaultPath = _T("");
	m_strShellPath = _T("");
	m_strAppPath = _T("");

	m_pfCallRingFunc = NULL;
	m_hEvalType = EVAL_EMPTYTYPE;
}

CDlgTest1Wnd::~CDlgTest1Wnd()
{
}

CDlgTest1Wnd& CDlgTest1Wnd::Instance()
{
	static CDlgTest1Wnd inst;
	return inst;
}

void CDlgTest1Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SYSDIR,			m_hSysDirTree);
	DDX_Control(pDX, IDC_LIST_SYSDIR,			m_hSysDirList);
	DDX_Control(pDX, IDC_COMBO_EVALNAME,		m_hComboEval);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1,					OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2,					OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3,					OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO_EVALNAME,		OnCbnSelchangeComboEvalname)

	ON_MESSAGE(WM_UPDATEFILENAME_MSG,			OnUpdateFileName)
END_MESSAGE_MAP()

BOOL CDlgTest1Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

	Init();
	return TRUE;  
}

void CDlgTest1Wnd::Init()
{
	InitCtrl();
	InitInfo();
}

void CDlgTest1Wnd::InitCtrl()
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
}

void CDlgTest1Wnd::InitInfo()
{
	int nIndex = 0;
	int nCount = 0;

	CString strItemString;
	EVAL_ITEMDATA hEvalItem[4];

	memset(&hEvalItem, 0x0, sizeof(hEvalItem));

	hEvalItem[0].hEvalType = EVAL_EMPTYTYPE;
	strcpy(hEvalItem[0].szItemName, _T("请选择..."));

	hEvalItem[1].hEvalType = EVAL_ALLFILENAME;
	strcpy(hEvalItem[1].szItemName, _T("全部名称"));

	hEvalItem[2].hEvalType = EVAL_SPECIFYNAME;
	strcpy(hEvalItem[2].szItemName, _T("指定名称"));

	hEvalItem[3].hEvalType = EVAL_SPECIFYNUMINDEX;
	strcpy(hEvalItem[3].szItemName, _T("数字索引"));

	for (nIndex=0; nIndex<sizeof(hEvalItem)/sizeof(EVAL_ITEMDATA); nIndex++)
	{
		m_hComboEval.InsertString(nIndex, hEvalItem[nIndex].szItemName);
		m_hComboEval.SetItemData(nIndex, hEvalItem[nIndex].hEvalType);
	}

	m_hComboEval.SetCurSel(0);
}

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

void CDlgTest1Wnd::OnCbnSelchangeComboEvalname()
{
	int nIndex = 0;
	DWORD dwDataPtr = 0;

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

			m_hEvalType = EVAL_EMPTYTYPE;
		}
		break;

	case EVAL_ALLFILENAME:
		{
			GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_SUBNAME)->SetWindowText(_T(""));

			m_hEvalType = EVAL_ALLFILENAME;
		}
		break;

	case EVAL_SPECIFYNAME:
		{
			GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);

			m_hEvalType = EVAL_SPECIFYNAME;
		}
		break;

	case EVAL_SPECIFYNUMINDEX:
		{
			GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);

			m_hEvalType = EVAL_SPECIFYNUMINDEX;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//http://blog.csdn.net/xujiezhige/article/details/6206133
void CDlgTest1Wnd::OnBnClickedButton1()
{
/*
	CVideoWndThread* pVideoWndThread = (CVideoWndThread*)AfxBeginThread(RUNTIME_CLASS(CVideoWndThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	ASSERT_VALID(pVideoWndThread);

	pVideoWndThread->m_bAutoDelete = FALSE;	//不让线程delete this
	pVideoWndThread->ResumeThread();

	if (WaitForSingleObject(pVideoWndThread->m_hThread, INFINITE) == WAIT_OBJECT_0)
	{
		pVideoWndThread->m_bAutoDelete = TRUE;
		pVideoWndThread->Delete();
	}
*/
}

void CDlgTest1Wnd::OnBnClickedButton2()
{
}	

void CDlgTest1Wnd::OnBnClickedButton3()
{
	int nIndex = 0;
	DWORD dwDataPtr = 0;

	CString strFindName;
	CString strSubName;

	if (m_strShellPath == _T("") || m_strShellPath.GetLength() <= 0)
	{
		MessageBox(_T("请选择正确的文件路径!"), _T("警告!"), MB_ICONWARNING|MB_OK);
		return;
	}

	GetDlgItem(IDC_EDIT_FINDNAME)->GetWindowText(strFindName);
	GetDlgItem(IDC_EDIT_SUBNAME)->GetWindowText(strSubName);

	switch(m_hEvalType)
	{
	case EVAL_EMPTYTYPE:
		{
			MessageBox(_T("请选择正确的更新规则!"), _T("警告!"), MB_ICONWARNING|MB_OK);
			return;
		}
		break;

	case EVAL_ALLFILENAME:
		{
			
		}
		break;

	case EVAL_SPECIFYNAME:
		{
			if (strFindName == _T("") || strFindName.GetLength() <= 0)
			{
				GetDlgItem(IDC_EDIT_FINDNAME)->GetFocus();
				MessageBox(_T("查找的名称为空, 请检查!"), _T("警告!"), MB_ICONWARNING|MB_OK);
				return;
			}
		}
		break;

	case EVAL_SPECIFYNUMINDEX:
		{

		}
		break;
	}

	if (strSubName == _T("") || strSubName.GetLength() <= 0)
	{
		GetDlgItem(IDC_EDIT_SUBNAME)->GetFocus();
		MessageBox(_T("替换的名称为空, 请检查!"), _T("警告!"), MB_ICONWARNING|MB_OK);
		return;
	}

	if (!m_hUpdateFile.CreateUpdateProc(this->GetSafeHwnd(), m_strShellPath, strFindName, strSubName, m_hEvalType))
	{
		return;
	}

	GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
}

/*
void CDlgTest1Wnd::OnBnClickedButton7()
{
	CString strFilePath;
	CString strNewName;
	CString strSpan;
	
	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return;
	}

	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(strFilePath);
	GetDlgItem(IDC_EDIT_NEWNAME)->GetWindowText(strNewName);
	GetDlgItem(IDC_EDIT_SPANSTR)->GetWindowText(strSpan);

	if (strFilePath == _T(""))
	{
		::MessageBox(this->GetSafeHwnd(), _T("文件路径不能为空!"), _T("Warning"), MB_ICONWARNING|MB_OK);
		return;
	}

	if (strNewName == _T(""))
	{
		::MessageBox(this->GetSafeHwnd(), _T("文件名称不能为空!"), _T("Warning"), MB_ICONWARNING|MB_OK);
		return;
	}

	if (pGlobal->EnumModifyName(strFilePath, strNewName, ((strSpan == _T("")) ? "" : strSpan)) > 0)
	{
		::MessageBox(this->GetSafeHwnd(), _T("文件名称修改完成!"), _T("Warning"), MB_ICONEXCLAMATION|MB_OK);
	}
}
*/

LRESULT CDlgTest1Wnd::OnUpdateFileName(WPARAM wParam, LPARAM lParam)
{
	int nIndex = 0;
	DWORD dwDataPtr = 0;

	DWORD dwTypeMsg = 0;
	DWORD dwTypeValue = 0;

	ENUM_EVALTYPE hEvalType = EVAL_EMPTYTYPE;

	dwTypeMsg = wParam;
	if (dwTypeMsg == 0)
	{
		return 0;
	}

	switch(dwTypeMsg)
	{
	case UPDATE_FILENAME_SUCCESSUL_RESULT:
		{
			
		}
		break;

	case UPDATE_FILENAME_FAILED_RESULT:
		{

		}
		break;
	}

	nIndex = m_hComboEval.GetCurSel();
	if (nIndex != -1)
	{
		dwDataPtr = m_hComboEval.GetItemData(nIndex);
		if (dwDataPtr != CB_ERR)
		{
			switch(dwDataPtr)
			{
			case EVAL_EMPTYTYPE:
				{
					GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(TRUE);
					GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);
				}
				break;

			case EVAL_ALLFILENAME:
				{
					GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(FALSE);
					GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);
				}
				break;

			case EVAL_SPECIFYNAME:
				{
					GetDlgItem(IDC_EDIT_FINDNAME)->EnableWindow(TRUE);
					GetDlgItem(IDC_EDIT_SUBNAME)->EnableWindow(TRUE);
				}
				break;
			}
		}
	}

	m_hSysDirList.PostMessage(WM_UPDATECTRLDIR_MSG, 0, 0);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	return 0;
}