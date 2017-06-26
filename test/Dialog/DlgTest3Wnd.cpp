#include "stdafx.h"
#include "../test.h"
#include "DlgTest3Wnd.h"

#include "../inc/GlobalInfo.h"

IMPLEMENT_DYNAMIC(CDlgTest3Wnd, CDialog)
CDlgTest3Wnd::CDlgTest3Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest3Wnd::IDD, pParent)
{
	m_strDefaultPath = _T("");
	m_strAppPath = _T("");

	m_bCameraOpen = FALSE;
}

CDlgTest3Wnd::~CDlgTest3Wnd()
{
}

void CDlgTest3Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest3Wnd, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ENUMDEVICE,	OnCbnSelchangeComboEnumdevice)
	ON_BN_CLICKED(IDC_BTN_CAPTUREIMAGE,		OnBnClickedBtnCaptureimage)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest3Wnd::OnInitDialog()
{
	USES_CONVERSION;

	CDialog::OnInitDialog();

	HRESULT hr = S_FALSE;
	DWORD dwIndex = 0;

	std::list<DeviceName> listDeviceName;
	std::list<DeviceName>::iterator iter;

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

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

	GetDlgItem(IDC_COMBO_ENUMDEVICE)->EnableWindow(FALSE);

	if (CCaptureWndVideo::EnumDevices(listDeviceName))
	{
		GetDlgItem(IDC_COMBO_ENUMDEVICE)->EnableWindow(TRUE);

		for (dwIndex=0,iter=listDeviceName.begin(); iter!=listDeviceName.end(); ++iter, ++dwIndex)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_ENUMDEVICE))->InsertString(dwIndex, iter->FriendlyName);
			((CComboBox*)GetDlgItem(IDC_COMBO_ENUMDEVICE))->SetItemData(dwIndex,  iter->nDeviceID);	
		}

		((CComboBox*)GetDlgItem(IDC_COMBO_ENUMDEVICE))->SetCurSel(0);

//		OnCbnSelchangeComboEnumdevice();
	}

	return TRUE;  
}

void CDlgTest3Wnd::OnCbnSelchangeComboEnumdevice()
{
	int nCurSel = -1;
	static int nPreSel = -1;

	DWORD dwCaptureID = 0;
	CString strText;

	if (((CComboBox*)GetDlgItem(IDC_COMBO_ENUMDEVICE))->GetCount() <= 0)
	{
		return;
	}

	nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_ENUMDEVICE))->GetCurSel();
	if (nCurSel < 0)
	{
		return;
	}

	if (nCurSel == nPreSel)
	{
		return;
	}

	if (nPreSel != -1)
	{
		m_CameraVideo.CloseCamera();
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_ENUMDEVICE))->GetLBText(nCurSel, strText);
	if (strText == _T(""))
	{
		return;
	}

	dwCaptureID = ((CComboBox*)GetDlgItem(IDC_COMBO_ENUMDEVICE))->GetItemData(nCurSel);

	if (!m_CameraVideo.OpenCamera(dwCaptureID, GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd()))
	{
		return;
	}

	m_bCameraOpen = TRUE;
	nPreSel = nCurSel;
}

void CDlgTest3Wnd::OnBnClickedBtnCaptureimage()
{
	if (!m_bCameraOpen)
	{
		return;
	}

	CFileDialog dlg(FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "位图文件(*.bmp)|*.bmp|All Files(*.*)|*.*||", this);
	dlg.m_ofn.lpstrInitialDir = m_strDefaultPath;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	if (!m_CameraVideo.CaptureImages(dlg.GetPathName()))
	{
		MessageBox(_T("视频截图失败, 请检查!"), _T("警告!"), MB_ICONERROR|MB_OK);
		return;
	}
}