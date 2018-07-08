#include "stdafx.h"
#include "DlgFileNameReplace.h"

IMPLEMENT_DYNAMIC(CDlgFileNameReplace, CDialog)
CDlgFileNameReplace::CDlgFileNameReplace(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameReplace::IDD, pParent)
{
}

CDlgFileNameReplace::~CDlgFileNameReplace()
{
}

void CDlgFileNameReplace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameReplace, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameReplace::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL CDlgFileNameReplace::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileNameReplace::OnPaint() 
{
	CPaintDC dc(this);
}