#include "stdafx.h"
#include "LeafEvent.h"

#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")

using namespace Leaf::System;
Leaf::System::CEvent::CEvent()
{
}

Leaf::System::CEvent::~CEvent()
{
}

HEVENT Leaf::System::CEvent::CreateEvent(bool bManualReset, bool bInitialState, const std::string &strEventName)
{
	bool bRet = false;

	v_int32_t nRet = 0;
	HEVENT pEvent = NULL;

	do 
	{
		pEvent = new EVENT;
		if (pEvent == NULL)
		{
			bRet = false;
			break;
		}

		pEvent->bState = bInitialState;
		pEvent->bAutoReset = bManualReset;

		if (pthread_mutex_init((pthread_mutex_t*)&pEvent->m_ptMutex, NULL) != 0)
		{
			bRet = false;
			break;
		}

		if (pthread_condattr_init((pthread_condattr_t*)&pEvent->m_ptCattr) != 0)
		{
			bRet = false;
			break;
		}

		if (pthread_cond_init((pthread_cond_t*)&pEvent->m_ptCond, (pthread_condattr_t*)&pEvent->m_ptCattr) != 0)
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	if (!bRet)
	{
		CloseEvent(pEvent);
	}

	return pEvent;
}

bool Leaf::System::CEvent::CloseEvent(HEVENT hEvent)
{
	bool bRet = false;

	do 
	{
		if (hEvent == NULL)
		{
			bRet = false;
			break;
		}

		pthread_condattr_destroy((pthread_condattr_t*)&hEvent->m_ptCattr);
		pthread_mutex_destroy((pthread_mutex_t*)&hEvent->m_ptMutex);
		pthread_cond_destroy((pthread_cond_t*)&hEvent->m_ptCond);

		delete hEvent;
		hEvent = NULL;

		bRet = true;
	} while (false);

	return bRet;
}

#ifdef _WIN_32_
static int gettimeofday(struct timeval *tv, struct timezone *tzp)
{
	time_t clock;
	struct tm tm;

	SYSTEMTIME wtm;
	GetLocalTime(&wtm);

	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;

	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;

	clock = mktime(&tm);
	tv->tv_sec = clock;
	tv->tv_usec = wtm.wMilliseconds * 1000;

	return (0);
}
#endif

v_uint32_t Leaf::System::CEvent::WaitForEvent(HEVENT hEvent, v_uint64_t uMilliseconds)
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	struct timeval stTime = {0};
	struct timespec abstime = {0};

	do 
	{
		if (hEvent == NULL)
		{
			bRet = false;
			break;
		}

		if (uMilliseconds == 0)
		{
			uRet = pthread_mutex_trylock((pthread_mutex_t*)&hEvent->m_ptMutex);
			if (uRet == EBUSY)
			{//WAIT_TIMEOUT
				bRet = false;
				break;
			}
		}
		else
		{
			uRet = pthread_mutex_lock((pthread_mutex_t*)&hEvent->m_ptMutex);
		}

		gettimeofday(&stTime, NULL);
		abstime.tv_sec = stTime.tv_sec + uMilliseconds/1000;
		abstime.tv_nsec = stTime.tv_usec*1000 + (uMilliseconds%1000)*1000000;
		if (abstime.tv_nsec >= 1000000000)
		{
			abstime.tv_nsec -= 1000000000;
			abstime.tv_sec++;
		}

		while (!hEvent->bState)
		{
			if (uMilliseconds != (v_uint64_t)-1)
			{
				if ((uRet = pthread_cond_timedwait((pthread_cond_t*)&hEvent->m_ptCond, (pthread_mutex_t*)&hEvent->m_ptMutex, &abstime)))
				{
					break;
				}
			}
			else
			{
				if ((uRet = pthread_cond_wait((pthread_cond_t*)&hEvent->m_ptCond, (pthread_mutex_t*)&hEvent->m_ptMutex)))
				{
					break;
				}
			}
		}

		if (uRet == 0 && hEvent->bAutoReset)
		{
			hEvent->bState = false;
		}

		pthread_mutex_unlock((pthread_mutex_t*)&hEvent->m_ptMutex);

		bRet = true;
	} while (false);

	return uRet;
}

v_uint32_t Leaf::System::CEvent::WaitForMultipleEvent(HEVENT *hEvents, v_uint32_t uCount, bool bWaitAll, v_uint64_t uMilliseconds)
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
		if (hEvents == NULL)
		{
			bRet = false;
			break;
		}


		bRet = true;
	} while (false);

	return uRet;
}

v_uint32_t Leaf::System::CEvent::SetEvent(HEVENT hEvent)
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
		if (hEvent == NULL)
		{
			bRet = false;
			break;
		}

		pthread_mutex_lock((pthread_mutex_t*)&hEvent->m_ptMutex);

		hEvent->bState = true;
		if (!hEvent->bAutoReset)
		{//
			uRet = pthread_cond_broadcast((pthread_cond_t*)&hEvent->m_ptCond);	//pthread_cond_signal	//pthread_cond_broadcast
		}
		else
		{
			uRet = pthread_cond_signal((pthread_cond_t*)&hEvent->m_ptCond);
		}

		pthread_mutex_unlock((pthread_mutex_t*)&hEvent->m_ptMutex);

		bRet = true;
	} while (false);

	return uRet;
}

v_uint32_t Leaf::System::CEvent::ResetEvent(HEVENT hEvent)
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
		if (hEvent == NULL)
		{
			bRet = false;
			break;
		}

		pthread_mutex_lock((pthread_mutex_t*)&hEvent->m_ptMutex);

		hEvent->bState = false;

		pthread_mutex_unlock((pthread_mutex_t*)&hEvent->m_ptMutex);

		bRet = true;
	} while (false);

	return uRet;
}

v_uint32_t Leaf::System::CEvent::PulseEvent(HEVENT hEvent)
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
		if (SetEvent(hEvent) != 0)
		{
			bRet = false;
			break;
		}

		if (ResetEvent(hEvent) != 0)
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	return uRet;
}