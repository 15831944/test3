#include "stdafx.h"
#include "DlgFileNameReplace.h"

IMPLEMENT_DYNAMIC(CDlgFileNameReplace, CDialog)
CDlgFileNameReplace::CDlgFileNameReplace(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameReplace::IDD, pParent)
{
	m_bInited = FALSE;
	m_bShowing = FALSE;

	memset(&m_stConfigData, 0x0, sizeof(UPDATE_FILEDATA));
}

CDlgFileNameReplace::~CDlgFileNameReplace()
{
}

void CDlgFileNameReplace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameReplace, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameReplace::OnInitDialog()
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

BOOL CDlgFileNameReplace::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileNameReplace::OnPaint() 
{
	CPaintDC dc(this);

	if (!m_bInited)
	{
		return;
	}

	DrawWndImage(&dc);
}

BOOL CDlgFileNameReplace::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgFileNameReplace::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInited)
	{
		return;
	}

	SetWndControlLayout();
}

void CDlgFileNameReplace::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_bShowing = bShow;
	CDialog::OnShowWindow(bShow, nStatus);
}

//////////////////////////////////////////////////////////////////////////
//



//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameReplace::InitCtrl()
{
	BOOL bRet = FALSE;

	do 
	{
		if (!InitWndSkin())
		{
			bRet = FALSE;
			break;
		}

		if (!InitWndInfo())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameReplace::InitInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		m_stConfigData.emConfigType = CONFIG_REPLACEFILENAME_TYPE;
		m_stConfigData.emUpdateStatus = STATE_EMPTYTYPE;

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameReplace::InitWndSkin()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameReplace::InitWndInfo()
{
	BOOL bRet = FALSE;
	DWORD dwStyle = 0;

	CEdit *pEditChar = NULL;
	CSpinButtonCtrl *pSpinCtrl = NULL;

	do 
	{
		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMESRCCHAR));
		if (pEditChar != NULL)
		{
			pEditChar->SetLimitText(MAX_EDITCHAR_SIZE);
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMEDESCCHAR));
		if (pEditChar != NULL)
		{
			pEditChar->SetLimitText(MAX_EDITCHAR_SIZE);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameReplace::UpdateWndCtrl()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameReplace::UpdateWndInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CDlgFileNameReplace::SetWndControlLayout()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);
}

BOOL CDlgFileNameReplace::DrawWndImage(CDC *pDC)
{
	BOOL bRet = FALSE;

	do 
	{
		if (pDC == NULL)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameReplace::SetConfigData()
{
	BOOL bRet = FALSE;

	CString strNameChar;

	CEdit *pEditChar = NULL;
	CSpinButtonCtrl *pSpinCtrl = NULL;

	do 
	{
		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMESRCCHAR));
		if (pEditChar == NULL)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			pEditChar->GetWindowText(strNameChar);
			if (strNameChar == _T(""))
			{
				bRet = FALSE;
				break;
			}

			sprintf(m_stConfigData.stcReplaceFileName.szFindName, _T("%s"), strNameChar);
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMEDESCCHAR));
		if (pEditChar == NULL)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			pEditChar->GetWindowText(strNameChar);
			if (strNameChar == _T(""))
			{
				bRet = FALSE;
				break;
			}

			sprintf(m_stConfigData.stcReplaceFileName.szFileName, _T("%s"), strNameChar);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameReplace::GetWndAddData(UPDATE_FILEDATA *pUpdateData)
{
	BOOL bRet = FALSE;

	do 
	{
		if (pUpdateData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (!SetConfigData())
		{
			bRet = FALSE;
			break;
		}

		memcpy(pUpdateData, &m_stConfigData, sizeof(UPDATE_FILEDATA));
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}