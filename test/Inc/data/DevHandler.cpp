/**
 *	@brief		
 *	@author		wanglei
 *	@date		2019.10.24
 */

#include "stdafx.h"
#include "DevHandler.h"

#include "./DevDataMgr.h"
#include "./DataFrameMgr.h"

//
CDevHandler::CDevHandler()
{
}

CDevHandler::~CDevHandler()
{
}

CDevHandler& CDevHandler::Instance()
{
	static CDevHandler inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
void CDevHandler::insertObjectHandler(unsigned int uiMsgId, CObjectPtr *pObjectPtr)
{
	if (uiMsgId == 0 || pObjectPtr == NULL)
	{
		return;
	}

	std::map<unsigned int, CObjectPtr>::iterator iterObject = m_mapData.find(uiMsgId);
	if (iterObject == m_mapData.end())
	{
		m_mapData.insert(make_pair(uiMsgId, *pObjectPtr));
	}
	else
	{
		iterObject->second = *pObjectPtr;
	}
}

//
void CDevHandler::regDevObj()
{
}

CObjectPtr* CDevHandler::getDevObj(unsigned int uiMsgId)
{
	if (uiMsgId == 0)
	{
		return NULL;
	}

	std::map<unsigned int, CObjectPtr>::iterator iterObject = m_mapData.find(uiMsgId);
	if (iterObject == m_mapData.end())
	{
		return NULL;
	}

	return &iterObject->second;
	return NULL;
}

void CDevHandler::clearDevObj()
{
	std::map<unsigned int, CObjectPtr>::iterator iterObject = m_mapData.begin();
	for (iterObject; iterObject != m_mapData.end();)
	{

	}
}