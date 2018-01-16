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
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTest4Wnd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTest4Wnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgTest4Wnd::OnBnClickedButton3)
	ON_WM_PAINT()
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
	DrawRectFrame(&dc);
}

//////////////////////////////////////////////////////////////////////////
//
void CDlgTest4Wnd::OnBnClickedButton1()
{
	m_hWavPlayer.CreatePlayerProc("1.wav", 0, 2);
}

void CDlgTest4Wnd::OnBnClickedButton2()
{
	m_hWavPlayer.ClosePlayerProc();
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

BOOL CDlgTest4Wnd::InitInfo()
{
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

BOOL CDlgTest4Wnd::DrawRectFrame(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if (pDC == NULL)
	{
		return FALSE;
	}

	//CMemDC MemDC(pDC);
	//Graphics graphics(MemDC->GetSafeHdc());

	//Pen pen(Color(255, 0, 255, 0), 3);
	return TRUE;
}