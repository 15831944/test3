#include "stdafx.h"
#include "waveplayer.h"
#pragma comment(lib,"Winmm")

#define SLEEP_STEP        100       //睡眠100ms

DWORD WINAPI PlayThreadFunc(void* pParam)
{
	CWavePlayer* pThis = (CWavePlayer*)pParam;
	pThis->PlayFunc();
	return 0;
}

CWavePlayer::CWavePlayer()
{
	m_hWaveOut              = NULL;
	m_lpWaveBuf             = NULL;
	m_iWaveBufSize          = 0;
	m_bPlaying              = FALSE;
	InitializeCriticalSection(&m_critiSec);
}

CWavePlayer::~CWavePlayer()
{
	DeleteCriticalSection(&m_critiSec);
	if(m_lpWaveBuf)
		delete [] m_lpWaveBuf;
}

BOOL CWavePlayer::PlayFile(CString szFile, int iDevID, int iPlayCount)
{
    WorkThreadClose();
	MMCKINFO      mmckinfoParent;
	MMCKINFO      mmckinfoSubChunk;
	DWORD         dwFmtSize   = 0, dwDataSize = 0, dwDataOffset = 0;
	HMMIO         hmmio       = NULL;        //音频文件句柄
	int           SoundOffset = 0, SoundLong = 0;
	//打开波形文件
	if(!(hmmio = mmioOpen(szFile.GetBuffer(), NULL, MMIO_READ|MMIO_ALLOCBUF)))
	{
		//File open Error
		printf("Failed to open the file.");//错误处理函数
		return FALSE;
	}
	//检查打开文件是否是声音文件
	mmckinfoParent.fccType =mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmmio, (LPMMCKINFO)&mmckinfoParent, NULL, MMIO_FINDRIFF))
	{
		printf("NOT WAVE FILE AND QUIT");
		goto _EXIT;
	}
	//寻找 'fmt ' 块
	mmckinfoSubChunk.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmmio, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK))
	{
		printf("Can't find [fmt ] chunk");
		goto _EXIT;
	}
	//获得 'fmt ' 块的大小，申请内存
	dwFmtSize = mmckinfoSubChunk.cksize;
	memset(&m_waveFrm, 0, sizeof(m_waveFrm));
	if(mmioRead(hmmio, (HPSTR)&m_waveFrm, dwFmtSize) != dwFmtSize)
	{
		printf("failed to read format chunk");
		goto _EXIT;
	}
	//离开 fmt 块
	mmioAscend(hmmio, &mmckinfoSubChunk, 0);
	//寻找 'data' 块
	mmckinfoSubChunk.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmmio, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK))
	{
		printf("Can't find [data] chunk");
		goto _EXIT;
	}
	//获得 'data'块的大小
	dwDataSize   = mmckinfoSubChunk.cksize ;
	dwDataOffset = mmckinfoSubChunk.dwDataOffset ;
	if(0 == dwDataSize)
	{
		printf("no data in the ’data’ chunk");
		goto _EXIT;
	}
	//为音频数据分配内存
	if(m_iWaveBufSize < dwDataSize)
	{
		m_lpWaveBuf=new char[dwDataSize];
		if(!m_lpWaveBuf)
		{
			printf("\ncan not alloc mem");
			goto _EXIT;
		}
		m_iWaveBufSize = dwDataSize;
	}
	memset(m_lpWaveBuf, 0, m_iWaveBufSize);
	if(mmioSeek(hmmio, dwDataOffset, SEEK_SET) < 0)
	{
		printf("Failed to read the data chunk");
		goto _EXIT;
	}
	int iReadDataLen = mmioRead(hmmio, m_lpWaveBuf, dwDataSize);
	if(iReadDataLen < 0)
	{
		printf("Failed to read the data chunk");
		goto _EXIT;
	}
	mmioClose(hmmio, 0);
	hmmio       = NULL;
	m_iWaveTime = (float)iReadDataLen / m_waveFrm.nAvgBytesPerSec * 1000 + 0.5;//取整
	int iDevCount  = waveOutGetNumDevs();
	if (iDevCount - 1 < iDevID)
	{
		iDevID = 0;
	}
	//检查音频输出设备是否能播放指定的音频文件
	if(waveOutOpen(&m_hWaveOut, iDevID, &m_waveFrm, NULL, NULL, CALLBACK_NULL) != 0)
	{
		printf("Failed to OPEN the wave out device [%d]\n", iDevID);
		return FALSE;
	}
	//准备待播放的数据
	m_waveOutHdr.lpData          = (HPSTR)m_lpWaveBuf;
	m_waveOutHdr.dwBufferLength  = iReadDataLen;
	m_waveOutHdr.dwFlags         = 0;
	m_waveOutHdr.dwBytesRecorded = 0;
	m_waveOutHdr.dwUser          = NULL;
	m_waveOutHdr.dwLoops         = 0;
	m_waveOutHdr.lpNext          = NULL;
	m_waveOutHdr.reserved        = NULL;
	if(waveOutPrepareHeader(m_hWaveOut, &m_waveOutHdr, sizeof(WAVEHDR)) != 0)
	{
		waveOutClose(m_hWaveOut);
		printf("Failed to prepare the wave data buffer");
		return FALSE;
	}
	m_bPlaying    = TRUE;
	m_iPlayCount  = iPlayCount;
	m_hWorkThread = CreateThread(NULL, 0, PlayThreadFunc, this, 0, 0);
	return TRUE;

