#include  "stdafx.h"
#include  "DlgTest1Wnd.h"

IMPLEMENT_DYNAMIC(CDlgTest1Wnd, CDialog)
CDlgTest1Wnd::CDlgTest1Wnd(CWnd* pParent)
	: CDialog(CDlgTest1Wnd::IDD, pParent)
{
}

CDlgTest1Wnd::~CDlgTest1Wnd()
{
}

void CDlgTest1Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
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

		bRet = TRUE;
	} while (FALSE);
	
	if (!bRet)
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
	}

	return bRet; 
}

BOOL CDlgTest1Wnd::PreTranslateMessage(MSG* pMsg)
{
// 	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
// 	{
// 		return TRUE;
// 	}
// 
// 	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
// 	{
// 		return TRUE;
// 	}

	if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
	{
		MSG msg;
		::CopyMemory(&msg, pMsg, sizeof(MSG));
		HWND hWndParent = ::GetParent(msg.hwnd);
		while (hWndParent && hWndParent != m_hWnd)
		{
			msg.hwnd = hWndParent;
			hWndParent = ::GetParent(hWndParent);
		}
		if (msg.hwnd)
		{
			m_ToolTip.UpdateTipText("11111", CWnd::FromHandle(pMsg->hwnd));
			m_ToolTip.RelayEvent(&msg);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::InitCtrl()
{
	m_ToolTip.Create(this, TTS_ALWAYSTIP );
	m_ToolTip.Activate(TRUE);

	m_ToolTip.SetTipTextColor(RGB(0, 255, 0));
	m_ToolTip.SetTipBkColor(RGB(255, 0, 0));

	//m_ToolTip.SetDelayTime(150);
	m_ToolTip.AddTool(GetDlgItem(IDC_BTN_TEST1), "test1");
	m_ToolTip.AddTool(GetDlgItem(IDC_BTN_TEST2), "test2");

	return TRUE;
}

BOOL CDlgTest1Wnd::InitInfo()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
void CDlgTest1Wnd::OnBnClickedButton1()
{
	std::string str;
	std::string s1;

#if 0
	std::tr1::array<int, 5> arry = {1, 2, 3, 4, 5};
	string str1("1994 is my birth year");

	string regex_str1("\\d{4}");
	std::tr1::regex pattern1(regex_str1, std::tr1::regex::icase);

	string s1;
	std::tr1::match_results<string::const_iterator> result;

	string::const_iterator iter = str1.begin();
	string::const_iterator iterEnd= str1.end();

	while (std::tr1::regex_search(iter, iterEnd, result, pattern))
	{
		s1=result[0];

		cout<<s1<<endl;
		iter = result[0].second; //更新搜索起始位置
	}
#endif

#if 0
	std::string str2 = _T("this is?#( a #@#test!");
	std::string regex_str2 = _T("((?=[\x21-\x7e]+)[^A-Za-z0-9])");	//(( )+|(\n)+)	//((?=[\x21-\x7e]+)[^A-Za-z0-9])
	std::tr1::regex pattern2(regex_str2, std::tr1::regex::icase);
	
	//ltrim(str2);
	//rtrim(str2);

	s1 = _T("");
	str = std::tr1::regex_replace(str2, pattern2, s1);
#endif
}

typedef enum {
	PRIORITY_IDLE = -1,
	PRIORITY_NORMAL = 0,
	PRIORITY_ABOVE_NORMAL = 1,
	PRIORITY_HIGH = 2,
}THREAD_PRIORITY_T;

#include "../../Inc/common/Leaf.System/LeafEvent.h"
using namespace Leaf::System;
void CDlgTest1Wnd::OnBnClickedButton2()
{
	BOOL bRet = FALSE;

	Leaf::System::CEvent event;
	do 
	{
// 		pthread_attr_init(&pMutex_t->ptAttr);
// 		if (priority != PRIORITY_NORMAL)
// 		{
// 			if (priority != PRIORITY_IDLE)
// 			{
// 				pthread_attr_setschedpolicy(&pMutex_t->ptAttr, SCHED_RR);			//实时,轮转法
// 				if (pthread_attr_getschedparam(&pMutex_t->ptAttr, &param) == 0)		//查询优先级
// 				{
// 					if (priority == PRIORITY_HIGH)
// 					{
// 						param.sched_priority = 6;									//6:HIGH
// 					}
// 					else
// 					{
// 						param.sched_priority = 4;									//4:ABOVE_NORMAL
// 					}
// 
// 					pthread_attr_setschedparam(&pMutex_t->ptAttr, &param);			//设置优先级
// 				}
// 			}
// 		}

// 		HEVENT hEvent = event.CreateEvent(false, false, "123");
// 		TRACE("1");
// 		event.WaitForEvent(hEvent, 2000);
// 		TRACE("2");
// 		event.CloseEvent(hEvent);

		bRet = TRUE;
	} while (FALSE);
}