#include "stdafx.h"
#include "../test.h"
#include "DlgTest4Wnd.h"

#include "../inc/GlobalInfo.h"
#include "../Control/TreeCtrl/StackedTreeCtrl.h"

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

	DWORD dwStyle = 0;
	CRect rect(0, 0, 10,20);
	CStackedTreeCtrl hTreeCtrl;
	
	dwStyle = WS_CHILD|WS_VISIBLE|SS_NOTIFY|SS_CENTER;
	hTreeCtrl.Create(dwStyle, rect, this, 0);
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
