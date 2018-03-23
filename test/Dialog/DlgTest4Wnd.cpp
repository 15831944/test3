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

/*
BOOL test1(const CK_UKEYPROCINFO *pUKeyInfo, CK_UKEYUSERDATA *pUKeyUserData, LPVOID lpParam)
{
	char szUserNum[64] = {0};
	char szUserPasswd[64] = {0};

	if (pUKeyInfo == NULL)
	{
		return FALSE;
	}

	if (pUKeyInfo->emUKeyOperateType == CK_OPERATEENUMSLOTTYPE)
	{
		if (pUKeyInfo->emUKeyState == CK_UKEYSTATEEMPTYTYPE)
		{
			TRACE(_T("无法枚举到UKEY设备!"));
		}
		if (pUKeyInfo->emUKeyState == CK_UKEYSTATEINSERTTYPE)
		{
			TRACE(_T("插入UKEY设备:%d"), pUKeyInfo->ulSlotId);
		}
		else if (pUKeyInfo->emUKeyState == CK_UKEYSTATEREMOVETYPE)
		{
			TRACE(_T("拔除UKEY设备:%d"), pUKeyInfo->ulSlotId);
		}
	}
	else if (pUKeyInfo->emUKeyOperateType == CK_OPERATEVERIFYTYPE)
	{
		if (pUKeyInfo->emUKeyType == CK_UKEYDEVFINGERTYPE)
		{
			if (pUKeyInfo->emUKeyState == CK_UKEYSTATEINPUTETYPE)
			{
				TRACE(_T("录入UKEY设备指纹数据成功!"));
			}
			else if (pUKeyInfo->emUKeyState == CK_UKEYSTATESUCCEDTYPE)
			{
				TRACE(_T("校验UKEY设备用户指纹数据成功!"));
			}
			else if (pUKeyInfo->emUKeyState == CK_UKEYSTATEFAILEDTYPE)
			{
				TRACE(_T("校验UKEY设备用户指纹数据失败!"));
			}
		}
		else if (pUKeyInfo->emUKeyType == CK_UKEYDEVNORMALTYPE)
		{
			if (pUKeyInfo->emUKeyState == CK_UKEYSTATEINPUTETYPE)
			{
				if (pUKeyUserData != NULL)
				{
					TRACE(_T("录入UKEY设备用户PIN码数据!"));
					strcpy(pUKeyUserData->szUserPIN, _T("12345678"));
				}
			}
			else if (pUKeyInfo->emUKeyState == CK_UKEYSTATESUCCEDTYPE)
			{
				TRACE(_T("校验UKEY设备用户PIN码数据成功!"));
			}
			else if (pUKeyInfo->emUKeyState == CK_UKEYSTATEFAILEDTYPE)
			{
				TRACE(_T("校验UKEY设备用户PIN码数据失败!"));
			}
		}
	}
	else if (pUKeyInfo->emUKeyOperateType == CK_OPERATEWRITETYPE)
	{
		if (pUKeyInfo->emUKeyState == CK_UKEYSTATEINPUTETYPE)
		{
			TRACE(_T("录入UKEY设备用户数据!"));
			strcpy(pUKeyUserData->szUserNum, _T("16104010006"));
			strcpy(pUKeyUserData->szUserPasswd, _T("12345678"));
			ResetEvent(pUKeyInfo->hEvent);
		}
		else if (pUKeyInfo->emUKeyState == CK_UKEYSTATESUCCEDTYPE)
		{
			TRACE(_T("写入UKEY设备用户数据成功!"));
		}
		else if (pUKeyInfo->emUKeyState == CK_UKEYSTATEFAILEDTYPE)
		{
			TRACE(_T("写入UKEY设备用户数据失败!"));
		}
	}
	else if (pUKeyInfo->emUKeyOperateType == CK_OPERATEREADTYPE)
	{
		if (pUKeyInfo->emUKeyState == CK_UKEYSTATESUCCEDTYPE)
		{
			TRACE(_T("读取UKEY设备用户数据成功!"));
			strcpy(szUserNum, pUKeyUserData->szUserNum);
			strcpy(szUserPasswd, pUKeyUserData->szUserPasswd);
		}
		else if (pUKeyInfo->emUKeyState == CK_UKEYSTATEFAILEDTYPE)
		{
			TRACE(_T("读取UKEY设备用户数据失败!"));
		}
	}

	return TRUE;
}
*/

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
// 	if (!m_UKeyVerifyProc.CreateVerifyProc(TRUE, test1, FALSE))
// 	{
// 		return FALSE;
// 	}
// 
// 	m_UKeyVerifyProc.SetThreadProcTime(500, 500);
// 	m_UKeyVerifyProc.SetThreadProcState(TRUE);

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