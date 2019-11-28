#include "stdafx.h"
#include "AudioWaveApi.h"

#include <sstream>
#include "../../data/DevHandler.h"

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
	CDevHandler::Instance().regDevObj(MSG_HANDLE_API_ID, new WaveHandle);
	return true;
}

void CAudioWaveAPi::audio_uninit()
{
}

bool CAudioWaveAPi::audio_enumDevice(DevMode devMode, std::vector<CDevDataMgr> &vecDevData)
{
	return audio_getDevInfo(devMode, vecDevData);
}

bool CAudioWaveAPi::audio_openDevice(AudioPcmFormat stAudioFormat, CDevDataMgr devData)
{
	bool bRet = false;

	do 
	{
		WaveHandle *pWaveHandle = (WaveHandle*)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		WAVEFORMATEX stWaveFormat = {0};
		UINT uiDevId = atol(devData.GetDevId().c_str());

		//IAudioData audioData = devData.GetDeviceAudio();
		//memcpy(&stWaveFormat, audioData.GetWaveFormat(), sizeof(WAVEFORMATEX));

		MMRESULT mmr;
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

		CDevHandler::Instance().regDevObj(MSG_DEVICE_DATA_ID, &devData);

		bRet = true;
	} while (false);

	return bRet;
}

void CAudioWaveAPi::audio_closeDevice()
{
	bool bRet = false;

	do 
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
		{
			bRet = false;
			break;
		}

		MMRESULT mmr;

		CDevHandler::Instance().removeDevObj(MSG_DEVICE_DATA_ID);

		bRet = true;
	} while (false);
}

void CAudioWaveAPi::audio_startStream()
{
	bool bRet = false;

	do 
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
		{
			bRet = false;
			break;
		}

		MMRESULT mmr;
		if (pDevData->GetDevMode() == DEV_RENDERMODE)
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
		else if (pDevData->GetDevMode() == DEV_CAPTUREMODE)
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

	do 
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
		{
			bRet = false;
			break;
		}

		MMRESULT mmr;
		if (pDevData->GetDevMode() == DEV_RENDERMODE)
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
		else if (pDevData->GetDevMode() == DEV_CAPTUREMODE)
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

	do 
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
		{
			bRet = false;
			break;
		}

		MMRESULT mmr;
		if (pDevData->GetDevMode() == DEV_RENDERMODE)
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
		else if (pDevData->GetDevMode() == DEV_CAPTUREMODE)
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

	do 
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
		{
			bRet = false;
			break;
		}

		MMRESULT mmr;
		if (pDevData->GetDevMode() == DEV_RENDERMODE)
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
		else if (pDevData->GetDevMode() == DEV_CAPTUREMODE)
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

void CAudioWaveAPi::audio_addBuffer(CDataFrameMgr &dataBuff)
{
	bool bRet = false;

	do
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
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

		MMRESULT mmr;
		if (pDevData->GetDevMode() == DEV_RENDERMODE)
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
		else if (pDevData->GetDevMode() == DEV_CAPTUREMODE)
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

void CAudioWaveAPi::audio_openBuffer(CDataFrameMgr &dataBuff)
{
	bool bRet = false;

	do 
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
		{
			bRet = false;
			break;
		}

		MMRESULT mmr;
		if (pDevData->GetDevMode() == DEV_RENDERMODE)
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
		else if (pDevData->GetDevMode() == DEV_CAPTUREMODE)
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

void CAudioWaveAPi::audio_closeBuffer(CDataFrameMgr &dataBuff)
{
	bool bRet = false;
	
	do
	{
		WaveHandle *pWaveHandle = (WaveHandle *)CDevHandler::Instance().getDevObj(MSG_HANDLE_API_ID);
		if (pWaveHandle == NULL)
		{
			bRet = false;
			break;
		}

		CDevDataMgr *pDevData = (CDevDataMgr *)CDevHandler::Instance().getDevObj(MSG_DEVICE_DATA_ID);
		if (pDevData == NULL)
		{
			bRet = false;
			break;
		}

		MMRESULT mmr;
		if (pDevData->GetDevMode() == DEV_RENDERMODE)
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
		else if (pDevData->GetDevMode() == DEV_CAPTUREMODE)
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

bool CAudioWaveAPi::audio_getDevInfo(DevMode devMode, std::vector<CDevDataMgr> &vecDevData)
{
	bool bRet = false;

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
		std::ostringstream ostr;

		MMRESULT mmr;
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

			CDevDataMgr devData;
			devData.SetDevType(DEV_AUDIOTYPE);
			devData.SetDevState(DEV_ACTIVESTATE);
			devData.SetDevMode(devMode);

			
			devData.SetDevId(strDevId);
			devData.SetDevName(strDevName);

			vecDevData.push_back(devData);
		}

		bRet = true;
	} while (false);

	return bRet;
}