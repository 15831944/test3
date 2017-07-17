#include "stdafx.h"
#include "../test.h"
#include "DlgTest4Wnd.h"

#include "../inc/GlobalInfo.h"

IMPLEMENT_DYNAMIC(CDlgTest4Wnd, CDialog)
CDlgTest4Wnd::CDlgTest4Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest4Wnd::IDD, pParent)
{
}

CDlgTest4Wnd::~CDlgTest4Wnd()
{
}

void CDlgTest4Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest4Wnd, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTest4Wnd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTest4Wnd::OnBnClickedButton2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest4Wnd::OnInitDialog()
{
	USES_CONVERSION;
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}
	
	return TRUE;  
}

void CDlgTest4Wnd::OnBnClickedButton1()
{
	m_hWavPlayer.CreatePlayerProc("1.wav", 0, 2);
}

void CDlgTest4Wnd::OnBnClickedButton2()
{
	m_hWavPlayer.ClosePlayerProc();
}
