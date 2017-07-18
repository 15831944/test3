#include "stdafx.h"
#include "SerialCom.h"

SerialCom::SerialCom()
{
	Init();
}

SerialCom::~SerialCom()
{
	Close();
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
//
void SerialCom::Init()
{
	::InitializeCriticalSection(&m_hSection);
	
	memset(m_szName, 0x0, sizeof(m_szName));
	memset(&m_dcb,	 0x0, sizeof(m_dcb));
	memset(&m_overlappedRead,  0x0, sizeof(m_overlappedRead));
	memset(&m_overlappedWrite, 0x0, sizeof(m_overlappedWrite));
	
	m_hComm			= INVALID_HANDLE_VALUE;
	m_dwPort		= (DWORD)-1;
	
	m_overlappedRead.hEvent  = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_overlappedWrite.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	m_TimeOuts.ReadIntervalTimeout			= 100;	//配置超时结构 字符最小间隔100ms
	m_TimeOuts.ReadTotalTimeoutMultiplier	= 0;
	m_TimeOuts.ReadTotalTimeoutConstant		= IsOverlappedMode() ? 500 :250;	
	m_TimeOuts.WriteTotalTimeoutMultiplier	= IsOverlappedMode() ? 1   :0;
	m_TimeOuts.WriteTotalTimeoutConstant	= IsOverlappedMode() ? 500 :250;
}

void SerialCom::Destroy()
{
	if(m_overlappedRead.hEvent != NULL)
	{
		CloseHandle(m_overlappedRead.hEvent);
	}
	if(m_overlappedWrite.hEvent != NULL)
	{
		CloseHandle(m_overlappedWrite.hEvent);
	}
	::DeleteCriticalSection(&m_hSection);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL SerialCom::IsOpen()
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

BOOL SerialCom::OpenPort(DWORD dwPort)
{
	if(!(dwPort>=1 && dwPort<=1024))
	{
		return FALSE;
	}
	
	if(IsOpen())
	{
		Close();
	}
	
	m_dwPort = dwPort;
	wsprintf(m_szName, _T("COM%d:"), dwPort);
	
	m_hComm = ::CreateFile(
						   m_szName,
						   GENERIC_READ | GENERIC_WRITE,
						   0,
						   NULL,
						   OPEN_EXISTING,
						   FILE_ATTRIBUTE_NORMAL,
						   NULL);
						   
	return IsOpen();					   
}

void SerialCom::Close(DWORD dwWaitForTime)
{
	if (IsOpen())
	{
		if (Purge(PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_RXABORT))
		{
			::CloseHandle(m_hComm);
			m_hComm = INVALID_HANDLE_VALUE;
		}
	}
}


BOOL SerialCom::SetState(DWORD dwBaudRate, BYTE Parity, BYTE ByteSize, BYTE StopBits)
{
	if(!IsOpen())
	{
		return FALSE;
	}
	
	if(!::GetCommState(m_hComm, &m_dcb))
	{
		return FALSE;
	}
	
	m_dcb.DCBlength = sizeof(DCB);
	m_dcb.BaudRate = dwBaudRate;
	m_dcb.Parity   = Parity;	
	m_dcb.ByteSize = ByteSize;
	m_dcb.StopBits = StopBits;
	m_dcb.fParity  = (Parity != NOPARITY);
	
	return ::SetCommState(m_hComm, &m_dcb);
}

BOOL SerialCom::Setup(DWORD dwInputSize, DWORD dwOutputSize)
{
	if(!IsOpen())
	{
		return FALSE;
	}
	
	return ::SetupComm(m_hComm, dwInputSize, dwOutputSize);
}

LPCOMMTIMEOUTS SerialCom::GetTimeouts(LPCOMMTIMEOUTS lpCommTime)
{
	if(!IsOpen())
	{
		return NULL;
	}
	
	if(!::GetCommTimeouts(m_hComm, lpCommTime))
	{
		return NULL;
	}
	
	return lpCommTime ? lpCommTime : &m_TimeOuts;
}

BOOL SerialCom::SetTimeouts(LPCOMMTIMEOUTS lpCommTime)
{
	if(!IsOpen())
	{
		return FALSE;
	}
	
	return ::SetCommTimeouts(m_hComm, lpCommTime ? lpCommTime :&m_TimeOuts);
}

BOOL SerialCom::Purge(DWORD dwParam)
{
	if(!IsOpen())
	{
		return FALSE;
	}
	
	return ::PurgeComm(m_hComm, dwParam);
}

void SerialCom::Flush()
{
	if (IsOpen())
	{
		FlushFileBuffers(m_hComm);
	}
}

DWORD SerialCom::Read(LPVOID pBuffer, DWORD dwLength, DWORD dwWaitForTime)
{
	ASSERT(pBuffer != NULL);
	return ReadPort(pBuffer, dwLength, dwWaitForTime);
}

DWORD SerialCom::Write(LPVOID pBuffer, DWORD dwLength)
{
	ASSERT(pBuffer != NULL);
	return WritePort(pBuffer, dwLength);
}

/////////////////////////////////////////////////////////////////////////////
BOOL SerialCom::IsOverlappedMode()
{
//	return m_dwFlags & EN_OVERLAPPED
	return FALSE;
}

DWORD SerialCom::ReadPort(LPVOID pBuffer, DWORD dwLength, DWORD dwWaitForTime)
{
	COMSTAT Stat;
	DWORD	dwError, dwReadResult = 0;
	
	if(!IsOpen() || !dwLength)
	{
		return 0;
	}
	
	if(::ClearCommError(m_hComm, &dwError, &Stat) && dwError>0)
	{
		if(!Purge(PURGE_RXABORT))
		{
			return 0;
		}
	}
	
	if(IsOverlappedMode())
	{
		if(!ReadFile(m_hComm, pBuffer, dwLength, &dwReadResult, &m_overlappedRead))
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				switch(WaitForSingleObject(m_overlappedRead.hEvent, INFINITE))
				{
				case WAIT_OBJECT_0:
					break;
				case WAIT_OBJECT_0 + 1:
					break;
				case WAIT_TIMEOUT:
					break;
				default:
					break;
				}
			}
		}
	}
	else
	{
		::ReadFile(m_hComm, pBuffer, dwLength, &dwReadResult, NULL);
	}
	return dwReadResult;
}

DWORD SerialCom::WritePort(LPVOID pBuffer, DWORD dwLength)
{
	if(!IsOpen() || !dwLength)
	{
		return 0;
	}
	
	DWORD	dwError, dwWriteResult = 0;
	if(::ClearCommError(m_hComm, &dwError, NULL) && dwError>0)
	{
		if(!Purge(PURGE_TXABORT))
		{
			return 0;
		}
	}
	
	if(IsOverlappedMode())
	{
	}
	else
	{
		::WriteFile(m_hComm, pBuffer, dwLength, &dwWriteResult, NULL);		
	}
	return dwWriteResult;
}