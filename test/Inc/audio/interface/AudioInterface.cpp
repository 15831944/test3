#include "stdafx.h"
#include "AudioInterface.h"

CDevData::CDevData()
{
	memset(&m_stAudioDev, 0x0, sizeof(DevHandle));
}

CDevData::~CDevData()
{
}

CDevData::CDevData(const CDevData& audioDev)
{
	memcpy(&m_stAudioDev, &audioDev.m_stAudioDev, sizeof(DevHandle));
}

CDevData &CDevData::operator = (const CDevData& audioDev)
{
	memcpy(&m_stAudioDev, &audioDev.m_stAudioDev, sizeof(DevHandle));
	return *this;
}

void CDevData::SetDevType(UINT uiDevType)
{
	m_stAudioDev.uiDevType = uiDevType;
}

UINT CDevData::GetDevType()
{
	return m_stAudioDev.uiDevType;
}

void CDevData::SetDevId(LPCTSTR lpszDevId)
{
	sprintf(m_stAudioDev.szDeviceId, "%s", lpszDevId);
}

std::string CDevData::GetDevId()
{
	return m_stAudioDev.szDeviceId;
}

void CDevData::SetDevFriendName(LPCTSTR lpszDevName)
{
	sprintf(m_stAudioDev.szFriendlyName, "%s", lpszDevName);
}

std::string CDevData::GetDevFriendName()
{
	return m_stAudioDev.szFriendlyName;
}

void CDevData::SetDevState(UINT uiDevState)
{
	m_stAudioDev.uiDevState = uiDevState;
}

UINT CDevData::GetDevState()
{
	return m_stAudioDev.uiDevState;
}

void CDevData::SetDevFormat(WAVEFORMATEX *pWavFormat)
{
	if (pWavFormat == NULL)
	{
		return;
	}
	
	memcpy(&m_stAudioDev.stDevFormat, pWavFormat, sizeof(WAVEFORMATEX));
}

WAVEFORMATEX* CDevData::GetDevFormat()
{
	return &m_stAudioDev.stDevFormat;
}