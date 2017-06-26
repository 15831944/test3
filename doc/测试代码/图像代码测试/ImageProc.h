#ifndef __IMAGE_PROC_H__
#define __IMAGE_PROC_H__

#include <opencv2/opencv.hpp>

#define  WND_NAME_LEN_MAX				256
typedef struct tagMouseParam
{
	char		szWndName[WND_NAME_LEN_MAX];
	IplImage	*plOrg;
	IplImage	*plImg;
	IplImage	*plTmp;
	IplImage	*plDst;
	CvRect		lRect;
	void		*pParam;
}Mouse_Param;


typedef struct tagGrayParam
{
	CvArr		*plSrc;
	CvArr		*plDst;
	int			code;
}Gray_Param;


typedef struct tagSmoothParam
{
	CvArr		*plSrc;	
	CvArr		*plDst;
	int			smoothtype;
	int			param1;
	int			param2;
	double		param3;
	double		param4;
}Smooth_Param;


typedef struct tagGlobalThresholdParam
{
	CvArr		*plSrc;
	CvArr		*plDst;
	double		threshold;
	double		max_value;
	int			threshold_type;	
}GlobalThreshold_Param;


typedef struct tagLocalThresholdParam
{
	CvArr		*plSrc;	
	CvArr		*plDst;
	double		max_value;
	int			adaptive_method; 
	int			threshold_type;
	int			block_size;
	double		param1;
}LocalThreshold_Param;


typedef struct tagEqualizeParam
{
	CvArr		*plSrc;	
	CvArr		*plDst;
}Equalize_Param;


typedef struct tagErodeParam
{
	CvArr		*plSrc;
	CvArr		*plDst;
	IplConvKernel *element;
	int			iterations;
}Erode_Param;


typedef struct tagDilateParam
{
	CvArr		*plSrc;
	CvArr		*plDst;
	IplConvKernel *element;
	int			iterations;
}Dilate_Param;
//////////////////////////////////////////////////////////////////////////
//
using namespace cv;
using namespace std;
class CImageProc
{
public:
	CImageProc();
	~CImageProc();

	BOOL								SetMouseEvent(LPCTSTR lpWndName, void *pParam, CvRect *pImageRect, IplImage **ppDstImage);
	void								MouseEvent(int event, int x, int y, int flags, void* param);

public:
	BOOL								GreyProc(Gray_Param		*pGray);
	BOOL								SmoothProc(Smooth_Param *pSmooth);

	BOOL								GlobalThresholdProc(GlobalThreshold_Param *pThreshold);
	BOOL								LocalThresholdProc(LocalThreshold_Param   *pThreshold);
	
	BOOL								EqualizeHistProc(Equalize_Param *pEqualize);
	BOOL								ErodeProc(Erode_Param   *pErode);
	BOOL								DilateProc(Dilate_Param *pDilate);
		
	BOOL								IsDeflection(const Mat& in, const double angle, double& slope);
public:
	CvRect								GetImageRect(IplImage *pSrc);
	IplImage*							SetImageRect(IplImage *pSrc, int nWidth, int nHeight);

	cv::Mat								Adjust1Image(Mat &src, double alpha, int beta);
	IplImage*							Adjust2Image(IplImage *pSrc, double xVal1, double xVal2, double yVal1, double yVal2, double gamma);
	IplImage*							RotateImage(IplImage *pSrc, int angle, bool clockwise);
	BOOL								GetVecImageProc(IplImage *pSrc,std::vector<IplImage*> &vecImage);

	int									GetContourProc(IplImage *pSrc);

protected:
	void								GetImageX(IplImage *pSrc, int *pMin, int *pMax);
	void								GetImageY(IplImage *pSrc, int *pMin, int *pMax);

private:
	CvRect								m_rcBox;
	BOOL								m_bDrwaBox;
};
 
#endif