#include "stdafx.h"
#include "../test.h"
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
	DDX_Control(pDX, IDC_LIST_USERLIST, m_UserListbox);
}

BEGIN_MESSAGE_MAP(CDlgTest5Wnd, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest5Wnd::OnInitDialog()
{
	USES_CONVERSION;
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
	CPaintDC dc(this);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest5Wnd::InitCtrl()
{
	m_UserListbox.AddString("test1");
	m_UserListbox.AddString("test2");
	m_UserListbox.AddString("test3");
	return TRUE;
}

BOOL CDlgTest5Wnd::InitInfo()
{
	return TRUE;
}