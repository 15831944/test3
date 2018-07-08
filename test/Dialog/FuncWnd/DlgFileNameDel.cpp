#include "stdafx.h"
#include "DlgFileNameDel.h"

IMPLEMENT_DYNAMIC(CDlgFileNameDel, CDialog)
CDlgFileNameDel::CDlgFileNameDel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameDel::IDD, pParent)
{
}

CDlgFileNameDel::~CDlgFileNameDel()
{
}

void CDlgFileNameDel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameDel, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameDel::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL CDlgFileNameDel::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileNameDel::OnPaint() 
{
	CPaintDC dc(this);
}