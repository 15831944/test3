/**
 *	@brief		
 *	@author		wanglei
 *	@date		2019.10.24
 */

#include "stdafx.h"
#include "DeviceInfo.h"

CDevData::CDevData()
{
	m_devType = DEV_EMPTYTYPE;
	m_devState = DEV_EMPTYSTATE;
	m_devMode = DEV_EMPTYMODE;

	m_strDevId = "";
	m_strDevName = "";
}

CDevData::~CDevData()
{
}

//
CDevData::CDevData(const CDevData &devData)
{
	m_devType = devData.m_devType;
	m_devMode = devData.m_devMode;
	m_devState = devData.m_devState;
	
	m_strDevId = devData.m_strDevId;
	m_strDevName = devData.m_strDevName;
}

CDevData &CDevData::operator = (const CDevData& devData)
{
	m_devType = devData.m_devType;
	m_devMode = devData.m_devMode;
	m_devState = devData.m_devState;
	
	m_strDevId = devData.m_strDevId;
	m_strDevName = devData.m_strDevName;

	return *this;
}

void CDevData::SetDevType(DevType devType)
{
	m_devType = devType;
}

DevType CDevData::GetDevType()
{
	return m_devType;
}

void CDevData::SetDevMode(DevMode devMode)
{
	m_devMode = devMode;
}

DevMode CDevData::GetDevMode()
{
	return m_devMode;
}

void CDevData::SetDevState(DevState devState)
{
	m_devState = devState;
}

DevState CDevData::GetDevState()
{
	return m_devState;
}

void CDevData::SetDevId(std::string strDevId)
{
	m_strDevId = strDevId;
}

std::string	CDevData::GetDevId()
{
	return m_strDevId;
}

void CDevData::SetDevName(std::string strDevName)
{
	m_strDevName = strDevName;
}

std::string CDevData::GetDevName()
{
	return m_strDevName;
}

//////////////////////////////////////////////////////////////////////////
//
CDevHandle::CDevHandle()
{
	m_pApiHandle = NULL;
}

CDevHandle::~CDevHandle()
{
}

void CDevHandle::SetDevData(CDevData devData)
{
	m_devData = devData;
}

CDevData& CDevHandle::GetDevData()
{
	return m_devData;
}

void CDevHandle::SetApiHandle(void *pApiHandle)
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

void* CDevHandle::GetApiHandle()
{
	return m_pApiHandle;
}

void CDevHandle::SetDataQueue(IDataFrame *pDataFrame)
{
	if (pDataFrame == NULL)
	{
		return;
	}
}

IDataFrame*	CDevHandle::GetDataQueue()
{
	return NULL;
}