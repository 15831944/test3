#ifndef __SERIALCOM_H__
#define __SERIALCOM_H__

class SerialCom
{
public:
	SerialCom();
	~SerialCom();
	
public:
	void				Init();
	void				Destroy();
	
	BOOL				IsOpen();
	BOOL				OpenPort(DWORD dwPort);
	void				Close(DWORD dwWaitForTime = 500);

	BOOL				SetState(DWORD dwBaudRate, BYTE Parity, BYTE ByteSize, BYTE StopBits);
	BOOL				Setup(DWORD dwInputSize = 4096, DWORD dwOutputSize = 4096);
	
	LPCOMMTIMEOUTS		GetTimeouts(LPCOMMTIMEOUTS lpCommTime = NULL);
	BOOL				SetTimeouts(LPCOMMTIMEOUTS lpCommTime = NULL);
	
	BOOL				Purge(DWORD dwParam = PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	void				Flush();
	
    DWORD				Read(LPVOID pBuffer, DWORD dwLength, DWORD dwWaitForTime = INFINITE);	
	DWORD				Write(LPVOID pBuffer, DWORD dwLength);

protected:	
	BOOL				IsOverlappedMode();
	
	DWORD				ReadPort(LPVOID  pBuffer, DWORD dwLength, DWORD dwWaitForTime = INFINITE);
	DWORD				WritePort(LPVOID pBuffer, DWORD dwLength);
private:
	CRITICAL_SECTION	m_hSection;
	
	HANDLE				m_hComm;
	TCHAR				m_szName[64];
	DWORD				m_dwPort;
	COMMTIMEOUTS		m_TimeOuts;
	DWORD				m_dwFlags;
	DCB					m_dcb;
	
	OVERLAPPED			m_overlappedRead;
	OVERLAPPED			m_overlappedWrite;
};
#endif