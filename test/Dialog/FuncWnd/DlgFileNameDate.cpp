#include "stdafx.h"
#include "DlgFileNameDate.h"

IMPLEMENT_DYNAMIC(CDlgFileNameDate, CDialog)
CDlgFileNameDate::CDlgFileNameDate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameDate::IDD, pParent)
{
}

CDlgFileNameDate::~CDlgFileNameDate()
{
}

void CDlgFileNameDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameDate, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameDate::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL CDlgFileNameDate::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileNameDate::OnPaint() 
{
	CPaintDC dc(this);
}