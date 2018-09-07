#include "StdAfx.h"
#include "waveplayer.h"

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2017.07.17 17:39
 */
/************************************************************************/
#pragma comment(lib,"Winmm")
CWavePlayer::CWavePlayer()
{
	m_bExit = FALSE;

	m_dwThreadID = 0;
	m_dwWaitTime = 0;
	m_dwDataSize = 0;
	
	m_hThread = NULL;
	m_hWaveOut = NULL;

	m_pWavData = NULL;
	m_hPlayMode = PlayMode_Once;
	
	m_stcWaveformat.wFormatTag = WAVE_FORMAT_PCM;
	m_stcWaveformat.nChannels = 2;
	m_stcWaveformat.cbSize = 0;
	m_stcWaveformat.wBitsPerSample = 16;
	m_stcWaveformat.nSamplesPerSec = 44100;
	m_stcWaveformat.nBlockAlign = (m_stcWaveformat.wBitsPerSample*m_stcWaveformat.nChannels) >> 3;
	m_stcWaveformat.nAvgBytesPerSec = m_stcWaveformat.nBlockAlign*m_stcWaveformat.nSamplesPerSec;
	memset(&m_stcWaveOutHdr, 0x0, sizeof(WAVEHDR));
	
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	m_hStartPlayEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndPlayEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CWavePlayer::~CWavePlayer()
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

	if (m_hStartPlayEvent != NULL)
	{
		CloseHandle(m_hStartPlayEvent);
		m_hStartPlayEvent = NULL;
	}
	
	if (m_hEndPlayEvent != NULL)
	{
		CloseHandle(m_hEndPlayEvent);
		m_hEndPlayEvent = NULL;
	}
}

CWavePlayer& CWavePlayer::Instance()
{
	static CWavePlayer inst;
	return inst;
}
//////////////////////////////////////////////////////////////////////////
//
DWORD CWavePlayer::WavePlayerThreadProc(LPVOID lpParam)
{
	CWavePlayer* pWavePlayerProc = (CWavePlayer*)lpParam;
	if(pWavePlayerProc != NULL)
	{
		pWavePlayerProc->PlayWavInfo();
	}
	
	return 0;
}

void CWavePlayer::PlayWavInfo()
{
	BOOL bRet = FALSE;

	do 
	{
		SetPlayEvent(TRUE);
		while(WaitForSingleObject(m_hEndEvent, m_dwWaitTime) != WAIT_OBJECT_0)
		{
			if (!m_bExit)
			{
				if (WaitForSingleObject(m_hStartPlayEvent, 0) == WAIT_OBJECT_0)
				{
					if (!PlayDevData(m_hWaveOut, m_stcWaveOutHdr, m_hEndPlayEvent))
					{
						m_dwWaitTime = 200;
						continue;
					}

					if (m_hPlayMode == PlayMode_Once)
					{
						m_bExit = TRUE;
						SetPlayEvent(FALSE);
						continue;
					}
					else
					{
						m_dwWaitTime = 500;
						continue;
					}
				}
				else
				{
					m_dwWaitTime = 200;
					continue;
				}
			}
			else
			{
				m_dwWaitTime = 0;
				ClearPlayDev(m_hWaveOut, m_stcWaveOutHdr);
				
				SetEvent(m_hEndEvent);
				continue;
			}
		}

		bRet = TRUE;
	} while (FALSE);
}

