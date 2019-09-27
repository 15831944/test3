#include "stdafx.h"
#include "DataBuffer.h"

CDataBuffer::CDataBuffer()
{
}

CDataBuffer::~CDataBuffer()
{
}

bool CDataBuffer::AllocDataBuffer(int nDataSize)
{
	if (nDataSize <= 0)
	{
		return false;
	}
	
	return true;
}

void CDataBuffer::FreeMemory()
{
}