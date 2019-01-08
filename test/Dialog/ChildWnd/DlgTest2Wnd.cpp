#include "stdafx.h"
#include "DlgTest2Wnd.h"

extern BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

IMPLEMENT_DYNAMIC(CDlgTest2Wnd, CDialog)
CDlgTest2Wnd::CDlgTest2Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest2Wnd::IDD, pParent)
{
	m_bInited = FALSE;
	m_bShowing = FALSE;

	m_nPrePage = 0;
	m_nDefaultSel = 0;

	m_strAppPath = _T("");
	m_strShellPath = _T("");
	m_strDefaultPath = _T("");
	
	m_hEditWnd = NULL;
	m_OldEditProc = NULL;
	m_pfCallRingFunc = NULL;
}

CDlgTest2Wnd::~CDlgTest2Wnd()
{
}

void CDlgTest2Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2_SYSDIR,			m_hSysDirTree);
	DDX_Control(pDX, IDC_LIST2_SYSDIR,			m_hSysDirList);
	DDX_Control(pDX, IDC_COMBO2_EVALNAME,		m_hComboEval);
}

BEGIN_MESSAGE_MAP(CDlgTest2Wnd, CDialog)
	ON_WM_CREATE()

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_BTN2_RUNMODIFY,			OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BTN2_OPENFLODER,			OnBnClickedBtnOpenFloder)
	ON_CBN_SELCHANGE(IDC_COMBO2_EVALNAME,		OnCbnSelchangeComboEvalname)
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
// 		if (m_strShellPath == _T(""))
// 		{
// 			bRet = FALSE;
// 			strPrompt = _T("请选择正确的文件路径, 请检查!");
// 			break;
// 		}

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
			::MessageBox(NULL, strPrompt, _T("警告!"), MB_ICONWARNING|MB_OK);
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
	if (nCurSel != m_nPrePage)
	{
		((CDialog*)m_pArPage[m_nPrePage])->ShowWindow(SW_HIDE);
		((CDialog*)m_pArPage[nCurSel])->ShowWindow(SW_SHOW);
		m_nPrePage = nCurSel;
	}
}

//////////////////////////////////////////////////////////////////////////
//
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

BOOL CDlgTest2Wnd::GetUpdateFileData(void *pUpdateData, void *pParentObject)
{
	BOOL bRet = FALSE;
	CDlgTest2Wnd* pWndInfo = NULL;

	do 
	{
		if (pUpdateData == NULL)
		{
			bRet = FALSE;
			break;
		}

		pWndInfo = (CDlgTest2Wnd*)pParentObject;
		if (pWndInfo == NULL)
		{
			bRet = FALSE;
			break;
		}


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
#if 0
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
#endif

		//this->GetDlgItem(IDC_EDIT_FINDNAME)->SendMessage(EM_SETCUEBANNER, 0, (LPARAM)(LPCWSTR)L"输入查找的名称...");
		//this->GetDlgItem(IDC_EDIT_SUBNAME)->SendMessage(EM_SETCUEBANNER, 0, (LPARAM)(LPCWSTR)L"输入替换的名称...");
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

	int nIndex = 0;
	CRect rcSubWnd;

	do 
	{
		//1:
		m_pArPage.Add(&m_dlgShowLogoWnd);
		m_pArPage.Add(&m_dlgFileNameAdd);
		m_pArPage.Add(&m_dlgFileNameDel);
		m_pArPage.Add(&m_dlgFileNameExt);
		m_pArPage.Add(&m_dlgFileNameReplace);
		m_pArPage.Add(&m_dlgFileNameDate);
		m_pArPage.Add(&m_dlgFileNameIndex);

		m_hComboEval.InsertString(nIndex++, _T("文件名添加..."));
		m_hComboEval.InsertString(nIndex++, _T("文件名删除..."));
		m_hComboEval.InsertString(nIndex++, _T("文件扩展名..."));
		m_hComboEval.InsertString(nIndex++, _T("文件名替换..."));
		m_hComboEval.InsertString(nIndex++, _T("文件名日期..."));
		m_hComboEval.InsertString(nIndex++, _T("文件名序号..."));

		GetDlgItem(IDC_STATIC2_RECT)->GetWindowRect(&rcSubWnd);
		GetDlgItem(IDC_STATIC2_RECT)->ShowWindow(SW_HIDE);
		ScreenToClient(&rcSubWnd);

		for (nIndex=0; nIndex<m_pArPage.GetSize(); nIndex++)
		{
			((CDialog*)m_pArPage[nIndex])->ShowWindow(SW_HIDE);
			((CDialog*)m_pArPage[nIndex])->MoveWindow(&rcSubWnd);
		}

		((CDialog*)m_pArPage[m_nDefaultSel])->ShowWindow(TRUE);
		((CDialog*)m_pArPage[m_nDefaultSel])->SetFocus();

		m_nPrePage = m_nDefaultSel;
		m_hComboEval.SetCurSel(m_nDefaultSel);

		//2:
		m_hSysDirTree.InitializeCtrl();
		m_hSysDirList.InitilizeCtrl(this, GetShellTreePath);
		m_hSysDirTree.SetSelectList(m_hSysDirList);
		
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