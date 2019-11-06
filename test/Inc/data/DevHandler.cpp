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
void* CDevHandler::getDevObj(unsigned int uiMsgId)
{
	if (uiMsgId == 0)
	{
		return NULL;
	}

	std::map<unsigned int, void*>::iterator iterObject = m_mapData.find(uiMsgId);
	if (iterObject == m_mapData.end())
	{
		return NULL;
	}

	return iterObject->second;
}

void CDevHandler::regDevObj(unsigned int uiMsgId, void *pObjectPtr)
{
	if (uiMsgId == 0 || pObjectPtr == NULL)
	{
		return;
	}

	std::map<unsigned int, void*>::iterator iterObject = m_mapData.find(uiMsgId);
	if (iterObject == m_mapData.end())
	{
		m_mapData.insert(make_pair(uiMsgId, pObjectPtr));
	}
	else
	{
		if (iterObject->second != NULL)
		{
			delete iterObject->second;
			iterObject->second = NULL;
		}

		iterObject->second = pObjectPtr;
	}
}

void CDevHandler::removeDevObj(unsigned int uiMsgId)
{
	std::map<unsigned int, void*>::iterator iterObject = m_mapData.find(uiMsgId);
	if (iterObject == m_mapData.end())
	{
		return;
	}

	if (iterObject->second != NULL)
	{
		delete iterObject->second;
		iterObject->second = NULL;
	}

	m_mapData.erase(iterObject);
}

void CDevHandler::clearDevObj()
{
	std::map<unsigned int, void*>::iterator iterObject = m_mapData.begin();
	for (iterObject; iterObject != m_mapData.end();)
	{
		if (iterObject->second != NULL)
		{
			delete iterObject->second;
			iterObject->second = NULL;
		}

		iterObject = m_mapData.erase(iterObject);
	}
}