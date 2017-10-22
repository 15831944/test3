#include  "stdafx.h"
#include  "../test.h"
#include  "DlgTest2Wnd.h"

//////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNAMIC(CDlgTest2Wnd, CDialog)
CDlgTest2Wnd::CDlgTest2Wnd(CWnd* pParent)
	: CDialog(CDlgTest2Wnd::IDD, pParent)
{
}

CDlgTest2Wnd::~CDlgTest2Wnd()
{
}

void CDlgTest2Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEST, m_ListBox);
}

BEGIN_MESSAGE_MAP(CDlgTest2Wnd, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_TEST,					OnLbnSelchangeListTest)
	ON_MESSAGE(WM_TEST2WND_CTRL,					OnTest2WndMessage)
END_MESSAGE_MAP()

BOOL CDlgTest2Wnd::OnInitDialog()
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

BOOL CDlgTest2Wnd::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTest2Wnd::OnLbnSelchangeListTest()
{
	int nCulSel = -1;

	CRect rcStatic;
	CString strFontName;

	nCulSel = m_ListBox.GetCurSel();
	if (nCulSel == -1)
	{
		return;
	}

// 	m_font.Detach();
// 	m_ListBox.GetText(nCulSel, strFontName);
// 
// 	GetDlgItem(IDC_STATIC_TEXT)->GetWindowRect(&rcStatic);
// 	m_font.CreateFont((rcStatic.Height()-5),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,strFontName);
// 	GetDlgItem(IDC_STATIC_TEXT)->SetFont(&m_font);
}

LRESULT CDlgTest2Wnd::OnTest2WndMessage(WPARAM wParam, LPARAM lParam)
{
	CString strFontName;
	HT_TESTWND_MSG* pWndMsg = NULL;

	switch(wParam)
	{
	case GETSYSTEM_FONT:
		{
			pWndMsg = (HT_TESTWND_MSG*)lParam;
			if (pWndMsg == NULL)
			{
				return -1;
			}

			if (pWndMsg->dwParam != GETSYSTEM_FONT)
			{
				return -1;
			}
			
 			strFontName = pWndMsg->szValue;
			if (m_ListBox.FindString(0, strFontName) > 0)
			{
				return -1;
			}

			m_ListBox.AddString(strFontName);
		}
		break;
	}

	if (pWndMsg != NULL)
	{
		delete pWndMsg;
		pWndMsg = NULL;
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest2Wnd::InitCtrl()
{
	DWORD dwStyle = 0;
	DWORD dwScrollWndId = 0;

	CRect rcClient;
	CRect rcDlgList;
	CRect rcDlgText;
	
	GetClientRect(&rcClient);
	m_ListBox.GetClientRect(&rcDlgList);

	rcDlgText.top    = rcClient.top;
	rcDlgText.left   = rcClient.left;
	rcDlgText.right  = rcClient.right;
	rcDlgText.bottom = rcClient.bottom - rcDlgList.bottom;

	dwStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;
	if (!m_dlgScrollWnd.Create(dwStyle, rcDlgText, this, dwScrollWndId))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgTest2Wnd::InitInfo()
{
	LOGFONT lf;
	lf.lfCharSet		= DEFAULT_CHARSET;
	lf.lfFaceName[0]	= NULL;
	lf.lfPitchAndFamily = 0;

	::EnumFontFamiliesEx(GetDC()->GetSafeHdc(), &lf, (FONTENUMPROC)EnumFontProc, (LPARAM)this, 0);
	return TRUE;
}

void CDlgTest2Wnd::CreateImageList()
{
	BOOL	bRetValue = FALSE;
	HICON	hIcon = NULL;

	bRetValue = m_ImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);

 	hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
 	m_ImageList.Add(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
	m_ImageList.Add(hIcon);
// 	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_SHELL32_137);
// 	m_ImageList.Add(hIcon);
// 	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_SHELL32_005);
// 	m_ImageList.Add(hIcon);
// 	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_SHELL32_110);
// 	m_ImageList.Add(hIcon);
// 	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_SHELL32_131);
// 	m_ImageList.Add(hIcon);
}