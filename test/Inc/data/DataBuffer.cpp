#include "stdafx.h"
#include "DataBuffer.h"

CDataBuffer::CDataBuffer()
{
	m_nDataLen = -1;
	m_pDataBuffer = NULL;
}

CDataBuffer::~CDataBuffer()
{
	FreeMemory();
}

bool CDataBuffer::AllocDataBuffer(int nDataSize)
{
	if (nDataSize <= 0)
	{
		return false;
	}

	if (m_pDataBuffer != NULL)
	{
		delete[] m_pDataBuffer;
		m_pDataBuffer = NULL;
	}

	m_pDataBuffer = new char[nDataSize + 1];
	if (m_pDataBuffer == NULL)
	{
		return false;
	}
	memset(m_pDataBuffer, 0x0, nDataSize + 1);

	m_nDataLen = nDataSize;
	return true;
}

void CDataBuffer::FreeMemory()
{
	m_nDataLen = -1;
	if (m_pDataBuffer != NULL)
	{
		delete[] m_pDataBuffer;
		m_pDataBuffer = NULL;
	}
}