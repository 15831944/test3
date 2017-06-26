#include "stdafx.h"
#include "ImageProc.h"

void OnMouseEvent(int event, int x, int y, int flags, void *param)
{
	Mouse_Param *pParam = (Mouse_Param *)param;
	if (pParam == NULL)
	{
		return;
	}

	CImageProc *pImageProc = (CImageProc*)pParam->pParam;
	if (pImageProc != NULL)
	{
		pImageProc->MouseEvent(event, x, y, flags, pParam);
	}
}

CImageProc::CImageProc()
{
	m_bDrwaBox = FALSE;
}


CImageProc::~CImageProc()
{
}

//////////////////////////////////////////////////////////////////////////
//灰度处理
BOOL CImageProc::GreyProc(Gray_Param *pGray)		//IplImage *pSrc, IplImage **ppGrayImage
{
	if (pGray == NULL && pGray->plSrc == NULL)
	{
		return FALSE;
	}

	
	IplImage *pDst = cvCreateImage(cvGetSize(pGray->plSrc), IPL_DEPTH_8U, 1);
	if (pDst == NULL)
	{
		return FALSE;
	}

	cvCvtColor(pGray->plSrc, pDst, pGray->code);	//CV_BGR2GRAY	

	if (pGray->plDst)
	{
		cvReleaseImage((IplImage**)&pGray->plDst);
	}

	(pGray->plDst) = cvCloneImage(pDst);
	
	cvReleaseImage(&pDst);

	return TRUE;
}


//滤波处理
BOOL CImageProc::SmoothProc(Smooth_Param *pSmooth)	//IplImage *pSrc, UINT nFlags, IplImage **ppSmoothImage
{
	if (pSmooth == NULL && pSmooth->plSrc == NULL)
	{
		return FALSE;
	}

	IplImage *pDst = cvCreateImage(cvGetSize(pSmooth->plSrc), IPL_DEPTH_8U, 1);
	if (pDst == NULL)
	{
		return FALSE;
	}

	cvSmooth(pSmooth->plSrc, pDst, pSmooth->smoothtype, pSmooth->param1, pSmooth->param2, pSmooth->param3, pSmooth->param4);	//5, 5, 0, 0

	if (pSmooth->plDst)
	{
		cvReleaseImage((IplImage**)&pSmooth->plDst);
	}

	(pSmooth->plDst) = cvCloneImage(pDst);

	cvReleaseImage(&pDst);

	return TRUE;
}


//全局二值化处理
BOOL CImageProc::GlobalThresholdProc(GlobalThreshold_Param *pThreshold)
{
	if (pThreshold == NULL && pThreshold->plSrc == NULL)
	{
		return FALSE;
	}

	IplImage *pDst = cvCreateImage(cvGetSize(pThreshold->plSrc), IPL_DEPTH_8U, 1);
	if (pDst == NULL)
	{
		return FALSE;
	}

	cvThreshold(pThreshold->plSrc, pDst, pThreshold->threshold, pThreshold->max_value, pThreshold->threshold_type);

	if (pThreshold->plDst)
	{
		cvReleaseImage((IplImage**)&pThreshold->plDst);
	}

	(pThreshold->plDst) = cvCloneImage(pDst);

	cvReleaseImage(&pDst);

	return TRUE;
}


//局部二值化处理
BOOL CImageProc::LocalThresholdProc(LocalThreshold_Param *pThreshold)
{
	if (pThreshold == NULL && pThreshold->plSrc == NULL)
	{
		return FALSE;
	}

	IplImage *pDst = cvCreateImage(cvGetSize(pThreshold->plSrc), IPL_DEPTH_8U, 1);
	if (pDst == NULL)
	{
		return FALSE;
	}

	cvAdaptiveThreshold(pThreshold->plSrc, pDst, pThreshold->max_value, pThreshold->adaptive_method, pThreshold->threshold_type, pThreshold->block_size, pThreshold->param1);

	if (pThreshold->plDst)
	{
		cvReleaseImage((IplImage**)&pThreshold->plDst);
	}

	(pThreshold->plDst) = cvCloneImage(pDst);

	cvReleaseImage(&pDst);

	return TRUE;
}


