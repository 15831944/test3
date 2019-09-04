#include "stdafx.h"
#include "DeviceData.h"


CDeviceData::CDeviceData()
{
}

CDeviceData::~CDeviceData()
{
}

CDeviceData::CDeviceData(const CDeviceData& audioDev)
{
	m_stAudioDev.nDevType = audioDev.m_stAudioDev.nDevType;
	m_stAudioDev.nDevState = audioDev.m_stAudioDev.nDevState;
	m_stAudioDev.nDataType = audioDev.m_stAudioDev.nDataType;
	
	m_stAudioDev.strDevId = audioDev.m_stAudioDev.strDevId;
	m_stAudioDev.strDevName = audioDev.m_stAudioDev.strDevName;
	memcpy(&m_stAudioDev.stWavFormat, &audioDev.m_stAudioDev.stWavFormat, sizeof(WavFormat));
}

CDeviceData &CDeviceData::operator = (const CDeviceData& audioDev)
{
	m_stAudioDev.nDevType = audioDev.m_stAudioDev.nDevType;
	m_stAudioDev.nDevState = audioDev.m_stAudioDev.nDevState;
	m_stAudioDev.nDataType = audioDev.m_stAudioDev.nDataType;

	m_stAudioDev.strDevId = audioDev.m_stAudioDev.strDevId;
	m_stAudioDev.strDevName = audioDev.m_stAudioDev.strDevName;
	memcpy(&m_stAudioDev.stWavFormat, &audioDev.m_stAudioDev.stWavFormat, sizeof(WavFormat));

	return *this;
}

void CDeviceData::SetDevType(int nDevType)
{
	m_stAudioDev.nDevType = nDevType;
}

int CDeviceData::GetDevType()
{
	return m_stAudioDev.nDevType;
}

void CDeviceData::SetDevState(int nDevState)
{
	m_stAudioDev.nDevState = nDevState;
}

int CDeviceData::GetDevState()
{
	return m_stAudioDev.nDevState;
}

void CDeviceData::SetDataType(int nDataType)
{
	m_stAudioDev.nDataType = nDataType;
}

int CDeviceData::GetDataType()
{
	return m_stAudioDev.nDataType;
}

void CDeviceData::SetDevId(std::string strDevId)
{
	m_stAudioDev.strDevId = strDevId;
}

std::string CDeviceData::GetDevId()
{
	return m_stAudioDev.strDevId;
}

void CDeviceData::SetDevName(std::string strDevName)
{
	m_stAudioDev.strDevName = strDevName;
}

std::string CDeviceData::GetDevName()
{
	return m_stAudioDev.strDevName;
}

void CDeviceData::SetWavFormat(WavFormat *pWavFormat)
{
	if (pWavFormat == NULL)
	{
		return;
	}
	
	memcpy(&m_stAudioDev.stWavFormat, pWavFormat, sizeof(WavFormat));
}

WavFormat* CDeviceData::GetWavFormat()
{
	return &m_stAudioDev.stWavFormat;
}
