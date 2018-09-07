#ifndef _WAVE_PLAYER_H_
#define _WAVE_PLAYER_H_

#include <afxmt.h>
#include <mmsystem.h>
#include <afxtempl.h>

typedef enum tagPlayMode
{
	PlayMode_Once = 0,	//单次
	PlayMode_Loop		//循环
}WAVE_PLAYMODE;

class CWavePlayer
{
public:
	CWavePlayer();
	~CWavePlayer();
	
public:
	BOOL					CreatePlayerProc(const char* pszWavFilePath, UINT nDevID, WAVE_PLAYMODE hPlayMode, UINT nSpanTime=500);
	void					ClosePlayerProc();
	
	static 	CWavePlayer&	Instance();
	
protected:
	static  DWORD WINAPI	WavePlayerThreadProc(LPVOID lpParam);
	static  void CALLBACK   WaveOutCallBackProc(HWAVEOUT hWavOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
protected:
	void					PlayWavInfo();
	void					WavePlayDone();
	
	void					SetPlayEvent(BOOL bStatus);
	BOOL					SetPlayData(const char* pszWavFilePath, UINT nDevID);
	BOOL					GetPlayData(const char* pszWavFilePath, WAVEFORMATEX *pWaveformat, BYTE **pWavData, DWORD &dwWavDataSize);
	
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

protected:
	HANDLE					m_hThread;
	HWAVEOUT				m_hWaveOut;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
	HANDLE					m_hStartPlayEvent;
	HANDLE					m_hEndPlayEvent;
	
	WAVEFORMATEX			m_stcWaveformat;
	WAVEHDR					m_stcWaveOutHdr;
	
private:
	BOOL					m_bExit;
	WAVE_PLAYMODE			m_hPlayMode;

	DWORD					m_dwThreadID;
	DWORD					m_dwWaitTime;

	DWORD					m_dwDataSize;
	HPSTR					m_pWavData;
};

#endif // !_WAVE_PLAYER_H_