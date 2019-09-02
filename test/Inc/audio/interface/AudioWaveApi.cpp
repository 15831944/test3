#include "stdafx.h"
#include "AudioWaveApi.h"

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
}

void CAudioWaveAPi::audio_uninit()
{
}

bool CAudioWaveAPi::audio_enumDevice(std::vector<CDeviceData> &vecAudioDev)
{
}

bool CAudioWaveAPi::audio_openDevice()
{
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
DWORD WINAPI CAudioWaveAPi::audio_wasApiThread(void *pWaveApiPtr)
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
}

void CAudioWaveAPi::audio_waveApiProc()
{
}