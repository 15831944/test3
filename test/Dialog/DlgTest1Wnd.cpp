#include "stdafx.h"
#include "../test.h"
#include "DlgTest1Wnd.h"

#include "../inc/GlobalInfo.h"

IMPLEMENT_DYNAMIC(CDlgTest1Wnd, CDialog)
CDlgTest1Wnd::CDlgTest1Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest1Wnd::IDD, pParent)
{
	m_strDefaultPath = _T("");
	m_strAppPath = _T("");

	m_pfCallRingFunc = NULL;
}

CDlgTest1Wnd::~CDlgTest1Wnd()
{
}

void CDlgTest1Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1,		OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2,		OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3,		OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4,		OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5,		OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6,		OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7,		OnBnClickedButton7)
END_MESSAGE_MAP()

BOOL CDlgTest1Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

//	m_hButton1.SubclassDlgItem(IDC_BUTTON1, this);

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
void CDlgTest1Wnd::Test1(CALLRING_CALLBACK_FUNC pfCallRingFunc)
{
	m_pfCallRingFunc = pfCallRingFunc;
}

void CDlgTest1Wnd::Test2(const char* pszFilePath, int nAudioCard)
{

}

void CDlgTest1Wnd::OnBnClickedButton1()
{
	Test1((CALLRING_CALLBACK_FUNC)Test2);
}

void CDlgTest1Wnd::OnBnClickedButton2()
{
	m_pfCallRingFunc("123", 3);
}	

void CDlgTest1Wnd::OnBnClickedButton3()
{
}


void CDlgTest1Wnd::OnBnClickedButton4()
{

}

void CDlgTest1Wnd::OnBnClickedButton5()
{	
}

void CDlgTest1Wnd::OnBnClickedButton6()
{		
}

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