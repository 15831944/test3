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
	ON_BN_CLICKED(IDC_BTN_DRAWTEST,			OnBnClickedBtnDrawtest)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest3Wnd::OnInitDialog()
{
	USES_CONVERSION;
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

	CRect rect1;
	GetDlgItem(IDC_STATIC_VIDEO)->GetClientRect(&rect1);
	m_openglDrawVideo1.CreateGLContext(rect1, GetDlgItem(IDC_STATIC_VIDEO));

	return TRUE;  
}

void CDlgTest3Wnd::OnCbnSelchangeComboEnumdevice()
{
}

void CDlgTest3Wnd::OnBnClickedBtnCaptureimage()
{
}

void CDlgTest3Wnd::OnBnClickedBtnDrawtest()
{
	GetDlgItem(IDC_STATIC_VIDEO)->ShowWindow(SW_HIDE);
}
