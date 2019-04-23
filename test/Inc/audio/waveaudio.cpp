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

	m_stWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	m_stWaveFormat.nChannels = 1;					//采样声道数,对于单声道音频设置为1,立体声设置为2;
	m_stWaveFormat.cbSize = 0;						//一般为0;
	m_stWaveFormat.wBitsPerSample = 16;				//采样比特,16bits/次;
	m_stWaveFormat.nSamplesPerSec = 16000;			//采样率,16000次/秒;
	m_stWaveFormat.nBlockAlign = 2;					//一个块的大小,采样bit的字节数乘以声道数;	
	m_stWaveFormat.nAvgBytesPerSec = 32000;			//每秒的数据率,就是每秒能采集多少字节的数据;

	m_pSpeechServer = NULL;

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

void CWaveAudio::WaveAudioCallBackProc(HWAVEIN hwavein, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	BOOL bRet = FALSE;

	CWaveAudio *pWaveAudio = NULL;

	do 
	{
		pWaveAudio = (CWaveAudio *)dwInstance;
		if (pWaveAudio == NULL)
		{
			bRet = FALSE;
			break;
		}

		switch (uMsg)
		{
		case WOM_DONE:
			{
				TRACE(_T("waveInStart msg : WOM_DONE"));
			}
			break;

		case WOM_CLOSE:
			{
				TRACE(_T("waveInClose msg : WOM_CLOSE"));
			}
			break;

		case WOM_OPEN:
			{
				TRACE(_T("waveInOpen msg : WOM_OPEN"));
			}
			break;

		case WIM_DATA:
			{
				TRACE(_T("waveInData msg : WIM_DATA"));

				//waveInAddBuffer(hwavein, (PWAVEHDR)dwParam1, sizeof(WAVEHDR));
			}
		}

		bRet = TRUE;
	} while (FALSE);
}

//////////////////////////////////////////////////////////////////////////
//
void CWaveAudio::AudioWavInfo()
{
	BOOL bRet = FALSE;

	do 
	{
// 		if (!SetAudioDev(TRUE, m_hWaveIn))
// 		{
// 			bRet = FALSE;
// 			break;
// 		}

		while(WaitForSingleObject(m_hEndEvent, m_dwWaitTime) != WAIT_OBJECT_0)
		{
			if (!m_bExit)
			{
				TRACE("test1");
			}
			else
			{
				m_dwWaitTime = 0;

				SetEvent(m_hEndEvent);
				continue;
			}
		}

		TRACE("test2");
		bRet = TRUE;
	} while (FALSE);
}

BOOL CWaveAudio::OpenAudioData(UINT uiDevID)
{
	BOOL bRet = FALSE;

	do 
	{
		//ClearAudioData(m_hWaveIn);

		if (m_hWaveIn == NULL)
		{
			if (!OpenAudioDev(uiDevID, &m_stWaveFormat, m_hWaveIn))
			{
				bRet = FALSE;
				break;
			}
		}

// 		if (!SetAudioData(m_hWaveIn, &m_stWaveFormat))
// 		{
// 			bRet = FALSE;
// 			break;
// 		}

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
				waveInUnprepareHeader(hWaveIn, &m_stWaveHdr[uiIndex], sizeof(WAVEHDR));
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

		for (uiIndex=0; uiIndex<AUDIO_HDRCOUNT; ++uiIndex)
		{
			dwDataSize = pWaveformat->nBlockAlign*pWaveformat->nSamplesPerSec;

			m_stWaveHdr[uiIndex].lpData = (LPSTR)new char[dwDataSize];
			m_stWaveHdr[uiIndex].dwBufferLength = dwDataSize;
			m_stWaveHdr[uiIndex].dwFlags = 0;
			m_stWaveHdr[uiIndex].dwBytesRecorded = 0;
			m_stWaveHdr[uiIndex].dwUser = uiIndex;
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
BOOL CWaveAudio::OpenAudioDev(UINT uiDevID, WAVEFORMATEX *pWaveformat, HWAVEIN &hWaveIn)
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

		hResult = waveInOpen(&hWaveIn, uiDevID, pWaveformat, (DWORD_PTR)WaveAudioCallBackProc, (DWORD)this, CALLBACK_FUNCTION);
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
		CloseAudioProc();
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

		if (m_hWaveIn != NULL && m_hWaveIn != INVALID_HANDLE_VALUE)
		{
			SetAudioEvent(FALSE, m_hWaveIn);
		}
		else
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