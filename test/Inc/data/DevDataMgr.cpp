/**
 *	@brief		
 *	@author		wanglei
 *	@date		2019.10.24
 */

#include "stdafx.h"
#include "DevDataMgr.h"

CDevDataMgr::CDevDataMgr()
{
	m_devType = DEV_EMPTYTYPE;
	m_devState = DEV_EMPTYSTATE;
	m_devMode = DEV_EMPTYMODE;

	m_strDevId = "";
	m_strDevName = "";
}

CDevDataMgr::~CDevDataMgr()
{
}

//
CDevDataMgr::CDevDataMgr(const CDevDataMgr &devData)
{
	m_devType = devData.m_devType;
	m_devMode = devData.m_devMode;
	m_devState = devData.m_devState;
	
	m_strDevId = devData.m_strDevId;
	m_strDevName = devData.m_strDevName;
}

CDevDataMgr &CDevDataMgr::operator = (const CDevDataMgr& devData)
{
	m_devType = devData.m_devType;
	m_devMode = devData.m_devMode;
	m_devState = devData.m_devState;
	
	m_strDevId = devData.m_strDevId;
	m_strDevName = devData.m_strDevName;

	return *this;
}

void CDevDataMgr::SetDevType(DevType devType)
{
	m_devType = devType;
}

DevType CDevDataMgr::GetDevType()
{
	return m_devType;
}

void CDevDataMgr::SetDevMode(DevMode devMode)
{
	m_devMode = devMode;
}

DevMode CDevDataMgr::GetDevMode()
{
	return m_devMode;
}

void CDevDataMgr::SetDevState(DevState devState)
{
	m_devState = devState;
}

DevState CDevDataMgr::GetDevState()
{
	return m_devState;
}

void CDevDataMgr::SetDevId(std::string strDevId)
{
	m_strDevId = strDevId;
}

std::string	CDevDataMgr::GetDevId()
{
	return m_strDevId;
}

void CDevDataMgr::SetDevName(std::string strDevName)
{
	m_strDevName = strDevName;
}

std::string CDevDataMgr::GetDevName()
{
	return m_strDevName;
}