#include "stdafx.h"
#include "ThreadEx.h"

#include <process.h>  

#ifdef _DEBUG
#define ASSERT_DLL(point)	ASSERT(point);
#else
#define ASSERT_DLL(point);
#endif 

#ifndef CheckPointer
#define CheckPointer(pointer,hr)		{if(pointer == NULL) return hr;};
#endif

unsigned WINAPI _ThreadEntry(LPVOID pParam)
{
	CThreadEx * pThread = (CThreadEx *)pParam;
	ASSERT_DLL(pThread != NULL);
	ASSERT_DLL(pThread->m_hThread != NULL);

	unsigned nResult = 0;
	
	if (pThread->InitInstance()) 
	{
		nResult = pThread->Run();
		while(!pThread->m_bExit)
		{
			
		}
		AfxMessageBox("1");
	}
	
	nResult = pThread->ExitInstance();
	pThread->m_hThread = NULL;
	
	_endthreadex(0);
	
	return nResult;
}


CThreadEx::CThreadEx()
{
	m_iSuspendCount = 0;
	m_hThread       = NULL;
	m_nThreadID     = 0;
	m_bExit		    = false;
	m_hExitEvent	= ::CreateEvent(NULL,FALSE,FALSE,NULL);

	CreateThread(CREATE_SUSPENDED,0,NULL);
}

CThreadEx::~CThreadEx()
{
	CloseThread();
}

//////////////////////////////////////////////////////////////////////////
//
bool CThreadEx::CreateThread(unsigned int dwCreateFlags, unsigned int nStackSize,LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
	ASSERT_DLL(m_hThread==NULL);

	m_hThread=(HANDLE)_beginthreadex(lpSecurityAttrs,nStackSize,_ThreadEntry,this,dwCreateFlags,&m_nThreadID);

	m_iSuspendCount=(dwCreateFlags | CREATE_SUSPENDED)?1:0;

	return (m_hThread)?true:false;
}


unsigned long CThreadEx::ResumeThread()
{
	unsigned long ulCount = 0;

	if (m_hThread != NULL)
	{
		if(m_iSuspendCount > 0)
		{
			ulCount = ::ResumeThread(m_hThread);
			if(ulCount != 0xFFFFFFFF)
			{
				InterlockedDecrement((LPLONG)&m_iSuspendCount);
			}
		}
	}

	return ulCount;
}

unsigned long CThreadEx::SuspendThread()
{
	unsigned long ulCount = 0;

	if (m_hThread != NULL)
	{
		if (m_iSuspendCount > 0)
		{
			ulCount = ::SuspendThread(m_hThread);
			if(ulCount != 0xFFFFFFFF)
			{
				InterlockedIncrement((LPLONG)&m_iSuspendCount);
			}
		}
	}
	
	return ulCount;
}


bool CThreadEx::SetThreadPriority(int nPriority)
{
	bool bRet = false;
	
	if (m_hThread != NULL)
	{
		if (::SetThreadPriority(m_hThread, nPriority))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}

int CThreadEx::GetThreadPriority()
{
	int nRet = 0;

	if (m_hThread != NULL)
	{
		nRet = ::GetThreadPriority(m_hThread);
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}

//////////////////////////////////////////////////////////////////////////
//
bool CThreadEx::InitInstance()
{
	ASSERT_DLL(this);

	bool bRet = false;

	if (m_hThread != NULL && !m_bExit)
	{
		if (m_hExitEvent != NULL)
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}



bool CThreadEx::CloseThread()
{
	bool bRet = false;
	
	if (m_hThread!=NULL)
	{
		InterlockedExchange((LPLONG)&m_bExit,true);
		
		while (m_iSuspendCount>0)
		{
			if (ResumeThread()==0xFFFFFFFF)
			{
				break;
			}
		}
		WaitForSingleObject(m_hExitEvent, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;

		bRet = true;
	}

	if (m_hExitEvent != NULL)
	{
		CloseHandle(m_hExitEvent);
		m_hExitEvent=NULL;
	}

	return bRet;
}

int  CThreadEx::ExitInstance()
{
	SetEvent(m_hExitEvent);
	return 0;
}

int  CThreadEx::Run()
{
	return 0;
}

