#include "stdafx.h"
#include "AudioWaveApi.h"

#include <sstream>

struct WaveHandle {
	HWAVEIN		hWaveIn;
	HWAVEOUT	hWaveOut;
	PWAVEHDR	WaveHdrBuff;

	struct WaveHandle() {
		hWaveIn = NULL;
		hWaveOut = NULL;
		WaveHdrBuff = NULL;
	}
};

CAudioWaveAPi::CAudioWaveAPi()
{
	m_nError = 0;

	m_devHandle = NULL;
	m_audioNotifyHandler = NULL;
}

CAudioWaveAPi::~CAudioWaveAPi()
{
}

int CAudioWaveAPi::audio_geterror()
{
	return m_nError;
}

bool CAudioWaveAPi::audio_init()
{
	return true;
}

void CAudioWaveAPi::audio_uninit()
{
}

bool CAudioWaveAPi::audio_enumDevice(DevMode devMode, std::vector<CDevData> &vecDevData)
{
	return audio_getDevInfo(devMode, vecDevData);
}

bool CAudioWaveAPi::audio_openDevice(AudioPcmFormat stAudioFormat, CDevData devData)
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;

	do 
	{
		m_devHandle = new CDevHandle;
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = new WaveHandle;
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		WAVEFORMATEX stWaveFormat = {0};
		UINT uiDevId = atol(devData.GetDevId().c_str());

		//IAudioData audioData = devData.GetDeviceAudio();
		//memcpy(&stWaveFormat, audioData.GetWaveFormat(), sizeof(WAVEFORMATEX));

		if (devData.GetDevMode() == DEV_RENDERMODE)
		{//播放
			HWAVEOUT hWaveOut;
			mmr = waveOutOpen(&hWaveOut, uiDevId, const_cast<WAVEFORMATEX*>(&stWaveFormat), reinterpret_cast<DWORD_PTR>(CAudioWaveAPi::audio_waveOutProc), reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION);
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}

			pWaveHandle->hWaveOut = hWaveOut;
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{//采集
			HWAVEIN hWaveIn;
			mmr = waveInOpen(&hWaveIn, uiDevId, const_cast<WAVEFORMATEX*>(&stWaveFormat), reinterpret_cast<DWORD_PTR>(CAudioWaveAPi::audio_waveInProc), reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION);
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}

			pWaveHandle->hWaveIn = hWaveIn;
		}

		LONG lDeviceState = 0;
		::InterlockedExchange(&lDeviceState, 1);

		m_devHandle->SetDevData(devData);
		m_devHandle->SetApiHandle(pWaveHandle);

		bRet = true;
	} while (false);

	if (!bRet)
	{
		if (pWaveHandle != NULL)
		{
			delete pWaveHandle;
			pWaveHandle = NULL;
		}

		if (m_devHandle != NULL)
		{
			delete m_devHandle;
			m_devHandle = NULL;
		}
	}
	
	return bRet;
}

