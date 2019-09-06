#include "stdafx.h"
#include "AudioWaveApi.h"

#include <sstream>

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
static void getInputDevice(std::vector<CDeviceData> &vecAudioDev)
{
	UINT uiIndex = 0;
	UINT uiDevCount = 0;

	MMRESULT hr;
	WAVEINCAPS waveIncaps;
	std::ostringstream ostr;

	uiDevCount = waveInGetNumDevs();
	for (uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
	{
		hr = waveInGetDevCaps(uiIndex, &waveIncaps, sizeof(WAVEINCAPS));
		if (hr != MMSYSERR_NOERROR)
		{
			continue;
		}

		CDeviceData audioDev;
		audioDev.SetDevType(DEV_AUDIOTYPE);
		audioDev.SetDevMode(DEVICE_CAPTUREMODE);

		ostr << uiIndex << endl;
		audioDev.SetDevId(ostr.str());
		audioDev.SetDevName(waveIncaps.szPname);

		vecAudioDev.push_back(audioDev);
	}
}

static void getOutputDevice(std::vector<CDeviceData> &vecAudioDev)
{
	UINT uiIndex = 0;
	UINT uiDevCount = 0;

	MMRESULT hr;
	std::ostringstream ostr;

	uiDevCount = waveOutGetNumDevs();
	for (uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
	{
		WAVEOUTCAPS waveOutcaps;
		hr = waveOutGetDevCaps(uiIndex, &waveOutcaps, sizeof(WAVEOUTCAPS));
		if (hr != MMSYSERR_NOERROR)
		{
			continue;
		}

		CDeviceData audioDev;
		audioDev.SetDevType(DEV_AUDIOTYPE);
		audioDev.SetDevMode(DEVICE_CAPTUREMODE);

		ostr << uiIndex << endl;
		audioDev.SetDevId(ostr.str());
		audioDev.SetDevName(waveOutcaps.szPname);

		vecAudioDev.push_back(audioDev);
	}
}

bool CAudioWaveAPi::audio_getDevInfo(std::vector<CDeviceData> &vecAudioDev)
{
	getInputDevice(vecAudioDev);
	getOutputDevice(vecAudioDev);
	return true;
}

void CAudioWaveAPi::audio_waveApiProc()
{
}