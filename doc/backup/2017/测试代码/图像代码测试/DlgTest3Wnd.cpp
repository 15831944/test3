#include "stdafx.h"
#include "../test.h"
#include "DlgTest3Wnd.h"

#include "../inc/GlobalInfo.h"
#include "../inc/cvthin.h"

enum { AFX_MENU1 = 0, AFX_MENU2, AFX_MENU3 };

IMPLEMENT_DYNAMIC(CDlgTest3Wnd, CDialog)
CDlgTest3Wnd::CDlgTest3Wnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest3Wnd::IDD, pParent)
{
	m_strDefaultPath = _T("");
	m_strAppPath = _T("");

	m_hOcrEngine.Init(NULL, "eng");
}

CDlgTest3Wnd::~CDlgTest3Wnd()
{
	m_hOcrEngine.Close();
}

void CDlgTest3Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ENUMDEVICE, m_hEnumDeviceList);
	DDX_Control(pDX, IDC_STATIC_VIDEO,	m_hVideoWnd);
}

BEGIN_MESSAGE_MAP(CDlgTest3Wnd, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ENUMDEVICE,	OnCbnSelchangeComboEnumdevice)
	
	ON_BN_CLICKED(IDC_BUTTON1,				OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2,				OnBnClickedButton2)

	ON_COMMAND(ID_MENU_CAPTURE,				OnMenuCapture)

	ON_COMMAND(ID_MENU_GLOBALTHRESHOLD,		OnMenuGlobalthreshold)
	ON_COMMAND(ID_MENU_LOCALTHRESHOLD,		OnMenuLocalthreshold)

	ON_COMMAND(ID_MENU_GRABCUT_TEST,		OnMenuGrabcutTest)
	ON_COMMAND(ID_MENU_CONTOUR_TEST,		OnMenuContourTest)
	
	ON_COMMAND(ID_MENU_KNN_OCR,				OnMenuKnnOcr)
	ON_COMMAND(ID_MENU_TESSERACT_OCR,		OnMenuTesseractOcr)
	ON_COMMAND(ID_MENU_BARCODE_DEMO,		OnMenuBarcodeDemo)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest3Wnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}


	TCHAR lpszDesktopPath[_MAX_PATH] ={0};
	if (::SHGetSpecialFolderPath(this->GetSafeHwnd(), lpszDesktopPath, CSIDL_DESKTOP, NULL))
	{
		m_strDefaultPath = lpszDesktopPath;
	}
	else
	{
		m_strDefaultPath = _T("C:\\");
	}

	m_strAppPath = pGlobal->GetAppPath();
	m_hEnumDeviceList.EnableWindow(FALSE);

	return TRUE;  
}

void CDlgTest3Wnd::OnCbnSelchangeComboEnumdevice()
{
	USES_CONVERSION;
	if (m_hEnumDeviceList.GetCount() <= 0)
	{
		return;
	}

	HRESULT hr;
	CString strText;

	int nIndex = m_hEnumDeviceList.GetCurSel();
	m_hEnumDeviceList.GetLBText(nIndex, strText);

	if (!strText.IsEmpty())
	{
		m_hcapWndVideo.DestroyGraphBuilder();

		hr = m_hcapWndVideo.InitCaptureGraphBuilder();
		if (hr != NOERROR)
		{
			return;
		}

		m_hEnumDeviceList.SetCurSel(nIndex);
		hr = m_hcapWndVideo.PreviewVideo(nIndex, A2W(strText.GetBuffer(0)), m_vecDeviceList, m_hVideoWnd.GetSafeHwnd());	
		if (hr != NOERROR)
		{
			return;
		}		
	}
}

//摄像头控制
void CDlgTest3Wnd::OnBnClickedButton1()
{
	USES_CONVERSION;
	m_hEnumDeviceList.ResetContent();
	
	HRESULT hr;
	static bool bStatus = false;
	if (!bStatus)
	{
		m_vecDeviceList.clear();
		hr = m_hcapWndVideo.EnumDevices(m_vecDeviceList);
		if (hr == S_OK)
		{
			int nSize = m_vecDeviceList.size();
			if (nSize > 0)
			{
				m_hEnumDeviceList.EnableWindow(TRUE);

				for (int i=0; i<nSize; i++)
				{
					m_hEnumDeviceList.AddString(W2A(m_vecDeviceList[i].FriendlyName));
				}

				m_hEnumDeviceList.SetCurSel(0);
				OnCbnSelchangeComboEnumdevice();
			}
		}
		bStatus = true;
	}
	else
	{
		hr = m_hcapWndVideo.DestroyGraphBuilder();
		if (hr != NOERROR)
		{
			return;
		}

		m_hEnumDeviceList.EnableWindow(FALSE);
		m_hVideoWnd.Invalidate();
		bStatus = false;
	}
}

