#include "stdafx.h"
#include "DlgFileNameAdd.h"

IMPLEMENT_DYNAMIC(CDlgFileNameAdd, CDialog)
CDlgFileNameAdd::CDlgFileNameAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameAdd::IDD, pParent)
{
	m_bInited = FALSE;
	m_bShowing = FALSE;

	memset(&m_stConfigData, 0x0, sizeof(UPDATE_FILEDATA));
}

CDlgFileNameAdd::~CDlgFileNameAdd()
{
}

void CDlgFileNameAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameAdd, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_CHECK_REVERSE,	OnBnClickedCheckReverse)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameAdd::OnInitDialog()
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

BOOL CDlgFileNameAdd::PreTranslateMessage(MSG* pMsg) 
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

void CDlgFileNameAdd::OnPaint() 
{
	CPaintDC dc(this);

	if (!m_bInited)
	{
		return;
	}

	DrawWndImage(&dc);
}

BOOL CDlgFileNameAdd::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgFileNameAdd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInited)
	{
		return;
	}

	SetWndControlLayout();
}

void CDlgFileNameAdd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_bShowing = bShow;
	CDialog::OnShowWindow(bShow, nStatus);
}

//////////////////////////////////////////////////////////////////////////
//
void CDlgFileNameAdd::OnBnClickedCheckReverse()
{
	CString strText;
	if (((CButton *)GetDlgItem(IDC_CHECK_REVERSE))->GetCheck() == 1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_NAMEINDEX))->GetWindowText(strText);
		if (strText == _T("") || strText == _T("0"))
		{
			((CEdit*)GetDlgItem(IDC_EDIT_NAMEINDEX))->SetWindowText(_T("1"));
		}		
	}
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameAdd::InitCtrl()
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

BOOL CDlgFileNameAdd::InitInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		m_stConfigData.emConfigType = CONFIG_ADDFILENAME_TYPE;
		m_stConfigData.emUpdateStatus = STATE_EMPTYTYPE;

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameAdd::InitWndSkin()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameAdd::InitWndInfo()
{
	BOOL bRet = FALSE;
	DWORD dwStyle = 0;

	CEdit *pEditChar = NULL;
	CSpinButtonCtrl *pSpinCtrl = NULL;

	do 
	{
		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMECHAR));
		if (pEditChar != NULL)
		{
			pEditChar->SetLimitText(MAX_EDITCHAR_SIZE);
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMEINDEX));
		pSpinCtrl = ((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_NAMEINDEX));
		if (pEditChar != NULL && pSpinCtrl != NULL)
		{
			dwStyle = pEditChar->GetStyle();
			::SetWindowLong(pEditChar->GetSafeHwnd(), GWL_STYLE, dwStyle|ES_NUMBER);

			pSpinCtrl->SetBuddy(pEditChar);
			pSpinCtrl->SetRange(0, 255);

			pSpinCtrl->SetBase(1);
			pSpinCtrl->SetPos(0);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameAdd::UpdateWndCtrl()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameAdd::UpdateWndInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CDlgFileNameAdd::SetWndControlLayout()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);
}

BOOL CDlgFileNameAdd::DrawWndImage(CDC *pDC)
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

BOOL CDlgFileNameAdd::SetConfigData()
{
	BOOL bRet = FALSE;
	int nIndex = -1;

	CString strNameChar;
	CString strNameIndex;

	CEdit *pEditChar = NULL;
	CSpinButtonCtrl *pSpinCtrl = NULL;

	do 
	{
		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMECHAR));
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

			sprintf(m_stConfigData.stcAddFileName.szFileName, _T("%s"), strNameChar);
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMEINDEX));
		if (pEditChar == NULL)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			pEditChar->GetWindowText(strNameIndex);
			if (strNameIndex == _T(""))
			{
				nIndex = 0;
			}
			else
			{
				nIndex = atoi(strNameIndex.GetBuffer(0));
			}
		}

		if (((CButton *)GetDlgItem(IDC_CHECK_REVERSE))->GetCheck() == 1)
		{
			if (nIndex > 0)
			{
				nIndex = ~nIndex+1;
			}
			else
			{
				nIndex = 0;
			}
		}

		m_stConfigData.stcAddFileName.iIndex = nIndex;
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameAdd::GetWndAddData(UPDATE_FILEDATA *pUpdateData)
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