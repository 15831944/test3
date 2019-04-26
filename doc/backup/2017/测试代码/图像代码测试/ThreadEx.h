#ifndef __THREADEX_H__
#define __THREADEX_H__

class CThreadEx  
{
public:
	CThreadEx();
	virtual ~CThreadEx();

public:	
	unsigned long 			ResumeThread();
	unsigned long 			SuspendThread();
	
	int    					GetThreadPriority();
	bool   					SetThreadPriority(int nPriority);
	
	bool   					CreateThread(unsigned int dwCreateFlags, unsigned int nStackSize,LPSECURITY_ATTRIBUTES lpSecurityAttrs=NULL);
	bool   					ThreadRunning() {return (m_iSuspendCount == 0);};

	virtual bool 			CloseThread();
	virtual bool 			InitInstance()	= 0;
	virtual int 			ExitInstance()	= 0;
	virtual int 			Run()			= 0;
	
public:
	HANDLE 					m_hThread;
	unsigned int 			m_nThreadID;
	
	int   					m_iSuspendCount;
	bool					m_bExit;
	HANDLE 					m_hExitEvent;	
};

#endif