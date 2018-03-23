#include "StdAfx.h"
#include "UKeyVerifyThread.h"

#include "../Inc/UKeyVerify.h"
#include "UKeyFindThread.h"

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2018.03.20 11:39
 */
/************************************************************************/

static CUKeyFindThread g_UKeyFindThread;
CUKeyVerifyThread::CUKeyVerifyThread()
{
	m_bExit = FALSE;
	m_bFlags = FALSE;
	
	m_dwThreadID = 0;
	m_dwProcTimeOver = 0;
	m_dwCloseTimeOver = 0;
	
	m_strUserNum = _T("");
	m_strUserPasswd = _T("");
	m_strUserPIN = _T("");
	
	m_hThread = NULL;
	m_lpParam = NULL;
	m_pfUKDataFunc = NULL;

	CUKeyVerify::Instance().Initialize();

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CUKeyVerifyThread::~CUKeyVerifyThread()
{
	CloseVerifyProc();

	if (m_hStartEvent != NULL)
	{
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
	}

	if (m_hEndEvent != NULL)
	{
		CloseHandle(m_hEndEvent);
		m_hEndEvent = NULL;
	}

	CUKeyVerify::Instance().Finalize();
}

//////////////////////////////////////////////////////////////////////////
//
DWORD CUKeyVerifyThread::UKeyVerifyThreadProc(LPVOID lpParam)
{
	CUKeyVerifyThread* pVerifyProc = (CUKeyVerifyThread*)lpParam;
	if(pVerifyProc != NULL)
	{
		pVerifyProc->UKeyVerifyInfo();
	}
	
	return 0;
}

void CUKeyVerifyThread::UKeyVerifyInfo()
{
	BOOL bRet = FALSE;
	while(WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			do 
			{
				if (m_bFlags)
				{
					if (!SetUKeyData(0))
					{
						bRet = FALSE;
						break;
					}
				}
				else
				{
					if (!GetUKeyData(0))
					{
						bRet = FALSE;
						break;
					}
				}

				bRet = TRUE;
			} while (FALSE);
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	};
}

BOOL CUKeyVerifyThread::SetUKeyData(int iSlotIndex)
{
	BOOL bRet = FALSE;
	int iIndex = 0;
	
	CK_SESSION_HANDLE hSession = NULL;
	CK_UKEYPROCINFO *pUKeyData = NULL;

	CK_UKEYPROCINFO stcUKeyData = {0};
	CK_UKEYUSERDATA stcUKeyUserData = {0};

	std::vector<CK_UKEYPROCINFO *> vecUKeyData;
	std::vector<CK_UKEYPROCINFO *>::iterator iterUKeyData;
	
	do 
	{
		if (!UKeyEnum(vecUKeyData))
		{
			bRet = FALSE;
			break;
		}

		for (iterUKeyData=vecUKeyData.begin(), iIndex=0; iterUKeyData!=vecUKeyData.end(); ++iterUKeyData, ++iIndex)
		{
			do 
			{
				pUKeyData = (CK_UKEYPROCINFO *)(*iterUKeyData);
				if (!GetUKeyInfo(iIndex, iSlotIndex, pUKeyData))
				{
					bRet = FALSE;
					break;
				}

				if (!pUKeyData->bFlags)
				{
					if (!UKeyVerify(pUKeyData))
					{
						bRet = FALSE;
						break;
					}
				}

				//UKEY设备输入用户数据
				memcpy(&stcUKeyData, pUKeyData, sizeof(CK_UKEYPROCINFO));
				stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEINPUTETYPE;
				stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEWRITETYPE;
				sprintf(stcUKeyUserData.szPromptText, _T("写入Key设备Slot:%d数据"), pUKeyData->ulSlotId);;
				m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);

				while (WaitForSingleObject(pUKeyData->hEvent, 0) != WAIT_OBJECT_0)
				{
					if (!UKeyWriteObjectData(pUKeyData, stcUKeyUserData.szUserNum, stcUKeyUserData.szUserPasswd))
					{
						bRet = FALSE;
						break;
					}
				}

				bRet = TRUE;
			} while (FALSE);

			if (!bRet)
			{
				if (iSlotIndex == -1 && iIndex == iSlotIndex)
				{
					break;
				}
				else
				{
					continue;
				}
			}
		}
	} while (FALSE);

	return bRet;
}

