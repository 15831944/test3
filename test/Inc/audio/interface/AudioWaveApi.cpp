#include "stdafx.h"
#include "AudioWaveApi.h"

#include <sstream>

struct WaveApiHandle {
	HWAVEIN		hWaveIn;
	HWAVEOUT	hWaveOut;
	PWAVEHDR	pWaveHdrBuff;

	struct WaveApiHandle() {
		hWaveIn = NULL;
		hWaveOut = NULL;
		pWaveHdrBuff = NULL;
	}
};

CAudioWaveAPi::CAudioWaveAPi()
{
	m_nError = 0;
	m_pDevHandle = NULL;
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

bool CAudioWaveAPi::audio_enumDevice(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecAudioDev)
{
	return audio_getDevInfo(emDevMode, vecAudioDev);
}

bool CAudioWaveAPi::audio_openDevice(CDeviceInfo *pDevInfo)
{
	bool bRet = false;
	UINT uiDevId;

	WAVEFORMATEX stWaveFormat = {0};
	
	MMRESULT mmr;
	CAudioData *pAudioData = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		m_pDevHandle = new CDeviceHandle;
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = new WaveApiHandle;
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		pAudioData = pDevInfo->GetDeviceAudio();
		if (pAudioData != NULL)
		{
			memcpy(&stWaveFormat, pAudioData->GetWaveFormat(), sizeof(WAVEFORMATEX));
		}

		uiDevId = atol(pDevInfo->GetDeviceId().c_str());
		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{//播放
			HWAVEOUT hWaveOut;
			mmr = waveOutOpen(&hWaveOut, uiDevId, const_cast<WAVEFORMATEX*>(&stWaveFormat), reinterpret_cast<DWORD_PTR>(CAudioWaveAPi::audio_waveOutProc), reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION);
			if (mmr != MMSYSERR_NOERROR)
			{
				return false;
			}

			pWaveApiHandle->hWaveOut = hWaveOut;
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{//采集
			HWAVEIN hWaveIn;
			mmr = waveInOpen(&hWaveIn, uiDevId, const_cast<WAVEFORMATEX*>(&stWaveFormat), reinterpret_cast<DWORD_PTR>(CAudioWaveAPi::audio_waveInProc), reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION);
			if (mmr != MMSYSERR_NOERROR)
			{
				return false;
			}

			pWaveApiHandle->hWaveIn = hWaveIn;
		}

		LONG lDeviceState = 0;
		::InterlockedExchange(&lDeviceState, 1);

		m_pDevHandle->SetDevInfo(pDevInfo);
		m_pDevHandle->SetApiHandle(pWaveApiHandle);

		bRet = true;
	} while (false);

	if (!bRet)
	{
		if (pWaveApiHandle != NULL)
		{
			delete pWaveApiHandle;
			pWaveApiHandle = NULL;
		}

		if (m_pDevHandle != NULL)
		{
			delete m_pDevHandle;
			m_pDevHandle = NULL;
		}
	}
	
	return bRet;
}

void CAudioWaveAPi::audio_closeDevice()
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_startStream()
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{
			if (pWaveApiHandle->hWaveOut == NULL)
			{
				bRet = false;
				break;
			}

			mmr = waveOutRestart(pWaveApiHandle->hWaveOut);
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{
			if (pWaveApiHandle->hWaveIn == NULL)
			{
				bRet = false;
				break;
			}

			mmr = waveInStart(pWaveApiHandle->hWaveIn);
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_closeStream()
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{
			if (pWaveApiHandle->hWaveOut != NULL)
			{
				mmr = waveOutClose(pWaveApiHandle->hWaveOut); 
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}

				pWaveApiHandle->hWaveOut = NULL;
			}
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{
			if (pWaveApiHandle->hWaveIn != NULL)
			{
				mmr = waveInClose(pWaveApiHandle->hWaveIn);
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}

				pWaveApiHandle->hWaveIn = NULL;
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_stopStream()
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{
			if (pWaveApiHandle->hWaveOut != NULL)
			{
				mmr = waveOutReset(pWaveApiHandle->hWaveOut);
				if (mmr == MMSYSERR_NOERROR)
				{
					mmr = waveOutPause(pWaveApiHandle->hWaveOut);
				}
			}
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{
			if (pWaveApiHandle->hWaveIn != NULL)
			{
				mmr = waveInReset(pWaveApiHandle->hWaveIn);
				if (mmr == MMSYSERR_NOERROR)
				{
					mmr = waveInStop(pWaveApiHandle->hWaveIn);
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
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{
			if (pWaveApiHandle->hWaveOut != NULL)
			{
				mmr = waveOutReset(pWaveApiHandle->hWaveOut);
				if (mmr != MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
			}
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{
			if (pWaveApiHandle->hWaveIn != NULL)
			{
				mmr = waveInReset(pWaveApiHandle->hWaveIn);
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

void CAudioWaveAPi::audio_addBuffer(const char *pszDataBuff, int nBuffSize)
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;
	
	do
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pszDataBuff == NULL || nBuffSize <= 0)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pWaveApiHandle->pWaveHdrBuff == NULL)
		{
			pWaveApiHandle->pWaveHdrBuff = new WAVEHDR;
			if (pWaveApiHandle->pWaveHdrBuff == NULL)
			{
				bRet = false;
				break;
			}
			memset(pWaveApiHandle->pWaveHdrBuff, 0x0, sizeof(WAVEHDR));
		}

		pWaveApiHandle->pWaveHdrBuff->lpData = pszDataBuff;
		pWaveApiHandle->pWaveHdrBuff->dwBufferLength = nBuffSize;
		pWaveApiHandle->pWaveHdrBuff->dwFlags = 0;
		pWaveApiHandle->pWaveHdrBuff->dwBytesRecorded = 0;
		pWaveApiHandle->pWaveHdrBuff->dwUser = 0;
		pWaveApiHandle->pWaveHdrBuff->dwLoops = 0;
		pWaveApiHandle->pWaveHdrBuff->lpNext = NULL;
		pWaveApiHandle->pWaveHdrBuff->reserved = NULL;

		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{
			if (pWaveApiHandle->hWaveOut == NULL)
			{
				bRet = false;
				break;
			}

			mmr = waveOutPrepareHeader(pWaveApiHandle->hWaveOut, pWaveApiHandle->pWaveHdrBuff, sizeof(WAVEHDR));
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{
			if (pWaveApiHandle->hWaveIn == NULL)
			{
				bRet = false;
				break;
			}

			mmr = waveInPrepareHeader(pWaveApiHandle->hWaveIn, pWaveApiHandle->pWaveHdrBuff, sizeof(WAVEHDR));
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}
		
		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_openBuffer()
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{
			if (pWaveApiHandle->hWaveOut == NULL)
			{
				bRet = false;
				break;
			}

			mmr = waveOutWrite(pWaveApiHandle->hWaveOut, pWaveApiHandle->pWaveHdrBuff, sizeof(WAVEHDR));
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{
			if (pWaveApiHandle->hWaveIn == NULL)
			{
				bRet = false;
				break;
			}

			mmr = waveInAddBuffer(pWaveApiHandle->hWaveIn, pWaveApiHandle->pWaveHdrBuff, sizeof(WAVEHDR));
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_closeBuffer()
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;
	
	do
	{
		if (m_pDevHandle == NULL)
		{
			bRet = false;
			break;
		}

		pDevInfo = m_pDevHandle->GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)m_pDevHandle->GetApiHandle();
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}
		
		if (pDevInfo->GetDeviceMode() == DEVICE_RENDERMODE)
		{
			if (pWaveApiHandle->hWaveOut == NULL)
			{
				bRet = false;
				break;
			}
			
			mmr = waveOutUnprepareHeader(pWaveApiHandle->hWaveOut, pWaveApiHandle->pWaveHdrBuff, sizeof(WAVEHDR));
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}
		else if (pDevInfo->GetDeviceMode() == DEVICE_CAPTUREMODE)
		{
			if (pWaveApiHandle->hWaveIn == NULL)
			{
				bRet = false;
				break;
			}
			
			mmr = waveInPrepareHeader(pWaveApiHandle->hWaveIn, pWaveApiHandle->pWaveHdrBuff, sizeof(WAVEHDR));
			if (mmr != MMSYSERR_NOERROR)
			{
				bRet = false;
				break;
			}
		}
		
		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_waveProcEvent(UINT uiMsg, DWORD dwParam1, DWORD dwParam2)
{

}

//////////////////////////////////////////////////////////////////////////
//
void CAudioWaveAPi::audio_waveInProc(HWAVEIN hWaveIn, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CAudioWaveAPi *pAudioWaveApi = reinterpret_cast<CAudioWaveAPi*>(dwInstance);
	if (pAudioWaveApi)
	{
		pAudioWaveApi->audio_waveProcEvent(uiMsg, dwParam1, dwParam2);
	}
}

void CAudioWaveAPi::audio_waveOutProc(HWAVEOUT hWaveOut, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CAudioWaveAPi *pAudioWaveApi = reinterpret_cast<CAudioWaveAPi*>(dwInstance);
	if (pAudioWaveApi)
	{
		pAudioWaveApi->audio_waveProcEvent(uiMsg, dwParam1, dwParam2);
	}
}

//////////////////////////////////////////////////////////////////////////
bool CAudioWaveAPi::audio_getDevInfo(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecDevInfo)
{
	bool bRet = false;

	UINT uiIndex = 0;
	UINT uiDevCount = 0;

	std::string strDeviceId;
	std::string strDeviceName;

	MMRESULT mmr;
	std::ostringstream ostr;

	do 
	{
		if (emDevMode == DEVICE_EMPTYMODE)
		{
			bRet = false;
			break;
		}

		if (emDevMode == DEVICE_RENDERMODE)
		{//播放
			uiDevCount = waveOutGetNumDevs();
		}
		else if (emDevMode == DEVICE_CAPTUREMODE)
		{//采集
			uiDevCount = waveInGetNumDevs();
		}

		for (uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
		{
			ostr << uiIndex << endl;
			strDeviceId = ostr.str();

			if (emDevMode == DEVICE_RENDERMODE)
			{
				WAVEOUTCAPS waveOutcaps;
				mmr = waveOutGetDevCaps(uiIndex, &waveOutcaps, sizeof(WAVEOUTCAPS));
				if (mmr != MMSYSERR_NOERROR)
				{
					continue;
				}

				strDeviceName = waveOutcaps.szPname;
			}
			else if (emDevMode == DEVICE_CAPTUREMODE)
			{
				WAVEINCAPS waveIncaps;
				mmr = waveInGetDevCaps(uiIndex, &waveIncaps, sizeof(WAVEINCAPS));
				if (mmr != MMSYSERR_NOERROR)
				{
					continue;
				}

				strDeviceName = waveIncaps.szPname;
			}

			CDeviceInfo devInfo;
			devInfo.SetDeviceType(DEVICE_AUDIOTYPE);
			devInfo.SetDeviceState(DEVICE_ACTIVESTATE);
			devInfo.SetDeviceMode(emDevMode);

			devInfo.SetDeviceId(strDeviceId);
			devInfo.SetDeviceName(strDeviceName);

			vecDevInfo.push_back(devInfo);
		}

		bRet = true;
	} while (false);

	return bRet;
}