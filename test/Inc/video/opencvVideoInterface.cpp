#include "stdafx.h"
#include "opencvVideoInterface.h"

#pragma comment(lib, "opencv_core249.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")

COpencvVideoInterface::COpencvVideoInterface()
{
	m_nError = 0;
	m_pCamera = NULL;
}

COpencvVideoInterface::~COpencvVideoInterface()
{
}

int COpencvVideoInterface::video_getError()
{
	return m_nError;
}

bool COpencvVideoInterface::video_enum(OUT std::vector<VIDEO_DEVDATA> &vecDevData)
{
	return true;
}

bool COpencvVideoInterface::video_isOpen()
{
	if (m_pCamera == NULL)
	{
		return false;
	}
	
	return true;
}

bool COpencvVideoInterface::video_open(IN int nDevId)
{
	m_pCamera = cvCaptureFromCAM(CV_CAP_DSHOW + nDevId);
	if (m_pCamera == NULL)
	{
		return false;
	}

	return true;
}

bool COpencvVideoInterface::video_close()
{
	if (m_pCamera == NULL)
	{
		return true;
	}

	cvReleaseCapture(&m_pCamera);
	m_pCamera = NULL;

	return true;
}

bool COpencvVideoInterface::video_getImage(OUT void *pImageData)
{
	if (m_pCamera == NULL)
	{
		return false;
	}

	if (pImageData == NULL)
	{
		return false;
	}
	
	IplImage* pVideoImage = cvQueryFrame(m_pCamera);
	if (pVideoImage == NULL)
	{
		return false;
	}
	
	return true;
}