//直方图均衡化
BOOL CImageProc::EqualizeHistProc(Equalize_Param *pEqualize)	//IplImage *pSrc, IplImage **ppEqualizeHist
{
	if (pEqualize == NULL && pEqualize->plSrc == NULL)
	{
		return FALSE;
	}


	IplImage *pDst = cvCreateImage(cvGetSize(pEqualize->plSrc), IPL_DEPTH_8U, 1);
	if (pDst == NULL)
	{
		return FALSE;
	}

	cvEqualizeHist(pEqualize->plSrc, pDst);

	if (pEqualize->plDst)
	{
		cvReleaseImage((IplImage**)&pEqualize->plDst);
	}

	(pEqualize->plDst) = cvCloneImage(pDst);

	cvReleaseImage(&pDst);

	return TRUE;
}


BOOL CImageProc::ErodeProc(Erode_Param *pErode)
{
	if (pErode == NULL && pErode->plSrc == NULL)
	{
		return FALSE;
	}

	IplImage *pDst = cvCreateImage(cvGetSize(pErode->plSrc), IPL_DEPTH_8U, 1);
	if (pDst == NULL)
	{
		return FALSE;
	}

	cvErode(pErode->plSrc, pDst, pErode->element, pErode->iterations);

	if (pErode->plDst)
	{
		cvReleaseImage((IplImage**)&pErode->plDst);
	}

	(pErode->plDst) = cvCloneImage(pDst);
	
	if (pErode->element != NULL)
	{
		cvReleaseStructuringElement(&pErode->element);
	}
	cvReleaseImage(&pDst);

	return TRUE;
}


BOOL CImageProc::DilateProc(Dilate_Param *pDilate)
{
	if (pDilate == NULL && pDilate->plSrc == NULL)
	{
		return FALSE;
	}

	IplImage *pDst = cvCreateImage(cvGetSize(pDilate->plSrc), IPL_DEPTH_8U, 1);
	if (pDst == NULL)
	{
		return FALSE;
	}

	cvDilate(pDilate->plSrc, pDst, pDilate->element, pDilate->iterations);

	if (pDilate->plDst)
	{
		cvReleaseImage((IplImage**)&pDilate->plDst);
	}

	(pDilate->plDst) = cvCloneImage(pDst);

	if (pDilate->element != NULL)
	{
		cvReleaseStructuringElement(&pDilate->element);
	}
	cvReleaseImage(&pDst);

	return TRUE;
}


//
BOOL CImageProc::GetVecImageProc(IplImage *pSrc,std::vector<IplImage*> &vecImage)
{
	if (pSrc == NULL)
	{
		return FALSE;
	}


	return TRUE;
}


//鼠标事件接口
BOOL CImageProc::SetMouseEvent(LPCTSTR lpWndName, void *pParam, CvRect *pImageRect, IplImage **ppDstImage)
{
	IplImage *pImage = (IplImage*)pParam;
	if (pImage == NULL)
	{
		return FALSE;
	}

	Mouse_Param  param;
	memset(&param, 0x0, sizeof(Mouse_Param));	
	strcpy(param.szWndName, lpWndName);
	param.plOrg  = pImage;
	param.plImg  = cvCloneImage(pImage);
	param.plTmp  = cvCloneImage(pImage);
	param.plDst  = NULL;
	param.pParam = this;

	cvNamedWindow(lpWndName, CV_WINDOW_AUTOSIZE);	
	cvShowImage(lpWndName, param.plImg);
	cvSetMouseCallback(lpWndName, OnMouseEvent, &param);

	cvWaitKey(0);

	if (param.plDst != NULL && ppDstImage != NULL)
	{
		(*ppDstImage) = param.plDst;
	}

	if (pImageRect != NULL)
	{
		memcpy(pImageRect, &param.lRect, sizeof(CvRect));
	}
	

	cvDestroyWindow(lpWndName);
	cvReleaseImage(&param.plImg);
	cvReleaseImage(&param.plTmp);

	param.plImg = NULL;
	param.plTmp = NULL;
	return TRUE;
}


