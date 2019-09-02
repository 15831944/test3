#include "stdafx.h"
#include "LeafEvent.h"

#ifdef POSIX
#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")
#endif

using namespace Leaf::System;
Leaf::System::CEvent::CEvent()
{
	m_pEvent = NULL;
}

Leaf::System::CEvent::~CEvent()
{
}

bool Leaf::System::CEvent::CreateEvent(bool bManualReset, bool bInitialState, const char *pEventName)
{
	bool bRet = false;

	v_int32_t nRet = 0;

	do 
	{
		if (m_pEvent == NULL)
		{
			m_pEvent = new EVENT;
			if (m_pEvent == NULL)
			{
				bRet = false;
				break;
			}
			memset(m_pEvent, 0x0, sizeof(EVENT));

			m_pEvent->bState = bInitialState;
			m_pEvent->bAutoReset = bManualReset;
			sprintf(m_pEvent->szEventName, "%s", pEventName);

#ifdef WIN32
#elif POSIX
			if (pthread_mutex_init((pthread_mutex_t*)&m_pEvent->m_ptMutex, NULL) != 0)
			{
				bRet = false;
				break;
			}

			if (pthread_condattr_init((pthread_condattr_t*)&m_pEvent->m_ptCattr) != 0)
			{
				bRet = false;
				break;
			}

			if (pthread_cond_init((pthread_cond_t*)&m_pEvent->m_ptCond, (pthread_condattr_t*)&m_pEvent->m_ptCattr) != 0)
			{
				bRet = false;
				break;
			}
#endif	
		}
		
		bRet = true;
	} while (false);

	if (!bRet)
	{
		CloseEvent();
	}

	return bRet;
}

void Leaf::System::CEvent::CloseEvent()
{
	bool bRet = false;

	do 
	{
		if (m_pEvent == NULL)
		{
			bRet = false;
			break;
		}

#ifdef WIN32
#elif POSIX
		pthread_condattr_destroy((pthread_condattr_t*)&m_pEvent->m_ptCattr);
		pthread_mutex_destroy((pthread_mutex_t*)&m_pEvent->m_ptMutex);
		pthread_cond_destroy((pthread_cond_t*)&m_pEvent->m_ptCond);
#endif
		delete m_pEvent;
		m_pEvent = NULL;

		bRet = true;
	} while (false);
}

#ifdef WIN32
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

v_uint32_t Leaf::System::CEvent::WaitForEvent(v_uint64_t uMilliseconds)
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
		if (m_pEvent == NULL)
		{
			bRet = false;
			break;
		}

#ifdef WIN32

#elif POSIX
		struct timeval stTime = {0};
		struct timespec abstime = {0};

		if (uMilliseconds == 0)
		{
			uRet = pthread_mutex_trylock((pthread_mutex_t*)&m_pEvent->m_ptMutex);
			if (uRet == EBUSY)
			{//WAIT_TIMEOUT
				bRet = false;
				break;
			}
		}
		else
		{
			uRet = pthread_mutex_lock((pthread_mutex_t*)&m_pEvent->m_ptMutex);
		}

		gettimeofday(&stTime, NULL);
		abstime.tv_sec = stTime.tv_sec + uMilliseconds/1000;
		abstime.tv_nsec = stTime.tv_usec*1000 + (uMilliseconds%1000)*1000000;

		if (abstime.tv_nsec >= 1000000000)
		{
			abstime.tv_nsec -= 1000000000;
			abstime.tv_sec++;
		}

		while (!m_pEvent->bState)
		{
			if (uMilliseconds != (v_uint64_t)-1)
			{
				if ((uRet = pthread_cond_timedwait((pthread_cond_t*)&m_pEvent->m_ptCond, (pthread_mutex_t*)&m_pEvent->m_ptMutex, &abstime)))
				{
					break;
				}
			}
			else
			{
				if ((uRet = pthread_cond_wait((pthread_cond_t*)&m_pEvent->m_ptCond, (pthread_mutex_t*)&m_pEvent->m_ptMutex)))
				{
					break;
				}
			}
		}

		if (uRet == 0 && m_pEvent->bAutoReset)
		{
			m_pEvent->bState = false;
		}

		pthread_mutex_unlock((pthread_mutex_t*)&m_pEvent->m_ptMutex);
#endif
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

#ifdef WIN32
#elif POSIX
#endif

		bRet = true;
	} while (false);

	return uRet;
}

v_uint32_t Leaf::System::CEvent::SetEvent()
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
		if (m_pEvent == NULL)
		{
			bRet = false;
			break;
		}

#ifdef WIN32
#elif POSIX
		pthread_mutex_lock((pthread_mutex_t*)&m_pEvent->m_ptMutex);

		m_pEvent->bState = true;
		if (!m_pEvent->bAutoReset)
		{//
			uRet = pthread_cond_broadcast((pthread_cond_t*)&m_pEvent->m_ptCond);	//pthread_cond_signal	//pthread_cond_broadcast
		}
		else
		{
			uRet = pthread_cond_signal((pthread_cond_t*)&m_pEvent->m_ptCond);
		}

		pthread_mutex_unlock((pthread_mutex_t*)&m_pEvent->m_ptMutex);
#endif
		bRet = true;
	} while (false);

	return uRet;
}

v_uint32_t Leaf::System::CEvent::ResetEvent()
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
		if (m_pEvent == NULL)
		{
			bRet = false;
			break;
		}
#ifdef WIN32
#elif POSIX

		pthread_mutex_lock((pthread_mutex_t*)&m_pEvent->m_ptMutex);

		m_pEvent->bState = false;

		pthread_mutex_unlock((pthread_mutex_t*)&m_pEvent->m_ptMutex);
#endif
		
		bRet = true;
	} while (false);

	return uRet;
}

v_uint32_t Leaf::System::CEvent::PulseEvent()
{
	bool bRet = false;

	v_uint32_t uRet = 0;

	do 
	{
#ifdef WIN32
#elif POSIX
		uRet = SetEvent();
		if (uRet != 0)
		{
			bRet = false;
			break;
		}

		uRet = ResetEvent();
		if (uRet != 0)
		{
			bRet = false;
			break;
		}
#endif
		bRet = true;
	} while (false);

	return uRet;
}