BOOL CUKeyVerifyThread::GetUKeyData(int iSlotIndex)
{
	BOOL bRet = FALSE;
	int iIndex = 0;

	CK_SESSION_HANDLE hSession = NULL;
	CK_UKEYPROCINFO *pUKeyData = NULL;

	std::vector<CK_UKEYPROCINFO *> vecUKeyData;
	std::vector<CK_UKEYPROCINFO *>::iterator iterUKeyData;

	do 
	{
		if (!UKeyEnum(vecUKeyData))
		{
			bRet = FALSE;
			break;
		}

		for (iterUKeyData=vecUKeyData.begin(), iIndex=0; iterUKeyData!=vecUKeyData.end(); ++iterUKeyData, ++iIndex)
		{
			do 
			{
				pUKeyData = (CK_UKEYPROCINFO *)(*iterUKeyData);
				if (!GetUKeyInfo(iIndex, iSlotIndex, pUKeyData))
				{
					bRet = FALSE;
					break;
				}

				if (!pUKeyData->bFlags)
				{
					if (!UKeyVerify(pUKeyData))
					{
						bRet = FALSE;
						break;
					}
				}

				while (WaitForSingleObject(pUKeyData->hEvent, 0) != WAIT_OBJECT_0)
				{
					if (!UKeyReadObjectData(pUKeyData))
					{
						bRet = FALSE;
						break;
					}
				}

				bRet = TRUE;
			} while (FALSE);

			if (!bRet)
			{
				if (iSlotIndex == -1 && iIndex == iSlotIndex)
				{
					break;
				}
				else
				{
					continue;
				}
			}
		}
	} while (FALSE);

	return bRet;
}

