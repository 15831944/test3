#include "stdafx.h"
#include "AudioWaveApi.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

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

bool CAudioWaveAPi::audio_enumDevice(std::vector<CDeviceData> &vecAudioDev)
{
	audio_getDevInfo(vecAudioDev);
	return true;
}

bool CAudioWaveAPi::audio_openDevice()
{
	return true;
}

void CAudioWaveAPi::audio_closeDevice()
{
}

void CAudioWaveAPi::audio_startStream()
{
}

void CAudioWaveAPi::audio_closeStream()
{
}

void CAudioWaveAPi::audio_stopStream()
{
}

void CAudioWaveAPi::audio_abortStream()
{
}

//////////////////////////////////////////////////////////////////////////
DWORD WINAPI CAudioWaveAPi::audio_waveApiThread(void *pWaveApiPtr)
{
	if (pWaveApiPtr != NULL)
	{
		((CAudioWaveAPi*)pWaveApiPtr)->audio_waveApiProc();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
bool CAudioWaveAPi::audio_getDevInfo(std::vector<CDeviceData> &vecAudioDev)
{
	bool bRet = false;

	UINT uiIndex = 0;
	UINT uiDevCount = 0;

	MMRESULT hr;

	do 
	{
		uiDevCount = waveInGetNumDevs();

		for (uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
		{
			WAVEINCAPS waveIncaps;
			hr = waveInGetDevCaps(uiIndex, &waveIncaps, sizeof(WAVEINCAPS));
			if (hr != MMSYSERR_NOERROR )
			{
				continue;
			}

			CDeviceData audioDev;
		}

		bRet = true;
	} while (false);

	return bRet;
}

void CAudioWaveAPi::audio_waveApiProc()
{
}