void CAudioWaveAPi::audio_closeDevice()
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;

	do 
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevData devData = m_devHandle->GetDevData();

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_startStream()
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;

	do 
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevData devData = m_devHandle->GetDevData();
		if (devData.GetDevMode() == DEV_RENDERMODE)
		{
			if (pWaveHandle->hWaveOut != NULL)
			{
				mmr = waveOutRestart(pWaveHandle->hWaveOut);
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{
			if (pWaveHandle->hWaveIn != NULL)
			{
				mmr = waveInStart(pWaveHandle->hWaveIn);
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_closeStream()
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;

	do 
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

 		CDevData devData = m_devHandle->GetDevData();
		if (devData.GetDevMode() == DEV_RENDERMODE)
		{
			if (pWaveHandle->hWaveOut != NULL)
			{
				mmr = waveOutClose(pWaveHandle->hWaveOut); 
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}

				pWaveHandle->hWaveOut = NULL;
			}
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{
			if (pWaveHandle->hWaveIn != NULL)
			{
				mmr = waveInClose(pWaveHandle->hWaveIn);
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}

				pWaveHandle->hWaveIn = NULL;
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_stopStream()
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;

	do 
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevData devData = m_devHandle->GetDevData();
		if (devData.GetDevMode() == DEV_RENDERMODE)
		{
			if (pWaveHandle->hWaveOut != NULL)
			{
				mmr = waveOutReset(pWaveHandle->hWaveOut);
				if (mmr == MMSYSERR_NOERROR)
				{
					mmr = waveOutPause(pWaveHandle->hWaveOut);
				}
			}
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{
			if (pWaveHandle->hWaveIn != NULL)
			{
				mmr = waveInReset(pWaveHandle->hWaveIn);
				if (mmr == MMSYSERR_NOERROR)
				{
					mmr = waveInStop(pWaveHandle->hWaveIn);
				}
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_abortStream()
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;

	do 
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevData devData = m_devHandle->GetDevData();
		if (devData.GetDevMode() == DEV_RENDERMODE)
		{
			if (pWaveHandle->hWaveOut != NULL)
			{
				mmr = waveOutReset(pWaveHandle->hWaveOut);
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{
			if (pWaveHandle->hWaveIn != NULL)
			{
				mmr = waveInReset(pWaveHandle->hWaveIn);
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_addBuffer(IDataBuffer &dataBuff)
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;
	
	do
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pWaveHandle->WaveHdrBuff == NULL)
		{
			pWaveHandle->WaveHdrBuff = new WAVEHDR;
			if (pWaveHandle->WaveHdrBuff == NULL)
			{
				bRet = false;
				break;
			}
			memset(pWaveHandle->WaveHdrBuff, 0x0, sizeof(WAVEHDR));
		}

// 		pWaveApiHandle->pWaveHdrBuff->lpData = (LPSTR)pszDataBuff;
// 		pWaveApiHandle->pWaveHdrBuff->dwBufferLength = nBuffSize;
// 		pWaveApiHandle->pWaveHdrBuff->dwFlags = 0;
// 		pWaveApiHandle->pWaveHdrBuff->dwBytesRecorded = 0;
// 		pWaveApiHandle->pWaveHdrBuff->dwUser = 0;
// 		pWaveApiHandle->pWaveHdrBuff->dwLoops = 0;
// 		pWaveApiHandle->pWaveHdrBuff->lpNext = NULL;
// 		pWaveApiHandle->pWaveHdrBuff->reserved = NULL;

		CDevData devData = m_devHandle->GetDevData();
		if (devData.GetDevMode() == DEV_RENDERMODE)
		{
			if (pWaveHandle->hWaveOut != NULL)
			{
				mmr = waveOutPrepareHeader(pWaveHandle->hWaveOut, pWaveHandle->WaveHdrBuff, sizeof(WAVEHDR));
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{
			if (pWaveHandle->hWaveIn != NULL)
			{
				mmr = waveInPrepareHeader(pWaveHandle->hWaveIn, pWaveHandle->WaveHdrBuff, sizeof(WAVEHDR));
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		
		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_openBuffer(IDataBuffer &dataBuff)
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;

	do 
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevData devData = m_devHandle->GetDevData();
		if (devData.GetDevMode() == DEV_RENDERMODE)
		{
			if (pWaveHandle->hWaveOut != NULL)
			{
				mmr = waveOutWrite(pWaveHandle->hWaveOut, pWaveHandle->WaveHdrBuff, sizeof(WAVEHDR));
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{
			if (pWaveHandle->hWaveIn != NULL)
			{
				mmr = waveInAddBuffer(pWaveHandle->hWaveIn, pWaveHandle->WaveHdrBuff, sizeof(WAVEHDR));
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_closeBuffer(IDataBuffer &dataBuff)
{
	bool bRet = false;

	MMRESULT mmr;
	WaveHandle *pWaveHandle = NULL;
	
	do
	{
		if (m_devHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveHandle = (WaveHandle*)m_devHandle->GetApiHandle();
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevData devData = m_devHandle->GetDevData();
		if (devData.GetDevMode() == DEV_RENDERMODE)
		{
			if (pWaveHandle->hWaveOut != NULL)
			{
				mmr = waveOutUnprepareHeader(pWaveHandle->hWaveOut, pWaveHandle->WaveHdrBuff, sizeof(WAVEHDR));
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		else if (devData.GetDevMode() == DEV_CAPTUREMODE)
		{
			if (pWaveHandle->hWaveIn != NULL)
			{
				mmr = waveInUnprepareHeader(pWaveHandle->hWaveIn, pWaveHandle->WaveHdrBuff, sizeof(WAVEHDR));
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		
		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_setNotifyHandler(IAudioNotifyHandler *pNotify)
{
	if (pNotify != NULL)
	{
		if (m_audioNotifyHandler == NULL)
		{
			::InterlockedExchangePointer(reinterpret_cast<void**>(&m_audioNotifyHandler), pNotify);
		}
	}
	else
	{
		::InterlockedExchangePointer(reinterpret_cast<void**>(&m_audioNotifyHandler), NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
//
void CAudioWaveAPi::audio_waveInProc(HWAVEIN hWaveIn, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CAudioWaveAPi *pAudioWaveApi = reinterpret_cast<CAudioWaveAPi*>(dwInstance);
	if (pAudioWaveApi)
	{
		pAudioWaveApi->audio_waveProcEvent(DEV_CAPTUREMODE, uiMsg, dwParam1, dwParam2);
	}
}

void CAudioWaveAPi::audio_waveOutProc(HWAVEOUT hWaveOut, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CAudioWaveAPi *pAudioWaveApi = reinterpret_cast<CAudioWaveAPi*>(dwInstance);
	if (pAudioWaveApi)
	{
		pAudioWaveApi->audio_waveProcEvent(DEV_RENDERMODE, uiMsg, dwParam1, dwParam2);
	}
}

void CAudioWaveAPi::audio_waveProcEvent(UINT uiDevMode, UINT uiMsg, DWORD dwParam1, DWORD dwParam2)
{
	switch (uiMsg)
	{
	case MM_WIM_OPEN:
		break;

	case MM_WIM_CLOSE:
		break;

	case MM_WIM_DATA:
		break;

	default:
		break;
	}
}

void CAudioWaveAPi::audio_waveProcData(HWAVEIN hWaveIn, WAVEHDR *pWaveHdr)
{
	bool bRet = false;

	do 
	{
		waveInUnprepareHeader(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
		if (hWaveIn == NULL || pWaveHdr == NULL)
		{
			bRet = false;
			break;
		}

		if (pWaveHdr->lpData == NULL)
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);
}

bool CAudioWaveAPi::audio_getDevInfo(DevMode devMode, std::vector<CDevData> &vecDevInfo)
{
	bool bRet = false;

	MMRESULT mmr;
	std::ostringstream ostr;

	do 
	{
		if (devMode == DEV_EMPTYMODE)
		{
			bRet = false;
			break;
		}

		UINT uiDevCount = 0;
		if (devMode == DEV_RENDERMODE)
		{//播放
			uiDevCount = waveOutGetNumDevs();
		}
		else if (devMode == DEV_CAPTUREMODE)
		{//采集
			uiDevCount = waveInGetNumDevs();
		}

		std::string strDevId;
		std::string strDevName;

		for (UINT uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
		{
			ostr << uiIndex << endl;
			strDevId = ostr.str();

			if (devMode == DEV_RENDERMODE)
			{
				WAVEOUTCAPS waveOutcaps;
				mmr = waveOutGetDevCaps(uiIndex, &waveOutcaps, sizeof(WAVEOUTCAPS));
				if (mmr != MMSYSERR_NOERROR)
				{
					continue;
				}

				strDevName = waveOutcaps.szPname;
			}
			else if (devMode == DEV_CAPTUREMODE)
			{
				WAVEINCAPS waveIncaps;
				mmr = waveInGetDevCaps(uiIndex, &waveIncaps, sizeof(WAVEINCAPS));
				if (mmr != MMSYSERR_NOERROR)
				{
					continue;
				}

				strDevName = waveIncaps.szPname;
			}

			CDevData devData;
			devData.SetDevType(DEV_AUDIOTYPE);
			devData.SetDevState(DEV_ACTIVESTATE);
			devData.SetDevMode(devMode);

			
			devData.SetDevId(strDevId);
			devData.SetDevName(strDevName);

			vecDevInfo.push_back(devData);
		}

		bRet = true;
	} while (false);

	return bRet;
}