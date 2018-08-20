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
	DDX_Control(pDX, IDC_LIST1_SHOWINFO, m_ListBox);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1_SHOWINFO,			OnLbnSelchangeListTest)
	ON_MESSAGE(WM_TEST1WND_CTRL,					OnTest1WndMessage)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!InitInfo())
	{
		return FALSE;
	}

	if (!InitCtrl())
	{
		return FALSE;
	}

	return TRUE; 
}

BOOL CDlgTest1Wnd::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTest1Wnd::OnDestroy()
{
	CDialog::OnDestroy();

	for (DWORD dwIndex=0; dwIndex<m_ListBox.GetCount(); dwIndex++)
	{
		LOGFONT* pLogFont = (LOGFONT*)m_ListBox.GetItemDataPtr(dwIndex);
		if (pLogFont == NULL)
		{
			continue;
		}

		delete pLogFont;
		pLogFont = NULL;
	}
}

void CDlgTest1Wnd::OnLbnSelchangeListTest()
{
	int nCulSel = -1;

	CRect rcStatic;
	CString strFontName;

	LOGFONT* pLogFont = NULL;

	nCulSel = m_ListBox.GetCurSel();
	if (nCulSel == -1)
	{
		return;
	}

	pLogFont = (LOGFONT*)m_ListBox.GetItemDataPtr(nCulSel);
	if (pLogFont == NULL)
	{
		return;
	}

	m_dlgScrollWnd.SetFont(50, pLogFont->lfFaceName);
}

LRESULT CDlgTest1Wnd::OnTest1WndMessage(WPARAM wParam, LPARAM lParam)
{
	DWORD dwIndex = 0;
	CString strFontName;

	LOGFONT* pLogFont = NULL;
	HT_TESTWND_MSG* pWndMsg = NULL;

	switch(wParam)
	{
	case GETSYSTEM_FONT:
		{
			pWndMsg = (HT_TESTWND_MSG*)lParam;
			if (pWndMsg == NULL)
			{
				break;
			}

			if (pWndMsg->dwParam != GETSYSTEM_FONT)
			{
				break;
			}
			
 			strFontName = pWndMsg->szValue;
			if (m_ListBox.FindString(0, strFontName) > 0)
			{
				break;
			}

			pLogFont = new LOGFONT;
			if (pLogFont == NULL)
			{
				break;
			}
			memset(pLogFont, 0x0, sizeof(LOGFONT));
			memcpy(pLogFont, &pWndMsg->hLogFont, sizeof(LOGFONT));

			dwIndex = m_ListBox.GetCount();
			m_ListBox.InsertString(dwIndex, strFontName);
			m_ListBox.SetItemDataPtr(dwIndex, pLogFont);

			m_ListBox.SetSel(0,TRUE);
			m_ListBox.SetCurSel(0);
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
BOOL CDlgTest1Wnd::InitCtrl()
{
	DWORD dwStyle = 0;
	DWORD dwScrollWndId = 0;

	CRect rcClient;
	CRect rcDlgList;
	CRect rcDlgText;

	CString strTextInfo;
	
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

	strTextInfo = _T("this is a test!");

	m_dlgScrollWnd.SetWndBkColor(RGB(0, 51, 102));
	m_dlgScrollWnd.SetWndText(strTextInfo, NULL, RGB(255, 0, 0));
	return TRUE;
}

BOOL CDlgTest1Wnd::InitInfo()
{
	LOGFONT lf;
	lf.lfCharSet		= DEFAULT_CHARSET;
	lf.lfFaceName[0]	= NULL;
	lf.lfPitchAndFamily = 0;

	::EnumFontFamiliesEx(GetDC()->GetSafeHdc(), &lf, (FONTENUMPROC)EnumFontProc, (LPARAM)this, 0);
	return TRUE;
}

void CDlgTest1Wnd::CreateImageList()
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