#include "stdafx.h"
#include "DlgFileNameAdd.h"

IMPLEMENT_DYNAMIC(CDlgFileNameAdd, CDialog)
CDlgFileNameAdd::CDlgFileNameAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameAdd::IDD, pParent)
{
}

CDlgFileNameAdd::~CDlgFileNameAdd()
{
}

void CDlgFileNameAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameAdd, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameAdd::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL CDlgFileNameAdd::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileNameAdd::OnPaint() 
{
	CPaintDC dc(this);
}