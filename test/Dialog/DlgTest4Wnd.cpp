#include "stdafx.h"
#include "../test.h"
#include "DlgTest4Wnd.h"

#include "../inc/GlobalInfo.h"
#include "DlgPTZControlWnd.h"

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
	DDX_Control(pDX, IDC_BUTTON3,	m_btnTest3);
	DDX_Control(pDX, IDC_LIST_TEST, m_ListTest);
}

BEGIN_MESSAGE_MAP(CDlgTest4Wnd, CDialog)
	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTest4Wnd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTest4Wnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgTest4Wnd::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgTest4Wnd::OnBnClickedButton4)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest4Wnd::OnInitDialog()
{
	USES_CONVERSION;
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

#if 0
	DWORD dwStyle;
	CRect rcClient;
	CRect rcTreeWnd;

	GetClientRect(&rcClient);
	dwStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;

	rcTreeWnd.left   = rcClient.left;
	rcTreeWnd.top    = rcClient.top;
	rcTreeWnd.right  = rcClient.right/3;
	rcTreeWnd.bottom = rcClient.bottom;

	m_hTreeCtrl.Create(dwStyle, rcTreeWnd, this, 0+1);
#endif

#if 0
	m_btnTest3.SetImage(_T("D:\\s1_Normal.png"), NULL, NULL, _T("D:\\s1_Hover.png"));
#endif

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
	DrawRectFrameLine(&dc);
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
#if 0
	CDlgPTZControlWnd	dlgPTZControl;
	dlgPTZControl.DoModal();
#endif

#if 0
	int nSel = 50;
int nItem = m_list.GetTopIndex();
    CRect rc;
    m_list.GetItemRect(nItem, rc, LVIR_BOUNDS);
    CSize sz(0, (nSel - nItem)*rc.Height());
    m_list.Scroll(sz);
    m_list.SetItemState(nSel, LVIS_SELECTED, LVIS_SELECTED);
#endif

#if 0
	if (!m_UKeyVerifyProc.CreateVerifyProc(TRUE, test1))
	{
		return;
	}

	m_UKeyVerifyProc.SetUserData(_T("16104010016"), _T("123456"));
	m_UKeyVerifyProc.SetThreadProcTime(500, 500);
	m_UKeyVerifyProc.SetThreadProcState(TRUE);
#endif
}