BOOL CUKeyVerifyThread::UKeyEnum(std::vector<CK_UKEYPROCINFO *> &vecUKeyData)
{
	BOOL bRet = FALSE;

	CK_UKEYPROCINFO stcUKeyData = {0};
	CK_UKEYUSERDATA stcUKeyUserData = {0};

	do 
	{
		if (!CUKeyVerify::Instance().GetSlotId(vecUKeyData))
		{
			bRet = FALSE;
			stcUKeyData.bFlags = FALSE;
			stcUKeyData.bExist = FALSE;

			stcUKeyData.emUKeyType = CK_UKEYDEVEMPTYTYPE;
			stcUKeyData.emUKeyState = CK_UKEYSTATEEMPTYTYPE;
			stcUKeyData.emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;

			sprintf(stcUKeyUserData.szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}

		if (vecUKeyData.size() == 0)
		{
			bRet = FALSE;
			stcUKeyData.bFlags = FALSE;
			stcUKeyData.bExist = FALSE;

			stcUKeyData.emUKeyType = CK_UKEYDEVEMPTYTYPE;
			stcUKeyData.emUKeyState = CK_UKEYSTATEEMPTYTYPE;
			stcUKeyData.emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;

			sprintf(stcUKeyUserData.szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);
	return bRet;
}

BOOL CUKeyVerifyThread::GetUKeyInfo(int iIndex, int iSlotIndex, CK_UKEYPROCINFO *pUKeyData)
{
	BOOL bRet = FALSE;
	CK_SESSION_HANDLE hSession = NULL;

	CK_UKEYPROCINFO stcUKeyData = {0};
	CK_UKEYUSERDATA stcUKeyUserData = {0};

	do 
	{
		if (pUKeyData == NULL)
		{
			bRet = FALSE;
			stcUKeyData.bFlags = FALSE;
			stcUKeyData.bExist = FALSE;

			stcUKeyData.emUKeyType = CK_UKEYDEVEMPTYTYPE;
			stcUKeyData.emUKeyState = CK_UKEYSTATEEMPTYTYPE;
			stcUKeyData.emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;

			sprintf(stcUKeyUserData.szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}

		if (iIndex != iSlotIndex && iSlotIndex != -1)
		{
			bRet = FALSE;
			stcUKeyData.bFlags = FALSE;
			stcUKeyData.bExist = FALSE;

			stcUKeyData.emUKeyType = CK_UKEYDEVEMPTYTYPE;
			stcUKeyData.emUKeyState = CK_UKEYSTATEEMPTYTYPE;
			stcUKeyData.emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;

			sprintf(stcUKeyUserData.szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}

		bRet = TRUE;
		stcUKeyData.hEvent = pUKeyData->hEvent;
		stcUKeyData.bExist = pUKeyData->bExist;
		stcUKeyData.emUKeyType = pUKeyData->emUKeyType;
		stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEINSERTTYPE;
		stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;
	} while (FALSE);

	m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);
	return bRet;
}

BOOL CUKeyVerifyThread::UKeyVerify(CK_UKEYPROCINFO *pUKeyData)
{
	BOOL bRet = FALSE;
	CK_SESSION_HANDLE hSession = NULL;

	CK_UKEYPROCINFO stcUKeyData = {0};
	CK_UKEYUSERDATA stcUKeyUserData = {0};

	if (pUKeyData == NULL)
	{
		return FALSE;
	}

	do 
	{
		if (!CUKeyVerify::Instance().CreateSession(pUKeyData->ulSlotId, hSession))
		{
			bRet = FALSE;
			stcUKeyData.bFlags = FALSE;
			stcUKeyData.bExist = pUKeyData->bExist;

			stcUKeyData.emUKeyType = pUKeyData->emUKeyType;
			stcUKeyData.emUKeyState = CK_UKEYSTATEEMPTYTYPE;
			stcUKeyData.emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;

			sprintf(stcUKeyUserData.szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}

		//UKEY设备插入成功
		stcUKeyData.bExist = pUKeyData->bExist;
		stcUKeyData.hSession = pUKeyData->hSession = (HANDLE)hSession;

		stcUKeyData.emUKeyType = pUKeyData->emUKeyType;
		stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEINSERTTYPE;
		stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;
		sprintf(stcUKeyUserData.szPromptText, _T("获取Key设备Slot:%d"), pUKeyData->ulSlotId);
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);

		//UKEY设备校验PIN输入
		stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEINPUTETYPE;
		stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEVERIFYTYPE;
		sprintf(stcUKeyUserData.szPromptText, _T("校验Key设备Slot:%d"), pUKeyData->ulSlotId);
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);

		if (!CUKeyVerify::Instance().LoginUser(hSession, stcUKeyUserData.szUserPIN, pUKeyData->ulFlags))
		{
			bRet = FALSE;
			stcUKeyData.bFlags = FALSE;
			stcUKeyData.hSession = pUKeyData->hSession = (HANDLE)hSession;

			stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEFAILEDTYPE;
			stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEVERIFYTYPE;

			sprintf(stcUKeyUserData.szPromptText, _T("校验Key设备Slot:%d失败, 请检查!"), pUKeyData->ulSlotId);
			break;
		}

		//UKEY设备校验PIN成功
		stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATESUCCEDTYPE;
		stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEVERIFYTYPE;
		sprintf(stcUKeyUserData.szPromptText, _T("校验Key设备Slot:%d成功!"), pUKeyData->ulSlotId);
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);

		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);
	}
	
	return bRet;
}

BOOL CUKeyVerifyThread::UKeyWriteObjectData(CK_UKEYPROCINFO *pUKeyData, LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd)
{
	BOOL bRet = FALSE;
	DWORD dwCount = 0;
	CK_SESSION_HANDLE hSession = NULL;

	char szUserData[512] = {0};
	DWORD dwUserDataLen = sizeof(szUserData)-1;

	CK_UKEYPROCINFO stcUKeyData = {0};
	CK_UKEYUSERDATA stcUKeyUserData = {0};

	if (pUKeyData == NULL || lpszUserNum == NULL || lpszUserPasswd == NULL)
	{
		return FALSE;
	}

	do 
	{
		hSession = (CK_SESSION_HANDLE)pUKeyData->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = FALSE;
			break;
		}

		if (!CUKeyVerify::Instance().FindObject(hSession, dwCount))
		{
			bRet = FALSE;
			stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEFAILEDTYPE;
			stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEWRITETYPE;
			sprintf(stcUKeyUserData.szPromptText, _T("校验Key设备Slot:%d失败, 请检查!"), pUKeyData->ulSlotId);
			break;
		}

		dwUserDataLen  = strlen(lpszUserNum);
		memcpy(szUserData, lpszUserNum, strlen(lpszUserNum));
		dwUserDataLen += 1;
		memcpy(szUserData+dwUserDataLen, lpszUserPasswd, strlen(lpszUserPasswd));
		dwUserDataLen += strlen(lpszUserPasswd);

		if (dwCount == 0)
		{
			if (!CUKeyVerify::Instance().CreateObject(hSession, szUserData, dwUserDataLen))
			{
				bRet = FALSE;
				stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEFAILEDTYPE;
				stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEWRITETYPE;
				sprintf(stcUKeyUserData.szPromptText, _T("写入Key设备Slot:%d用户数据失败, 请检查!"), pUKeyData->ulSlotId);
				break;
			}
		}
		else
		{
			if (!CUKeyVerify::Instance().SetObjectValue(hSession, (CK_BYTE*)szUserData, dwUserDataLen))
			{
				bRet = FALSE;
				stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEFAILEDTYPE;
				stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEWRITETYPE;
				sprintf(stcUKeyUserData.szPromptText, _T("写入Key设备Slot:%d用户数据失败, 请检查!"), pUKeyData->ulSlotId);
				break;
			}
		}

		//
		bRet = TRUE;
		SetEvent(pUKeyData->hEvent);

		stcUKeyData.hEvent = pUKeyData->hEvent;
		stcUKeyData.bFlags = pUKeyData->bFlags = TRUE;
		
		stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATESUCCEDTYPE;
		stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEWRITETYPE;
		sprintf(stcUKeyUserData.szPromptText, _T("写入Key设备Slot:%d用户数据成功!"), pUKeyData->ulSlotId);
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);
	} while (FALSE);

	if (!bRet)
	{
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);
	}

	return bRet;
}

