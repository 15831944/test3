#ifndef __RZT_OPENCV_FUNC_H__
#define __RZT_OPENCV_FUNC_H__

#ifdef OPENCVRZT_EXPORTS
	#define RZT_OPENCV_API extern "C" _declspec(dllexport)
#else
	#define RZT_OPENCV_API extern "C" _declspec(dllimport)
#endif

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

RZT_OPENCV_API CvCapture* rzt_cvCaptureFromCAM(int index);
RZT_OPENCV_API IplImage* rzt_cvQueryFrame(CvCapture* capture);


RZT_OPENCV_API void rzt_cvReleaseCapture(CvCapture** capture);

RZT_OPENCV_API void rzt_cvNamedWindow(const char* name, int flags);
RZT_OPENCV_API void rzt_cvDestroyWindow(const char* name);

RZT_OPENCV_API void rzt_imwrite(char* image_filename, IplImage* frame);
RZT_OPENCV_API void	rzt_cvSaveImage(const char* name, IplImage* frame);

RZT_OPENCV_API IplImage* rzt_cvCloneImage(IplImage* srcFrame);
RZT_OPENCV_API void rzt_cvReleaseImage(IplImage** srcFrame);

RZT_OPENCV_API void rzt_cvShowImage(const char* name, IplImage* frame);
RZT_OPENCV_API int rzt_cvWaitKey(int delay);


#endif