void CImageProc::MouseEvent(int event, int x, int y, int flags, void* param)
{
	Mouse_Param *pParam = (Mouse_Param *)param;
	if (pParam == NULL)
	{
		return;
	}

	static CvPoint pre_pt = {-1,-1};
	static CvPoint cur_pt = {-1,-1}; 

	CvFont font;  
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA); 

	char szTemp[16] = {0};

	switch(event)
	{
	case CV_EVENT_LBUTTONDOWN:
		{
			cvCopy(pParam->plOrg,pParam->plImg);
			pre_pt = cvPoint(x,y);
			sprintf(szTemp, _T("%d,%d"), x, y);

			cvPutText(pParam->plImg,szTemp, pre_pt, &font, cvScalar(0,0, 0, 255));
			cvCircle(pParam->plImg , pre_pt, 3,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );
			cvShowImage(pParam->szWndName, pParam->plImg);  
			cvCopy(pParam->plImg,pParam->plTmp); 

			m_bDrwaBox = TRUE;
		}
		break;

	case CV_EVENT_MOUSEMOVE:
		{
			if (m_bDrwaBox)
			{
				cvCopy(pParam->plTmp,pParam->plImg);
				cur_pt = cvPoint(x,y);
				sprintf(szTemp, _T("%d,%d"), x, y);

				cvPutText(pParam->plImg,szTemp, cur_pt, &font, cvScalar(0,0, 0, 255)); 
				if (flags & CV_EVENT_FLAG_LBUTTON)
				{
					cvRectangle(pParam->plImg, pre_pt, cur_pt, cvScalar(0,255,0,0), 1, 8, 0);
				}
				cvShowImage(pParam->szWndName, pParam->plImg);
			}
		}
		break;

	case CV_EVENT_LBUTTONUP:
		{
			if (m_bDrwaBox)
			{
				m_bDrwaBox = FALSE;

				cvCopy(pParam->plTmp,pParam->plImg);
				cur_pt = cvPoint(x,y); 
				sprintf(szTemp, _T("%d,%d"), x, y);

				cvPutText(pParam->plImg,szTemp, cur_pt, &font, cvScalar(0,0, 0, 255)); 
				cvCircle( pParam->plImg, cur_pt, 3,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );
				cvRectangle(pParam->plImg, pre_pt, cur_pt, cvScalar(0,255,0,0), 1, 8, 0);

				cvShowImage(pParam->szWndName, pParam->plImg);  
				cvCopy(pParam->plImg,pParam->plTmp); 

				CvRect rect;  
				int m = 0, n = 0;
				if(pre_pt.x<cur_pt.x && pre_pt.y<cur_pt.y)  
				{// 1 
					if (cur_pt.x > pParam->plImg->width || cur_pt.y > pParam->plImg->height)
					{
						m = abs(pParam->plImg->width  - pre_pt.x);
						n = abs(pParam->plImg->height - pre_pt.y);
					}
					else
					{
						m = abs(pre_pt.x-cur_pt.x);
						n = abs(pre_pt.y-cur_pt.y);
					}

					rect=cvRect(pre_pt.x,pre_pt.y,m,n); 
				}  
				else if(pre_pt.x>cur_pt.x && pre_pt.y<cur_pt.y)  
				{//  2
					if (cur_pt.x < 0 || cur_pt.y > pParam->plImg->height)
					{
						m = abs(pre_pt.x);
						n = abs(pParam->plImg->height - pre_pt.y);
					}
					else
					{
						m = abs(pre_pt.x-cur_pt.x);
						n = abs(pre_pt.y-cur_pt.y);
					}

					rect=cvRect(cur_pt.x,pre_pt.y,m,n); 
				}  
				else if(pre_pt.x>cur_pt.x && pre_pt.y>cur_pt.y)  
				{//  3
					if (cur_pt.x < 0 || cur_pt.y < 0)
					{
						m = abs(pre_pt.x);
						n = abs(pre_pt.y);
					}
					else
					{
						m = abs(pre_pt.x-cur_pt.x);
						n = abs(pre_pt.y-cur_pt.y);
					}	

					rect=cvRect(cur_pt.x,cur_pt.y,m,n);
				}  
				else if(pre_pt.x<cur_pt.x && pre_pt.y>cur_pt.y)  
				{//  4`
					if (cur_pt.x > pParam->plImg->width || cur_pt.y < 0)
					{
						m = abs(pParam->plImg->width  - cur_pt.x);
						n = abs(pre_pt.y);
					}
					else
					{
						m = abs(pre_pt.x-cur_pt.x);
						n = abs(pre_pt.y-cur_pt.y);
					}  

					rect=cvRect(pre_pt.x,cur_pt.y,m,n);
				} 


				if(m==0 || n==0)  
				{
					return;
				}

				IplImage *lpDst=cvCreateImage(cvSize(m,n),pParam->plOrg->depth,pParam->plOrg->nChannels);
				cvSetImageROI(pParam->plOrg,rect);  
				cvCopy(pParam->plOrg,lpDst);  
				cvResetImageROI(pParam->plOrg); 

				pParam->plDst = cvCloneImage(lpDst);
				memcpy(&pParam->lRect, &rect, sizeof(CvRect));

				cvReleaseImage(&lpDst);
			}
		}
		break;
	}
}


