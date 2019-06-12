#include "stdafx.h"
#include "LeafThreadEx.h"

using namespace Leaf::System;
Leaf::System::CThreadEx::CThreadEx()
{
	m_suspend = 0;
	m_thread_t = 0;

#ifdef WIN32
	m_thread = NULL;
#elif POSIX
#endif

	m_event.CreateEvent(false, false);
}

Leaf::System::CThreadEx::~CThreadEx()
{
	CloseThread();
}

STACKFLAG Leaf::System::CThreadEx::_ThreadEntry(LPVOID pParam)
{
	CThreadEx *pThreadEx = reinterpret_cast<CThreadEx*>(pParam);
	if (pThreadEx == NULL)
	{
		return STACKRETURN;
	}

	if (pThreadEx->InitInstance())
	{
		pThreadEx->Run();
	}
	
	pThreadEx->ExitInstance();
	return STACKRETURN;
}

bool Leaf::System::CThreadEx::CreateThread(THREAD_FLAGS_T emThreadFlag, v_uint32_t uiStackSize, void *pSecurityAttrs)
{
	bool bRet = false;

	void *pSecurity = NULL;
	v_uint32_t uiCreateFlags = 0;

	do 
	{
	#ifdef WIN32
		pSecurity = pSecurityAttrs;
		if (emThreadFlag == FLAGS_NORMAL)
		{
			uiCreateFlags = 0;
		}
		else if (emThreadFlag == FLAGS_SUSPENDED)
		{
			uiCreateFlags = CREATE_SUSPENDED;
		}

		m_thread = (HANDLE)_beginthreadex(pSecurity, uiStackSize, _ThreadEntry, this, uiCreateFlags, &m_thread_t);
		if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

	#elif POSIX
		pthread_attr_t attr;
		if (pSecurityAttrs == NULL)
		{
			pSecurity = pSecurityAttrs;
		}
		else
		{
			pthread_attr_init(&attr);

			pthread_attr_setschedpolicy(&attr, SCHED_RR);		//实时,轮转法
			if (pthread_attr_getschedparam(&attr, NULL) == 0)	//查询优先级
			{
				pthread_attr_setschedparam(&attr, NULL);		//设置优先级
			}

			pSecurity = &attr;
		}
		
		if (pthread_create(&m_thread_t, (pthread_attr_t *)pSecurity, _ThreadEntry, this) != 0)
		{
			bRet = false;
			break;
		}

	#endif

		m_suspend = (emThreadFlag == FLAGS_SUSPENDED) ? 1 : 0;

		bRet = true;
	} while (false);

	return bRet;
}

bool Leaf::System::CThreadEx::CloseThread()
{
	bool bRet = false;

	do 
	{
	#ifdef WIN32
		if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		while (m_suspend > 0)
		{
			if (ResumeThread() == 0xFFFFFFFF)
			{
				break;
			}
		}

		m_event.WaitForEvent(-1);

		CloseHandle(m_thread);
		m_thread = NULL;

	#elif POSIX
		if (pthread_join(m_thread_t, NULL) != 0)
		{
			bRet = false;
			break;
		}

	#endif

		bRet = true;
	} while (false);

	return bRet;
}

bool Leaf::System::CThreadEx::InitInstance()
{
	bool bRet = false;

	do 
	{
		bRet = true;
	} while (false);

	return bRet;
}

bool Leaf::System::CThreadEx::ExitInstance()
{
	bool bRet = false;

	do 
	{
	#ifdef WIN32
		if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		DECRC((LPLONG)&m_suspend);
		m_event.SetEvent();

	#elif POSIX
	#endif

		bRet = true;
	} while (false);

	return bRet;
}

v_uint32_t Leaf::System::CThreadEx::ResumeThread()
{
	bool bRet = false;
	v_uint32_t uCount = 0;

	do 
	{
	#ifdef WIN32
		if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		if (m_suspend == 0)
		{
			bRet = false;
			break;
		}

		uCount = ::ResumeThread(m_thread);
		if (uCount != 0xFFFFFFFF)
		{
			DECRC((LPLONG)&m_suspend);
		}

	#elif POSIX
	#endif

		bRet = true;
	} while (false);

	return uCount;
}

v_uint32_t Leaf::System::CThreadEx::SuspendThread()
{
	bool bRet = false;
	v_uint32_t uCount = 0;

	do 
	{
	#ifdef WIN32
		if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		if (m_suspend == 0)
		{
			bRet = false;
			break;
		}

		uCount = ::SuspendThread(m_thread);
		if (uCount != 0xFFFFFFFF)
		{
			INCRC((LPLONG)&m_suspend);
		}
	#elif POSIX
		INCRC((long)&m_suspend);

	#endif

		bRet = true;
	} while (false);

	return uCount;
}

bool Leaf::System::CThreadEx::SetThreadPriority(THREAD_PRIORITY_T emPriority)
{
	bool bRet = false;

	v_int32_t priority = 0;

	do 
	{
	#ifdef WIN32
		if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		if (emPriority == PRIORITY_IDLE)
		{
			priority = THREAD_PRIORITY_IDLE;
		}
		else if (emPriority == PRIORITY_NORMAL)
		{
			priority = THREAD_PRIORITY_NORMAL;
		}
		else if (emPriority == PRIORITY_ABOVE_NORMAL)
		{
			priority = THREAD_PRIORITY_ABOVE_NORMAL;
		}
		else if (emPriority == PRIORITY_HIGH)
		{
			priority = THREAD_PRIORITY_HIGHEST;
		}

		if (!::SetThreadPriority(m_thread, priority))
		{
			bRet = false;
			break;
		}

	#elif POSIX
	#endif

		bRet = true;
	} while (false);
	
	return bRet;
}

THREAD_PRIORITY_T Leaf::System::CThreadEx::GetThreadPriority()
{
	bool bRet = false;

	v_int32_t priority = 0;
	THREAD_PRIORITY_T emPriority = PRIORITY_NORMAL;

	do 
	{
	#ifdef WIN32
		if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		priority = ::GetThreadPriority(m_thread);
		if (priority == THREAD_PRIORITY_IDLE)
		{
			emPriority = PRIORITY_IDLE;
		}
		else if (priority == THREAD_PRIORITY_NORMAL)
		{
			emPriority = PRIORITY_NORMAL;
		}
		else if (priority == THREAD_PRIORITY_ABOVE_NORMAL)
		{
			emPriority = PRIORITY_ABOVE_NORMAL;
		}
		else if (priority == THREAD_PRIORITY_HIGHEST)
		{
			emPriority = PRIORITY_HIGH;
		}

	#elif POSIX
	#endif
	
		bRet = true;
	} while (false);

	return emPriority;
}

// 		pthread_attr_init(&pMutex_t->ptAttr);
// 		if (priority != PRIORITY_NORMAL)
// 		{
// 			if (priority != PRIORITY_IDLE)
// 			{
// 				pthread_attr_setschedpolicy(&pMutex_t->ptAttr, SCHED_RR);			//实时,轮转法
// 				if (pthread_attr_getschedparam(&pMutex_t->ptAttr, &param) == 0)		//查询优先级
// 				{
// 					if (priority == PRIORITY_HIGH)
// 					{
// 						param.sched_priority = 6;									//6:HIGH
// 					}
// 					else
// 					{
// 						param.sched_priority = 4;									//4:ABOVE_NORMAL
// 					}
// 
// 					pthread_attr_setschedparam(&pMutex_t->ptAttr, &param);			//设置优先级
// 				}
// 			}
// 		}