BOOL CDlgTest3Wnd::PopupMenu(UINT nMenuID, UINT nSubMenuPos, CRect *prcRect, CWnd *pParentWnd)
{
	BOOL bRet = FALSE;

	CMenu menu;
	if (!menu.LoadMenu(nMenuID))
	{
		return FALSE;
	}

	CMenu* pSub = menu.GetSubMenu(nSubMenuPos);
	if (pSub == NULL)
	{
		return FALSE;
	}

/*
	{
		CCmdUI state;
		state.m_pMenu = pSub;
		state.m_pParentMenu = pSub;
		state.m_nIndexMax = pSub->GetMenuItemCount();

		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++) 
		{
			state.m_nID = pSub->GetMenuItemID(state.m_nIndex);
			if (state.m_nID == 0) 
			{
				continue; 
			}

			if (state.m_nID == (UINT)-1)
			{
				CMenu* pSub=pSub->GetSubMenu(state.m_nIndex);
				if(pSub)
				{
					CmdRouteMenu(pParentWnd,pSub);
				}
			}
			else
			{
				state.m_pSubMenu = NULL;
				state.DoUpdate(pParentWnd, FALSE);
			}
		}
	}
*/
	POINT pt;
	if (prcRect == NULL)
	{
		::GetCursorPos(&pt);
	}
	else
	{
		pt.x = prcRect->right;
		pt.y = prcRect->top;
	}
	
	if (pParentWnd != NULL)
	{
		pParentWnd->SetForegroundWindow();
	}

	pSub->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, pParentWnd, NULL);
	if (pParentWnd != NULL)
	{
		pParentWnd->PostMessage(WM_NULL, 0, 0);
	}

	return TRUE;
}

void CDlgTest3Wnd::OnBnClickedButton2()
{
	CRect rect;
	GetDlgItem(IDC_BUTTON2)->GetWindowRect(&rect);
	PopupMenu(IDR_MENU1, AFX_MENU1, &rect, this);
}

void CDlgTest3Wnd::OnMenuCapture()
{
	CFileDialog dlg(FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "位图文件(*.bmp)|*.bmp|All Files(*.*)|*.*||", this);
	dlg.m_ofn.lpstrInitialDir = m_strDefaultPath;
	if (dlg.DoModal() == IDOK)
	{
		m_hcapWndVideo.CaptureImages(dlg.GetPathName());
	}
}

