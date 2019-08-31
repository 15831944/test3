#ifndef __WAVERECORD_H__
#define __WAVERECORD_H__

class WaveRecord
{
public:
	WaveRecord();
	~WaveRecord();
	
	static WaveRecord&		Instance();
	
public:
	void					SetPlayerProcEvent(BOOL bFlag);
	
	BOOL					CreateRecordProc();
	void					CloseRecordProc();
	
protected:
	static DWORD WINAPI		WaveRecordThreadProc(LPVOID lpParam);
	static  void CALLBACK   WaveInCallBackProc(HWAVEIN hWavIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
protected:
	void					RecordWavInfo();
	
	BOOL					OpenRecordDev(UINT uiDevID, WAVEFORMATEX *pWaveformat, HWAVEIN &hWaveIn);
	void					CloseRecordDev(HWAVEIN hWaveIn);
	
private:
	BOOL					m_bExit;
	
	UINT					m_uiDevID;
	
	DWORD					m_dwThreadID;
	DWORD					m_dwWaitTime;
	
	DWORD					m_dwDataSize;
	HPSTR					m_pWavData;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
	HANDLE					m_hThread;
	HWAVEIN					m_hWaveIn;
	
	WAVEFORMATEX			m_stcWaveformat;
	WAVEHDR					m_stcWaveOutHdr;
};

#endif