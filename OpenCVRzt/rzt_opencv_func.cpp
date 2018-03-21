#include "io.h"
#include "stdio.h"
#include "rzt_opencv_func.h"

//////////////////////////////////////////////////////////////////////////
//
CvCapture* rzt_cvCaptureFromCAM(int index)
{
	return cvCaptureFromCAM(index);
}

IplImage* rzt_cvQueryFrame(CvCapture* capture)
{
	return cvQueryFrame(capture);
}

void rzt_cvReleaseCapture(CvCapture** capture)
{
	cvReleaseCapture(capture);
}

void rzt_cvNamedWindow(const char* name, int flags)
{
	cvNamedWindow(name, flags);
}

void rzt_cvDestroyWindow(const char* name)
{
	cvDestroyWindow(name);
}

void rzt_imwrite(char* image_filename, IplImage* frame)
{
	Mat Image = Mat(frame);
	imwrite(image_filename, Image); 
}

void rzt_cvSaveImage(const char* name, IplImage* frame)
{
	cvSaveImage(name, frame);
}

IplImage* rzt_cvCloneImage(IplImage* srcFrame)
{
	return cvCloneImage(srcFrame);
}

void rzt_cvReleaseImage(IplImage** srcFrame)
{
	return cvReleaseImage(srcFrame);
}

void rzt_cvShowImage(const char* name, IplImage* frame)
{
	cvShowImage(name,frame);  
}

int rzt_cvWaitKey(int delay)
{
	return cvWaitKey(delay);
}