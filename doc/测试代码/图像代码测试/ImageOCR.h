#ifndef __IMAGE_OCR_H__
#define __IMAGE_OCR_H__

#include <opencv2/opencv.hpp>
#include "ImageProc.h"

using namespace cv;
using namespace std;
class CImageOCR
{
public:
	CImageOCR();
	~CImageOCR();

public:
	void								InitImageOCR(LPCTSTR lpOCRPath);
	void								GetImageData(LPCTSTR lpOCRPath);
	int									GetOCRResult(IplImage *pSrc);


private:
	CvKNearest							*m_pKNN;	
	CImageProc							m_ImageProc;

	CvMat								*m_pImageDataMat;
	CvMat								*m_pImageClassMat;

	int									m_nTrainSize;
	int									m_nClassSize;
	int									m_nSize;

	static const int					m_nK = 10;
};

#endif