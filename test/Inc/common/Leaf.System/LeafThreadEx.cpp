#include "stdafx.h"
#include "LeafThreadEx.h"

#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")

using namespace Leaf::System;
Leaf::System::CThreadEx::CThreadEx()
{
	m_bExit = false;
	m_thread_t = NULL;

	m_StartEvent.CreateEvent(false, false);
	m_EndEvent.CreateEvent(false, false);
}

Leaf::System::CThreadEx::~CThreadEx()
{
	m_StartEvent.CloseEvent();
	m_EndEvent.CloseEvent();
}

void* Leaf::System::CThreadEx::_ThreadEntry(LPVOID pParam)
{
	CThreadEx *pThreadEx = (CThreadEx*)pParam;
	if (pThreadEx != NULL)
	{
		pThreadEx->Run();
	}
	
	return NULL;
}

bool Leaf::System::CThreadEx::CreateThread()
{
	bool bRet = false;
	int nRet = 0;
	
	do
	{
		if (m_StartEvent.WaitForEvent(0) != 0)
		{
			m_StartEvent.SetEvent();
			m_EndEvent.ResetEvent();

			if (pthread_create((pthread_t*)&m_thread_t, NULL, _ThreadEntry, this) != 0)
			{
				bRet = false;
				break;
			}

			m_bExit = false;
			bRet = true;
		}
	} while (false);
	
	return bRet;
}

bool Leaf::System::CThreadEx::CloseThread()
{
	bool bRet = false;
	
	int nRet = 0;
	
	do
	{
		//nRet = pthread_join((pthread_t)*m_thread_t, NULL);
		if (nRet != 0)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while (false);
	
	return bRet;
}

bool Leaf::System::CThreadEx::IsRunning()
{
	return true;
}

unsigned int Leaf::System::CThreadEx::ResumeThread()
{
	return 0;
}

unsigned int Leaf::System::CThreadEx::SuspendThread()
{
	return 0;
}

bool Leaf::System::CThreadEx::SetThreadPriority(int nPriority)
{
	return true;
}

int Leaf::System::CThreadEx::GetThreadPriority()
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
