#include "stdafx.h"
#include "waveaudio.h"

#pragma comment(lib,"Winmm.lib")
CWaveAudio::CWaveAudio()
{
	m_bExit = FALSE;

	m_dwThreadID = 0;
	m_dwWaitTime = 0;

	m_hThread = NULL;
	m_hWaveIn = NULL;
	m_pSpeechServer = NULL;

	m_stWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	m_stWaveFormat.nChannels = 1;					//采样声道数,对于单声道音频设置为1,立体声设置为2;
	m_stWaveFormat.cbSize = 0;						//一般为0;
	m_stWaveFormat.wBitsPerSample = 16;				//采样比特,16bits/次;
	m_stWaveFormat.nSamplesPerSec = 16000;			//采样率,16000次/秒;
	m_stWaveFormat.nBlockAlign = (m_stWaveFormat.wBitsPerSample*m_stWaveFormat.nChannels)>>3;		//一个块的大小,采样bit的字节数乘以声道数;	
	m_stWaveFormat.nAvgBytesPerSec = (m_stWaveFormat.nBlockAlign*m_stWaveFormat.nSamplesPerSec);	//每秒的数据率,就是每秒能采集多少字节的数据;

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CWaveAudio::~CWaveAudio()
{
	CloseAudioProc();
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
}

//////////////////////////////////////////////////////////////////////////
//
DWORD CWaveAudio::WaveAudioThreadProc(LPVOID lpParam)
{
	CWaveAudio *pWaveAudioProc = (CWaveAudio*)lpParam;
	if (pWaveAudioProc != NULL)
	{
		pWaveAudioProc->AudioWavInfo();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void CWaveAudio::AudioWavInfo()
{
	BOOL bRet = FALSE;

	MSG msg;
	HWAVEIN hWaveIn = NULL;
	WAVEHDR* pWaveHdr = NULL;

	do 
	{
		while(WaitForSingleObject(m_hEndEvent, m_dwWaitTime) != WAIT_OBJECT_0)
		{
			if (!m_bExit)
			{
				PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
				if (GetMessage(&msg, NULL, 0, 0))
				{
					if (msg.message == MM_WIM_OPEN)
					{
						TRACE("MM_WIM_OPEN");
					}
					else if (msg.message == MM_WIM_CLOSE)
					{
						TRACE("MM_WIM_CLOSE");
						m_bExit = TRUE;
					}
					else if (msg.message == MM_WIM_DATA)
					{
						TRACE("MM_WIM_DATA");

						hWaveIn = (HWAVEIN)msg.wParam;
						pWaveHdr = (WAVEHDR*)msg.lParam;

						if (hWaveIn == NULL || pWaveHdr == NULL)
						{
							continue;
						}

						if (pWaveHdr->lpData != NULL)
						{
							waveInUnprepareHeader(hWaveIn, pWaveHdr, sizeof(WAVEHDR));

							waveInPrepareHeader(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
							waveInAddBuffer(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
						}
					}
				}
			}
			else
			{
				TRACE("ClearAudioData");
				ClearAudioData(m_hWaveIn);

				SetEvent(m_hEndEvent);
				break;
			}
		}

		bRet = TRUE;
	} while (FALSE);
}

BOOL CWaveAudio::OpenAudioData(UINT uiDevID)
{
	BOOL bRet = FALSE;

	do 
	{
		ClearAudioData(m_hWaveIn);
		if (m_hWaveIn == NULL)
		{
			if (!OpenAudioDev(uiDevID, m_dwThreadID, &m_stWaveFormat, m_hWaveIn))
			{
				bRet = FALSE;
				break;
			}
		}

		if (!SetAudioData(m_hWaveIn, &m_stWaveFormat))
		{
			bRet = FALSE;
			break;
		}

		if (!SetAudioDev(TRUE, m_hWaveIn))
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CWaveAudio::SetAudioEvent(BOOL bFlag, HWAVEIN hWaveIn)
{
	BOOL bRet = FALSE;

	do 
	{
		if (!bFlag)
		{//暂停采集
			if (hWaveIn != NULL)
			{
				if (waveInStop(hWaveIn) != 0)
				{
					bRet = FALSE;
					break;
				}

				if (waveInReset(hWaveIn) != 0)
				{
					bRet = FALSE;
					break;
				}
			}
		}
		else
		{//开始采集
			if (hWaveIn != NULL)
			{
				if (waveInStart(hWaveIn) != 0)
				{
					bRet = FALSE;
					break;
				}
			}
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CWaveAudio::ClearAudioData(HWAVEIN hWaveIn)
{
	BOOL bRet = FALSE;

	UINT uiIndex = 0;

	do 
	{
		SetAudioEvent(FALSE, hWaveIn);
		if (hWaveIn != NULL)
		{
			for (uiIndex=0; uiIndex<AUDIO_HDRCOUNT; ++uiIndex)
			{
				if (m_stWaveHdr[uiIndex].lpData != NULL)	//WHDR_PREPARED & header->dwFlags
				{
					waveInUnprepareHeader(hWaveIn, &m_stWaveHdr[uiIndex], sizeof(WAVEHDR));
					delete[] m_stWaveHdr[uiIndex].lpData;
					m_stWaveHdr[uiIndex].lpData = NULL;
				}
				
			}
		}

		bRet = TRUE;
	} while (FALSE);
}

BOOL CWaveAudio::SetAudioData(HWAVEIN hWaveIn, WAVEFORMATEX *pWaveformat)
{
	BOOL bRet = FALSE;

	UINT uiIndex = 0;
	DWORD dwDataSize = 0;

	do 
	{
		if (hWaveIn == NULL || pWaveformat == NULL)
		{
			bRet = FALSE;
			break;
		}

		dwDataSize = pWaveformat->nBlockAlign*pWaveformat->nSamplesPerSec;

		for (uiIndex=0; uiIndex<AUDIO_HDRCOUNT; ++uiIndex)
		{			
			m_stWaveHdr[uiIndex].lpData = (LPSTR)new char[dwDataSize];
			m_stWaveHdr[uiIndex].dwBufferLength = dwDataSize;
			m_stWaveHdr[uiIndex].dwFlags = 0;
			m_stWaveHdr[uiIndex].dwBytesRecorded = 0;
			m_stWaveHdr[uiIndex].dwUser = 0;
			m_stWaveHdr[uiIndex].dwLoops = 0;
			m_stWaveHdr[uiIndex].lpNext = NULL;
			m_stWaveHdr[uiIndex].reserved = NULL;

			if (waveInPrepareHeader(hWaveIn, &m_stWaveHdr[uiIndex], sizeof(WAVEHDR)) != 0)
			{
				continue;
			}

			if (waveInAddBuffer(hWaveIn, &m_stWaveHdr[uiIndex], sizeof(WAVEHDR)) != 0)
			{
				continue;
			}			
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CWaveAudio::OpenAudioDev(UINT uiDevID, UINT uiThreadId, WAVEFORMATEX *pWaveformat, HWAVEIN &hWaveIn)
{
	BOOL bRet = FALSE;

	UINT uiDevNum = 0;
	MMRESULT hResult = 0;

	WAVEINCAPS pwic;

	do 
	{
		uiDevNum = waveInGetNumDevs();
		if (uiDevNum == 0)
		{
			bRet = FALSE;
			break;
		}

		if (uiDevID > (uiDevNum-1))
		{
			bRet = FALSE;
			break;
		}

		hResult = waveInGetDevCaps(uiDevID, &pwic, sizeof(WAVEINCAPS));
		if (hResult != 0)
		{
			bRet = FALSE;
			break;
		}

		hResult = waveInOpen(&hWaveIn, uiDevID, pWaveformat, (DWORD_PTR)uiThreadId, (DWORD)this, CALLBACK_THREAD);	//CALLBACK_FUNCTION
		if (hResult != 0)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CWaveAudio::CloseAudioDev(HWAVEIN hWaveIn)
{
	BOOL bRet = FALSE;

	do 
	{
		if (hWaveIn == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (waveInClose(hWaveIn) != 0)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);
}

BOOL CWaveAudio::SetAudioDev(BOOL bFlag, HWAVEIN hWaveIn)
{
	BOOL bRet = FALSE;

	do 
	{
		if (!bFlag)
		{
			if (!SetAudioEvent(FALSE, hWaveIn))
			{
				bRet = FALSE;
				break;
			}
		}
		else
		{
			if (!SetAudioEvent(TRUE, hWaveIn))
			{
				bRet = FALSE;
				break;
			}
		}
		
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CWaveAudio::CreateAudioProc(UINT uiDevID, UINT nSpanTime)
{
	BOOL bRet = FALSE;

	do 
	{
		if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
		{
			m_hThread = CreateThread(NULL, 0, WaveAudioThreadProc, (LPVOID)this, 0, &m_dwThreadID);
			if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
			{
				bRet = FALSE;
				break;
			}

			if (!OpenAudioData(uiDevID))
			{
				bRet = FALSE;
				break;
			}

			SetEvent(m_hStartEvent);
			ResetEvent(m_hEndEvent);

			m_bExit = FALSE;
			m_dwWaitTime = nSpanTime;

			bRet = TRUE;
		}		
	} while (FALSE);

	return bRet;
}

void CWaveAudio::CloseAudioProc()
{
	BOOL bRet = FALSE;

	do 
	{
		if (m_hThread == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (m_hWaveIn == NULL || m_hWaveIn == INVALID_HANDLE_VALUE)
		{
			SetEvent(m_hEndEvent);
		}

		m_bExit = TRUE;
		WaitForSingleObject(m_hEndEvent, INFINITE);
		CloseAudioDev(m_hWaveIn);

		if (m_hThread != NULL && m_hThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		m_bExit = FALSE;
		ResetEvent(m_hStartEvent);

		bRet = TRUE;
	} while (FALSE);
}

void CWaveAudio::SetParentProc(void *pSpeechServer)
{
	m_pSpeechServer = pSpeechServer;
}