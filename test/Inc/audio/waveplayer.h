#ifndef __WAVEPLAYER_H__
#define __WAVEPLAYER_H__

#include <afxmt.h>
#include <mmsystem.h>
#include <afxtempl.h>

class WavePlayer
{
public:
	WavePlayer();
	~WavePlayer();
	
public:
	BOOL					CreatePlayerProc(const char* pszWavFilePath, UINT nDevID, UINT nCount, UINT nSpanTime=500);
	BOOL					ClosePlayerProc();
	
	static 	WavePlayer&		Instance();
	
protected:
	static  DWORD WINAPI	WavePlayerThreadProc(LPVOID lpParam);
	static  void CALLBACK   WaveOutCallBackProc(HWAVEOUT hWavOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
protected:
	void					PlayWavInfo();
	void					WavePlayDone();
	
	BOOL					OpenWavFile();
	BOOL					ReadWavFile();
	void					CloseWavFile();

	BOOL					OpenPlayWav();
	BOOL					PlayWavData();
	BOOL					ClearWavData();

protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	HANDLE					m_hPlayEvent;
	
	HANDLE					m_hFormat;
	HMMIO					m_hMmioFile;
	
	WAVEFORMATEX * 			m_lpFormat;
	WAVEHDR					m_pWaveOutHdr; 
	HWAVEOUT				m_hWaveOut;
	
private:
	BOOL					m_bExit;
	BOOL					m_bOpenFile;
	
	UINT					m_nDevID;
	UINT					m_nCount;
	
	DWORD					m_dwFmtSize;
	DWORD					m_dwDataSize;
	DWORD					m_dwDataOffset;

	DWORD					m_dwThreadID;
	DWORD					m_dwWaitTime;

	HPSTR					m_pWavData;
	std::string				m_strWavFilePath;
};

#endif