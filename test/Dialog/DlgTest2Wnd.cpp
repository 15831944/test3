#include  "stdafx.h"
#include  "../test.h"
#include  "DlgTest2Wnd.h"


int CALLBACK EnumFontProc(ENUMLOGFONTEX *lpelf,NEWTEXTMETRICEX *lpntm,DWORD nFontType,long lParam)
{
	CDlgTest2Wnd *pDlg = (CDlgTest2Wnd*)lParam;
	if (pDlg != NULL)
	{
		pDlg->SetListWndInfo(lpelf->elfLogFont.lfFaceName);
		return 1;	 
	}

	return 0;
}

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
	ON_LBN_SELCHANGE(IDC_LIST_TEST, OnLbnSelchangeListTest)
END_MESSAGE_MAP()

BOOL CDlgTest2Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDC *pDC = this->GetDC();

	CRect rcList;
	GetDlgItem(IDC_LIST_TEST)->GetWindowRect(&rcList);
	this->ScreenToClient(&rcList);

	CreateImageList();
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(_T("This is a Test!"));

	LOGFONT lf;
	lf.lfCharSet		= DEFAULT_CHARSET;
	lf.lfFaceName[0]	= NULL;
	lf.lfPitchAndFamily = 0;

	::EnumFontFamiliesEx(pDC->m_hDC, &lf, (FONTENUMPROC)EnumFontProc, (LPARAM)this, 0);

	m_ListBox.SetCurSel(0);
	OnLbnSelchangeListTest();
	return TRUE; 
}

BOOL CDlgTest2Wnd::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
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

void CDlgTest2Wnd::SetListWndInfo(LPCTSTR lpszText)
{
	if (m_ListBox.FindString(0, lpszText) < 0)
	{
		m_ListBox.AddString(lpszText);
	}	
}

void CDlgTest2Wnd::OnLbnSelchangeListTest()
{
	CRect rcStatic;
	CString strFontName;

	int nCulSel = m_ListBox.GetCurSel();
	if (nCulSel != -1)
	{
		m_font.Detach();
		m_ListBox.GetText(nCulSel, strFontName);
		
		GetDlgItem(IDC_STATIC_TEXT)->GetWindowRect(&rcStatic);
		m_font.CreateFont((rcStatic.Height()-5),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,strFontName);
		GetDlgItem(IDC_STATIC_TEXT)->SetFont(&m_font);
	}
}
