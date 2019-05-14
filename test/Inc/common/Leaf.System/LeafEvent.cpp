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

		pEvent->bState = false;
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
		if (pEvent != NULL)
		{
			delete pEvent;
			pEvent = NULL;
		}
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

		bRet = true;
	} while (false);

	return bRet;
}

v_uint32_t Leaf::System::CEvent::WaitForEvent(HEVENT hEvent, v_uint64_t uMilliseconds)
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
		{
		}
		else
		{
		}

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
		if (hEvent == NULL)
		{
			bRet = false;
			break;
		}


		bRet = true;
	} while (false);

	return uRet;
}