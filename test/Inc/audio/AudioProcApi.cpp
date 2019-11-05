#include "stdafx.h"
#include "AudioProcAPi.h"

CAudioProc::CAudioProc(AudioApi emAudioApi)
{
	m_audioInterface = NULL;
	initApi(emAudioApi);
}

CAudioProc::~CAudioProc()
{
	unInitApi();
}

//////////////////////////////////////////////////////////////////////////
//
AudioApi CAudioProc::getCurrentApi()
{
	return UNSPECIFIED;
}

bool CAudioProc::initApi(AudioApi emAudioApi)
{
	if (emAudioApi == UNSPECIFIED)
	{
		return false;
	}

	unInitApi();
	if (emAudioApi == WINDOWS_WAVE)
	{
		m_audioInterface = new CAudioWaveAPi;
		if (m_audioInterface == NULL)
		{
			return false;
		}
	}
	
	if (emAudioApi == WINDOWS_ASIO)
	{
	}
	
	if (emAudioApi == WINDOWS_DS)
	{
	}
	
	if (emAudioApi == WINDOWS_WASAPI)
	{
	}
	
	if (!m_audioInterface->audio_init())
	{
		return false;
	}

	m_audioInterface->audio_setNotifyHandler(this);
	return true;
}

void CAudioProc::unInitApi()
{
	if (m_audioInterface != NULL)
	{
		m_audioInterface->audio_uninit();
		m_audioInterface->audio_setNotifyHandler(NULL);
		
		delete m_audioInterface;
		m_audioInterface = NULL;
	}
}

bool CAudioProc::getEnumDevice(DevMode devMode, std::vector<CDevData> &vecDevData)
{
	if (m_audioInterface == NULL || devMode == DEV_EMPTYMODE)
	{
		return false;
	}
	
	return m_audioInterface->audio_enumDevice(devMode, vecDevData);
}

bool CAudioProc::isStreamOpen()
{
	if (m_audioInterface == NULL)
	{
		return false;
	}
	
	return true;
}

bool CAudioProc::openStream(AudioPcmFormat stAudioFormat, CDevData devData)
{
	if (m_audioInterface == NULL)
	{
		return false;
	}
	
	if (devData.GetDevId() == "")
	{
		return false;
	}
	
	if (!m_audioInterface->audio_openDevice(stAudioFormat, devData))
	{
		return false;
	}
	
	return true;
}

void CAudioProc::closeStream()
{
	if (m_audioInterface == NULL)
	{
		return;
	}
	
	m_audioInterface->audio_closeDevice();
}

bool CAudioProc::startStream()
{
	if (m_audioInterface == NULL)
	{
		return false;
	}
	
	return true;
}

void CAudioProc::stopStream()
{
	if (m_audioInterface == NULL)
	{
		return;
	}
}

void CAudioProc::abortStream()
{
	if (m_audioInterface == NULL)
	{
		return;
	}
}