#include "stdafx.h"
#include "ImageOCR.h"

CImageOCR::CImageOCR()
{
	m_pKNN = NULL;

	m_pImageDataMat  = NULL;
	m_pImageClassMat = NULL;

	m_nTrainSize  = 0;
	m_nClassSize = 0;
	m_nSize = 0;
}


CImageOCR::~CImageOCR()
{}


void CImageOCR::InitImageOCR(LPCTSTR lpOCRPath)
{
	m_nTrainSize = 50;
	m_nClassSize = 10;
	m_nSize = 40;

	m_pImageDataMat  = cvCreateMat(m_nClassSize*m_nTrainSize, m_nSize*m_nSize, CV_32FC1);
	m_pImageClassMat = cvCreateMat(m_nClassSize*m_nTrainSize, 1, CV_32FC1);

	GetImageData(lpOCRPath);

	m_pKNN = new CvKNearest(m_pImageDataMat, m_pImageClassMat, 0, false, m_nK);
}


void CImageOCR::GetImageData(LPCTSTR lpOCRPath)
{
	CString strFileName;

	IplImage *pImageSrc = NULL;
	IplImage *pImageDst = NULL;

	CvMat RowMat,DataMat; 

	for (int i=0; i<m_nClassSize; i++)
	{
		for (int j=0; j<m_nTrainSize; j++)
		{
			strFileName.Format(_T("%s\\%d\\%03d.bmp"), lpOCRPath, i, j);
			

			pImageSrc = cvLoadImage(strFileName, CV_LOAD_IMAGE_GRAYSCALE);
			if (pImageSrc == NULL)
			{
				continue;
			}

			pImageDst = m_ImageProc.SetImageRect(pImageSrc, m_nSize, m_nSize);
			if (pImageDst == NULL)
			{
				continue;
			}


			cvGetRow(m_pImageClassMat, &RowMat, i*m_nTrainSize+j);									//取出矩阵中的一行
			cvSet(&RowMat, cvRealScalar(i));
			cvGetRow(m_pImageDataMat,  &RowMat, i*m_nTrainSize+j);

			IplImage *pImage = cvCreateImage(cvSize(m_nSize,m_nSize), IPL_DEPTH_32F, 1);
			if (pImage == NULL)
			{
				continue;
			}

			cvConvertScale(pImageDst, pImage, 0.0039215, 0);										//线性转换; pImage = pImageDst * 0.0039215 + 0;	
			cvGetSubRect(pImage, &DataMat, cvRect(0,0,m_nSize,m_nSize));

			CvMat RowHeader, *Row;
			Row = cvReshape( &DataMat, &RowHeader, 0, 1);											//修改矩阵的形状; 转为一维向量;
			cvCopy(Row, &RowMat, NULL);

		}
	}
}


int CImageOCR::GetOCRResult(IplImage *pSrc)
{
	int nResult = -1;

	if (pSrc == NULL)
	{
		return -1;
	}

	IplImage *pImageDst = cvCreateImage(cvSize(m_nSize, m_nSize), pSrc->depth, pSrc->nChannels);
	if (pImageDst == NULL)
	{
		return -1;
	}

	cvResize(pSrc, pImageDst, CV_INTER_LINEAR);

	IplImage *pImage = cvCreateImage(cvSize(m_nSize, m_nSize), IPL_DEPTH_32F, 1);
	if (pImage != NULL)
	{
		CvMat DataMat;
		cvConvertScale(pImageDst, pImage, 0.0039215, 0);
		cvGetSubRect(pImage, &DataMat, cvRect(0,0, m_nSize, m_nSize));

		CvMat *pNearest = cvCreateMat(1, m_nK, CV_32FC1);
		if (pNearest != NULL)
		{
			CvMat RowHeader, *Row;
			Row = cvReshape( &DataMat, &RowHeader, 0, 1);

			float result=m_pKNN->find_nearest(Row,m_nK,0,0,pNearest,0);
			nResult = (UINT)result;

			int accuracy=0;
			for(int i=0; i<m_nK; i++)
			{
				if( pNearest->data.fl[i] == result)
				{
					accuracy++;
				}
			}

			float pre=100*((float)accuracy/(float)m_nK);
		}
	}

	return nResult;
}