//获取图像X坐标
void CImageProc::GetImageX(IplImage *pSrc, int *pMin, int *pMax)
{
	int nWidth    = pSrc->width;
	int nHeight	  = pSrc->height;
	int nMinCount = nWidth*255/5;
	CvScalar maxVal = cvRealScalar(nWidth * 255);
	CvScalar Val=cvRealScalar(0);
	
	bool bRet = false;
	int  nCount = 0;
	CvMat ImageMat;
	
	for (int i=0; i<nWidth; i++)
	{
		cvGetCol(pSrc, &ImageMat, i);
		Val = cvSum(&ImageMat);
		
		if (Val.val[0] < maxVal.val[0])
		{
			nCount = Val.val[0];
			if ((nCount>nMinCount) && (nCount<nWidth*255))
			{
				*pMax = i;
				if (!bRet)
				{
					*pMin = i;
					bRet = true;
				}
			}
		}
	}
}


//获取图像Y坐标
void CImageProc::GetImageY(IplImage *pSrc, int *pMin, int *pMax)
{
	int nWidth    = pSrc->width;
	int nHeight   = pSrc->height;
	int nMinCount = nWidth*255/5;
	CvScalar maxVal = cvRealScalar(nHeight * 255);
	CvScalar Val=cvRealScalar(0);

	bool bRet = false;
	int  nCount = 0;
	CvMat ImageMat;

	for (int i=0; i<nHeight; i++)
	{
		cvGetRow(pSrc, &ImageMat, i);
		Val = cvSum(&ImageMat);

		if (Val.val[0] < maxVal.val[0])
		{
			nCount = Val.val[0];
			if ((nCount>nMinCount) && (nCount<nHeight*255))
			{
				*pMax = i;
				if (!bRet)
				{
					*pMin = i;
					bRet = true;
				}
			}
		}
	}
}


//获取图像最小范围
CvRect CImageProc::GetImageRect(IplImage *pSrc)
{
	CvRect ImageRect;

	int nMin_X, nMax_X, nMin_Y, nMax_Y;
	nMin_X = nMax_X = nMin_Y = nMax_Y = 0;

	GetImageX(pSrc, &nMin_X, &nMax_X);
	GetImageY(pSrc, &nMin_Y, &nMax_Y);

	ImageRect = cvRect(nMin_X, nMin_Y, (nMax_X-nMin_X), (nMax_Y-nMin_Y));

	return ImageRect;
}


