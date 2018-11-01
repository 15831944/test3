#ifndef __HTTP_SERVERDATA_THREAD_H__
#define __HTTP_SERVERDATA_THREAD_H__

class CHttpServerDataThread
{
public:
	CHttpServerDataThread();
	~CHttpServerDataThread();
	
public:
	static CHttpServerDataThread&		Instance();
	static DWORD WINAPI					HttpServerDataThreadProc(LPVOID lpParam);
	
public:
	BOOL								CreateHttpServerDataProc();
	void								CloseHttpserverDataProc();
	
protected:
	void								HttpServerData();
	
protected:
	HANDLE								m_hThread;
	
	HANDLE								m_hStartEvent;
	HANDLE								m_hEndEvent;
	
private:
	mutable volatile BOOL				m_bExit;
	
	DWORD								m_dwThreadId;
	
	DWORD								m_dwProcTimeOver;
	DWORD								m_dwCloseTimeOver;
};

#endif