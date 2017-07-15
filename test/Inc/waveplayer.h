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
	BOOL					CreatePlayerProc(const char* pszWavFilePath, UINT nDevID, UINT nCount);
	BOOL					CloseProc();
	
	static 	WavePlayer&		Instance();
	static  DWORD WINAPI	WavePlayerThreadProc(LPVOID lpParam);
	
protected:
	void					PlayWavInfo();
	
	BOOL					OpenWavFile();
	BOOL					ReadWavFile();
	void					CloseWavFile();

	BOOL					InitSoundDev();
	BOOL					PlayWavData();

protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
	HANDLE					m_hFormat;
	HMMIO					m_hMmioFile;
	
	WAVEFORMATEX * 			m_lpFormat;
	WAVEHDR					m_pWaveOutHdr; 
	
private:
	BOOL					m_bExit;
	
	UINT					m_nDevID;
	UINT					m_nCount;
	
	DWORD					m_dwFmtSize;
	DWORD					m_dwDataSize;
	DWORD					m_dwDataOffset;
	DWORD					m_dwThreadID;

	HPSTR					m_pWavData;
	std::string				m_strWavFilePath;
};

#endif