#include "StdAfx.h"
#include "waveplayer.h"

#pragma comment(lib,"Winmm")

WavePlayer::WavePlayer()
{
	m_bExit = FALSE;
	m_nDevID = 0;
	m_nCount = 0;
	
	m_hFormat = NULL;
	m_dwFmtSize = 0;
	
	m_hThread = NULL;
	m_hMmioFile = NULL;
	m_strWavFilePath = _T("");
	
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

WavePlayer::~WavePlayer()
{
}

//////////////////////////////////////////////////////////////////////////
//
WavePlayer& WavePlayer::Instance()
{
	static WavePlayer inst;
	return inst;
}

DWORD WavePlayer::WavePlayerThreadProc(LPVOID lpParam)
{
	WavePlayer* pWavePlayerProc = (WavePlayer*)lpParam;
	if(pWavePlayerProc != NULL)
	{
		pWavePlayerProc->PlayWavInfo();
	}
	
	return 0;
}

void WavePlayer::PlayWavInfo()
{
	if(!OpenWavFile())
	{
		return;
	}
	
	if(!ReadWavFile())
	{
		return;
	}

	CloseWavFile();
	
	if(!InitSoundDev())
	{
		return;
	}

	while(1)
	{
		if (!m_bExit)
		{
			PlayWavData();
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	};

	ResetEvent(m_hStartEvent);
}
//////////////////////////////////////////////////////////////////////////
//
BOOL WavePlayer::CreatePlayerProc(const char* pszWavFilePath, UINT nDevID, UINT nCount)
{
	BOOL bRet = FALSE;
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);
		
		m_nDevID = nDevID;
		m_nCount = nCount;
		m_strWavFilePath =  pszWavFilePath;
		
		m_hThread = CreateThread(NULL, 0, WavePlayerThreadProc, (LPVOID)this, 0, &m_dwThreadID);
		if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}

		bRet = TRUE;
		m_bExit = FALSE;
		
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
	return bRet;
}

BOOL WavePlayer::CloseProc()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL WavePlayer::OpenWavFile()
{
	BOOL bRet = FALSE;
	
	LONG lReadBytes = 0;

	MMCKINFO mmckinfoParent;
	MMCKINFO mmckinfoSubChunk;

	MMRESULT hResult = 0;
	
	if(m_strWavFilePath == _T(""))
	{
		return FALSE;
	}
	
	m_hMmioFile = mmioOpen((LPSTR)m_strWavFilePath.c_str(), NULL, MMIO_READ|MMIO_ALLOCBUF);
	if(m_hMmioFile == NULL)
	{
		return FALSE;
	}
	
	mmckinfoParent.fccType =mmioFOURCC('W','A','V','E');
	hResult = mmioDescend(m_hMmioFile, (LPMMCKINFO)&mmckinfoParent, NULL, MMIO_FINDRIFF);
	if(hResult != 0)
	{
		bRet = FALSE;
		goto part1;
	}

 	mmckinfoSubChunk.ckid = mmioFOURCC('f','m','t',' ');
	hResult = mmioDescend(m_hMmioFile, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK);
	if(hResult != 0)
	{
		bRet = FALSE;
		goto part1;
	}
	
	m_dwFmtSize = mmckinfoSubChunk.cksize;
	m_hFormat=LocalAlloc(LMEM_MOVEABLE,LOWORD(m_dwFmtSize)); 
	if(m_hFormat == NULL)
	{
		bRet = FALSE;
		goto part1;
	}
	
	m_lpFormat = (WAVEFORMATEX*)LocalLock(m_hFormat);
	if(m_lpFormat == NULL)
	{
		bRet = FALSE;
		goto part1;
	}
 	
 	lReadBytes = mmioRead(m_hMmioFile, (HPSTR)m_lpFormat, m_dwFmtSize);
	if (lReadBytes != m_dwFmtSize)
	{
		bRet = FALSE;
		goto part1;
	}

	hResult = mmioAscend(m_hMmioFile, &mmckinfoSubChunk, 0);
	if (hResult != 0)
	{
		bRet = FALSE;
		goto part1;
	}

	mmckinfoSubChunk.ckid=mmioFOURCC('d','a','t','a'); 
	hResult = mmioDescend(m_hMmioFile,&mmckinfoSubChunk,&mmckinfoParent,MMIO_FINDCHUNK);
	if (hResult != 0)
	{
		bRet = FALSE;
		goto part1;
	}

	m_dwDataSize = mmckinfoSubChunk.cksize;
	m_dwDataOffset = mmckinfoSubChunk.dwDataOffset;

	if (m_dwDataSize == 0L)
	{
		bRet = FALSE;
		goto part1;
	}

	bRet = TRUE;

 part1:
	if (!bRet)
	{
		if (m_hMmioFile != NULL)
		{
			mmioClose(m_hMmioFile, 0);
			m_hMmioFile = NULL;
		}
	}
	
	return bRet;
}

