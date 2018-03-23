#include "stdafx.h"
#include "UKeyFindThread.h"

#include "../Inc/UKeyVerify.h"

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2018.03.23 11:00
 */
/************************************************************************/

CUKeyFindThread::CUKeyFindThread()
{
	m_bExit = FALSE;
	
	m_dwThreadID = 0;
	m_dwProcTimeOver = 0;
	m_dwCloseTimeOver = 0;
	
	m_hThread = NULL;
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CUKeyFindThread::~CUKeyFindThread()
{
	if (m_hStartEvent != NULL)
	{
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
	}

	if (m_hEndEvent != NULL)
	{
		CloseHandle(m_hEndEvent);
		m_hEndEvent = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//
DWORD CUKeyFindThread::UKeyFindThreadProc(LPVOID lpParam)
{
	CUKeyFindThread* pFindProc = (CUKeyFindThread*)lpParam;
	if(pFindProc != NULL)
	{
		pFindProc->UKeyFindInfo();
	}
	
	return 0;
}

void CUKeyFindThread::UKeyFindInfo()
{
	BOOL bRet = FALSE;

	while(WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			if (!GetUKeySlotInfo())
			{
				continue;
			}
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	};
}

BOOL CUKeyFindThread::GetUKeySlotInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		if (!CUKeyVerify::Instance().GetSlotList())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CUKeyFindThread::CreateFindProc()
{
	BOOL bRet = FALSE;

	if (m_hThread != NULL)
	{
		return FALSE;
	}
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);

		m_hThread = CreateThread(NULL, 0, UKeyFindThreadProc, (LPVOID)this, 0, &m_dwThreadID);
		if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}
		
		bRet = TRUE;
		m_bExit = FALSE;
	}
	
	return bRet;
}

BOOL CUKeyFindThread::CloseFindProc()
{
	m_bExit = TRUE;

	if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	WaitForSingleObject(m_hEndEvent, m_dwCloseTimeOver);
	ResetEvent(m_hStartEvent);
	
	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return TRUE;
}

void CUKeyFindThread::SetThreadProcTime(DWORD dwProcTime, DWORD dwCloseTime)
{
	m_dwProcTimeOver = dwProcTime;
	m_dwCloseTimeOver = dwCloseTime;
}