#include "stdafx.h"
#include "testDlg.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_bInited = FALSE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CenterWindow();
	return TRUE; 
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		CRect rect;
		GetClientRect(&rect);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

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
//
CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	m_nDefaultSel = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTestDlg::~CTestDlg()
{
	m_pArPage.RemoveAll();
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCTRL, m_hTabCtrl);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()

	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnTcnSelchangeTabWndCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
BOOL CTestDlg::OnInitDialog()
{
	BOOL bRet = FALSE;
	CDialog::OnInitDialog();

	do 
	{
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
		::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	}

	return bRet;  
}

int CTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	return 0;
}

void CTestDlg::OnDestroy()
{
	__super::OnDestroy();

	DestroyChildWnd();
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
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		GetClientRect(&rect);
		dc.Draw3dRect(rect, RGB(255,0,0), RGB(0,255,0));
		CDialog::OnPaint();
	}
}

HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnTcnSelchangeTabWndCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCurSel = m_hTabCtrl.GetCurSel();
	if (nCurSel != m_nPrePage)
	{
		((CDialog*)m_pArPage[m_nPrePage])->ShowWindow(SW_HIDE);
		((CDialog*)m_pArPage[nCurSel])->ShowWindow(SW_SHOW);

		m_nPrePage = nCurSel;
	}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CTestDlg::InitCtrl()
{
	if (!CreateChildWnd())
	{
		return FALSE;
	}

	if (!InitWndCtrl())
	{
		return FALSE;
	}

	if (!InitWndInfo())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CTestDlg::InitInfo()
{
	return TRUE;
}

BOOL CTestDlg::InitWndCtrl()
{
	BOOL bRet = FALSE;

	DWORD dwIndex = 0;
	CRect rcClient;
	CRect rcDialog;

	do 
	{
		m_hTabCtrl.GetClientRect(&rcClient);

		m_hTabCtrl.InsertItem(dwIndex++, _T("test1"));
		m_hTabCtrl.InsertItem(dwIndex++, _T("test2"));
		m_hTabCtrl.InsertItem(dwIndex++, _T("test3"));
		m_hTabCtrl.InsertItem(dwIndex++, _T("test4"));
		m_hTabCtrl.InsertItem(dwIndex++, _T("test5"));

		rcDialog.left = rcClient.left + 2;
		rcDialog.top  = rcClient.top + 20;
		rcDialog.right  = rcClient.right - 2;
		rcDialog.bottom = rcClient.bottom - 2;

		for (dwIndex=0; dwIndex<m_pArPage.GetSize(); ++dwIndex)
		{
			((CDialog*)m_pArPage[dwIndex])->MoveWindow(&rcDialog);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CTestDlg::InitWndInfo()
{
	((CDialog*)m_pArPage[m_nDefaultSel])->ShowWindow(TRUE);
	((CDialog*)m_pArPage[m_nDefaultSel])->SetFocus();

	m_hTabCtrl.SetCurSel(m_nDefaultSel);
	m_nPrePage = m_nDefaultSel;

	return TRUE;
}

BOOL CTestDlg::CreateChildWnd()
{
	BOOL bRet = FALSE;

	do 
	{
		m_hDlgTest1Wnd.Create(IDD_DIALOG1, GetDlgItem(IDC_TABCTRL));
		m_hDlgTest2Wnd.Create(IDD_DIALOG2, GetDlgItem(IDC_TABCTRL));
		m_hDlgTest3Wnd.Create(IDD_DIALOG3, GetDlgItem(IDC_TABCTRL));
		m_hDlgTest4Wnd.Create(IDD_DIALOG4, GetDlgItem(IDC_TABCTRL));
		m_hDlgTest5Wnd.Create(IDD_DIALOG5, GetDlgItem(IDC_TABCTRL));

		m_pArPage.Add(&m_hDlgTest1Wnd);
		m_pArPage.Add(&m_hDlgTest2Wnd);
		m_pArPage.Add(&m_hDlgTest3Wnd);
		m_pArPage.Add(&m_hDlgTest4Wnd);
		m_pArPage.Add(&m_hDlgTest5Wnd);

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CTestDlg::DestroyChildWnd()
{
	DWORD dwIndex = 0;
	CDialog *pDialog = NULL;

	for (dwIndex; dwIndex<m_pArPage.GetSize(); ++dwIndex)
	{
		pDialog = (CDialog *)m_pArPage.GetAt(dwIndex);
		if (pDialog == NULL)
		{
			continue;
		}

		pDialog->DestroyWindow();
		pDialog = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//
int (WINAPI *Real_Messagebox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)= MessageBoxA;
extern "C" _declspec(dllexport) BOOL WINAPI MessageBox_Mine(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	CString temp= lpText;
	temp+="该MessageBox已被Detours截获！";
	return Real_Messagebox(hWnd,temp,lpCaption,uType);
}

void CTestDlg::Hook()
{
	DetourRestoreAfterWith();	//恢复原来状态
	DetourTransactionBegin();	//拦截开始
	DetourUpdateThread(GetCurrentThread());	//刷新当前线程
	DetourAttach(&(PVOID&)Real_Messagebox,MessageBox_Mine); //设置detour

	DetourTransactionCommit();	//拦截生效
}

void CTestDlg::UnHook()
{
	DetourTransactionBegin();	//拦截开始
	DetourUpdateThread(GetCurrentThread());	//刷新当前线程
	DetourDetach(&(PVOID&)Real_Messagebox,MessageBox_Mine);//卸载detour

	DetourTransactionCommit();	//拦截生效
}