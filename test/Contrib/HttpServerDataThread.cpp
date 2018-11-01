#include "stdafx.h"
#include "HttpServerDataThread.h"

CHttpServerDataThread::CHttpServerDataThread()
{
	m_bExit = FALSE;
	
	m_dwThreadId = 0;
	m_hThread = NULL;
	
	m_dwProcTimeOver = 200;
	m_dwCloseTimeOver = INFINITE;
	
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CHttpServerDataThread::~CHttpServerDataThread()
{
	CloseHttpserverDataProc();
	if(m_hStartEvent != NULL)
	{
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
	}

	if(m_hEndEvent != NULL)
	{
		CloseHandle(m_hEndEvent);
		m_hEndEvent = NULL;
	}
}

CHttpServerDataThread& CHttpServerDataThread::Instance()
{
	static CHttpServerDataThread inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
DWORD CHttpServerDataThread::HttpServerDataThreadProc(LPVOID lpParam)
{
	CHttpServerDataThread *pHttpServerData = (CHttpServerDataThread*)lpParam;
	if (pHttpServerData != NULL)
	{
		pHttpServerData->HttpServerData();
	}
	
	return 0;
}

void CHttpServerDataThread::HttpServerData()
{
	BOOL bRet = FALSE;
	
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
		
		bRet = TRUE;
	} while(FALSE);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CHttpServerDataThread::CreateHttpServerDataProc()
{
	BOOL bRet = FALSE;
	
	do
	{
		if (m_hThread != NULL && m_hThread != INVALID_HANDLE_VALUE)
		{
			bRet = FALSE;
			break;
		}
		
		if (WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
		{
			m_hThread = CreateThread(NULL, 0, HttpServerDataThreadProc, (LPVOID)this, 0, &m_dwThreadId);
			if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
			{
				bRet = FALSE;
				break;
			}

			SetEvent(m_hStartEvent);
			ResetEvent(m_hEndEvent);

			m_bExit = FALSE;
			bRet = TRUE;
		}
		
	} while(FALSE);
	
	return bRet;
}

void CHttpServerDataThread::CloseHttpserverDataProc()
{
	BOOL bRet = FALSE;
	
	do
	{
		if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			bRet = FALSE;
			break;
		}
		
		m_bExit = TRUE;
		WaitForSingleObject(m_hEndEvent, m_dwCloseTimeOver);
		
		if (m_hThread != NULL)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		m_bExit = FALSE;
		ResetEvent(m_hStartEvent);
		
		bRet = TRUE;
	} while (FALSE);
}