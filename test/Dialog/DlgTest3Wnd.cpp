#include "stdafx.h"
#include "../test.h"
#include "DlgTest3Wnd.h"

#include "../inc/GlobalInfo.h"

IMPLEMENT_DYNAMIC(CDlgTest3Wnd, CDialog)
CDlgTest3Wnd::CDlgTest3Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest3Wnd::IDD, pParent)
{
	m_strDefaultPath = _T("");
	m_strAppPath = _T("");
}

CDlgTest3Wnd::~CDlgTest3Wnd()
{
}

void CDlgTest3Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTest3Wnd, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ENUMDEVICE,	OnCbnSelchangeComboEnumdevice)
	ON_BN_CLICKED(IDC_BTN_CAPTUREIMAGE,		OnBnClickedBtnCaptureimage)
	ON_BN_CLICKED(IDC_BTN_DRAWTEST,			OnBnClickedBtnDrawtest)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest3Wnd::OnInitDialog()
{
	USES_CONVERSION;
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

	CRect rect1;
	GetDlgItem(IDC_STATIC_VIDEO)->GetClientRect(&rect1);
	m_openglDrawVideo1.CreateGLContext(rect1, GetDlgItem(IDC_STATIC_VIDEO));

	return TRUE;  
}

void CDlgTest3Wnd::OnCbnSelchangeComboEnumdevice()
{
	
}

void CDlgTest3Wnd::OnBnClickedBtnCaptureimage()
{
}

void CDlgTest3Wnd::OnBnClickedBtnDrawtest()
{
	unsigned char* pBlockBuf = NULL;
	unsigned long BLOCK_SIZE_IN = 0;
	unsigned long BLOCK_SIZE_OUT = 0;
	
	unsigned long ulFileLen = 0;
	unsigned long uBlockLen = 0;
	unsigned long ulRemainLen = 0;
	
	unsigned long ulPixelWidth = 1280;
	unsigned long ulPixelHeight = 720;

	//
	FILE* file = fopen("720p.yuv", "rb+");
	if (file == NULL)
	{
		return;
	}

	//
	fseek(file, 0, SEEK_END);
	ulFileLen = ftell(file);
	if (ulFileLen == 0)
	{
		if(file)
		{
			fclose(file);
			file = NULL;
		}		
		return;
	}
	
	BLOCK_SIZE_IN = ulPixelWidth*ulPixelHeight*3/2;
	BLOCK_SIZE_OUT = BLOCK_SIZE_IN * 3;
	
	pBlockBuf = new unsigned char[BLOCK_SIZE_OUT];
	if(pBlockBuf == NULL)
	{
		if(file)
		{
			fclose(file);
			file = NULL;
		}	
		return;
	}
	memset(pBlockBuf, 0x0, BLOCK_SIZE_OUT);

	ulRemainLen = ulFileLen;
	fseek(file, 0, SEEK_SET);

	while(ulRemainLen > 0)
	{
		if(ulRemainLen >= BLOCK_SIZE_IN)
		{
			uBlockLen    = BLOCK_SIZE_IN;
			ulRemainLen -= BLOCK_SIZE_IN;
		}
		else
		{
			uBlockLen    = ulRemainLen;
			ulRemainLen -= ulRemainLen;
		}
		
		memset(pBlockBuf, 0x0, BLOCK_SIZE_OUT);
		
		fread(pBlockBuf, 1, uBlockLen, file);
		m_openglDrawVideo1.setframedata(pBlockBuf, uBlockLen, ulPixelWidth, ulPixelHeight);
	}

	if(file)
	{
		fclose(file);
		file = NULL;
	}
	
	if (pBlockBuf)
	{
		delete[] pBlockBuf;
		pBlockBuf = NULL;
	}
}
