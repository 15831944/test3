#include "stdafx.h"
#include "DeviceData.h"


CDeviceData::CDeviceData()
{
	memset(&m_stAudioDev, 0x0, sizeof(DevHandle));
}

CDeviceData::~CDeviceData()
{
}

CDeviceData::CDeviceData(const CDeviceData& audioDev)
{
	memcpy(&m_stAudioDev, &audioDev.m_stAudioDev, sizeof(DevHandle));
}

CDeviceData &CDeviceData::operator = (const CDeviceData& audioDev)
{
	memcpy(&m_stAudioDev, &audioDev.m_stAudioDev, sizeof(DevHandle));
	return *this;
}

void CDeviceData::SetDevType(UINT uiDevType)
{
	m_stAudioDev.uiDevType = uiDevType;
}

UINT CDeviceData::GetDevType()
{
	return m_stAudioDev.uiDevType;
}

void CDeviceData::SetDevId(LPCTSTR lpszDevId)
{
	sprintf(m_stAudioDev.szDeviceId, "%s", lpszDevId);
}

std::string CDeviceData::GetDevId()
{
	return m_stAudioDev.szDeviceId;
}

void CDeviceData::SetDevFriendName(LPCTSTR lpszDevName)
{
	sprintf(m_stAudioDev.szFriendlyName, "%s", lpszDevName);
}

std::string CDeviceData::GetDevFriendName()
{
	return m_stAudioDev.szFriendlyName;
}

void CDeviceData::SetDevState(UINT uiDevState)
{
	m_stAudioDev.uiDevState = uiDevState;
}

UINT CDeviceData::GetDevState()
{
	return m_stAudioDev.uiDevState;
}

/*
void CDeviceData::SetDevFormat(WAVEFORMATEX *pWavFormat)
{
	if (pWavFormat == NULL)
	{
		return;
	}
	
	memcpy(&m_stAudioDev.stDevFormat, pWavFormat, sizeof(WAVEFORMATEX));
}

WAVEFORMATEX* CDeviceData::GetDevFormat()
{
	return &m_stAudioDev.stDevFormat;
}
*/