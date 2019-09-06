#include "stdafx.h"
#include "DeviceInfo.h"

CDeviceInfo::CDeviceInfo()
{
}

CDeviceInfo::~CDeviceInfo()
{
}

CDeviceInfo::CDeviceInfo(const CDeviceInfo& devInfo)
{
	m_stAudioDev.emDeviceType = devInfo.m_stAudioDev.emDeviceType;
	m_stAudioDev.emDeviceState = devInfo.m_stAudioDev.emDeviceState;
	m_stAudioDev.emDeviceMode = devInfo.m_stAudioDev.emDeviceMode;
	
	m_stAudioDev.strDeviceId = devInfo.m_stAudioDev.strDeviceId;
	m_stAudioDev.strDeviceName = devInfo.m_stAudioDev.strDeviceName;
	
	if (devInfo.m_stAudioDev.emDeviceType == DEVICE_AUDIOTYPE)
	{
		memcpy(&m_stAudioDev.stAudioDev, &devInfo.m_stAudioDev.stAudioDev, sizeof(AudioDev));
	}
	else if (devInfo.m_stAudioDev.emDeviceType == DEVICE_VIDEOTYPE)
	{
		memcpy(&m_stAudioDev.stVideoDev, &devInfo.m_stAudioDev.stVideoDev, sizeof(VideoDev));
	}
}

CDeviceInfo &CDeviceInfo::operator = (const CDeviceInfo& devInfo)
{
	m_stAudioDev.emDeviceType = devInfo.m_stAudioDev.emDeviceType;
	m_stAudioDev.emDeviceState = devInfo.m_stAudioDev.emDeviceState;
	m_stAudioDev.emDeviceMode = devInfo.m_stAudioDev.emDeviceMode;

	m_stAudioDev.strDeviceId = devInfo.m_stAudioDev.strDeviceId;
	m_stAudioDev.strDeviceName = devInfo.m_stAudioDev.strDeviceName;
	
	if (devInfo.m_stAudioDev.emDeviceType == DEVICE_AUDIOTYPE)
	{
		memcpy(&m_stAudioDev.stAudioDev, &devInfo.m_stAudioDev.stAudioDev, sizeof(AudioDev));
	}
	else if (devInfo.m_stAudioDev.emDeviceType == DEVICE_VIDEOTYPE)
	{
		memcpy(&m_stAudioDev.stVideoDev, &devInfo.m_stAudioDev.stVideoDev, sizeof(VideoDev));
	}
	
	return *this;
}

void CDeviceInfo::SetDevType(DeviceType emDevType)
{
	m_stAudioDev.emDeviceType = emDevType;
}

DeviceType CDeviceInfo::GetDevType()
{
	return m_stAudioDev.emDeviceType;
}

void CDeviceInfo::SetDevState(DeviceState emDevState)
{
	m_stAudioDev.emDeviceState = emDevState;
}

DeviceState CDeviceInfo::GetDevState()
{
	return m_stAudioDev.emDeviceState;
}

void CDeviceInfo::SetDevMode(DeviceMode emDevMode)
{
	m_stAudioDev.emDeviceMode = emDevMode;
}

DeviceMode CDeviceInfo::GetDevMode()
{
	return m_stAudioDev.emDeviceMode;
}

void CDeviceInfo::SetDevId(std::string strDevId)
{
	m_stAudioDev.strDeviceId = strDevId;
}

std::string CDeviceInfo::GetDevId()
{
	return m_stAudioDev.strDeviceId;
}

void CDeviceInfo::SetDevName(std::string strDevName)
{
	m_stAudioDev.strDeviceName = strDevName;
}

std::string CDeviceInfo::GetDevName()
{
	return m_stAudioDev.strDeviceName;
}

void CDeviceInfo::SetDevData(DeviceType emDevType, void *pDevData)
{
	if (emDevType == DEVICE_EMPTYTYPE)
	{
		return;
	}
	
	if (emDevType == DEVICE_AUDIOTYPE)
	{
		memcpy(&m_stAudioDev.stAudioDev, (AudioDev*)pDevData, sizeof(AudioDev));
	}
	else if (emDevType == DEVICE_VIDEOTYPE)
	{
		memcpy(&m_stAudioDev.stVideoDev, (VideoDev*)pDevData, sizeof(VideoDev));
	}
}

void* CDeviceInfo::GetDevData(DeviceType emDevType)
{
	void *pDevData = NULL;
	if (emDevType == DEVICE_EMPTYTYPE)
	{
		return NULL;
	}
	
	if (emDevType == DEVICE_AUDIOTYPE)
	{
		pDevData = &m_stAudioDev.stAudioDev;
	}
	else if (emDevType == DEVICE_VIDEOTYPE)
	{
		pDevData = &m_stAudioDev.stVideoDev;
	}
	
	return pDevData;
}