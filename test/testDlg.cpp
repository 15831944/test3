#include "stdafx.h"
#include "test.h"
#include "testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//                
CAboutDlg::CAboutDlg() 
	: CSkinDialog(CAboutDlg::IDD, TEXT("360Safe.xml"))
{
	m_bInited = FALSE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CenterWindow();
	return TRUE; 
}

void CAboutDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		CRect rect;
		GetClientRect(&rect);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CSkinDialog::OnPaint();
	}
}

void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	if (!m_bInited)
	{
		return;
	}
}

HCURSOR CAboutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB1, m_hTabCtrl);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1,OnTcnSelchangeTabWndctrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers
BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		


	Init();
	return TRUE;  
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CTestDlg::OnPaint() 
{
	CRect rect;
	CPaintDC dc(this); 

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		GetClientRect(&rect);
//		dc.Draw3dRect(rect, RGB(255,0,0), RGB(0,255,0));

		CDialog::OnPaint();
	}
}

HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnTcnSelchangeTabWndctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCulSel = m_hTabCtrl.GetCurSel();
	
	if (nCulSel != m_nPrePage)
	{
		((CDialog*)m_pArPage[m_nPrePage])->ShowWindow(SW_HIDE);
		((CDialog*)m_pArPage[nCulSel])->ShowWindow(SW_SHOW);
		m_nPrePage = nCulSel;
	}

	*pResult = 0;
}

void CTestDlg::Init()
{
	InitCtrl();
}

void CTestDlg::InitCtrl()
{
	m_hDlgTest1Wnd.Create(IDD_DIALOG1, GetDlgItem(IDC_TAB1));
	m_hDlgTest2Wnd.Create(IDD_DIALOG2, GetDlgItem(IDC_TAB1));
	m_hDlgTest3Wnd.Create(IDD_DIALOG3, GetDlgItem(IDC_TAB1));
	m_hDlgTest4Wnd.Create(IDD_DIALOG4, GetDlgItem(IDC_TAB1));

	m_pArPage.Add(&m_hDlgTest1Wnd);
	m_pArPage.Add(&m_hDlgTest2Wnd);
	m_pArPage.Add(&m_hDlgTest3Wnd);
	m_pArPage.Add(&m_hDlgTest4Wnd);

	m_hTabCtrl.InsertItem(0, _T("test1"));
	m_hTabCtrl.InsertItem(1, _T("test2"));
	m_hTabCtrl.InsertItem(2, _T("test3"));
	m_hTabCtrl.InsertItem(3, _T("test4"));

	CRect rcClient;
	m_hTabCtrl.GetClientRect(&rcClient);

	rcClient.left   +=2;
	rcClient.top    +=20;
	rcClient.right  -=2;
	rcClient.bottom -=4;

	for(int i=0; i<m_pArPage.GetSize(); i++)
	{
		((CDialog*)m_pArPage[i])->MoveWindow(&rcClient);
	}

	((CDialog*)m_pArPage[0])->ShowWindow(TRUE);
	((CDialog*)m_pArPage[0])->SetFocus();

	m_hTabCtrl.SetCurSel(0);
	m_nPrePage = 0;
}