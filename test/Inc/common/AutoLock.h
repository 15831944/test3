#ifndef __AUTOLOCK_H__
#define __AUTOLOCK_H__

class CLock
{
public:
	CLock()
	{
#ifdef _WIN32
		InitializeCriticalSection(&m_section);
#endif		
	}
	
	~CLock()
	{
#ifdef _WIN32
		DeleteCriticalSection(&m_section);
#endif		
	}
	
	void lock()
	{
#ifdef _WIN32
		EnterCriticalSection(&m_section);
#endif		
	}
	
	void unlock()
	{
#ifdef _WIN32
		LeaveCriticalSection(&m_section);
#endif		
	}
	
private:
#ifdef _WIN32
	CRITICAL_SECTION m_section;	
#endif
};

class CAutoLock
{
public:
	CAutoLock(CLock *pLock)
	{
		m_pLock = pLock;
		if (pLock != NULL)
		{
			pLock->lock();
		}		
	}
	
	~CAutoLock()
	{
		if (m_pLock != NULL)
		{
			m_pLock->unlock();
		}
	}
	
private:
	CLock*	m_pLock;
};

#endif