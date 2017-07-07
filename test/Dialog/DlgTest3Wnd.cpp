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
	ON_BN_CLICKED(IDC_BTN_TEST1,			OnBnClickedBtnTest1)
	ON_BN_CLICKED(IDC_BTN_TEST2,			OnBnClickedBtnTest2)
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
	
	return TRUE;  
}

void CDlgTest3Wnd::OnCbnSelchangeComboEnumdevice()
{
	
}

void CDlgTest3Wnd::OnBnClickedBtnCaptureimage()
{
	CRect rect1;
	GetDlgItem(IDC_STATIC_VIDEO)->GetClientRect(&rect1);

	m_openglDrawVideo.SetProcTimeOver(50);
	m_openglDrawVideo.CreateGLContext(FRAME_YUV420PDRAW_TYPE, rect1, GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd());
}

void CDlgTest3Wnd::OnBnClickedBtnTest1()
{
	m_openglDrawVideo.CloseGLProc();
	GetDlgItem(IDC_STATIC_VIDEO)->Invalidate(TRUE);
}

void CDlgTest3Wnd::OnBnClickedBtnTest2()
{
	unsigned char* pBlockBuf = NULL;
	unsigned long BLOCK_SIZE_IN = 0;
	unsigned long BLOCK_SIZE_OUT = 0;

	unsigned long ulFileLen = 0;
	unsigned long uBlockLen = 0;
	unsigned long ulRemainLen = 0;

	unsigned long ulPixelWidth = 320;
	unsigned long ulPixelHeight = 180;

	//
	FILE* file = fopen("test_yuv420p_320x180.yuv", "rb+");
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
		m_openglDrawVideo.SetFrameData(pBlockBuf, uBlockLen, ulPixelWidth, ulPixelHeight);

		Sleep(50);
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