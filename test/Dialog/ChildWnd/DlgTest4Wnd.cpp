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
	DDX_Control(pDX, IDC_BTN4_TEST3,			m_btnTest3);
	DDX_Control(pDX, IDC_LIST4_SHOWINFO,		m_ListTest);
}

BEGIN_MESSAGE_MAP(CDlgTest4Wnd, CDialog)
	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_BTN4_TEST1,				OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN4_TEST2,				OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN4_TEST3,				OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN4_TEST4,				OnBnClickedButton4)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest4Wnd::OnInitDialog()
{
	USES_CONVERSION;
	CDialog::OnInitDialog();

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
	m_btnTest3.SetImage(_T("C:\\s1.png"), NULL, _T("C:\\s2.png"), NULL);
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
	static BOOL bFlag = FALSE;

	CString strFileName;
	CString strFilePath;

	if (!bFlag)
	{
		bFlag = TRUE;
		strFileName = _T("1.wav");
	}
	else
	{
		bFlag = FALSE;
		strFileName = _T("2.wav");
	}

#if 0
	strFilePath.Format(_T("%s\\%s"), CGlobalInfo::CreateInstance()->GetAppPath(), strFileName);
	if (!CGlobalInfo::CreateInstance()->IsFileExists(strFilePath))
	{
		return;
	}
#endif

#if 0
	m_WavePlayer.SetPlayerProcEvent(FALSE);
	if (!m_WavePlayer.SetPlayerProcData(strFilePath, 0))
	{
		return;
	}

	m_WavePlayer.SetPlayerProcEvent(TRUE);
#endif

#if 0
	if (!m_WavePlayer.CreatePlayerProc(strFilePath, 0, PlayMode_Loop))
	{
		return;
	}
#endif
}

void CDlgTest4Wnd::OnBnClickedButton2()
{
#if 0
	static BOOL bFlag = FALSE;
	if (!bFlag)
	{
		bFlag = TRUE;
		m_WavePlayer.SetPlayerProcEvent(FALSE);
	}
	else
	{
		bFlag = FALSE;
		m_WavePlayer.SetPlayerProcEvent(TRUE);
	}
#endif

#if 0
	m_WavePlayer.ClosePlayerProc();
#endif
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

void CDlgTest4Wnd::OnBnClickedButton4()
{
	BOOL bRet = FALSE;

	do 
	{
#if 0
		m_CheckLinkProc.CloseCheckLinkProc();
#endif
		bRet = TRUE;
	} while (FALSE);
}

BOOL test1(LINK_ADAPTER_INFO *pLinkAdapterInfo)
{
	BOOL bRet = FALSE;

	do 
	{
		if (pLinkAdapterInfo == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (pLinkAdapterInfo->emLinkStateType == LINK_STATEINPUTTYPE)
		{
			strcpy(pLinkAdapterInfo->szIpAddress, _T("192.168.2.113"));
		}
		else if (pLinkAdapterInfo->emLinkStateType == LINK_STATEOUTPUTTYPE)
		{
			if (pLinkAdapterInfo->bIsConnected)
			{
				TRACE(_T("%s-ÍøÂçÁ¬½Ó!"), pLinkAdapterInfo->szIpAddress);
			}
			else
			{
				TRACE(_T("%s-ÍøÂç¶Ï¿ª!"), pLinkAdapterInfo->szIpAddress);
			}
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
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
#if 0
	m_CheckLinkProc.CreateCheckLinkProc(test1);
#endif
	
#if 0
	if (!m_WavePlayer.CreatePlayerProc())
	{
		return FALSE;
	}
#endif
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