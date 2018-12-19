#include "stdafx.h"
#include "camera_video_thread.h"

CCameraVideoThread::CCameraVideoThread()
{
	m_bExit = false;

	m_dwThreadId = 0;
	m_hThread = NULL;

	m_dwProcTimeOver = 200;
	m_dwCloseTimeOver = INFINITE;

	m_pFaceDetect = NULL;
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CCameraVideoThread::~CCameraVideoThread()
{
}

DWORD CCameraVideoThread::CameraVideoProc(LPVOID lpParam)
{
	CCameraVideoThread *pCameraVideo = (CCameraVideoThread *)lpParam;
	if (pCameraVideo != NULL)
	{
		pCameraVideo->CameraVideoInfo();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void CCameraVideoThread::CameraVideoInfo()
{
	bool bRet = false;
	
	do
	{
		while (WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) != WAIT_OBJECT_0)
		{
			if (!m_bExit)
			{
			}
			else
			{
				SetEvent(m_hEndEvent);
			}
		}
		
		bRet = true;
	} while (false);
}

//////////////////////////////////////////////////////////////////////////
//
bool CCameraVideoThread::CreateCameraVideoProc(CVideoControlInterface *pFaceDetect)
{
	bool bRet = false;
	
	do
	{
		if (pFaceDetect == NULL)
		{
			bRet = false;
			break;
		}

		if (m_hThread != NULL && m_hThread != INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}
		
		if (WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
		{
			m_pFaceDetect = pFaceDetect;

			m_hThread = CreateThread(NULL, 0, CameraVideoProc, (LPVOID)this, CREATE_SUSPENDED, &m_dwThreadId);
			if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
			{
				bRet = false;
				break;
			}
			
			SetEvent(m_hStartEvent);
			ResetEvent(m_hEndEvent);
			
			m_bExit = false;
			bRet = TRUE;
		}
		
	} while (false);
	
	return bRet;
}

void CCameraVideoThread::CloseCameraVideoProc()
{
	bool bRet = false;

	do 
	{
		if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		m_bExit = true;
		WaitForSingleObject(m_hEndEvent, m_dwCloseTimeOver);

		if (m_hThread != NULL)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		m_bExit = false;
		ResetEvent(m_hStartEvent);

		bRet = true;
	} while (false);
}

void CCameraVideoThread::SetProcState(BOOL bState)
{
	bool bRet = false;
	DWORD dwRet = 0;

	do
	{
		if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		if (!bState)
		{
			dwRet = SuspendThread(m_hThread);
			if (dwRet == (DWORD)-1)
			{
				bRet = false;
				break;
			}
		}
		else
		{
			dwRet = ResumeThread(m_hThread);
			if (dwRet != 0 && dwRet != 1)
			{
				bRet = false;
				break;
			}
		}

		bRet = true;
	} while (false);
}