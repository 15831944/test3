#include "stdafx.h"
#include "czhtFaceInterface.h"

#pragma comment(lib, _T("czht_rzt_face_interface.lib"))
CCzhtFaceInterface::CCzhtFaceInterface()
{
}

CCzhtFaceInterface::~CCzhtFaceInterface()
{
}

int CCzhtFaceInterface::rzt_face_geterror()
{
	return m_ierror;
}

bool CCzhtFaceInterface::rzt_face_init(IN char *pszSvrAddr, IN char *pszSvrPort)
{
	int iRet = -1;

	iRet = czht_face_init(pszSvrAddr, NULL, pszSvrPort);
	if (iRet != 0)
	{
		m_ierror = FACE_INIT_FAILED;
		return false;
	}

	return true;
}

bool CCzhtFaceInterface::rzt_face_uninit()
{
	return true;
}

bool CCzhtFaceInterface::rzt_face_image_insert(IN char *pszFaceDbName, IN char *pszFaceImage, IN FACE_USERDATA &UserData)
{
	int iRet = -1;
	stUserInfo UserInfo = {0};

	strcpy(UserInfo.userId, UserData.szUserId);
	strcpy(UserInfo.userName, UserData.szUserName);
	strcpy(UserInfo.userDep, UserData.szUserDep);

	iRet = czht_face_image_insert(pszFaceDbName, pszFaceImage, UserInfo);
	if (iRet != 0)
	{
		if (iRet == -1)
		{
			m_ierror = FACE_DETECT_FAILED;
		}
		else if (iRet == -2)
		{
			m_ierror = FACE_INSERT_TIMEOUT;
		}
		else if (iRet == -3)
		{
			m_ierror = FACE_INSERTDB_FAILED;
		}

		return false;
	}

	return true;
}

bool CCzhtFaceInterface::rzt_face_image_search(IN char *pszFaceDbName, IN char *pszFaceImage, OUT char **pszFaceSearchInfo)
{
	int iRet = -1;

	iRet = czht_face_image_search(pszFaceDbName, pszFaceImage, pszFaceSearchInfo);
	if (iRet != 0)
	{
		if (iRet == -1)
		{
			m_ierror = FACE_DETECT_FAILED;
		}
		else if (iRet == -2)
		{
			m_ierror = FACE_DATA_ERROR;
		}

		return false;
	}

	return true;
}

bool CCzhtFaceInterface::rzt_face_getuserdata(OUT std::map<int, FACE_USERRESULT> &mapFaceResult)
{
	if (m_mapFaceResult.size() == 0)
	{
		return false;
	}

	mapFaceResult = m_mapFaceResult;
	return true;
}