void CDlgTest3Wnd::OnMenuGlobalthreshold()
{
	IplImage *pImage       = NULL;
	IplImage *pScrImage    = NULL;
	IplImage *pGrayImage   = NULL;
	IplImage *pSmoothImage = NULL;
	IplImage *pHistImage   = NULL;
	IplImage *pBinaryImage = NULL;
 
	CImageProc ImageProc;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*||", this);
	if (dlg.DoModal() == IDOK)
	{
		pImage = cvLoadImage(dlg.GetPathName());
		if (pImage == NULL)
		{
			goto part1;
		}

		ImageProc.SetMouseEvent("TEST", pImage, NULL, &pScrImage);
		if (pScrImage == NULL)
		{
			goto part1;
		}

		Gray_Param gray;
		gray.plSrc = pScrImage;
		gray.plDst = NULL;
		gray.code  = CV_BGR2GRAY;
		if (!ImageProc.GreyProc(&gray))
		{
			goto part1;
		}
		else
		{
			pGrayImage = (IplImage*)gray.plDst;
		}

		Smooth_Param smooth;
		smooth.plSrc	  = pGrayImage;
		smooth.plDst	  = NULL;
		smooth.smoothtype = CV_GAUSSIAN;
		smooth.param1	  = 7;
		smooth.param2	  = 7;
		smooth.param3     = 0;
		smooth.param4     = 0;
		if (!ImageProc.SmoothProc(&smooth))
		{
			goto part1;
		}
		else
		{
			pSmoothImage = (IplImage*)smooth.plDst;
		}

/*
		Equalize_Param Hist;
		Hist.plSrc		  = pSmoothImage;
		Hist.plDst		  = NULL;
		if (!ImageProc.EqualizeHistProc(&Hist))
		{
			goto part1;
		}
		else
		{
			pHistImage = (IplImage *)Hist.plDst;
		}
*/

		GlobalThreshold_Param threshold;
		threshold.plSrc			= pSmoothImage;
		threshold.plDst			= NULL;
		threshold.threshold		= 0;
		threshold.max_value		= 255;
		threshold.threshold_type= CV_THRESH_OTSU | CV_THRESH_BINARY;		//CV_THRESH_BINARY; //CV_THRESH_BINARY_INV; //CV_THRESH_TRUNC; //CV_THRESH_TOZERO; //CV_THRESH_OTSU; //CV_THRESH_TOZERO_INV
		if (!ImageProc.GlobalThresholdProc(&threshold))
		{
			goto part1;
		}
		else
		{
			pBinaryImage = (IplImage*)threshold.plDst;
		}

		cvNamedWindow("2");
		cvShowImage("2", pBinaryImage);

#if 0
		if (pGrayImage != NULL)
		{
			time_t t=time(NULL);
			struct tm *p = localtime(&t);

			CString strFileName;
			strFileName.Format("%s\\%04d%02d%02d%02d%02d%02d.jpg", m_strDefaultPath,1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
			cvSaveImage(strFileName, pGrayImage);
		}
#endif
	}

part1:
	cvWaitKey(0);
	cvDestroyAllWindows();

	if (pBinaryImage != NULL)
	{
		cvReleaseImage(&pBinaryImage);
		pBinaryImage = NULL;
	}
	
	if (pHistImage != NULL)
	{
		cvReleaseImage(&pHistImage);
		pHistImage = NULL;
	}
	
	if (pSmoothImage != NULL)
	{
		cvReleaseImage(&pSmoothImage);
		pSmoothImage = NULL;
	}
	
	if (pGrayImage != NULL)
	{
		cvReleaseImage(&pGrayImage);
		pGrayImage = NULL;
	}
	
	if (pScrImage != NULL)
	{
		cvReleaseImage(&pScrImage);
		pScrImage = NULL;
	}
	
	if (pImage != NULL)
	{
		cvReleaseImage(&pImage);
		pImage = NULL;
	}

}

void CDlgTest3Wnd::OnMenuLocalthreshold()
{
	IplImage *pImage       = NULL;
	IplImage *pScrImage	   = NULL;
	IplImage *pGrayImage   = NULL;
	IplImage *pSmoothImage = NULL;
	IplImage *pHistImage   = NULL;
	IplImage *pBinaryImage = NULL;

	CImageProc ImageProc;	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*||", this);
	if (dlg.DoModal() == IDOK)
	{
		pImage = cvLoadImage(dlg.GetPathName());
		if (pImage == NULL)
		{
			goto part1;
		}

		ImageProc.SetMouseEvent("TEST", pImage, NULL, &pScrImage);
		if (pScrImage == NULL)
 		{
 			goto part1;
 		}

		Gray_Param gray;
		gray.plSrc = pScrImage;
		gray.plDst = NULL;
		gray.code  = CV_BGR2GRAY;
		if (!ImageProc.GreyProc(&gray))
		{
			goto part1;
		}
		else
		{
			pGrayImage = (IplImage*)gray.plDst;
		}

		Smooth_Param smooth;
		smooth.plSrc	  = pGrayImage;
		smooth.plDst	  = NULL;
		smooth.smoothtype = CV_GAUSSIAN;	//CV_GAUSSIAN
		smooth.param1	  = 5;
		smooth.param2	  = 5;
		smooth.param3     = 0;
		smooth.param4     = 0;
		if (!ImageProc.SmoothProc(&smooth))
		{
			goto part1;
		}
		else
		{
			pSmoothImage = (IplImage *)smooth.plDst;
		}

/*
 		Equalize_Param Hist;
		Hist.plSrc		  = pSmoothImage;
		Hist.plDst		  = NULL;
		if (!ImageProc.EqualizeHistProc(&Hist))
		{
			goto part1;
		}
		else
		{
			pHistImage = (IplImage *)Hist.plDst;
		}
*/
		LocalThreshold_Param threshold;
		threshold.plSrc			  = pSmoothImage;
		threshold.plDst			  = NULL;
		threshold.max_value		  = 255;
		threshold.adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;	//CV_ADAPTIVE_THRESH_MEAN_C; //CV_ADAPTIVE_THRESH_GAUSSIAN_C
		threshold.threshold_type  = CV_THRESH_BINARY;
		threshold.block_size	  = 71;
		threshold.param1		  = 15;
		if (!ImageProc.LocalThresholdProc(&threshold))
		{
			goto part1;
		}
		else
		{
			pBinaryImage = (IplImage*)threshold.plDst;
		}


		cvShowImage("1", pBinaryImage);

#if 0
		if (pBinaryImage != NULL)
		{
			time_t t=time(NULL);
			struct tm *p = localtime(&t);

			CString strFileName;
			strFileName.Format("%s\\%04d%02d%02d%02d%02d%02d.jpg", m_strDefaultPath,1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
			cvSaveImage(strFileName, pBinaryImage);
		}
#endif
	}


part1:
	cvWaitKey(0);
	cvDestroyAllWindows();

	if (pBinaryImage != NULL)
	{
		cvReleaseImage(&pBinaryImage);
		pBinaryImage = NULL;
	}
	
	if (pSmoothImage != NULL)
	{
		cvReleaseImage(&pSmoothImage);
		pSmoothImage = NULL;
	}
	
	if (pGrayImage != NULL)
	{
		cvReleaseImage(&pGrayImage);
		pGrayImage = NULL;
	}
	
	if (pScrImage != NULL)
	{
		cvReleaseImage(&pScrImage);
		pScrImage = NULL;
	}
	
	if (pImage != NULL)
	{
		cvReleaseImage(&pImage);
		pImage = NULL;
	}
}

void CDlgTest3Wnd::OnMenuGrabcutTest()
{	
}

void CDlgTest3Wnd::OnMenuContourTest()
{
	int nRet = 0;
	IplImage *pImage       = NULL;
	IplImage *pScrImage    = NULL;
	IplImage *p = NULL;

	CvRect rcImage;
	CString s1;
	char strVal[MAX_PATH] = {0};

	CImageProc ImageProc;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*||", this);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	pImage = cvLoadImage(dlg.GetPathName());
	if (pImage == NULL)
	{
		return;
	}

	ImageProc.SetMouseEvent("TEST", pImage, &rcImage, NULL);
	nRet = digitRecog(pImage,rcImage, strVal, NO_FILM);

	s1.Format("%s", strVal);
	AfxMessageBox(s1);
	
part1:
	if (pImage)
	{
		cvReleaseImage(&pImage);
	}

	if (pScrImage)
	{
		cvReleaseImage(&pScrImage);
	}
}

void CDlgTest3Wnd::OnMenuKnnOcr()
{
	IplImage *pImage = NULL;
	IplImage *pGrayImage = NULL;
	IplImage *pScrImage  = NULL;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*||", this);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	pImage = cvLoadImage(dlg.GetPathName());
	if (pImage == NULL)
	{
		return;
	}

	cvNamedWindow("TEST", CV_WINDOW_AUTOSIZE);

	CvRect rcSel;
	rcSel.x = 190;
	rcSel.y = 102;
	rcSel.width  = 323;
	rcSel.height = 227;


	cvRectangle(pImage, cvPoint(rcSel.x, rcSel.y), cvPoint(rcSel.x+rcSel.width, rcSel.y+rcSel.height), CV_RGB(255,0,0), 1, 8, 0);
	cvShowImage("TEST", pImage);
}

void CDlgTest3Wnd::OnMenuTesseractOcr()
{
	CImageProc ImageProc;

	IplImage *pImage = NULL;
	IplImage *pScrImage = NULL;

	string strResult;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*||", this);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	pImage = cvLoadImage(dlg.GetPathName());	//CImageConvert::hBitmap2Ipl(hBitmap);
	if (pImage == NULL)
	{
		return;
	}

	ImageProc.SetMouseEvent("TEST", pImage, NULL, &pScrImage);
	if (pScrImage == NULL)
	{
		goto part1;
	}

	m_hOcrEngine.SetVariable("classify_bln_numeric_mode", "0123456789");
	m_hOcrEngine.SetImage((unsigned char*)pScrImage->imageData, pScrImage->width, pScrImage->height, pScrImage->nChannels, pScrImage->widthStep);

	strResult = m_hOcrEngine.GetText();
	AfxMessageBox(strResult.c_str());

	m_hOcrEngine.Clear();

part1:
	if (pScrImage != NULL)
	{
		cvReleaseImage(&pScrImage);
	}
	
	if (pImage != NULL)
	{
		cvReleaseImage(&pImage);
	}
}

void CDlgTest3Wnd::OnMenuBarcodeDemo()
{
	CImageProc ImageProc;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*||", this);	
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	int threshold = 0;
	IplImage *pImage = cvLoadImage(dlg.GetPathName());

	IplImage *pGrayImage   = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
	IplImage *pHistImage   = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
	IplImage *pThinImage   = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
	IplImage *pErodeImage  = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);

	cvSmooth(pImage, pImage, CV_MEDIAN, 3, 0);
	cvSmooth(pImage, pImage, CV_GAUSSIAN, 3, 0);
	
	cvCvtColor(pImage, pGrayImage, CV_BGR2GRAY);
	cvShowImage("1", pGrayImage);

	cvEqualizeHist(pGrayImage, pHistImage);
	cvShowImage("2", pHistImage);

//	threshold = Otsu1((unsigned char*)pGrayImage->imageData, pGrayImage->width, pGrayImage->height, pGrayImage->widthStep);
//	threshold = Otsu2(pGrayImage);
	threshold = 80;
	cvThreshold(pHistImage, pBinaryImage, threshold, 255, CV_THRESH_BINARY_INV);
	cvShowImage("3", pBinaryImage);

/*
	cvZero(pThinImage);
	cvThin(pBinaryImage, pThinImage, 8);


	CvSize size = cvGetSize(pThinImage);  
	for (int i=0; i<size.height; i++)
	{
		for (int j=0; j<size.width; j++)
		{
			if(CV_IMAGE_ELEM(pThinImage,uchar,i,j)==1)  
			{  
				CV_IMAGE_ELEM(pThinImage,uchar,i,j) = 0;  
			}  
			else  
			{  
				CV_IMAGE_ELEM(pThinImage,uchar,i,j) = 255;  
			} 
		}
	}
	cvShowImage("4", pThinImage);
*/

	cvErode(pBinaryImage, pErodeImage, NULL, 1);
	cvShowImage("5", pErodeImage);

	IplConvKernel *conv=cvCreateStructuringElementEx(3, 3, 0, 0, CV_SHAPE_RECT);
	cvDilate(pErodeImage,pErodeImage,conv,1);
	cvShowImage("6", pErodeImage);
	cvReleaseStructuringElement(&conv);


	for (int y=0; y<3; y++)
	{
		for (int x=0; x<pErodeImage->width; x++)
		{
			((uchar*)(pErodeImage->imageData + pErodeImage->widthStep*y))[x] = 0;
		}
	}

	IplImage *p = cvCloneImage(pErodeImage);
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contours = NULL;

	int nVal1 = pErodeImage->width/12;
	int nVal2 = pErodeImage->height/10;
	CString s1;
	CString s2;

	cvFindContours(p, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	for (; contours != 0; contours=contours->h_next)
	{
		CvRect rc = cvBoundingRect(contours, 0);
		s1.Format("%s : %d-%d-%d-%d\n", s1, rc.x, rc.y, rc.width, rc.height);

		if ((rc.width<15 || rc.width>35) || (rc.height<35 || rc.height>45))
		{
			s2.Format("%s : %d-%d-%d-%d\n", s2, rc.x, rc.y, rc.width, rc.height);
			cvDrawContours(pErodeImage, contours, CV_RGB(0,0,0), CV_RGB(0,0,0), -1, CV_FILLED, 8);
		}
	}


	cvShowImage("7", pErodeImage);
	cvSaveImage("D:\\1.bmp", pErodeImage);
	

	cvWaitKey(0);
	cvDestroyAllWindows();
}