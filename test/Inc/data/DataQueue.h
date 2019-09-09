#ifndef __DATA_QUEUE_H__
#define __DATA_QUEUE_H__

#include <queue>
#include "./AutoLock.h"

template <typename T>
class CDataQueue
{
public:
	CDataQueue();
	~CDataQueue();
	
public:
	void	EmptyBuffer();
	size_t	GetSize();
	
	size_t	PushBuffer(T* pBuffer);
	bool	PullBuffer(T* &pBuffer);
	
private:
	CLock	m_lock;
	std::queue<T *>	m_dataBuffer;
};

template <typename T>
CDataQueue<T>::CDataQueue()
{
}

template <typename T>
CDataQueue<T>::~CDataQueue()
{
}

template <typename T>
void CDataQueue<T>::EmptyBuffer()
{
	CAutoLock autolock(&m_lock);
	while (!m_dataBuffer.empty())
	{
		m_dataBuffer.pop();
	}
}

template <typename T>
size_t CDataQueue<T>::GetSize()
{
	CAutoLock autolock(&m_lock);
	const size_t szSize = m_dataBuffer.size();
	return szSize;
}

template <typename T>
size_t CDataQueue<T>::PushBuffer(T* pBuffer)
{
	CAutoLock autolock(&m_lock);
	if (pBuffer != NULL)
	{
		m_dataBuffer.push(pBuffer)
	}

	const size_t szSize = m_dataBuffer.size();
	return szSize;
}

template <typename T>
bool CDataQueue<T>::PullBuffer(T* &pBuffer)
{
	CAutoLock autolock(&m_lock);
	if (m_dataBuffer.size() != 0)
	{
		pBuffer = m_dataBuffer.front();
		m_dataBuffer.pop_front();
	}

	return true;
}

#endif