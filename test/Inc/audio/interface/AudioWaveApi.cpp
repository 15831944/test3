#include "stdafx.h"
#include "AudioWaveApi.h"

#include <sstream>

struct WaveApiHandle {
	HWAVEIN		hWaveIn;
	HWAVEOUT	hWaveOut;
	
	struct WaveApiHandle() {
		hWaveIn = NULL;
		hWaveOut = NULL;
	}
};

CAudioWaveAPi::CAudioWaveAPi()
{
}

CAudioWaveAPi::~CAudioWaveAPi()
{
}

int CAudioWaveAPi::audio_geterror()
{
	return m_iError;
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
	audio_getDevInfo(emDevMode, vecAudioDev);
	return true;
}

bool CAudioWaveAPi::audio_openDevice(CDeviceInfo &devInfo, CDeviceHandle &devHandle)
{
	bool bRet = false;
	UINT uiDevId;
	
	MMRESULT mmr;
	WAVEFORMATEX stWaveFormat = {0};
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		pWaveApiHandle = new WaveApiHandle;
		if (pWaveApiHandle == NULL)
		{
			bRet = false;
			break;
		}

		uiDevId = atol(devInfo.GetDeviceId().c_str());
		if (devInfo.GetDeviceMode() == DEVICE_RENDERMODE)
		{//²¥·Å
			HWAVEOUT hWaveOut;
			mmr = waveOutOpen(&hWaveOut, uiDevId, const_cast<WAVEFORMATEX*>(&stWaveFormat), reinterpret_cast<DWORD_PTR>(CAudioWaveAPi::audio_waveOutProc), reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION);
			if (mmr != MMSYSERR_NOERROR)
			{
				return false;
			}

			pWaveApiHandle->hWaveOut = hWaveOut;
		}
		else if (devInfo.GetDeviceMode() == DEVICE_CAPTUREMODE)
		{//²É¼¯
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

		devHandle.SetDevInfo(&devInfo);
		devHandle.SetApiHandle(pWaveApiHandle);

		bRet = true;
	} while (false);

	if (!bRet)
	{
		if (pWaveApiHandle != NULL)
		{
			delete pWaveApiHandle;
			pWaveApiHandle = NULL;
		}
	}
	
	return bRet;
}

void CAudioWaveAPi::audio_closeDevice(CDeviceHandle &devHandle)
{
}

void CAudioWaveAPi::audio_startStream(CDeviceHandle &devHandle)
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		pDevInfo = devHandle.GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)devHandle.GetApiHandle();
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

void CAudioWaveAPi::audio_closeStream(CDeviceHandle &devHandle)
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		pDevInfo = devHandle.GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)devHandle.GetApiHandle();
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

void CAudioWaveAPi::audio_stopStream(CDeviceHandle &devHandle)
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		pDevInfo = devHandle.GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)devHandle.GetApiHandle();
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

void CAudioWaveAPi::audio_abortStream(CDeviceHandle &devHandle)
{
	bool bRet = false;

	MMRESULT mmr;
	CDeviceInfo *pDevInfo = NULL;
	WaveApiHandle *pWaveApiHandle = NULL;

	do 
	{
		pDevInfo = devHandle.GetDevInfo();
		if (pDevInfo == NULL)
		{
			bRet = false;
			break;
		}

		pWaveApiHandle = (WaveApiHandle*)devHandle.GetApiHandle();
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
				if (mmr == MMSYSERR_NOERROR)
				{
					bRet = false;
					break;
				}
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
static void getInputDevice(std::vector<CDeviceInfo> &vecDevInfo)
{
	UINT uiIndex = 0;
	UINT uiDevCount = 0;

	MMRESULT mmr;
	WAVEINCAPS waveIncaps;
	std::ostringstream ostr;

	uiDevCount = waveInGetNumDevs();
	for (uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
	{
		mmr = waveInGetDevCaps(uiIndex, &waveIncaps, sizeof(WAVEINCAPS));
		if (mmr != MMSYSERR_NOERROR)
		{
			continue;
		}

		CDeviceInfo devInfo;
		devInfo.SetDeviceType(DEVICE_AUDIOTYPE);
		devInfo.SetDeviceMode(DEVICE_CAPTUREMODE);
		devInfo.SetDeviceState(DEVICE_ACTIVESTATE);
		
		ostr << uiIndex << endl;
		devInfo.SetDeviceId(ostr.str());
		devInfo.SetDeviceName(waveIncaps.szPname);

		vecDevInfo.push_back(devInfo);
	}
}

static void getOutputDevice(std::vector<CDeviceInfo> &vecDevInfo)
{
	UINT uiIndex = 0;
	UINT uiDevCount = 0;

	MMRESULT mmr;
	std::ostringstream ostr;

	uiDevCount = waveOutGetNumDevs();
	for (uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
	{
		WAVEOUTCAPS waveOutcaps;
		mmr = waveOutGetDevCaps(uiIndex, &waveOutcaps, sizeof(WAVEOUTCAPS));
		if (mmr != MMSYSERR_NOERROR)
		{
			continue;
		}

		CDeviceInfo devInfo;
		devInfo.SetDeviceType(DEVICE_AUDIOTYPE);
		devInfo.SetDeviceMode(DEVICE_CAPTUREMODE);
		devInfo.SetDeviceState(DEVICE_ACTIVESTATE);

		ostr << uiIndex << endl;
		devInfo.SetDeviceId(ostr.str());
		devInfo.SetDeviceName(waveOutcaps.szPname);

		vecDevInfo.push_back(devInfo);
	}
}

bool CAudioWaveAPi::audio_getDevInfo(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecDevInfo)
{
	if (emDevMode == DEVICE_RENDERMODE)
	{
		getInputDevice(vecDevInfo);
	}
	else if (emDevMode == DEVICE_CAPTUREMODE)
	{
		getOutputDevice(vecDevInfo);
	}
	else
	{
		getInputDevice(vecDevInfo);
		getOutputDevice(vecDevInfo);
	}
	
	return true;
}

void CAudioWaveAPi::audio_waveApiProc()
{
}