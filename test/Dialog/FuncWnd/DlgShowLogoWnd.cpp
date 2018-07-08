#include "stdafx.h"
#include "DlgShowLogoWnd.h"

IMPLEMENT_DYNAMIC(CDlgShowLogoWnd, CDialog)
CDlgShowLogoWnd::CDlgShowLogoWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShowLogoWnd::IDD, pParent)
{
}

CDlgShowLogoWnd::~CDlgShowLogoWnd()
{
}

void CDlgShowLogoWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgShowLogoWnd, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgShowLogoWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL CDlgShowLogoWnd::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgShowLogoWnd::OnPaint() 
{
	CPaintDC dc(this);
}