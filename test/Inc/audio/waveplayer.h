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
	BOOL					SetPlayerProcData(const char* pszWavFilePath, UINT uiDevID);
	void					SetPlayerProcEvent(BOOL bFlag);

	BOOL					CreatePlayerProc(UINT nSpanTime=500);
	BOOL					ClosePlayerProc();

	static 	WavePlayer&		Instance();
	
protected:
	static  DWORD WINAPI	WavePlayerThreadProc(LPVOID lpParam);
	static  void CALLBACK   WaveOutCallBackProc(HWAVEOUT hWavOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
protected:
	void					PlayWavInfo();
	void					WavePlayDone();
	
	BOOL					GetWavData(const char* pszWavFilePath, WAVEFORMATEX *pWaveformat, BYTE **pWavData, DWORD &dwWavDataSize);

	//文件操作
	BOOL					OpenWavFile(const char *pszPlayerFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk, HMMIO &hMmioFile, WAVEFORMATEX **pWavFormat);
	void					CloseWavFile(HMMIO hMmioFile, WAVEFORMATEX *pWavFormat);

	BOOL					ResetWavFile(HMMIO hMmioFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk);
	BOOL					ReadWavFile(HMMIO hMmioFile, UINT uiRead, BYTE **pbDataBuf, MMCKINFO &ckSubChunk, UINT &uiReadSize);
	
	//设备操作
	BOOL					OpenPlayDev(UINT uiDevID, WAVEFORMATEX *pWaveformat, HWAVEOUT &hWaveOut);
	void					ClosePlayDev(HWAVEOUT hWaveOut);

	BOOL					SetPlayData(HWAVEOUT hWaveOut, HPSTR pWavData, DWORD dwWavDataSize, WAVEHDR &stcWaveOutHdr);
	BOOL					PlayDevData(HWAVEOUT hWaveOut, WAVEHDR stcWaveOutHdr, HANDLE hEvent);
	void					ClearPlayDev(HWAVEOUT hWaveOut, WAVEHDR &stcWaveOutHdr);

private:
	BOOL					m_bExit;

	UINT					m_uiDevID;
	
	DWORD					m_dwThreadID;
	DWORD					m_dwWaitTime;

	DWORD					m_dwDataSize;
	HPSTR					m_pWavData;

	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;

	HANDLE					m_hStartPlayEvent;
	HANDLE					m_hEndPlayEvent;

	HANDLE					m_hThread;
	HWAVEOUT				m_hWaveOut;

	WAVEFORMATEX			m_stcWaveformat;
	WAVEHDR					m_stcWaveOutHdr;
};

#endif