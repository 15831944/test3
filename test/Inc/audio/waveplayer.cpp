#include "StdAfx.h"
#include "waveplayer.h"

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2017.07.17 17:39
 */
/************************************************************************/
#pragma comment(lib,"Winmm")
WavePlayer::WavePlayer()
{
	m_bExit = FALSE;

	m_nDevID = 0;
	m_nCount = 0;
	m_dwWaitTime = 0;
	
	m_hThread = NULL;
	
	m_hWaveOut = NULL;
	m_pWavData = NULL; 
	m_hMmioFile = NULL;
	
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

WavePlayer& WavePlayer::Instance()
{
	static WavePlayer inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
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
BOOL WavePlayer::OpenWavFile(const char *pszPlayerFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk, HMMIO &hMmioFile, WAVEFORMATEX *pWavFormat)
{
	BOOL bRet = FALSE;

	LONG lReadBytes = 0;
	DWORD dwFmtSize = 0;
	MMRESULT hResult = 0;

	CString strPrompt;
	PCMWAVEFORMAT pcmWaveFormat = {0};
	do 
	{
		if(pszPlayerFile == NULL || pszPlayerFile == '\0')
		{
			bRet = FALSE;
			strPrompt = _T("播放文件路径不能为空, 请检查!");
			break;
		}

		//打开一个RIFF文件
		hMmioFile = mmioOpen((LPSTR)pszPlayerFile, NULL, MMIO_READ|MMIO_ALLOCBUF);
		if(hMmioFile == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("打开媒体文件失败, 请检查!");
			break;
		}

		//在RIFF文件中，查找WAVE块信息
		ckParent.fccType =mmioFOURCC('W','A','V','E');
		hResult = mmioDescend(hMmioFile, (LPMMCKINFO)&ckParent, NULL, MMIO_FINDRIFF);
		if(hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件查找WAVE块信息失败,请检查!");
			break;
		}

		//在RIFF文件中， 查找fmt块信息
		ckSubChunk.ckid = mmioFOURCC('f','m','t',' ');
		hResult = mmioDescend(hMmioFile, &ckSubChunk, &ckParent, MMIO_FINDCHUNK);	//通过mmckinfo作为父块,以块块搜索块的方式搜索'fmt'信息
		if(hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件查找fmt块信息失败, 请检查!");
			break;
		}

		//获取fmt块的大小
		dwFmtSize = ckSubChunk.cksize;
		if (dwFmtSize < (LONG)sizeof(PCMWAVEFORMAT))
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件fmt大小获取失败, 请检查!");
			break;
		}

		//读取RIFF文件PCMWAVEFORMAT包头信息
		lReadBytes = mmioRead(hMmioFile, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat));
		if (lReadBytes != sizeof(pcmWaveFormat))
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件获取PCMWAVEFORMAT失败, 请检查!");
			break;
		}

		if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
		{
			pWavFormat = (WAVEFORMATEX*)new char[sizeof(WAVEFORMATEX)];
			if (pWavFormat == NULL)
			{
				bRet = FALSE;
				strPrompt = _T("WAVEFORMATEX结构内存创建, 请检查!");
				break;
			}
			memset(pWavFormat, 0x0, sizeof(WAVEFORMATEX));

			memcpy(pWavFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
			pWavFormat->cbSize = 0;
		}
		else
		{
			WORD cbExtraBytes = 0L;
			if (mmioRead(hMmioFile, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			{
				bRet = FALSE;
				strPrompt = _T("媒体文件获取扩展字节失败, 请检查!");
				break;
			}

			pWavFormat = (WAVEFORMATEX *)new char[sizeof(WAVEFORMATEX) + cbExtraBytes];
			if (pWavFormat == NULL)
			{
				bRet = FALSE;
				strPrompt = _T("WAVEFORMATEX结构内存创建, 请检查!");
				break;
			}
			memset(pWavFormat, 0x0, sizeof(WAVEFORMATEX));

			memcpy(pWavFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
			pWavFormat->cbSize = cbExtraBytes;

			if (mmioRead(hMmioFile, (CHAR*)(((BYTE*)&(pWavFormat->cbSize)) + sizeof(WORD)), cbExtraBytes) != cbExtraBytes)
			{
				bRet = FALSE;
				strPrompt = _T("媒体文件读取扩展数据失败, 请检查!");
				break;
			}
		}

		//跳出fmt数据块
		if (mmioAscend(hMmioFile, &ckSubChunk, 0) != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件跳出fmt块失败, 请检查!");
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		if (pWavFormat != NULL)
		{
			delete[] pWavFormat;
			pWavFormat = NULL;
		}

		if (hMmioFile != NULL)
		{
			mmioClose(hMmioFile, 0);
			hMmioFile = NULL;
		}

		if (strPrompt != _T(""))
		{
			MessageBox(NULL, strPrompt, _T("提示!"), MB_ICONERROR | MB_OK);
		}
	}

	return bRet;
}

BOOL WavePlayer::ResetWavFile(HMMIO hMmioFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk)
{
	BOOL bRet = FALSE;
	CString strPrompt;

	do 
	{
		if (hMmioFile == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件没有打开, 请检查!");
			break;
		}

		if (mmioSeek(hMmioFile, ckParent.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件指针移动错误, 请检查!");
			break;
		}

		ckSubChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if (mmioDescend(hMmioFile, &ckSubChunk, &ckParent, MMIO_FINDCHUNK) != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件查找data块信息失败, 请检查!");
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		if (strPrompt != _T(""))
		{
			MessageBox(NULL, strPrompt, _T("提示!"), MB_ICONERROR | MB_OK);
		}
	}

	return bRet;
}

BOOL WavePlayer::ReadWavFile(HMMIO hMmioFile, UINT uiRead, BYTE *pbDataBuf, MMCKINFO &ckSubChunk, UINT &uiReadSize)
{
	BOOL bRet = FALSE;
	
	UINT uiIndex = 0;
	UINT uiDataIn = 0;
	
	LONG lReadBytes = 0;
	LONG lSeekOffset = 0;

	MMIOINFO mmioinfoIn;
	MMRESULT hResult = 0;
	CString strPrompt;

	do 
	{
		if (hMmioFile == NULL)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件没有打开, 请检查!");
			break;
		}

		if (mmioGetInfo(hMmioFile, &mmioinfoIn, 0) != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件获取当前信息失败, 请检查!");
			break;
		}

		uiDataIn = uiRead;
		if (uiDataIn > ckSubChunk.cksize)
		{
			uiDataIn = ckSubChunk.cksize;
		}

		ckSubChunk.cksize -= uiDataIn;
		for (uiIndex=0; uiIndex<uiDataIn; uiIndex++)
		{
			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
			{
				if ((mmioAdvance(hMmioFile, &mmioinfoIn, MMIO_READ)) != 0)
				{
					bRet = FALSE;
					strPrompt = _T("媒体文件mmioAdvance失败，请检查!");
					break;
				}

				if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
				{
					bRet = FALSE;
					strPrompt = _T("媒体文件读取失败, 请检查!");
					break;
				}
			}

			*((BYTE*)pbDataBuf+uiIndex) = *((BYTE*&)mmioinfoIn.pchNext)++;
		}

		if (!bRet)
		{
			break;
		}

		if ((mmioSetInfo(hMmioFile, &mmioinfoIn, 0)) != 0)
		{
			bRet = FALSE;
			strPrompt = _T("媒体文件mmioSetInfo失败, 请检查!");
			break;
		}

		uiReadSize = uiDataIn;
		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		if (strPrompt != _T(""))
		{
			MessageBox(NULL, strPrompt, _T("提示!"), MB_ICONERROR|MB_OK);
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

BOOL WavePlayer::OpenPlayWav()
{
	BOOL bRet = FALSE;
	UINT uDevNum = 0;
	MMRESULT hResult = 0;

	WAVEOUTCAPS pwoc;
	CString strPrompt;

	do 
	{
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

//////////////////////////////////////////////////////////////////////////
//
BOOL WavePlayer::CreatePlayerProc(UINT nDevID, UINT nCount, UINT nSpanTime)
{
	BOOL bRet = FALSE;

	if (WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);

		m_nDevID = nDevID;
		m_nCount = nCount;

		m_dwWaitTime = nSpanTime;
		m_hThread = CreateThread(NULL, 0, WavePlayerThreadProc, (LPVOID)this, 0, &m_dwThreadID);
		if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
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

BOOL WavePlayer::OpenWavPlayerFile(const char* pszWavFilePath)
{
	BOOL bRet = FALSE;

	UINT uiWaveSize = 0;
	UINT uiReadSize = 0;

	MMCKINFO mmckinfoParent = {0};
	MMCKINFO mmckinfoSubChunk = {0};

	WAVEFORMATEX *lpFormat = NULL;
	do 
	{
		if (pszWavFilePath == NULL || *pszWavFilePath == '\0')
		{
			bRet = FALSE;
			break;
		}

		if (!OpenWavFile(pszWavFilePath, mmckinfoParent, mmckinfoSubChunk, m_hMmioFile, lpFormat))
		{
			bRet = FALSE;
			break;
		}

		if (!ResetWavFile(m_hMmioFile, mmckinfoParent, mmckinfoSubChunk))
		{
			bRet = FALSE;
			break;
		}

		uiWaveSize = mmckinfoSubChunk.cksize;
		if ((m_pWavData = (HPSTR)GlobalAlloc(GMEM_FIXED, uiWaveSize)) == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (!ReadWavFile(m_hMmioFile, uiWaveSize, (BYTE*)m_pWavData, mmckinfoSubChunk, uiReadSize))
		{
			bRet = FALSE;
			break;
		}

		CloseWavFile();
		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		if (m_pWavData != NULL)
		{
			GlobalFree(m_pWavData);
			m_pWavData = NULL;
		}

		if (m_hMmioFile != NULL)
		{
			mmioClose(m_hMmioFile, 0);
			m_hMmioFile = NULL;
		}

		if (lpFormat != NULL)
		{
			delete[] lpFormat;
			lpFormat = NULL;
		}
	}

	return bRet;
}