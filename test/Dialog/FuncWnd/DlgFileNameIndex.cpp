#include "stdafx.h"
#include "DlgFileNameIndex.h"

IMPLEMENT_DYNAMIC(CDlgFileNameIndex, CDialog)
CDlgFileNameIndex::CDlgFileNameIndex(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameIndex::IDD, pParent)
{
}

CDlgFileNameIndex::~CDlgFileNameIndex()
{
}

void CDlgFileNameIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameIndex, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameIndex::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL CDlgFileNameIndex::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileNameIndex::OnPaint() 
{
	CPaintDC dc(this);
}