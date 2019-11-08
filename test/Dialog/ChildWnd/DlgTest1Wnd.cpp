#include  "stdafx.h"
#include  "DlgTest1Wnd.h"

IMPLEMENT_DYNAMIC(CDlgTest1Wnd, CDialog)
CDlgTest1Wnd::CDlgTest1Wnd(CWnd* pParent)
	: CDialog(CDlgTest1Wnd::IDD, pParent)
{
	m_bInited = FALSE;
}

CDlgTest1Wnd::~CDlgTest1Wnd()
{
}

void CDlgTest1Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()

	ON_WM_NCHITTEST()

	ON_BN_CLICKED(IDC_BTN_TEST1,		OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_TEST2,		OnBnClickedButton2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::OnInitDialog()
{
	BOOL bRet = FALSE;
	CDialog::OnInitDialog();

	do 
	{
		if (!InitCtrl())
		{
			bRet = FALSE;
			break;
		}

		if (!InitInfo())
		{
			bRet = FALSE;
			break;
		}

		m_bInited = TRUE;
		bRet = TRUE;
	} while (FALSE);
	
	if (!bRet)
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	}

	return bRet; 
}

BOOL CDlgTest1Wnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTest1Wnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInited)
	{
		return;
	}

	SetWndControlLayout();
}

void CDlgTest1Wnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (!bShow)
	{
	}

	CDialog::OnShowWindow(bShow, nStatus);
}

LRESULT CDlgTest1Wnd::OnNcHitTest(CPoint point)
{
	return CDialog::OnNcHitTest(point);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::InitCtrl()
{
	return TRUE;
}

BOOL CDlgTest1Wnd::InitInfo()
{
	return TRUE;
}

void CDlgTest1Wnd::SetWndControlLayout()
{
}

//////////////////////////////////////////////////////////////////////////
//
void CDlgTest1Wnd::OnBnClickedButton1()
{
}

#include "../../MemDC.hpp"
void CDlgTest1Wnd::OnBnClickedButton2()
{
	for (int i=0; i<100000; ++i)
	{
		CDC *pdcScreen = CDC::FromHandle(::GetDC(NULL));

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pdcScreen, 1280, 800);

		CDC dcMem;
		dcMem.CreateCompatibleDC(pdcScreen);
		CBitmap *pbmpOld = dcMem.SelectObject(&bmpMem);

		dcMem.BitBlt(0, 0, 1280, 800, pdcScreen, 0, 0, SRCCOPY);

		dcMem.SelectObject(pbmpOld);

		HBITMAP hBmpScreenBkg = (HBITMAP)bmpMem.GetSafeHandle();
		::DeleteObject(hBmpScreenBkg);

		dcMem.DeleteDC();
		bmpMem.DeleteObject();
		::ReleaseDC(NULL, pdcScreen->m_hDC);
	}
}