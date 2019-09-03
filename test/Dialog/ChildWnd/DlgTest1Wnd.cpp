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

	m_TipWnd.RelayEvent(pMsg);
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
		//m_TipWnd.ShowWindow(SW_HIDE);
	}

	CDialog::OnShowWindow(bShow, nStatus);
}

LRESULT CDlgTest1Wnd::OnNcHitTest(CPoint point)
{
	SetTipWndLayout();
	return CDialog::OnNcHitTest(point);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::InitCtrl()
{
	m_TipWnd.Create(this);
	return TRUE;
}

BOOL CDlgTest1Wnd::InitInfo()
{
	return TRUE;
}

void CDlgTest1Wnd::SetWndControlLayout()
{
}

void CDlgTest1Wnd::SetTipWndLayout()
{
	CRect rcTipWnd;

	CRect rcClient;
	GetWindowRect(&rcClient);

	rcTipWnd.left = rcClient.left;
	rcTipWnd.top = rcClient.bottom - 30;
	rcTipWnd.right = rcClient.right;
	rcTipWnd.bottom = rcClient.bottom;
	m_TipWnd.MoveWindow(rcTipWnd);
}

//////////////////////////////////////////////////////////////////////////
//
void CDlgTest1Wnd::OnBnClickedButton1()
{
	std::string str;
	std::string s1;

	CString strBtnText;

	GetDlgItem(IDC_BTN_TEST1)->GetWindowText(strBtnText);
	m_TipWnd.ShowWnd(strBtnText);

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

#include "../../inc/audio/interface/AudioWaveApi.h"
void CDlgTest1Wnd::OnBnClickedButton2()
{
	CString strBtnText;

	GetDlgItem(IDC_BTN_TEST2)->GetWindowText(strBtnText);
	m_TipWnd.ShowWnd(strBtnText);

#if 0
	Leaf::System::CEvent event;;
	event.CreateEvent(false, false);

	TRACE("event1");
	event.WaitForEvent(0);

	TRACE("event2");
	event.CloseEvent();
#endif
 	std::vector<CDeviceData> vecAudioDev;

 	CAudioWaveAPi waveApi;
 	waveApi.audio_enumDevice(vecAudioDev);
}