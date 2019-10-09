#include "stdafx.h"
#include "DeviceInfo.h"

IDeviceInfo::IDeviceInfo()
{
	m_devType = DEVICE_EMPTYTYPE;
	m_devState = DEVICE_EMPTYSTATE;
	m_devMode = DEVICE_EMPTYMODE;

	m_strDevId = "";
	m_strDevName = "";
}

IDeviceInfo::~IDeviceInfo()
{
}

IDeviceInfo::IDeviceInfo(const IDeviceInfo &devInfo)
{
	m_devType = devInfo.m_devType;
	m_devMode = devInfo.m_devMode;
	m_devState = devInfo.m_devState;
	
	m_strDevId = devInfo.m_strDevId;
	m_strDevName = devInfo.m_strDevName;

	m_audioData = devInfo.m_audioData;
}

IDeviceInfo &IDeviceInfo::operator = (const IDeviceInfo& devInfo)
{
	m_devType = devInfo.m_devType;
	m_devMode = devInfo.m_devMode;
	m_devState = devInfo.m_devState;
	
	m_strDevId = devInfo.m_strDevId;
	m_strDevName = devInfo.m_strDevName;

	m_audioData = devInfo.m_audioData;
	return *this;
}

void IDeviceInfo::SetDeviceType(DeviceType devType)
{
	m_devType = devType;
}

DeviceType IDeviceInfo::GetDeviceType()
{
	return m_devType;
}

void IDeviceInfo::SetDeviceMode(DeviceMode devMode)
{
	m_devMode = devMode;
}

DeviceMode IDeviceInfo::GetDeviceMode()
{
	return m_devMode;
}

void IDeviceInfo::SetDeviceState(DeviceState devState)
{
	m_devState = devState;
}

DeviceState IDeviceInfo::GetDeviceState()
{
	return m_devState;
}

void IDeviceInfo::SetDeviceId(std::string strDevId)
{
	m_strDevId = strDevId;
}

std::string	IDeviceInfo::GetDeviceId()
{
	return m_strDevId;
}

void IDeviceInfo::SetDeviceName(std::string strDevName)
{
	m_strDevName = strDevName;
}

std::string IDeviceInfo::GetDeviceName()
{
	return m_strDevName;
}

IAudioData& IDeviceInfo::GetDeviceAudio()
{
	return m_audioData;
}

//////////////////////////////////////////////////////////////////////////
//
IDeviceHandle::IDeviceHandle()
{
	m_pApiHandle = NULL;
}

IDeviceHandle::~IDeviceHandle()
{
}

void IDeviceHandle::SetDevInfo(IDeviceInfo devInfo)
{
	m_devInfo = devInfo;
}

IDeviceInfo& IDeviceHandle::GetDevInfo()
{
	return m_devInfo;
}

void IDeviceHandle::SetApiHandle(void *pApiHandle)
{
	if (m_pApiHandle != NULL)
	{
		delete m_pApiHandle;
		m_pApiHandle = NULL;
	}

	if (pApiHandle != NULL)
	{
		m_pApiHandle = pApiHandle;
	}
}

void* IDeviceHandle::GetApiHandle()
{
	return m_pApiHandle;
}

void IDeviceHandle::SetDataQueue(IDataFrame *pDataFrame)
{
	if (pDataFrame == NULL)
	{
		return;
	}
}

IDataFrame*	IDeviceHandle::GetDataQueue()
{
	return NULL;
}