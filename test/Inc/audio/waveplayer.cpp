#include "StdAfx.h"
#include "waveplayer.h"

#pragma comment(lib,"Winmm")

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2017.07.17 17:39
 */
/************************************************************************/
WavePlayer::WavePlayer()
{
	m_bExit = FALSE;

	m_nDevID = 0;
	m_nCount = 0;
	
	m_dwFmtSize  = 0;
	m_dwWaitTime = 0;
	
	m_hThread = NULL;
	m_hFormat = NULL;
	
	m_hWaveOut = NULL;
	m_pWavData = NULL; 
	m_hMmioFile = NULL;

	m_lpFormat = NULL;
	m_strWavFilePath = _T("");
	
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hPlayEvent  = CreateEvent(NULL, TRUE, FALSE, NULL);
}

WavePlayer::~WavePlayer()
{
	ClosePlayerProc();

	if (m_hStartEvent != NULL)
	{
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
	}

	if (m_hEndEvent != NULL)
	{
		CloseHandle(m_hEndEvent);
		m_hEndEvent = NULL;
	}

	if (m_hPlayEvent != NULL)
	{
		CloseHandle(m_hPlayEvent);
		m_hPlayEvent = NULL;
	}
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
	BOOL bRet = FALSE;
	DWORD dwIndex = 0;

	if(!OpenWavFile())
	{
		bRet = FALSE;
		goto part;
	}
	
	if(!ReadWavFile())
	{
		bRet = FALSE;
		CloseWavFile();
		goto part;
	}

	CloseWavFile();
	
	if(!OpenPlayWav())
	{
		bRet = FALSE;
		goto part;
	}
	else
	{
		bRet = TRUE;
	}

	while(WaitForSingleObject(m_hEndEvent, m_dwWaitTime) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			if (dwIndex < m_nCount)
			{
				if (!PlayWavData())
				{
					m_dwWaitTime = 500;
					continue;
				}

				dwIndex++;
			}
			else
			{
				m_bExit = TRUE;
			}
		}
		else
		{
			if (ClearWavData())
			{
				SetEvent(m_hEndEvent);
			}
		}
	};

part:
	if (!bRet)
	{
		m_bExit = FALSE;

		if (m_hFormat != NULL || m_lpFormat != NULL)
		{
			GlobalUnlock(m_hFormat);
			GlobalFree(m_hFormat);

			m_hFormat = NULL;
			m_lpFormat = NULL;
		}

		SetEvent(m_hEndEvent);
	}

}

void CALLBACK WavePlayer::WaveOutCallBackProc(HWAVEOUT hWavOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	WavePlayer* pWavPlayer = (WavePlayer*)dwInstance;
	if (pWavPlayer == NULL)
	{
		return;
	}

	switch(uMsg)
	{
	case WOM_DONE:
		{
			OutputDebugString("waveOutWrite msg: WOM_DONE");
			pWavPlayer->WavePlayDone();
		}
		break;

	case WOM_CLOSE:
		{
			OutputDebugString(_T("waveOutClose msg: WOM_CLOSE!"));
		}
		break;

	case WOM_OPEN:
		{
			OutputDebugString(_T("waveOutOpen msg: WOM_OPEN!"));
		}
		break;
	}
}