BOOL CUKeyVerifyThread::UKeyReadObjectData(CK_UKEYPROCINFO *pUKeyData)
{
	BOOL bRet = FALSE;
	DWORD dwCount = 0;
	DWORD dwIndex = 0;

	char szUserData[512] = {0};
	DWORD dwUserDataLen = sizeof(szUserData)-1;

	CK_UKEYPROCINFO stcUKeyData = {0};
	CK_UKEYUSERDATA stcUKeyUserData = {0};

	char *pValue = NULL;
	CK_SESSION_HANDLE hSession = NULL;

	if (pUKeyData == NULL)
	{
		return FALSE;
	}

	do 
	{
		memcpy(&stcUKeyData, pUKeyData, sizeof(CK_UKEYPROCINFO));

		hSession = (CK_SESSION_HANDLE)pUKeyData->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = FALSE;
			break;
		}

		if (!CUKeyVerify::Instance().GetObjectValue(hSession, (CK_BYTE*)szUserData, &dwUserDataLen))
		{
			bRet = FALSE;
			stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATEOUTPUTTYPE;
			stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEREADTYPE;
			sprintf(stcUKeyUserData.szPromptText, _T("读取Key设备Slot:%d失败, 请检查！"), pUKeyData->ulSlotId);
			break;
		}

		pValue = szUserData;
		while (*pValue != '\0')
		{
			if (dwIndex == 0)
			{
				memcpy(stcUKeyUserData.szUserNum, pValue, strlen(pValue));
			}
			else if (dwIndex == 1)
			{
				memcpy(stcUKeyUserData.szUserPasswd, pValue, strlen(pValue));
			}
			else
			{
				break;
			}

			dwIndex++;
			pValue = pValue + strlen(pValue) + 1;
		}

		//
		bRet = TRUE;
		SetEvent(pUKeyData->hEvent);

		stcUKeyData.hEvent = pUKeyData->hEvent;
		stcUKeyData.bFlags = pUKeyData->bFlags = TRUE;

		stcUKeyData.emUKeyState = pUKeyData->emUKeyState = CK_UKEYSTATESUCCEDTYPE;
		stcUKeyData.emUKeyOperateType = pUKeyData->emUKeyOperateType = CK_OPERATEREADTYPE;
		sprintf(stcUKeyUserData.szPromptText, _T("读取Key设备Slot:%d用户数据成功!"), pUKeyData->ulSlotId);
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);
	} while (FALSE);

	if (!bRet)
	{
		m_pfUKDataFunc(&stcUKeyData, &stcUKeyUserData, m_lpParam);
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
//
/*
 * bFlags : TRUE, 写入设备操作; FALSE, 读取设备操作;
 * pfUKDataFunc : 接口操作回调函数;
 * lpParam : 用户传入数据;
*/ 
BOOL CUKeyVerifyThread::CreateVerifyProc(BOOL bFlags, CK_UKDATA_CALLBACK_FUNC pfUKDataFunc, LPVOID lpParam)
{
	BOOL bRet = FALSE;

	if (m_hThread != NULL)
	{
		return FALSE;
	}
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);
		
		m_bFlags = bFlags;
		m_lpParam = lpParam;
		m_pfUKDataFunc = pfUKDataFunc;
		
		m_hThread = CreateThread(NULL, 0, UKeyVerifyThreadProc, (LPVOID)this, CREATE_SUSPENDED, &m_dwThreadID);
		if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}

		if (!g_UKeyFindThread.CreateFindProc())
		{
			if (m_hThread != NULL)
			{
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}

			ResetEvent(m_hStartEvent);
			return FALSE;
		}
		
		bRet = TRUE;
		m_bExit = FALSE;
	}
	
	return bRet;
}