//设置图像指定大小范围
IplImage* CImageProc::SetImageRect(IplImage *pSrc, int nWidth, int nHeight)
{	
	if (pSrc == NULL)
	{
		return NULL;
	}

	IplImage *pImageResult = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	if (pImageResult == NULL)
	{
		return NULL;
	}

	CvMat  ImageMat;																					
	CvRect rcBox = GetImageRect(pSrc);																	
	if (rcBox.width <= 0 || rcBox.height <= 0)
	{
		cvReleaseImage(&pImageResult);
		return NULL;
	}

		
	cvGetSubRect(pSrc, &ImageMat, cvRect(rcBox.x, rcBox.y, rcBox.width, rcBox.height));					//
	int nSize = (rcBox.width>rcBox.height) ? (rcBox.width) : (rcBox.height);

	IplImage *pImageBox = cvCreateImage(cvSize(nSize, nSize), IPL_DEPTH_8U, 1);
	if (pImageBox == NULL)
	{
		return NULL;
	}

 	cvSet(pImageBox, CV_RGB(255,255,255), NULL);
 	int nX = (int)floor((float)(nSize-rcBox.width)/2.0f);
 	int nY = (int)floor((float)(nSize-rcBox.height)/2.0f);


	CvMat ImageData;																					//
	cvGetSubRect(pImageBox, &ImageData, cvRect(nX, nY, rcBox.width, rcBox.height));						//
	cvCopy(&ImageMat, &ImageData, NULL);																//			
	
	cvResize(pImageBox, pImageResult, CV_INTER_NN);														//		
	cvReleaseImage(&pImageBox);

	return pImageResult;
}

//alpha: 1.0 - 3.0
//beta:  0 - 100
/*
cv::Mat CImageProc::Adjust1Image(Mat &src, double alpha, int beta)
{
	cv::Mat matImage;

	for (int y=0; y<src.rows; y++)
	{
		for (int x=0; x<src.cols; x++)
		{
			for (int c=0; c<3; c++)
			{
				matImage.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(alpha*(src.at<Vec3b>(y,x)[c]) + beta);
			}
		}
	}

	imshow("1", matImage);
	return matImage;
}
*/

//AdjustImage(pImage, 0, 0.5, 0.5, 1, 1);
/*
IplImage* CImageProc::Adjust2Image(IplImage *pSrc, double xVal1, double xVal2, double yVal1, double yVal2, double gamma)
{
	if (pSrc == NULL)
	{
		return NULL;
	}

	double val;

	double xMinVal = xVal1*255;
	double xMaxVal = xVal2*255;

	double yMinVal = yVal1*255;
	double yMaxVal = yVal2*255;

	double xVal = xMaxVal - xMinVal;
	double yVal = yMaxVal - yMinVal;

	IplImage *pDstImage = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);
	
	for (int y=0; y<pSrc->height; y++)
	{
		for (int x=0; y<pSrc->width; x++)
		{
			val = ((uchar*)(pSrc->imageData +  pSrc->widthStep*y))[x];
			val = pow((val - xMinVal)/xVal, gamma)*yVal + yMinVal;

			if (val > 255)
				val = 255;

			if (val < 0)
				val = 0;

			((uchar*)(pDstImage->imageData + pDstImage->widthStep*y))[x] = (uchar)val;
		}
	}

	return pDstImage;
}
*/

/*
	angle:		旋转的度数;
	clockwise:	true,顺时针; false,逆时针;
*/
IplImage* CImageProc::RotateImage(IplImage* pSrc, int angle, bool clockwise)
{
	angle = abs(angle) % 180;  
	if (angle > 90)  
	{  
		angle = 90 - (angle % 90);  
	}  
	IplImage* dst = NULL;  
	int width =  
		(double)(pSrc->height * sin(angle * CV_PI / 180.0)) +  
		(double)(pSrc->width * cos(angle * CV_PI / 180.0 )) + 1;  
	int height =  
		(double)(pSrc->height * cos(angle * CV_PI / 180.0)) +  
		(double)(pSrc->width * sin(angle * CV_PI / 180.0 )) + 1;  
	int tempLength = sqrt(double(pSrc->width * pSrc->width + pSrc->height * pSrc->height)) + 10;  
	int tempX = (tempLength + 1) / 2 - pSrc->width / 2;  
	int tempY = (tempLength + 1) / 2 - pSrc->height / 2;  
	int flag = -1;  

	dst = cvCreateImage(cvSize(width, height), pSrc->depth, pSrc->nChannels);  
	cvZero(dst);  
	IplImage* temp = cvCreateImage(cvSize(tempLength, tempLength), pSrc->depth, pSrc->nChannels);  
	cvZero(temp);  

	cvSetImageROI(temp, cvRect(tempX, tempY, pSrc->width, pSrc->height));  
	cvCopy(pSrc, temp, NULL);  
	cvResetImageROI(temp);  

	if (clockwise)  
		flag = 1;  

	float m[6];  
	int w = temp->width;  
	int h = temp->height;  
	m[0] = (float) cos(flag * angle * CV_PI / 180.);  
	m[1] = (float) sin(flag * angle * CV_PI / 180.);  
	m[3] = -m[1];  
	m[4] = m[0];  
	// 将旋转中心移至图像中间  
	m[2] = w * 0.5f;  
	m[5] = h * 0.5f;  
	//  
	CvMat M = cvMat(2, 3, CV_32F, m);  
	cvGetQuadrangleSubPix(temp, dst, &M);  
	cvReleaseImage(&temp);  
	return dst; 
}