void WavePlayer::WavePlayDone()
{
	SetEvent(m_hPlayEvent);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL WavePlayer::CreatePlayerProc(const char* pszWavFilePath, UINT nDevID, UINT nCount, UINT nSpanTime)
{
	BOOL bRet = FALSE;
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);
		
		m_nDevID = nDevID;
		m_nCount = nCount;

		m_dwWaitTime = nSpanTime;
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

BOOL WavePlayer::ClosePlayerProc()
{
	MMRESULT hResult = 0;
	m_bExit = TRUE;
	if (m_hWaveOut != NULL && m_hWaveOut != INVALID_HANDLE_VALUE)
	{
		waveOutPause(m_hWaveOut);
		SetEvent(m_hPlayEvent);
	}
	else
	{
		SetEvent(m_hEndEvent);
	}

	WaitForSingleObject(m_hEndEvent, INFINITE);
	if (m_hWaveOut != NULL)
	{
		hResult = waveOutClose(m_hWaveOut);
		if (hResult == 0)
		{
			m_hWaveOut = NULL;
		}
	}
	
	if (m_pWavData != NULL)
	{
		free(m_pWavData);
		m_pWavData = NULL;
	}

	m_bExit = FALSE;
	ResetEvent(m_hPlayEvent);
	ResetEvent(m_hStartEvent);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL WavePlayer::OpenWavFile()
{
	BOOL bRet = FALSE;
	LONG lReadBytes = 0;

	MMCKINFO mmckinfoParent = {0};
	MMCKINFO mmckinfoSubChunk = {0};

	MMRESULT hResult = 0;
	CString strPrompt;
	
	do 
	{
		if(m_strWavFilePath == _T(""))
		{
			bRet = FALSE;
			strPrompt = _T("播放文件路径不能为空, 请检查!");
			break;
		}

		m_hMmioFile = mmioOpen((LPSTR)m_strWavFilePath.c_str(), NULL, MMIO_READ|MMIO_ALLOCBUF);
		if(m_hMmioFile == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("打开媒体文件失败, 请检查!");
			break;
		}

		mmckinfoParent.fccType =mmioFOURCC('W','A','V','E');
		hResult = mmioDescend(m_hMmioFile, (LPMMCKINFO)&mmckinfoParent, NULL, MMIO_FINDRIFF);
		if(hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("错误的媒体格式,请检查!");
			break;
		}

		mmckinfoSubChunk.ckid = mmioFOURCC('f','m','t',' ');
		hResult = mmioDescend(m_hMmioFile, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK);
		if(hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体标识检测错误, 请检查!");
			break;
		}

		m_dwFmtSize = mmckinfoSubChunk.cksize;
		m_hFormat=LocalAlloc(LMEM_MOVEABLE,LOWORD(m_dwFmtSize)); 
		if(m_hFormat == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("媒体内存空间分配失败, 请检查!");
			break;
		}

		m_lpFormat = (WAVEFORMATEX*)LocalLock(m_hFormat);
		if(m_lpFormat == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("媒体内存空间加锁失败, 请检查!");
			break;
		}

		lReadBytes = mmioRead(m_hMmioFile, (HPSTR)m_lpFormat, m_dwFmtSize);
		if (lReadBytes != m_dwFmtSize)
		{
			bRet = FALSE;
			strPrompt = _T("读取媒体文件失败, 请检查!");
			break;
		}

		hResult = mmioAscend(m_hMmioFile, &mmckinfoSubChunk, 0);
		if (hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体句柄mmioAscend失败, 请检查!");
			break;
		}

		mmckinfoSubChunk.ckid=mmioFOURCC('d','a','t','a'); 
		hResult = mmioDescend(m_hMmioFile,&mmckinfoSubChunk,&mmckinfoParent,MMIO_FINDCHUNK);
		if (hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体句柄mmioDescend失败, 请检查!");
			break;
		}

		m_dwDataSize = mmckinfoSubChunk.cksize;
		m_dwDataOffset = mmckinfoSubChunk.dwDataOffset;
		if (m_dwDataSize == 0L)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件大小获取失败, 请检查!");
			break;
		}

		m_bOpenFile = TRUE;
		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		m_bOpenFile = FALSE;

		if (m_hMmioFile != NULL)
		{
			mmioClose(m_hMmioFile, 0);
			m_hMmioFile = NULL;
		}

		if (m_hFormat != NULL || m_lpFormat != NULL)
		{
			GlobalUnlock(m_hFormat);
			GlobalFree(m_hFormat);

			m_hFormat = NULL;
			m_lpFormat = NULL;
		}

		if (strPrompt != _T(""))
		{
			MessageBox(NULL, strPrompt, _T("提示!"), MB_ICONERROR|MB_OK);
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
	CString strPrompt;

	do 
	{
		if (m_hMmioFile == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("错误的媒体句柄, 请检查!");
			break;
		}

		m_pWavData = (HPSTR)malloc(m_dwDataSize);
		if (m_pWavData == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("数据内存空间分配失败, 请检查!");
			break;
		}
		memset(m_pWavData, 0x0, m_dwDataSize);

		lSeekOffset = mmioSeek(m_hMmioFile, m_dwDataOffset, SEEK_SET);
		if (lSeekOffset < 0)
		{
			bRet = FALSE;
			strPrompt = _T("数据指针移动失败, 请检查!");
			break;
		}

		lReadBytes = mmioRead(m_hMmioFile, m_pWavData, m_dwDataSize);
		if (lReadBytes < 0)
		{
			bRet = FALSE;
			strPrompt = _T("读取数据文件失败, 请检查!");
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		if (m_pWavData != NULL)
		{
			free(m_pWavData);
			m_pWavData = NULL;
		}

		if (m_hFormat != NULL || m_lpFormat != NULL)
		{
			GlobalUnlock(m_hFormat);
			GlobalFree(m_hFormat);

			m_hFormat = NULL;
			m_lpFormat = NULL;
		}

		if (strPrompt != _T(""))
		{
			MessageBox(NULL, strPrompt, _T("提示!"), MB_ICONERROR|MB_OK);
		}
	}

	return bRet;
}

void WavePlayer::CloseWavFile()
{
	if (m_bOpenFile)
	{
		if (m_hMmioFile != NULL)
		{
			mmioClose(m_hMmioFile, 0);
			m_hMmioFile = NULL;
		}
	}
}

BOOL WavePlayer::OpenPlayWav()
{
	BOOL bRet = FALSE;
	UINT uDevNum = 0;
	MMRESULT hResult = 0;

	WAVEOUTCAPS pwoc;
	CString strPrompt;

	do 
	{
		if (m_lpFormat == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("错误的媒体格式数据, 请检查!");
			break;
		}

		if (m_pWavData == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("错误的媒体数据, 请检查!");
			break;
		}

		uDevNum = waveOutGetNumDevs();
		if (uDevNum == 0)
		{
			bRet = FALSE;
			strPrompt = _T("获取媒体输出设备失败, 请检查!");
			break;
		}

		if (m_nDevID > (uDevNum-1))
		{
			bRet = FALSE;
			strPrompt = _T("选择的媒体设备ID与设备数量错误, 请检查!");
			break;
		}

		hResult = waveOutGetDevCaps(m_nDevID, &pwoc, sizeof(WAVEOUTCAPS));	//WAVE_MAPPER
		if (hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("waveOutGetDevCaps失败, 请检查!");
			break;
		}

		hResult = waveOutOpen(&m_hWaveOut, m_nDevID, m_lpFormat, (DWORD_PTR)WaveOutCallBackProc, (DWORD)this, CALLBACK_FUNCTION);	//WAVE_MAPPER //CALLBACK_FUNCTION	//CALLBACK_NULL
		if (hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("waveOutOpen失败, 请检查!");
			break;
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
			strPrompt = _T("waveOutPrepareHeader失败, 请检查!");
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	if (m_hFormat != NULL || m_lpFormat != NULL)
	{
		GlobalUnlock(m_hFormat);
		GlobalFree(m_hFormat);

		m_hFormat = NULL;
		m_lpFormat = NULL;
	}

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

		if (strPrompt != _T(""))
		{
			MessageBox(NULL, strPrompt, _T("提示!"), MB_ICONERROR|MB_OK);
		}
	}

	return bRet;
}

//libmad解码
BOOL WavePlayer::PlayWavData()
{
	MMRESULT hResult = 0;

	if (m_hWaveOut == NULL)
	{
		return FALSE;
	}
	
	if (m_pWaveOutHdr.lpData == NULL)
	{
		return FALSE;
	}

	hResult = waveOutWrite(m_hWaveOut, &m_pWaveOutHdr, sizeof(WAVEHDR));
	if (hResult != 0)
	{
		return FALSE;
	}

	WaitForSingleObject(m_hPlayEvent, INFINITE);
	ResetEvent(m_hPlayEvent);

	return TRUE;
}

BOOL WavePlayer::ClearWavData()
{
	BOOL bRet = FALSE;
	MMRESULT hResult = 0;

	if (m_hWaveOut == NULL)
	{
		return FALSE;
	}

	hResult = waveOutReset(m_hWaveOut);
	if (hResult != 0)
	{
		return FALSE;
	}

	hResult = waveOutUnprepareHeader(m_hWaveOut, &m_pWaveOutHdr, sizeof(WAVEHDR));
	if (hResult != 0)
	{
		if (hResult == MMSYSERR_INVALHANDLE)
		{
			OutputDebugString("waveOutUnprepareHeader error: MMSYSERR_INVALHANDLE");
		}
		else if (hResult == MMSYSERR_NODRIVER)
		{
			OutputDebugString("waveOutUnprepareHeader error: MMSYSERR_NODRIVER");
		}
		else if (hResult == MMSYSERR_NOMEM)
		{
			OutputDebugString("waveOutUnprepareHeader error: MMSYSERR_NOMEM");
		}
		else if (hResult == WAVERR_STILLPLAYING)
		{
			OutputDebugString("waveOutUnprepareHeader error: WAVERR_STILLPLAYING");
		}
		return FALSE;
	}

	memset(&m_pWaveOutHdr, 0x0, sizeof(WAVEHDR));
	return TRUE;
}