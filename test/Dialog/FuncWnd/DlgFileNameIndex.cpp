#include "stdafx.h"
#include "DlgFileNameIndex.h"

IMPLEMENT_DYNAMIC(CDlgFileNameIndex, CDialog)
CDlgFileNameIndex::CDlgFileNameIndex(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameIndex::IDD, pParent)
{
	m_bInited = FALSE;
	m_bShowing = FALSE;

	memset(&m_stConfigData, 0x0, sizeof(UPDATE_FILEDATA));
}

CDlgFileNameIndex::~CDlgFileNameIndex()
{
}

void CDlgFileNameIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameIndex, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameIndex::OnInitDialog()
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

BOOL CDlgFileNameIndex::PreTranslateMessage(MSG* pMsg) 
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

void CDlgFileNameIndex::OnPaint() 
{
	CPaintDC dc(this);

	if (!m_bInited)
	{
		return;
	}

	DrawWndImage(&dc);
}

BOOL CDlgFileNameIndex::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgFileNameIndex::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInited)
	{
		return;
	}

	SetWndControlLayout();
}

void CDlgFileNameIndex::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_bShowing = bShow;
	CDialog::OnShowWindow(bShow, nStatus);
}

//////////////////////////////////////////////////////////////////////////
//



//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameIndex::InitCtrl()
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

BOOL CDlgFileNameIndex::InitInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		m_stConfigData.emConfigType = CONFIG_INDEXFILENAME_TYPE;
		m_stConfigData.emUpdateStatus = STATE_EMPTYTYPE;

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameIndex::InitWndSkin()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameIndex::InitWndInfo()
{
	BOOL bRet = FALSE;

	DWORD dwStyle = 0;

	CEdit *pEditChar = NULL;
	CSpinButtonCtrl *pSpinCtrl = NULL;

	do 
	{
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

			pEditChar->SetWindowText(_T("1"));
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NEWINDEX));
		pSpinCtrl = ((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_NEWINDEX));
		if (pEditChar != NULL && pSpinCtrl != NULL)
		{
			dwStyle = pEditChar->GetStyle();
			::SetWindowLong(pEditChar->GetSafeHwnd(), GWL_STYLE, dwStyle|ES_NUMBER);

			pSpinCtrl->SetBuddy(pEditChar);
			pSpinCtrl->SetRange(0, 255);

			pSpinCtrl->SetBase(1);
			pSpinCtrl->SetPos(0);

			pEditChar->SetWindowText(_T("1"));
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_SPANINDEX));
		pSpinCtrl = ((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SPANINDEX));
		if (pEditChar != NULL && pSpinCtrl != NULL)
		{
			dwStyle = pEditChar->GetStyle();
			::SetWindowLong(pEditChar->GetSafeHwnd(), GWL_STYLE, dwStyle|ES_NUMBER);

			pSpinCtrl->SetBuddy(pEditChar);
			pSpinCtrl->SetRange(0, 255);

			pSpinCtrl->SetBase(1);
			pSpinCtrl->SetPos(0);

			pEditChar->SetWindowText(_T("1"));
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_INDEXBIT));
		pSpinCtrl = ((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_BITINDEX));
		if (pEditChar != NULL && pSpinCtrl != NULL)
		{
			dwStyle = pEditChar->GetStyle();
			::SetWindowLong(pEditChar->GetSafeHwnd(), GWL_STYLE, dwStyle|ES_NUMBER);

			pSpinCtrl->SetBuddy(pEditChar);
			pSpinCtrl->SetRange(0, 255);

			pSpinCtrl->SetBase(1);
			pSpinCtrl->SetPos(0);

			pEditChar->SetWindowText(_T("1"));
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameIndex::UpdateWndCtrl()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameIndex::UpdateWndInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CDlgFileNameIndex::SetWndControlLayout()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);
}

BOOL CDlgFileNameIndex::DrawWndImage(CDC *pDC)
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

BOOL CDlgFileNameIndex::SetConfigData()
{
	BOOL bRet = FALSE;

	int nIndex = -1;
	int nNewIndex = -1;

	int nSpan = -1;
	int nBit = -1;

	CString strNameIndex;
	CString strSpan;
	CString strBit;

	CEdit *pEditChar = NULL;
	CSpinButtonCtrl *pSpinCtrl = NULL;

	do 
	{
		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NEWINDEX));	//ÐÂ±àºÅ
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
				nNewIndex = 0;
			}
			else
			{
				nNewIndex = atoi(strNameIndex.GetBuffer(0));
			}

			m_stConfigData.stcIndexFileName.nNewIndex = nNewIndex;
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_SPANINDEX));	//¼ä¸ô
		if (pEditChar == NULL)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			pEditChar->GetWindowText(strSpan);
			if (strSpan == _T(""))
			{
				nSpan = 0;
			}
			else
			{
				nSpan = atoi(strSpan.GetBuffer(0));
			}

			m_stConfigData.stcIndexFileName.nSpan = nSpan;
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_INDEXBIT));	//±àºÅÎ»Êý
		if (pEditChar == NULL)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			pEditChar->GetWindowText(strBit);
			if (strBit == _T(""))
			{
				nBit = 0;
			}
			else
			{
				nBit = atoi(strBit.GetBuffer(0));
			}

			m_stConfigData.stcIndexFileName.nBit = nBit;
		}

		pEditChar = ((CEdit*)GetDlgItem(IDC_EDIT_NAMEINDEX));	//ÐòºÅÎ»ÖÃ
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

		m_stConfigData.stcIndexFileName.nIndex = nIndex;

		if (((CButton *)GetDlgItem(IDC_CHECK_RECOUNTER))->GetCheck() == 1)
		{
			m_stConfigData.stcIndexFileName.bIsReCounter = TRUE;
		}
		else
		{
			m_stConfigData.stcIndexFileName.bIsReCounter = FALSE;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameIndex::GetWndAddData(UPDATE_FILEDATA *pUpdateData)
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