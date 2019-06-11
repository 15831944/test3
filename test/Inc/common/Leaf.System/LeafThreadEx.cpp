#include "stdafx.h"
#include "LeafThreadEx.h"

using namespace Leaf::System;
Leaf::System::CThreadEx::CThreadEx()
{
}

Leaf::System::CThreadEx::~CThreadEx()
{
}

STACKFLAG Leaf::System::CThreadEx::_ThreadEntry(LPVOID pParam)
{
	CThreadEx *pThreadEx = (CThreadEx*)pParam;
	if (pThreadEx == NULL)
	{
		pThreadEx->Run();
	}

	return STACKRETURN;
}

bool Leaf::System::CThreadEx::CreateThread(v_uint32_t uiCreateFlags, v_uint32_t uiStackSize, void *pSecurityAttrs)
{
#ifdef WIN32
	m_thread = (HANDLE)_beginthreadex(NULL, uiStackSize, _ThreadEntry, this, uiCreateFlags, &m_thread_t);
	if (m_thread == NULL || m_thread == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	m_suspend = (uiCreateFlags | CREATE_SUSPENDED) ? 1 : 0;

#elif POSIX
	if (pthread_create(&m_thread_t, NULL, _ThreadEntry, this) != 0)
	{
		return false;
	}

#endif

	return true;
}

bool Leaf::System::CThreadEx::CloseThread()
{
#ifdef WIN32

#elif POSIX
	if (pthread_join(m_thread_t, NULL) != 0)
	{
		return false;
	}
#endif
	
	return true;
}

bool Leaf::System::CThreadEx::IsRunning()
{
	return true;
}

v_uint32_t Leaf::System::CThreadEx::ResumeThread()
{
	bool bRet = false;
	v_uint32_t uiCount = 0;

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

		uiCount = ::ResumeThread(m_thread);
		if (uiCount != 0xFFFFFFFF)
		{
			InterlockedDecrement((LPLONG)&m_suspend);
		}

	#elif POSIX
	#endif

		bRet = true;
	} while (false);

	return uiCount;
}

v_uint32_t Leaf::System::CThreadEx::SuspendThread()
{
	return 0;
}

bool Leaf::System::CThreadEx::SetThreadPriority(v_int32_t uiPriority)
{
	return true;
}

v_int32_t Leaf::System::CThreadEx::GetThreadPriority()
{
	return 0;
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
