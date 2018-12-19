#include "stdafx.h"
#include "opencvVideoInterface.h"

#pragma comment(lib, "opencv_core249.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
COpencvVideoInterface::COpencvVideoInterface()
{
	m_ierror = 0;
	m_pCamera = NULL;
}

COpencvVideoInterface::~COpencvVideoInterface()
{
}

int COpencvVideoInterface::rzt_video_geterror()
{
	return m_ierror;
}

bool COpencvVideoInterface::rzt_video_enumCamera(std::vector<VIDEO_CAMERADATA> &vecCamera)
{
	return true;
}

bool COpencvVideoInterface::rzt_video_isCameraOpen()
{
	if (m_pCamera == NULL)
	{
		return false;
	}

	return true;
}

bool COpencvVideoInterface::rzt_video_openDevice(int iCameraId)
{
	m_pCamera = cvCaptureFromCAM(CV_CAP_DSHOW + iCameraId);
	if (m_pCamera == NULL)
	{
		return false;
	}

	return true;
}

bool COpencvVideoInterface::rzt_video_closeDevice()
{
	if (m_pCamera == NULL)
	{
		return true;
	}

	cvReleaseCapture(&m_pCamera);
	m_pCamera = NULL;

	return true;
}

bool COpencvVideoInterface::rzt_video_getVideoImage(FRAME_DATA_BUFFER *pFrameData)
{
	if (m_pCamera == NULL)
	{
		return false;
	}

	if (pFrameData == NULL)
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