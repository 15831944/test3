#ifndef _WAVEPLAYER_H_
#define _WAVEPLAYER_H_

#include <afxmt.h>
#include <mmsystem.h>
#include <afxtempl.h>

#define PLAY_ONCE         1           //播放一次
#define PALY_LOOP         0xefffffff  //播放无限次

class CWavePlayer
{
public:
	CWavePlayer();
	~CWavePlayer();
public:
	BOOL PlayFile(CString szFile, int iDevID, int iPlayCount);
	void StopPlay();


public:
	void PlayFunc();
	void clear();
private:
	void WorkThreadClose();
public:
	BOOL                 m_bPlaying;
	int                  m_iPlayCount;
	int                  m_iWaveTime;
    CRITICAL_SECTION     m_critiSec;
private:
	HWAVEOUT             m_hWaveOut;
	HANDLE               m_hWorkThread;
	int                  m_iDevID;
	WAVEFORMATEX         m_waveFrm;
	WAVEHDR              m_waveOutHdr;
	DWORD                m_iWaveBufSize;
	HPSTR                m_lpWaveBuf;         //音频数据
	DWORD                m_dwVolOld;          //原始的音量
};


#endif //_WAVEPLAYER_H_