#include "stdafx.h"
#include "DlgFileNameDate.h"

IMPLEMENT_DYNAMIC(CDlgFileNameDate, CDialog)
CDlgFileNameDate::CDlgFileNameDate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameDate::IDD, pParent)
{
	m_bInited = FALSE;
	m_bShowing = FALSE;

	memset(&m_stConfigData, 0x0, sizeof(UPDATE_FILEDATA));
}

CDlgFileNameDate::~CDlgFileNameDate()
{
}

void CDlgFileNameDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFileNameDate, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameDate::OnInitDialog()
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

BOOL CDlgFileNameDate::PreTranslateMessage(MSG* pMsg) 
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

void CDlgFileNameDate::OnPaint() 
{
	CPaintDC dc(this);

	if (!m_bInited)
	{
		return;
	}

	DrawWndImage(&dc);
}

BOOL CDlgFileNameDate::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgFileNameDate::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInited)
	{
		return;
	}

	SetWndControlLayout();
}

void CDlgFileNameDate::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_bShowing = bShow;
	CDialog::OnShowWindow(bShow, nStatus);
}

//////////////////////////////////////////////////////////////////////////
//



//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameDate::InitCtrl()
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

BOOL CDlgFileNameDate::InitInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		m_stConfigData.emConfigType = CONFIG_DATEFILENAME_TYPE;
		m_stConfigData.emUpdateStatus = STATE_EMPTYTYPE;

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameDate::InitWndSkin()
{
	BOOL bRet = FALSE;

	DWORD dwStyle = 0;
	DWORD dwIndex = 0;

	CComboBox *pComboBoxDateFormat = NULL;
	CComboBox *pComboBoxFileProPerty = NULL;

	do 
	{
		pComboBoxDateFormat = (CComboBox *)GetDlgItem(IDC_COMBO_DATEFORMAT);
		if (pComboBoxDateFormat != NULL)
		{

			dwIndex = 0;
			pComboBoxDateFormat->InsertString(dwIndex, _T("yyyymmddhhmmss"));
			pComboBoxDateFormat->SetItemData(dwIndex, DATE_FORMAT1TYPE);

			dwIndex++;
			pComboBoxDateFormat->InsertString(dwIndex, _T("yyyy-mm-dd hhmmss"));
			pComboBoxDateFormat->SetItemData(dwIndex, DATE_FORMAT2TYPE);

			dwIndex++;
			pComboBoxDateFormat->InsertString(dwIndex, _T("yyyy-mm-dd"));
			pComboBoxDateFormat->SetItemData(dwIndex, DATE_FORMAT3TYPE);

			dwIndex++;
			pComboBoxDateFormat->InsertString(dwIndex, _T("yyyy年mm月dd日hh时mm分ss秒"));
			pComboBoxDateFormat->SetItemData(dwIndex, DATE_FORMAT4TYPE);

			dwIndex++;
			pComboBoxDateFormat->InsertString(dwIndex, _T("yy年m月d日h时m分s秒"));
			pComboBoxDateFormat->SetItemData(dwIndex, DATE_FORMAT5TYPE);
			
			dwIndex++;
			pComboBoxDateFormat->InsertString(dwIndex, _T("<SELF>-yyyymmddhhmmss"));
			pComboBoxDateFormat->SetItemData(dwIndex, DATE_FORMAT6TYPE);
 
			pComboBoxDateFormat->SetCurSel(0);
		}

		pComboBoxFileProPerty = (CComboBox*)GetDlgItem(IDC_COMBO_FILEPROPERTY);
		if (pComboBoxFileProPerty != NULL)
		{
			dwIndex = 0;
			pComboBoxFileProPerty->InsertString(dwIndex, _T("忽略不处理"));
			pComboBoxFileProPerty->SetItemData(dwIndex, DATE_EMPTYTYPE);

			dwIndex++;
			pComboBoxFileProPerty->InsertString(dwIndex, _T("文件创建日期"));
			pComboBoxFileProPerty->SetItemData(dwIndex, DATE_CREATETIME_TYPE);

			dwIndex++;
			pComboBoxFileProPerty->InsertString(dwIndex, _T("文件修改日期"));
			pComboBoxFileProPerty->SetItemData(dwIndex, DATE_MODIFYTIME_TYPE);

			dwIndex++;
			pComboBoxFileProPerty->InsertString(dwIndex, _T("文件访问日期"));
			pComboBoxFileProPerty->SetItemData(dwIndex, DATE_ACCESSTIME_TYPE);
			
			pComboBoxFileProPerty->SetCurSel(0);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameDate::InitWndInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameDate::UpdateWndCtrl()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgFileNameDate::UpdateWndInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CDlgFileNameDate::SetWndControlLayout()
{
	BOOL bRet = FALSE;

	do 
	{
		bRet = TRUE;
	} while (FALSE);
}

BOOL CDlgFileNameDate::DrawWndImage(CDC *pDC)
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

BOOL CDlgFileNameDate::SetConfigData()
{
	BOOL bRet = FALSE;

	DWORD dwStyle = 0;
	DWORD dwIndex = 0;
	DWORD dwValue = 0;

	CString strDateFormat;
	CString strFileProperty;

	CComboBox *pComboBoxDateFormat = NULL;
	CComboBox *pComboBoxFileProPerty = NULL;

	do 
	{
		pComboBoxDateFormat = (CComboBox *)GetDlgItem(IDC_COMBO_DATEFORMAT);
		if (pComboBoxDateFormat != NULL)
		{
			if (pComboBoxDateFormat->GetCurSel() < 0)
			{
				bRet = FALSE;
				break;
			}

			dwIndex = pComboBoxDateFormat->GetCurSel();
			pComboBoxDateFormat->GetLBText(dwIndex, strDateFormat);

			dwValue = pComboBoxDateFormat->GetItemData(dwIndex);
			m_stConfigData.stcDateFileName.emDateFormat = (UPDATE_FORMATTYPE)dwValue;
		}

		pComboBoxFileProPerty = (CComboBox*)GetDlgItem(IDC_COMBO_FILEPROPERTY);
		if (pComboBoxFileProPerty != NULL)
		{
			if (pComboBoxFileProPerty->GetCurSel() < 0)
			{
				bRet = FALSE;
				break;
			}

			dwIndex = pComboBoxFileProPerty->GetCurSel();
			pComboBoxFileProPerty->GetLBText(dwIndex, strFileProperty);

			dwValue = pComboBoxFileProPerty->GetItemData(dwIndex);
			m_stConfigData.stcDateFileName.emDateType = (UPDATE_DATETYPE)dwValue;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgFileNameDate::GetWndAddData(UPDATE_FILEDATA *pUpdateData)
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