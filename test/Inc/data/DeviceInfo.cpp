#include "stdafx.h"
#include "DeviceInfo.h"

CDeviceInfo::CDeviceInfo()
{
	m_emDeviceType = DEVICE_EMPTYTYPE;
	m_emDeviceState = DEVICE_EMPTYSTATE;
	m_emDeviceMode = DEVICE_EMPTYMODE;

	m_strDeviceId = "";
	m_strDeviceName = "";
}

CDeviceInfo::~CDeviceInfo()
{
}

CDeviceInfo::CDeviceInfo(const CDeviceInfo &devInfo)
{
	m_emDeviceType = devInfo.m_emDeviceType;
	m_emDeviceState = devInfo.m_emDeviceState;
	m_emDeviceMode = devInfo.m_emDeviceMode;

	m_strDeviceId = devInfo.m_strDeviceId;
	m_strDeviceName = devInfo.m_strDeviceName;
}

CDeviceInfo &CDeviceInfo::operator = (const CDeviceInfo& devInfo)
{
	m_emDeviceType = devInfo.m_emDeviceType;
	m_emDeviceState = devInfo.m_emDeviceState;
	m_emDeviceMode = devInfo.m_emDeviceMode;

	m_strDeviceId = devInfo.m_strDeviceId;
	m_strDeviceName = devInfo.m_strDeviceName;

	return *this;
}

void CDeviceInfo::SetDeviceType(DeviceType emDeviceType)
{
	m_emDeviceType = emDeviceType;
}

DeviceType CDeviceInfo::GetDeviceType()
{
	return m_emDeviceType;
}

void CDeviceInfo::SetDeviceState(DeviceState emDeviceState)
{
	m_emDeviceState = emDeviceState;
}

DeviceState CDeviceInfo::GetDeviceState()
{
	return m_emDeviceState;
}

void CDeviceInfo::SetDeviceMode(DeviceMode emDeviceMode)
{
	m_emDeviceMode = emDeviceMode;
}

DeviceMode CDeviceInfo::GetDeviceMode()
{
	return m_emDeviceMode;
}

void CDeviceInfo::SetDeviceId(std::string strDeviceId)
{
	m_strDeviceId = strDeviceId;
}

std::string	CDeviceInfo::GetDeviceId()
{
	return m_strDeviceId;
}

void CDeviceInfo::SetDeviceName(std::string strDeviceName)
{
	m_strDeviceName = strDeviceName;
}

std::string CDeviceInfo:: GetDeviceName()
{
	return m_strDeviceName;
}

//////////////////////////////////////////////////////////////////////////
//
CDeviceHandle::CDeviceHandle()
{
	m_pApiHandle = NULL;
}

CDeviceHandle::~CDeviceHandle()
{
}

void CDeviceHandle::SetDevInfo(CDeviceInfo *pDevInfo)
{
	if (pDevInfo == NULL)
	{
		return;
	}

	m_deviceInfo = *pDevInfo;
}

CDeviceInfo* CDeviceHandle::GetDevInfo()
{
	return &m_deviceInfo;
}

void CDeviceHandle::SetApiHandle(void *pApiHandle)
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

void* CDeviceHandle::GetApiHandle()
{
	return m_pApiHandle;
}

void CDeviceHandle::SetDevData(DeviceType emDevType, void *pDevData)
{
	// 	if (emDevType == DEVICE_EMPTYTYPE)
	// 	{
	// 		return;
	// 	}
	// 	
	// 	if (emDevType == DEVICE_AUDIOTYPE)
	// 	{
	// 		memcpy(&m_stDevInfo.stAudioDev, (AudioDev*)pDevData, sizeof(AudioDev));
	// 	}
	// 	else if (emDevType == DEVICE_VIDEOTYPE)
	// 	{
	// 		memcpy(&m_stDevInfo.stVideoDev, (VideoDev*)pDevData, sizeof(VideoDev));
	// 	}
}

void* CDeviceHandle::GetDevData(DeviceType emDevType)
{
	void *pDevData = NULL;
	// 	if (emDevType == DEVICE_EMPTYTYPE)
	// 	{
	// 		return NULL;
	// 	}
	// 	
	// 	if (emDevType == DEVICE_AUDIOTYPE)
	// 	{
	// 		pDevData = &m_stDevInfo.stAudioDev;
	// 	}
	// 	else if (emDevType == DEVICE_VIDEOTYPE)
	// 	{
	// 		pDevData = &m_stDevInfo.stVideoDev;
	// 	}
	// 	
	return pDevData;
}

void CDeviceHandle::SetDataQueue(CDataFrame *pDataFrame)
{
	if (pDataFrame == NULL)
	{
		return;
	}


}

CDataFrame*	CDeviceHandle::GetDataQueue()
{
	return NULL;
}