void CALLBACK CWavePlayer::WaveOutCallBackProc(HWAVEOUT hWavOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CWavePlayer* pWavPlayer = (CWavePlayer*)dwInstance;
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

void CWavePlayer::WavePlayDone()
{
	SetEvent(m_hEndPlayEvent);
}

//////////////////////////////////////////////////////////////////////////
//
void CWavePlayer::SetPlayEvent(BOOL bStatus)
{
	if (m_hWaveOut == NULL)
	{
		return;
	}
	
	if (bStatus)
	{//从新播放
		waveOutRestart(m_hWaveOut);

		SetEvent(m_hStartPlayEvent);
		ResetEvent(m_hEndPlayEvent);
	}
	else
	{//暂停播放
		waveOutPause(m_hWaveOut);
		waveOutReset(m_hWaveOut);

		ResetEvent(m_hStartPlayEvent);
		SetEvent(m_hEndPlayEvent);
	}
}

BOOL CWavePlayer::SetPlayData(const char* pszWavFilePath, UINT uiDevID)
{
	BOOL bRet = FALSE;
	WAVEFORMATEX stcWaveformat = {0};
	
	do
	{
		SetPlayEvent(FALSE);
		ClearPlayDev(m_hWaveOut, m_stcWaveOutHdr);
		
		if (m_pWavData != NULL)
		{
			GlobalFree(m_pWavData);
			m_pWavData = NULL;
		}
		
		if (!GetPlayData(pszWavFilePath, &stcWaveformat, (BYTE**)&m_pWavData, m_dwDataSize))
		{
			bRet = FALSE;
			break;
		}
		
		if (memcmp(&stcWaveformat, &m_stcWaveformat, sizeof(WAVEFORMATEX)) != 0)
		{
			ClosePlayDev(m_hWaveOut);
			memcpy(&m_stcWaveformat, &stcWaveformat, sizeof(WAVEFORMATEX));
		}
		
		if (m_hWaveOut == NULL)
		{
			if (!OpenPlayDev(uiDevID, &stcWaveformat, m_hWaveOut))
			{
				bRet = FALSE;
				break;
			}
		}
		
		if (!SetPlayData(m_hWaveOut, m_pWavData, m_dwDataSize, m_stcWaveOutHdr))
		{
			bRet = FALSE;
			break;
		}
		
		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

BOOL CWavePlayer::GetPlayData(const char* pszWavFilePath, WAVEFORMATEX *pWaveformat, BYTE **pWavData, DWORD &dwWavDataSize)
{
	BOOL bRet = FALSE;

	UINT uiWaveSize = 0;
	UINT uiReadSize = 0;

	MMCKINFO mmckinfoParent = {0};
	MMCKINFO mmckinfoSubChunk = {0};

	HMMIO hMmioFile;
	WAVEFORMATEX *lpFormat = NULL;
	
	do
	{
		if (pszWavFilePath == NULL || pWaveformat == NULL)
		{
			bRet = FALSE;
			break;
		}
		
		if (!OpenWavFile(pszWavFilePath, mmckinfoParent, mmckinfoSubChunk, hMmioFile, &lpFormat))
		{
			bRet = FALSE;
			break;
		}
		
		if (!ResetWavFile(hMmioFile, mmckinfoParent, mmckinfoSubChunk))
		{
			bRet = FALSE;
			break;
		}
		
		uiWaveSize = mmckinfoSubChunk.cksize;
		if ((*pWavData = (BYTE*)GlobalAlloc(GMEM_FIXED, uiWaveSize)) == NULL)
		{
			bRet = FALSE;
			break;
		}
		
		if (!ReadWavFile(hMmioFile, uiWaveSize, pWavData, mmckinfoSubChunk, uiReadSize))
		{
			bRet = FALSE;
			break;
		}
		
		dwWavDataSize = uiWaveSize;
		memcpy(pWaveformat, lpFormat, sizeof(WAVEFORMATEX));
		
		bRet = TRUE;
	} while (FALSE);
	
	if (!bRet)
	{
		if (pWavData != NULL)
		{
			GlobalFree(pWavData);
			pWavData = NULL;
		}
	}
	
	CloseWavFile(hMmioFile, lpFormat);
	return bRet;
}

BOOL CWavePlayer::OpenWavFile(const char *pszPlayerFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk, HMMIO &hMmioFile, WAVEFORMATEX **pWavFormat)
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
			*pWavFormat = (WAVEFORMATEX*)new char[sizeof(WAVEFORMATEX)];
			if (*pWavFormat == NULL)
			{
				bRet = FALSE;
				strPrompt = _T("WAVEFORMATEX结构内存创建, 请检查!");
				break;
			}
			memset(*pWavFormat, 0x0, sizeof(WAVEFORMATEX));

			memcpy(*pWavFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
			(*pWavFormat)->cbSize = 0;
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

			*pWavFormat = (WAVEFORMATEX *)new char[sizeof(WAVEFORMATEX) + cbExtraBytes];
			if (*pWavFormat == NULL)
			{
				bRet = FALSE;
				strPrompt = _T("WAVEFORMATEX结构内存创建, 请检查!");
				break;
			}
			memset(*pWavFormat, 0x0, sizeof(WAVEFORMATEX));

			memcpy(*pWavFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
			(*pWavFormat)->cbSize = cbExtraBytes;

			if (mmioRead(hMmioFile, (CHAR*)(((BYTE*)&((*pWavFormat)->cbSize)) + sizeof(WORD)), cbExtraBytes) != cbExtraBytes)
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
		if (*pWavFormat != NULL)
		{
			delete[] *pWavFormat;
			*pWavFormat = NULL;
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

void CWavePlayer::CloseWavFile(HMMIO hMmioFile, WAVEFORMATEX *pWavFormat)
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
}

BOOL CWavePlayer::ResetWavFile(HMMIO hMmioFile, MMCKINFO &ckParent, MMCKINFO &ckSubChunk)
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

BOOL CWavePlayer::ReadWavFile(HMMIO hMmioFile, UINT uiRead, BYTE **pbDataBuf, MMCKINFO &ckSubChunk, UINT &uiReadSize)
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

			bRet = TRUE;
			*((BYTE*)*pbDataBuf+uiIndex) = *((BYTE*&)mmioinfoIn.pchNext)++;
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

BOOL CWavePlayer::OpenPlayDev(UINT uiDevID, WAVEFORMATEX *pWaveformat, HWAVEOUT &hWaveOut)
{
	BOOL bRet = FALSE;

	UINT uiDevNum = 0;
	MMRESULT hResult = 0;

	WAVEOUTCAPS pwoc;
	CString strPrompt;

	do 
	{
		uiDevNum = waveOutGetNumDevs();
		if (uiDevNum == 0)
		{
			bRet = FALSE;
			strPrompt = _T("获取媒体输出设备失败, 请检查!");
			break;
		}

		if (uiDevID > (uiDevNum-1))
		{
			bRet = FALSE;
			strPrompt = _T("媒体设备输出ID发生错误, 请检查!");
			break;
		}

		//查询输出设备的性能
		hResult = waveOutGetDevCaps(uiDevID, &pwoc, sizeof(WAVEOUTCAPS));	//WAVE_MAPPER
		if (hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("waveOutGetDevCaps失败, 请检查!");
			break;
		}

		hResult = waveOutOpen(&hWaveOut, uiDevID, pWaveformat, (DWORD_PTR)WaveOutCallBackProc, (DWORD)this, CALLBACK_FUNCTION);	//WAVE_MAPPER //CALLBACK_FUNCTION	//CALLBACK_NULL
		if (hResult != 0)
		{
			bRet = FALSE;
			strPrompt = _T("waveOutOpen失败, 请检查!");
			break;
		}

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

void CWavePlayer::ClosePlayDev(HWAVEOUT hWaveOut)
{
	if (m_hWaveOut == NULL)
	{
		return;
	}

	if (waveOutClose(m_hWaveOut) == 0)
	{
		m_hWaveOut = NULL;
	}
}

BOOL CWavePlayer::SetPlayData(HWAVEOUT hWaveOut, HPSTR pWavData, DWORD dwWavDataSize, WAVEHDR &stcWaveOutHdr)
{
	BOOL bRet = FALSE;
	CString strPrompt;

	do 
	{
		if (hWaveOut == NULL)
		{
			bRet = FALSE;
			break;
		}

		stcWaveOutHdr.lpData  = (HPSTR)pWavData;
		stcWaveOutHdr.dwBufferLength = dwWavDataSize;
		stcWaveOutHdr.dwFlags = 0;
		stcWaveOutHdr.dwBytesRecorded = 0;
		stcWaveOutHdr.dwUser  = NULL;
		stcWaveOutHdr.dwLoops = 0;
		stcWaveOutHdr.lpNext  = NULL;
		stcWaveOutHdr.reserved = NULL;

		if (waveOutPrepareHeader(hWaveOut, &stcWaveOutHdr, sizeof(WAVEHDR)) != 0)
		{
			bRet = FALSE;
			strPrompt = _T("waveOutPrepareHeader失败, 请检查!");
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		if (waveOutUnprepareHeader(hWaveOut, &stcWaveOutHdr, sizeof(WAVEHDR)) == 0)
		{
			if (hWaveOut != NULL)
			{
				waveOutReset(hWaveOut);
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
BOOL CWavePlayer::PlayDevData(HWAVEOUT hWaveOut, WAVEHDR stcWaveOutHdr, HANDLE hEvent)
{
	BOOL bRet = FALSE;
	MMRESULT hResult = 0;
	
	do 
	{
		if (hWaveOut == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (stcWaveOutHdr.lpData == NULL)
		{
			bRet = FALSE;
			break;
		}

		hResult = waveOutWrite(hWaveOut, &stcWaveOutHdr, sizeof(WAVEHDR));
		if (hResult != 0)
		{
			bRet = FALSE;
			break;
		}

		WaitForSingleObject(hEvent, INFINITE);
		ResetEvent(hEvent);

		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

void CWavePlayer::ClearPlayDev(HWAVEOUT hWaveOut, WAVEHDR &stcWaveOutHdr)
{
	BOOL bRet = FALSE;
	MMRESULT hResult = 0;

	do 
	{
		if (hWaveOut == NULL)
		{
			bRet = FALSE;
			break;
		}

		hResult = waveOutReset(hWaveOut);
		if (hResult != 0)
		{
			bRet = FALSE;
			break;
		}

		hResult = waveOutUnprepareHeader(hWaveOut, &stcWaveOutHdr, sizeof(WAVEHDR));
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
			break;
		}

		memset(&stcWaveOutHdr, 0x0, sizeof(WAVEHDR));
		bRet = TRUE;
	} while (FALSE);
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CWavePlayer::CreatePlayerProc(const char* pszWavFilePath, UINT uiDevID, WAVE_PLAYMODE hPlayMode,UINT nSpanTime)
{
	BOOL bRet = FALSE;

	do
	{
		ClosePlayerProc();
		if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
		{
			SetEvent(m_hStartEvent);
			ResetEvent(m_hEndEvent);
			
			m_hPlayMode = hPlayMode;
			
			m_dwWaitTime = nSpanTime;
			if (!SetPlayData(pszWavFilePath, uiDevID))
			{
				bRet = FALSE;
				break;
			}
			
			m_hThread = CreateThread(NULL, 0, WavePlayerThreadProc, (LPVOID)this, 0, &m_dwThreadID);
			if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
			{
				bRet = FALSE;
				break;
			}
		
			m_bExit = FALSE;
			bRet = TRUE;
		}
	} while (FALSE);

	if (!bRet)
	{
		ResetEvent(m_hStartEvent);
	}
	
	return bRet;
}

void CWavePlayer::ClosePlayerProc()
{
	m_bExit = TRUE;
	if (m_hThread == NULL)
	{
		return;
	}

	if (m_hWaveOut != NULL && m_hWaveOut != INVALID_HANDLE_VALUE)
	{
		waveOutPause(m_hWaveOut);
		SetEvent(m_hEndPlayEvent);
	}
	else
	{
		SetEvent(m_hEndEvent);
	}

	WaitForSingleObject(m_hEndEvent, INFINITE);
	ClosePlayDev(m_hWaveOut);
	
	if (m_pWavData != NULL)
	{
		GlobalFree(m_pWavData);
		m_pWavData = NULL;
	}

	if (m_hThread != NULL && m_hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	m_bExit = FALSE;
	ResetEvent(m_hEndPlayEvent);
	ResetEvent(m_hStartEvent);
}