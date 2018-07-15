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
	ON_EN_CHANGE(IDC_EDIT1, &CDlgFileNameDel::OnEnChangeEdit1)
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

void CDlgFileNameDel::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