BOOL CImageProc::IsDeflection(const Mat& in, const double angle, double& slope)
{
	int nRows = in.rows;
    int nCols = in.cols;

    assert(in.channels() == 1);

    int comp_index[3];
    int len[3];

    comp_index[0] = nRows / 4;
    comp_index[1] = nRows / 4 * 2;
    comp_index[2] = nRows / 4 * 3;

    const uchar* p;
    
    for (int i = 0; i < 3; i++)
    {
        int index = comp_index[i];
        p = in.ptr<uchar>(index);

        int j = 0;
        int value = 0;
        while (0 == value && j < nCols)
            value = int(p[j++]);

        len[i] = j;
    }

    //cout << "len[0]:" << len[0] << endl;
    //cout << "len[1]:" << len[1] << endl;
    //cout << "len[2]:" << len[2] << endl;
    
    double maxlen = max(len[2], len[0]);
    double minlen = min(len[2], len[0]);
    double difflen = abs(len[2] - len[0]);
    //cout << "nCols:" << nCols << endl;

    double PI = 3.14159265;
    double g = tan(angle * PI / 180.0);

    if (maxlen - len[1] > nCols/32 || len[1] - minlen > nCols/32 ) {
        // 如果斜率为正，则底部在下，反之在上
        double slope_can_1 = double(len[2] - len[0]) / double(comp_index[1]);
        double slope_can_2 = double(len[1] - len[0]) / double(comp_index[0]);
        double slope_can_3 = double(len[2] - len[1]) / double(comp_index[0]);

        /*cout << "slope_can_1:" << slope_can_1 << endl;
        cout << "slope_can_2:" << slope_can_2 << endl;
        cout << "slope_can_3:" << slope_can_3 << endl;*/
 
        slope = abs(slope_can_1 - g) <= abs(slope_can_2 - g) ? slope_can_1 : slope_can_2;

        /*slope = max(  double(len[2] - len[0]) / double(comp_index[1]),
            double(len[1] - len[0]) / double(comp_index[0]));*/
        
        //cout << "slope:" << slope << endl;
        return true;
    }
    else {
        slope = 0;
    }

    return false;
}

int CImageProc::GetContourProc(IplImage *pSrc)
{
	IplImage *pImage = cvCloneImage(pSrc);

	CvMemStorage *storage  = NULL;
	CvSeq *contours = NULL;
	CvSeq *cont     = NULL; 
	CvSeq *mcont    = NULL;

	int nWidth  = pImage->width;
	int nHeight = pImage->height;

	cvMorphologyEx( pImage, pImage, NULL, NULL, CV_MOP_OPEN,  1 );
	cvMorphologyEx( pImage, pImage, NULL, NULL, CV_MOP_CLOSE, 1 );

	if (storage == NULL)
	{
		storage = cvCreateMemStorage(0);
	}
	else
	{
		cvClearMemStorage(storage);
	}

	CvContourScanner scanner = cvStartFindContours(pImage, storage, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	while((cont = cvFindNextContour(scanner)) != NULL)
	{
		double len = cvContourPerimeter(cont);
		double q   = (nHeight+nWidth)/8;

		if (len < q)
		{
			cvSubstituteContour(scanner, NULL);
		}
		else
		{
			mcont = cvApproxPoly(cont, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 2, 0);
			cvSubstituteContour(scanner, mcont);
		}
	}

	contours = cvEndFindContours(&scanner);

	return 0;
}