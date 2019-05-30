#include "stdafx.h"
#include "LeafThreadEx.h"

#include "LeafEvent.h"

#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")

using namespace Leaf::System;
Leaf::System::CThreadEx::CThreadEx()
{
	m_bExit = false;
	m_thread_t = NULL;
	
	
}

Leaf::System::CThreadEx::~CThreadEx()
{
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
		nRet = pthread_create(&m_thread_t, NULL, _ThreadEntry, this);
		if (nRet != 0)
		{
			bRet = false;
			break;
		}
		
		m_bExit = false;
		
		bRet = true;
	} while (false);
	
	return bRet;
}

bool Leaf::System::CThreadEx::CloseThread()
{
	bool bRet = false;
	
	int nRet = 0;
	
	do
	{
		nRet = pthread_join(m_thread_t, NULL);
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

