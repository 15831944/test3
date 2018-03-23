#ifndef __UKEYFIND_THREAD_H__
#define __UKEYFIND_THREAD_H__

class CUKeyFindThread
{
public:
	CUKeyFindThread();
	~CUKeyFindThread();
	
public:
	BOOL					CreateFindProc();
	BOOL					CloseFindProc();
	
	void					SetThreadProcTime(DWORD dwProcTime, DWORD dwCloseTime);
	
protected:
	static DWORD WINAPI		UKeyFindThreadProc(LPVOID lpParam);	
	void					UKeyFindInfo();	

	BOOL					GetUKeySlotInfo();
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;

private:
	BOOL					m_bExit;
	
	DWORD					m_dwThreadID;
	DWORD					m_dwProcTimeOver;
	DWORD					m_dwCloseTimeOver;
};

#endif