void CDlgTest4Wnd::OnBnClickedButton4()
{
	rzt_closeUKeyProc(m_hUKeyProc);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest4Wnd::InitCtrl()
{
	if (!InitList())
	{
		return FALSE;
	}

	if (!InitWndRect())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL test1(const CK_UKEYENUM *pUKeyEnum)
{
	if (pUKeyEnum == NULL)
	{
		return FALSE;
	}

	if (!pUKeyEnum->bExist)
	{
		TRACE(_T("UKey Device is not Exist!"));
		return FALSE;
	}

	if (pUKeyEnum->emUKeyState == CK_UKEYSTATEINSERTTYPE)
	{
		TRACE(_T("UKey Device ID:%d"), pUKeyEnum->ulSlotId);
	}

	if (pUKeyEnum->emUKeyType == CK_UKEYDEVFINGERTYPE)
	{
		TRACE(_T("UKey Device is FingerMode!"));
	}
	else
	{
		TRACE(_T("UKey Device is not FingerMode!"));
	}

	return TRUE;
}

BOOL test2(CK_UKEYVERIFY *pUKeyVerify)
{
	char szUserPIN[UKEYPIN_MAX_LEN] = {0};
	char szNewUserPIN[UKEYPIN_MAX_LEN] = {0};

	if (pUKeyVerify->emUKeyState == CK_UKEYSTATEINPUTETYPE)
	{
		if (pUKeyVerify->emUKeyType == CK_UKEYDEVNORMALTYPE)
		{
			//scanf(_T("%s"), szUserPIN);
			strcpy(szUserPIN, _T("12345678"));	//12345678
			TRACE(_T("UKey Input UserPIN:%s"), szUserPIN);

			strcpy(pUKeyVerify->szUserPIN, szUserPIN);
		}
		else
		{
			TRACE(_T("UKey Finger Verify!"));
		}	
	}
	else if (pUKeyVerify->emUKeyState == CK_UKEYSTATEMODIFYTYPE)
	{
		if (pUKeyVerify->emUKeyType == CK_UKEYDEVNORMALTYPE)
		{
			//scanf(_T("%s"), szNewUserPIN);
			strcpy(szNewUserPIN, _T("87654321"));
			TRACE(_T("UKey Modify UserPIN:%s"), szNewUserPIN);

			strcpy(pUKeyVerify->szNewUserPIN, szNewUserPIN);
		}
	}
	else if (pUKeyVerify->emUKeyState == CK_UKEYSTATESUCCEDTYPE)
	{
		TRACE(_T("UKey Verify Successful!"));
	}
	else if (pUKeyVerify->emUKeyState == CK_UKEYSTATEFAILEDTYPE)
	{
		TRACE(_T("UKey Verify Failed!"));
	}

	return TRUE;
}

BOOL test3(CK_UKEYREADDATA *pUKeyRead)
{
	if (pUKeyRead->emUKeyState == CK_UKEYSTATEINPUTETYPE)
	{
		TRACE(_T("UKey Read InputData!"));
		ResetEvent(pUKeyRead->hEvent);
	}
	else if (pUKeyRead->emUKeyState == CK_UKEYSTATEOUTPUTTYPE)
	{
		TRACE(_T("UKey Read OutputData!"));
	}
	else if (pUKeyRead->emUKeyState == CK_UKEYSTATESUCCEDTYPE)
	{
		TRACE(_T("UKey Read UserData Successful! UserName:%s UserPasswd:%s"), pUKeyRead->szUserNum, pUKeyRead->szUserPasswd);
	}
	else if (pUKeyRead->emUKeyState == CK_UKEYSTATEFAILEDTYPE)
	{
		TRACE(_T("UKey Read UserData Failed!"));
	}
	return TRUE;
}

BOOL test4(CK_UKEYWRITEDATA *pUKeyWrite)
{
	if (pUKeyWrite->emUKeyState == CK_UKEYSTATEINPUTETYPE)
	{
		strcpy(pUKeyWrite->szUserNum, _T("16104010006"));
		strcpy(pUKeyWrite->szUserPasswd, _T("123456"));

		TRACE(_T("UKey Write UserData!"));
		//ResetEvent(pUKeyWrite->hEvent);
	}
	else if (pUKeyWrite->emUKeyState == CK_UKEYSTATEOUTPUTTYPE)
	{
	}
	else if (pUKeyWrite->emUKeyState == CK_UKEYSTATESUCCEDTYPE)
	{
	}
	else if (pUKeyWrite->emUKeyState == CK_UKEYSTATEFAILEDTYPE)
	{
	}
	return TRUE;
}

BOOL CDlgTest4Wnd::InitInfo()
{
	rzt_openUKeyProc(test1, test2, test3, NULL, m_hUKeyProc);
	return TRUE;
}

COleDateTime GenRandDate()
{
	COleDateTime now = COleDateTime::GetCurrentTime();
	COleDateTimeSpan span(rand() % 3650, rand() % 24, rand() % 60, rand() % 60);
	now -= span;
	return now;
}

BOOL CDlgTest4Wnd::InitList()
{
	m_ListTest.SetColumnHeader(_T("Student ID, 100; Enroll Date, 150; Score, 80, 2"));

	//::InitializeFlatSB(m_ListTest.m_hWnd);
	//::FlatSB_EnableScrollBar(m_ListTest.m_hWnd, SB_BOTH, ESB_DISABLE_BOTH); 

	m_ListTest.ModifyStyle(0, LVS_REPORT);
	m_ListTest.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	for (int i = 0; i < 50; i++)
	{
		const int IDX = m_ListTest.InsertItem(0, _T(""));
		m_ListTest.SetItemText(IDX, 0, rand() % 3000);
		m_ListTest.SetItemText(IDX, 1, GenRandDate());
		m_ListTest.SetItemText(IDX, 2, (rand() % 51) + 50);
	}

	m_ListTest.SetGridLines(TRUE); 
	m_ListTest.SetCheckboxeStyle(RC_CHKBOX_NONE); 
	m_ListTest.SetEditable(TRUE); 
	m_ListTest.SortItems(0, TRUE); 

	m_ListTest.SetItemTextColor(-1, 2, RGB(0, 0, 0));
	m_ListTest.SetItemBkColor(-1, 2, RGB(255, 255, 0));

	for (int n = 0; n < m_ListTest.GetItemCount(); n++)
	{
		const int SCORE = _ttoi(m_ListTest.GetItemText(n, 2));

		if (SCORE < 60)
		{
			m_ListTest.SetItemBkColor(n, -1, ::GetSysColor(COLOR_INFOBK));
			m_ListTest.SetItemTextColor(n, -1, RGB(255, 0, 0));

			m_ListTest.SetItemBkColor(n, 2, RGB(255, 0, 0));
			m_ListTest.SetItemTextColor(n, 2, RGB(255, 255, 255));
		}
		else if (SCORE > 90)
		{
			m_ListTest.SetItemBkColor(n, -1, RGB(160, 255, 192));
			m_ListTest.SetItemTextColor(n, -1, RGB(0, 0, 255));

			m_ListTest.SetItemTextColor(n, 2, RGB(255, 255, 255));
			m_ListTest.SetItemBkColor(n, 2, RGB(0, 160, 0));
		}
	}

	return TRUE;
}

BOOL CDlgTest4Wnd::InitWndRect()
{
	return TRUE;
}

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