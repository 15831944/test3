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
	DDX_Control(pDX, IDC_TREE_SYSDIR, m_hSysDirTree);
	DDX_Control(pDX, IDC_LIST_SYSDIR, m_hSysDirList);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1,		OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2,		OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3,		OnBnClickedButton3)
END_MESSAGE_MAP()

BOOL CDlgTest1Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

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
	return TRUE;  
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
	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return;
	}

	unsigned char szValue[4] = {0};
	memcpy(szValue, _T("\xAA\x00\x80\x55"), 4);

	char szHex[4*2+1] = {0};

	pGlobal->BytesHexToString(szHex, szValue, 4);

	AfxMessageBox(szHex);
}	

void CDlgTest1Wnd::OnBnClickedButton3()
{
	CString strFindName;
	CString strSubName;

	if (m_strShellPath == _T("") || m_strShellPath.GetLength() <= 0)
	{
		MessageBox(_T("请选择正确的文件路径!"), _T("警告!"), MB_ICONWARNING|MB_OK);
		return;
	}

	GetDlgItem(IDC_EDIT_FINDNAME)->GetWindowText(strFindName);
	GetDlgItem(IDC_EDIT_SUBNAME)->GetWindowText(strSubName);

	if (strFindName == _T("") || strFindName.GetLength() <= 0)
	{
		GetDlgItem(IDC_EDIT_FINDNAME)->GetFocus();
		MessageBox(_T("查找的名称为空, 请检查!"), _T("警告!"), MB_ICONWARNING|MB_OK);
		return;
	}

	if (strSubName == _T("") || strSubName.GetLength() <= 0)
	{
		GetDlgItem(IDC_EDIT_SUBNAME)->GetFocus();
		MessageBox(_T("替换的名称为空, 请检查!"), _T("警告!"), MB_ICONWARNING|MB_OK);
		return;
	}

	if (!m_hUpdateFile.CreateUpdateProc(m_strShellPath, strFindName, strSubName))
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