#ifndef __THREAD_QUEUE_H__
#define __THREAD_QUEUE_H__

#include "PubClass.h"
#include <queue>

template<class T>
class thread_queue
{
public:
	thread_queue();
	~thread_queue();
	
public:
	BOOL			IsEmpty();
	int				GetSize();
	
	void			Empty();
	void			Push(const T& obj);
	void			Pop();

	T&				Front();
	T&				Back();

private:
	int				nSize;
	
	std::queue<T>	m_queue;
	HANDLE			m_mutex;
};

template <class T> thread_queue<T>::thread_queue()
{
	nSize = 0;
	m_mutex = CreateMutex(NULL, FALSE, NULL);
}	

template <class T> thread_queue<T>::~thread_queue()
{
	CloseHandle(m_mutex);
}

template <class T> BOOL thread_queue<T>::IsEmpty()
{
	if(0 == nSize)
	{
		return TRUE;
	}
	
	return FALSE;
}

template <class T> int thread_queue<T>::GetSize()
{
	return nSize;
}

template <class T> void thread_queue<T>::Empty()
{
	WaitForSingleObject(m_mutex, INFINITE);
	
	while(!m_queue.empty())
	{
		m_queue.pop();
	}
	
	nSize = 0;
	ReleaseMutex(m_mutex);
}

template <class T> void thread_queue<T>::Push(const T& obj)
{
	WaitForSingleObject(m_mutex, INFINITE);
	
	m_queue.push(obj);
	
	nSize = m_queue.size();
	ReleaseMutex(m_mutex);
}

template <class T> void thread_queue<T>::Pop()
{
	WaitForSingleObject(m_mutex, INFINITE);
	
	m_queue.pop();
	
	nSize = m_queue.size();
	ReleaseMutex(m_mutex);
}

template <class T> T& thread_queue<T>::Front()
{
	WaitForSingleObject(m_mutex, INFINITE);
	
    T& obj = m_queue.front();
	
    ReleaseMutex(m_mutex);
    return obj;
}

template <class T> T& thread_queue<T>::Back()
{
	WaitForSingleObject(m_mutex, INFINITE);
	 
    T& obj = m_queue.back();
	
    ReleaseMutex(m_mutex);
    return obj;
}

#endif