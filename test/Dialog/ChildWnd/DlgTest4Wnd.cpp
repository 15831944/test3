#include "stdafx.h"
#include "DlgTest4Wnd.h"

IMPLEMENT_DYNAMIC(CDlgTest4Wnd, CDialog)
CDlgTest4Wnd::CDlgTest4Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest4Wnd::IDD, pParent)
{
}

CDlgTest4Wnd::~CDlgTest4Wnd()
{
}

void CDlgTest4Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest4Wnd, CDialog)
	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_BTN_TEST1,				OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_TEST2,				OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_TEST3,				OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_TEST4,				OnBnClickedButton4)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest4Wnd::OnInitDialog()
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

void CDlgTest4Wnd::OnPaint()
{
	CPaintDC dc(this);
}

//////////////////////////////////////////////////////////////////////////
//
void CDlgTest4Wnd::OnBnClickedButton1()
{
}

void CDlgTest4Wnd::OnBnClickedButton2()
{
}

void CDlgTest4Wnd::OnBnClickedButton3()
{
}

void CDlgTest4Wnd::OnBnClickedButton4()
{
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest4Wnd::InitCtrl()
{
	return TRUE;
}

BOOL CDlgTest4Wnd::InitInfo()
{
	return TRUE;
}

/*
BOOL CDlgTest4Wnd::DrawRectFrameLine(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if (pDC == NULL)
	{
		return FALSE;
	}

 	CMemDCEx MemDCEx(pDC, NULL);
 	Graphics graphics(MemDCEx->GetSafeHdc());

	Pen pen(Color(255, 0, 255, 0), 3);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	PointF ptLeftTop(rcClient.left, rcClient.top);
	PointF ptRightTop(rcClient.right, rcClient.top);

	PointF ptLeftBottom(rcClient.left, rcClient.bottom);
	PointF ptRightBottom(rcClient.right, rcClient.bottom);

	graphics.DrawLine(&pen, ptLeftTop,    ptRightTop);
	graphics.DrawLine(&pen, ptLeftTop,    ptLeftBottom);
	graphics.DrawLine(&pen, ptLeftBottom, ptRightBottom);
	graphics.DrawLine(&pen, ptRightTop,   ptRightBottom);
	return TRUE;
}
*/