#include "StdAfx.h"
#include "UKeyVerifyThread.h"

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2018.03.20 11:39
 */
/************************************************************************/

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

	memset(&m_stcCurUKeyData, 0x0, sizeof(CK_UKEYPROCINFO));

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_UKeyVerify.Initialize();
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

	m_UKeyVerify.Finalize();
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
	CK_ULONG ulFlags = 0;
	CK_ULONG ulSlotId = 0;

	CK_UKEYPROCINFO stcUKeyProc;
	CK_UKEYUSERDATA stcUkeyUserData;
	
	while(WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			do 
			{
				memset(&stcUKeyProc, 0x0, sizeof(CK_UKEYPROCINFO));
				memset(&stcUkeyUserData, 0x0, sizeof(CK_UKEYUSERDATA));

				if (!GetUKeySlotInfo(-1, &stcUKeyProc, &stcUkeyUserData, ulSlotId, ulFlags))
				{
					bRet = FALSE;
					memset(&m_stcCurUKeyData, 0x0, sizeof(CK_UKEYPROCINFO));
					break;
				}

				if (m_stcCurUKeyData.bFlags && m_stcCurUKeyData.ulSlotId == ulSlotId)
				{
					bRet = FALSE;
					break;
				}

				if (!UKeySlotVerify(ulSlotId, ulFlags, &stcUKeyProc, &stcUkeyUserData))
				{
					bRet = FALSE;
					break;
				}

				if (m_bFlags)
				{
					stcUKeyProc.emUKeyOperateType = CK_OPERATEWRITETYPE;
					stcUkeyUserData.emUKeyOperateType = CK_OPERATEWRITETYPE;

					if (!SetObjectData(m_strUserNum, m_strUserPasswd, &stcUKeyProc, &stcUkeyUserData))
					{
						bRet = FALSE;
						break;
					}
				}
				else
				{
					stcUKeyProc.emUKeyOperateType = CK_OPERATEREADTYPE;
					stcUkeyUserData.emUKeyOperateType = CK_OPERATEREADTYPE;

					if (!GetObjectData(stcUkeyUserData.szUserNum, stcUkeyUserData.szUserPasswd, &stcUKeyProc, &stcUkeyUserData))
					{
						bRet = FALSE;
						break;
					}
				}

				bRet = TRUE;
				memcpy(&m_stcCurUKeyData, &stcUKeyProc, sizeof(CK_UKEYPROCINFO));
			} while (FALSE);

			ClearUKeyInfo();
			m_pfUKDataFunc(&stcUKeyProc, &stcUkeyUserData, m_lpParam);
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	};
}

