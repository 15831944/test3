#include "stdafx.h"
#include "DlgTest3Wnd.h"

IMPLEMENT_DYNAMIC(CDlgTest3Wnd, CDialog)
CDlgTest3Wnd::CDlgTest3Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest3Wnd::IDD, pParent)
{
	m_strAppPath = _T("");
	m_strDefaultPath = _T("");
}

CDlgTest3Wnd::~CDlgTest3Wnd()
{
}

void CDlgTest3Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest3Wnd, CDialog)
	ON_BN_CLICKED(IDC_BTN_TEST1,			OnBnClickedBtnTest1)
	ON_BN_CLICKED(IDC_BTN_TEST2,			OnBnClickedBtnTest2)
	ON_BN_CLICKED(IDC_BTN_TEST3,			OnBnClickedBtnTest3)

	ON_CBN_SELCHANGE(IDC_COMBO_ENUMDEVICE,	OnCbnSelchangeComboEnumdevice)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest3Wnd::OnInitDialog()
{
	USES_CONVERSION;
	CDialog::OnInitDialog();
	return TRUE;  
}

void CDlgTest3Wnd::OnBnClickedBtnTest1()
{
}

void CDlgTest3Wnd::OnBnClickedBtnTest2()
{
}

void CDlgTest3Wnd::OnBnClickedBtnTest3()
{
}

void CDlgTest3Wnd::OnCbnSelchangeComboEnumdevice()
{
}

//////////////////////////////////////////////////////////////////////////
//