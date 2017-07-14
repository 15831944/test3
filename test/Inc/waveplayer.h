#ifndef _WAVEPLAYER_H_
#define _WAVEPLAYER_H_

#include <afxmt.h>
#include <mmsystem.h>
#include <afxtempl.h>

#define PLAY_ONCE         1           //����һ��
#define PALY_LOOP         0xefffffff  //�������޴�

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
	HPSTR                m_lpWaveBuf;         //��Ƶ����
	DWORD                m_dwVolOld;          //ԭʼ������
};


#endif //_WAVEPLAYER_H_