BOOL CUKeyVerifyThread::CloseVerifyProc()
{
	m_bExit = TRUE;

	if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	WaitForSingleObject(m_hEndEvent, m_dwCloseTimeOver);
	ResetEvent(m_hStartEvent);

	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return TRUE;
}

/*
 * bFlags : TRUE, 启动线程操作; FALSE, 暂停线程操作;
*/
BOOL CUKeyVerifyThread::SetThreadProcState(BOOL bFlags)
{
	DWORD dwRet = 0;

	if (m_hThread == NULL)
	{
		return FALSE;
	}

	if (bFlags)
	{
		dwRet = ResumeThread(m_hThread);
		if (dwRet == 0)
		{
			return TRUE;
		}
		else if (dwRet == 1)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		dwRet = SuspendThread(m_hThread);
		if (dwRet == (DWORD)-1)
		{
			return FALSE;
		}
	}

	return TRUE;
}

/*
 * dwProcTime : 线程运行等待时间;
 * dwCloseTime : 线程退出等待时间;
*/ 
void CUKeyVerifyThread::SetThreadProcTime(DWORD dwProcTime, DWORD dwCloseTime)
{
	m_dwProcTimeOver = dwProcTime;
	m_dwCloseTimeOver = dwCloseTime;
}

/*
 * lpszUserNum : UKEY设备写入操作时, 传入的用户号码;
 * lpszUserPasswd : UKEY设备写入操作时, 传入的用户密码;
*/
void CUKeyVerifyThread::SetUserData(LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd)
{
	m_strUserNum = lpszUserNum;
	m_strUserPasswd = lpszUserPasswd;
}

void CUKeyVerifyThread::SetUserPIN(LPCTSTR lpszUserPIN)
{
	m_strUserPIN = lpszUserPIN;
}