_EXIT:
	mmioClose(hmmio, 0);
	hmmio = NULL;
	return FALSE;
}
void CWavePlayer::clear()
{
	m_iPlayCount  = 0;
	m_bPlaying    = FALSE;
	waveOutSetVolume(m_hWaveOut, m_dwVolOld);
	waveOutReset(m_hWaveOut);
	waveOutUnprepareHeader(m_hWaveOut, &m_waveOutHdr, sizeof(WAVEHDR));
	waveOutClose(m_hWaveOut);
	m_hWaveOut    = NULL;
	CloseHandle(m_hWorkThread);
	m_hWorkThread = NULL;
}
void CWavePlayer::PlayFunc()
{
	waveOutGetVolume(m_hWaveOut, &m_dwVolOld);

	//响铃音量为100
	DWORD dwLeftVol  = 65535;
	DWORD dwRightVol = dwLeftVol << 16;
	DWORD dwVol      = dwLeftVol + dwRightVol;
	waveOutSetVolume(m_hWaveOut,dwVol);

	int iElapseTime = m_iWaveTime, iRemainTime = m_iWaveTime, iWantSleep = 0;
	int iCount = 0;
	while (1)
	{
		EnterCriticalSection(&m_critiSec);
		if(FALSE == m_bPlaying)			
		{
			clear();
			LeaveCriticalSection(&m_critiSec);
			return;
		}
		if(iElapseTime >= m_iWaveTime)
		{	
			if(iCount == m_iPlayCount)	
			{
				clear();
				LeaveCriticalSection(&m_critiSec);
				return;
			}
			waveOutReset(m_hWaveOut);
			//播放音频数据文件
			if(0 != waveOutWrite(m_hWaveOut, &m_waveOutHdr, sizeof(WAVEHDR)))
			{
				printf("Failed to write the wave data buffer");
			}
			iElapseTime = 0;
			iCount++;
		}
        LeaveCriticalSection(&m_critiSec);

		iRemainTime = m_iWaveTime - iElapseTime;
		iWantSleep = SLEEP_STEP < iRemainTime ? SLEEP_STEP : iRemainTime;
		Sleep(iWantSleep);
		iElapseTime += iWantSleep;
	}
}

void CWavePlayer::StopPlay()
{
	WorkThreadClose();
}

void CWavePlayer::WorkThreadClose()
{
	EnterCriticalSection(&m_critiSec);
	if(FALSE == m_bPlaying)
	{
		LeaveCriticalSection(&m_critiSec);
		return;
	}
	m_bPlaying = FALSE;
	LeaveCriticalSection(&m_critiSec);

	DWORD dwStatus = 0;
	int iWantSleep = SLEEP_STEP < m_iWaveTime ? SLEEP_STEP : m_iWaveTime;
	int iCount = 1;
    while(WAIT_TIMEOUT == WaitForSingleObject(m_hWorkThread, iWantSleep))
	{
		CString strInfo;
		strInfo.Format(_T("*** *** waiting for waveplay thread exit, %d times\n"), iCount++);
		OutputDebugString(strInfo);
	}

	return;
}