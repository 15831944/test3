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
	BOOL					OpenWavPlayerFile(const char* pszWavFilePath);
	BOOL					CreatePlayerProc(UINT nDevID, UINT nCount, UINT nSpanTime=500);
	BOOL					ClosePlayerProc();
	
	static 	WavePlayer&		Instance();
	
protected:
	static  DWORD WINAPI	WavePlayerThreadProc(LPVOID lpParam);
	static  void CALLBACK   WaveOutCallBackProc(HWAVEOUT hWavOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
protected:
	void					PlayWavInfo();
	void					WavePlayDone();
	
	BOOL					OpenWavFile(const char *pszPlayerFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk, HMMIO &hMmioFile, WAVEFORMATEX *pWavFormat);
	BOOL					ResetWavFile(HMMIO hMmioFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk);
	BOOL					ReadWavFile(HMMIO hMmioFile, UINT uiRead, BYTE *pbDataBuf, MMCKINFO &ckSubChunk, UINT &uiReadSize);
	void					CloseWavFile();

	BOOL					OpenPlayWav();
	BOOL					PlayWavData();
	BOOL					ClearWavData();

protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	HANDLE					m_hPlayEvent;
	
	HMMIO					m_hMmioFile;
	WAVEHDR					m_pWaveOutHdr; 
	HWAVEOUT				m_hWaveOut;
	
private:
	BOOL					m_bExit;

	UINT					m_nDevID;
	UINT					m_nCount;
	
	DWORD					m_dwThreadID;
	DWORD					m_dwWaitTime;

	DWORD					m_dwDataSize;
	HPSTR					m_pWavData;
};

#endif