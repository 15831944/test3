#include "stdafx.h"
#include "rzt_server_control.h"

CRztServerControl::CRztServerControl()
{
	m_pFaceDetect = NULL;
	m_pVideoControl = NULL;
}

CRztServerControl::~CRztServerControl()
{
}

//////////////////////////////////////////////////////////////////////////
//




//////////////////////////////////////////////////////////////////////////
//
bool CRztServerControl::InitServer()
{
	bool bRet = false;

	do 
	{
		m_pFaceDetect = new CCzhtFaceInterface;
		if (m_pFaceDetect == NULL)
		{
			bRet = false;
			break;
		}

		m_pVideoControl = new COpencvVideoInterface;
		if (m_pVideoControl == NULL)
		{
			bRet = false;
			break;
		}

		if (!m_CameraVideoThread.CreateCameraVideoProc(m_pVideoControl))
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	if (!bRet)
	{
		if (m_pFaceDetect != NULL)
		{
			delete m_pFaceDetect;
			m_pFaceDetect = NULL;
		}

		if (m_pVideoControl != NULL)
		{
			delete m_pVideoControl;
			m_pVideoControl;
		}
	}

	return bRet;
}

void CRztServerControl::UnInitServer()
{
	
}

bool CRztServerControl::EnumCamera()
{
	bool bRet = false;

	std::vector<VIDEO_CAMERADATA> vecCamera;

	do 
	{
		if (!m_pVideoControl->rzt_video_enumCamera(vecCamera))
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while (false);

	return bRet;
}

bool CRztServerControl::OpenCamera(int iCameraId)
{
	bool bRet = false;

	do 
	{
		if (!m_pVideoControl->rzt_video_isCameraOpen())
		{
			if (!m_pVideoControl->rzt_video_openDevice(iCameraId))
			{
				bRet = false;
				break;
			}
		}
		
		bRet = true;
	} while (false);

	return bRet;
}

bool CRztServerControl::CloseCamera()
{
	bool bRet = false;

	do 
	{
		if (!m_pVideoControl->rzt_video_isCameraOpen())
		{
			if (!m_pVideoControl->rzt_video_closeDevice())
			{
				bRet = false;
				break;
			}
		}
		
		bRet = true;
	} while (false);

	return bRet;
}