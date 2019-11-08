#include "stdafx.h"
#include "DlgTest5Wnd.h"

IMPLEMENT_DYNAMIC(CDlgTest5Wnd, CDialog)
CDlgTest5Wnd::CDlgTest5Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest5Wnd::IDD, pParent)
{
}

CDlgTest5Wnd::~CDlgTest5Wnd()
{
}

void CDlgTest5Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest5Wnd, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest5Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!InitCtrl())
	{
		return FALSE;
	}

	if (!InitInfo())
	{
		return FALSE;
	}
	
	return TRUE;  
}

void CDlgTest5Wnd::OnPaint()
{
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest5Wnd::InitCtrl()
{
	return TRUE;
}

BOOL CDlgTest5Wnd::InitInfo()
{
	return TRUE;
}