BOOL WavePlayer::ReadWavFile()
{
	BOOL bRet = FALSE;

	LONG lReadBytes = 0;
	LONG lSeekOffset = 0;
	MMRESULT hResult = 0;

	if (m_hMmioFile == NULL)
	{
		return FALSE;
	}

	m_pWavData = (HPSTR)malloc(m_dwDataSize);
	if (m_pWavData == NULL)
	{
		return FALSE;
	}
	memset(m_pWavData, 0x0, m_dwDataSize);

	lSeekOffset = mmioSeek(m_hMmioFile, m_dwDataOffset, SEEK_SET);
	if (lSeekOffset < 0)
	{
		bRet = FALSE;
		goto part2;
	}

	lReadBytes = mmioRead(m_hMmioFile, m_pWavData, m_dwDataSize);
	if (lReadBytes < 0)
	{
		bRet = FALSE;
		goto part2;
	}

	bRet = TRUE;

part2:
	if (!bRet)
	{
		if (m_pWavData != NULL)
		{
			free(m_pWavData);
			m_pWavData = NULL;
		}
	}

	return bRet;
}

void WavePlayer::CloseWavFile()
{
	if (m_hMmioFile != NULL)
	{
		mmioClose(m_hMmioFile, 0);
		m_hMmioFile = NULL;
	}
}

BOOL WavePlayer::InitSoundDev()
{

	BOOL bRet = FALSE;
	UINT uDevNum = 0;
	MMRESULT hResult = 0;

	WAVEOUTCAPS pwoc; 

	if (m_lpFormat == NULL)
	{
		return FALSE;
	}

	if (m_pWavData == NULL)
	{
		return FALSE;
	}

	uDevNum = waveOutGetNumDevs();
	if (uDevNum == 0)
	{
		return FALSE;
	}

	hResult = waveOutGetDevCaps(WAVE_MAPPER, &pwoc, sizeof(WAVEOUTCAPS));
	if (hResult != 0)
	{
		return FALSE;
	}

	hResult = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, m_lpFormat, NULL, NULL, CALLBACK_NULL);
	if (hResult != 0)
	{
		return FALSE;
	}

	m_pWaveOutHdr.lpData  = (HPSTR)m_pWavData;
	m_pWaveOutHdr.dwBufferLength = m_dwDataSize;
	m_pWaveOutHdr.dwFlags = 0;
	m_pWaveOutHdr.dwBytesRecorded = 0;
	m_pWaveOutHdr.dwUser  = NULL;
	m_pWaveOutHdr.dwLoops = 0;
	m_pWaveOutHdr.lpNext  = NULL;
	m_pWaveOutHdr.reserved = NULL;

	hResult = waveOutPrepareHeader(m_hWaveOut, &m_pWaveOutHdr, sizeof(WAVEHDR));
	if (hResult != 0)
	{
		bRet = FALSE;
		goto part3;
	}

	bRet = TRUE;

part3:
	if (!bRet)
	{
		hResult = waveOutUnprepareHeader(m_hWaveOut, &m_pWaveOutHdr, sizeof(WAVEHDR));
		if (hResult == 0)
		{
			if (m_hWaveOut != NULL)
			{
				waveOutReset(m_hWaveOut);
				waveOutClose(m_hWaveOut);
				m_hWaveOut = NULL;
			}
		}
	}

	LocalUnlock(m_hFormat);   
	LocalFree(m_hFormat);

	m_hFormat = NULL;
	m_lpFormat = NULL;
	return bRet;
}

//libmad½âÂë
BOOL WavePlayer::PlayWavData()
{
	BOOL bRet = FALSE;
	MMRESULT hResult = 0;

	if (m_hWaveOut == NULL)
	{
		return FALSE;
	}

	hResult = waveOutWrite(m_hWaveOut, &m_pWaveOutHdr, sizeof(WAVEHDR));
	if (hResult != 0)
	{
		return FALSE;
	}

// 	if (!(m_pWaveOutHdr.dwFlags & WHDR_DONE))
// 	{
// 		WaitForSingleObject(m_hEndEvent, INFINITE);
// 	}

	//while (WaitForSingleObject(m_hEndEvent, m_dwWaitTime) != WAIT_OBJECT_0)
	return bRet;
}