BOOL CUKeyVerifyThread::GetUKeySlotInfo(CK_LONG ulSlotIndex, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData, CK_ULONG &ulSlotId, CK_ULONG &ulFlags)
{
	BOOL bRet = FALSE;
	if (pstcUKeyProc == NULL || pstcUKeyUserData == NULL)
	{
		return FALSE;
	}

	do 
	{
		if (!m_UKeyVerify.GetSlotList())
		{
			bRet = FALSE;
			pstcUKeyProc->bExist = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			pstcUKeyProc->emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;
			sprintf(pstcUKeyUserData->szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}
		
		if (!m_UKeyVerify.GetSlotId(ulSlotIndex, ulSlotId))
		{
			bRet = FALSE;
			pstcUKeyProc->bExist = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			pstcUKeyProc->emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;
			sprintf(pstcUKeyUserData->szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}

		if (!m_UKeyVerify.GetSlotInfo(ulSlotId, ulFlags))
		{
			bRet = FALSE;

			if (ulFlags & CKR_TOKEN_NOT_PRESENT)	//CKR_DEVICE_REMOVED	//CKF_REMOVABLE_DEVICE
			{
				pstcUKeyProc->bExist = FALSE;
				pstcUKeyProc->emUKeyState = CK_UKEYREMOVETYPE;
			}

			pstcUKeyProc->bFlags = FALSE;
			pstcUKeyProc->ulSlotId = ulSlotId;
			pstcUKeyProc->emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;
			sprintf(pstcUKeyUserData->szPromptText, _T("获取UKey设备Slot:%d信息失败, 请检查!"), ulSlotId);
			break;
		}
		else
		{
			if (ulFlags & CKF_TOKEN_PRESENT)
			{
				if (ulFlags & CKF_PROTECTED_AUTHENTICATION_PATH)
				{
					pstcUKeyProc->emUKeyType = CK_UKFINGERTYPE;
				}
				else
				{
					pstcUKeyProc->emUKeyType = CK_UKNORMALTYPE;
				}

				pstcUKeyProc->bExist = TRUE;
				pstcUKeyProc->ulSlotId = ulSlotId;
				pstcUKeyProc->emUKeyState = CK_UKEYINSERTTYPE;
			}
			else
			{
				pstcUKeyProc->bExist = FALSE;
				pstcUKeyProc->bFlags = FALSE;
				pstcUKeyProc->emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;
				sprintf(pstcUKeyUserData->szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
				break;
			}
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CUKeyVerifyThread::UKeySlotVerify(CK_ULONG ulSlotId, CK_ULONG ulFlags, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData)
{
	BOOL bRet = FALSE;
	if (pstcUKeyProc == NULL || pstcUKeyUserData == NULL)
	{
		return FALSE;
	}

	do
	{
		if (!m_UKeyVerify.CreateSession(ulSlotId))
		{
			bRet = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyUserData->szPromptText, _T("UKey设备Slot:%d创建Session失败, 请检查!"), ulSlotId);
			break;
		}
	
		pstcUKeyProc->bFlags = TRUE;
		pstcUKeyProc->emUKeyOperateType = CK_OPERATEVERIFYTYPE;
		m_pfUKDataFunc(pstcUKeyProc, pstcUKeyUserData, m_lpParam);

		if (!m_UKeyVerify.LoginUser(pstcUKeyUserData->szUserPIN, ulFlags))
		{
			bRet = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyUserData->szPromptText, _T("UKey设备Slot:%d口令校验失败, 请检查!"), ulSlotId);
			break;
		}
		
		bRet = TRUE;
	} while (FALSE);
	
	if (!bRet)
	{
		ClearUKeyInfo();
	}
	
	return bRet;
}

void CUKeyVerifyThread::ClearUKeyInfo()
{
	m_UKeyVerify.LogoutUser();
	m_UKeyVerify.CloseSession();
}

BOOL CUKeyVerifyThread::SetObjectData(LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData)
{
	BOOL bRet = FALSE;
	DWORD dwCount = 0;
	
	char szUserData[1024] = {0};
	DWORD dwUserDataLen = sizeof(szUserData)-1;
	
	if (pstcUKeyProc == NULL || pstcUKeyUserData == NULL)
	{
		return FALSE;
	}
	
	if ((lpszUserNum != NULL && *lpszUserNum != '\0') && (lpszUserPasswd != NULL && *lpszUserPasswd != '\0'))
	{
		dwUserDataLen  = strlen(lpszUserNum);
		memcpy(szUserData, lpszUserNum, strlen(lpszUserNum));
		dwUserDataLen += 1;
		memcpy(szUserData+dwUserDataLen, lpszUserPasswd, strlen(lpszUserPasswd));
		dwUserDataLen += strlen(lpszUserPasswd);
	}
	else
	{
		m_pfUKDataFunc(pstcUKeyProc, pstcUKeyUserData, m_lpParam);

		dwUserDataLen  = strlen(pstcUKeyUserData->szUserNum);
		memcpy(szUserData, pstcUKeyUserData->szUserNum, strlen(pstcUKeyUserData->szUserNum));
		dwUserDataLen += 1;
		memcpy(szUserData+dwUserDataLen, pstcUKeyUserData->szUserPasswd, strlen(pstcUKeyUserData->szUserPasswd));
		dwUserDataLen += strlen(pstcUKeyUserData->szUserPasswd);
	}

	if (!m_UKeyVerify.FindObject(dwCount))
	{
		pstcUKeyProc->bFlags = FALSE;
		sprintf(pstcUKeyUserData->szPromptText, _T("UKey设备查找对象失败, 请检查!"));
		return FALSE;
	}
	
	if (dwCount == 0)
	{
		if (!m_UKeyVerify.CreateObject(szUserData, dwUserDataLen))
		{
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyUserData->szPromptText, _T("UKey设备创建对象失败, 请检查!"));
			return FALSE;
		}
	}
	else
	{
		if (!m_UKeyVerify.SetObjectValue(szUserData, dwUserDataLen))
		{
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyUserData->szPromptText, _T("UKey设备修改对象失败, 请检查!"));
			return FALSE;
		}
	}
	
	pstcUKeyProc->bFlags = TRUE;
	return TRUE;
}

BOOL CUKeyVerifyThread::GetObjectData(char *pszUserNum, char *pszUserPasswd, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData)
{
	char szUserData[1024] = {0};
	DWORD dwUserDataLen = sizeof(szUserData)-1;
	
	char *pValue = NULL;
	CK_ULONG ulIndex = 0;
	
	if (pszUserNum == NULL || pszUserPasswd == NULL)
	{
		return FALSE;
	}

	if (pstcUKeyProc == NULL || pstcUKeyUserData == NULL)
	{
		return FALSE;
	}
	
	if (!m_UKeyVerify.GetObjectValue((CK_BYTE*)szUserData, &dwUserDataLen))
	{
		pstcUKeyProc->bFlags = FALSE;
		sprintf(pstcUKeyUserData->szPromptText, _T("UKey设备获取对象数据失败, 请检查!"));
		return FALSE;
	}
	
	pValue = szUserData;
	while (*pValue != '\0')
	{
		if (ulIndex == 0)
		{
			memcpy(pszUserNum, pValue, strlen(pValue));
		}
		else if (ulIndex == 1)
		{
			memcpy(pszUserPasswd, pValue, strlen(pValue));
		}
		else
		{
			break;
		}
		
		ulIndex++;
		pValue += strlen(pValue);
		pValue += 1;
	};
	
	pstcUKeyProc->bFlags = TRUE;
	return TRUE;
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