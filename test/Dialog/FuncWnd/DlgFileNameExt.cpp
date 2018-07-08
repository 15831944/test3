#include "stdafx.h"
#include "DlgFileNameExt.h"

IMPLEMENT_DYNAMIC(CDlgFileNameExt, CDialog)
CDlgFileNameExt::CDlgFileNameExt(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameExt::IDD, pParent)
{
}

CDlgFileNameExt::~CDlgFileNameExt()
{
}

void CDlgFileNameExt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameExt, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameExt::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL CDlgFileNameExt::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileNameExt::OnPaint() 
{
	CPaintDC dc(this);
}