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
	m_pfUKDataFunc = NULL;

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
	CK_UKPROCINFO stcUKeyProc = {0};
	
	while(WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			do 
			{
				if (!UKeySlotVerify(&stcUKeyProc))
				{
					bRet = FALSE;
					break;
				}

				if (m_bFlags)
				{
					stcUKeyProc.emOperateType = CK_OPERATEWRITETYPE;
					if (!SetObjectData(m_strUserNum, m_strUserPasswd, &stcUKeyProc))
					{
						bRet = FALSE;
						break;
					}
				}
				else
				{
					stcUKeyProc.emOperateType = CK_OPERATEREADTYPE;
					if (!GetObjectData(stcUKeyProc.szUserNum, stcUKeyProc.szUserPasswd, &stcUKeyProc))
					{
						bRet = FALSE;
						break;
					}
				}

				bRet = TRUE;
			} while (FALSE);

			m_pfUKDataFunc(&stcUKeyProc, NULL);
			bRet ? m_bExit = TRUE : 0;
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	};
}

BOOL CUKeyVerifyThread::UKeySlotVerify(CK_UKPROCINFO *pstcUKeyProc)
{
	BOOL bRet = FALSE;
	CK_ULONG ulFlags = 0;
	CK_ULONG ulSlotId = 0;

	char szUserPIN[64] = {0};

	if (pstcUKeyProc == NULL)
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
			pstcUKeyProc->emOperateType = CK_OPERATEENUMSLOTTYPE;
			sprintf(pstcUKeyProc->szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}

		if (!m_UKeyVerify.GetSlotId(0, ulSlotId))
		{
			bRet = FALSE;
			pstcUKeyProc->bExist = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			pstcUKeyProc->emOperateType = CK_OPERATEENUMSLOTTYPE;
			sprintf(pstcUKeyProc->szPromptText, _T("获取UKey设备Slot失败, 请检查!"));
			break;
		}
		else
		{
			pstcUKeyProc->bExist = TRUE;
			pstcUKeyProc->ulSlotId = ulSlotId;
		}

		if (!m_UKeyVerify.CreateSession(ulSlotId))
		{
			bRet = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyProc->szPromptText, _T("UKey设备Slot:%d创建Session失败, 请检查!"), ulSlotId);
			break;
		}
	
		if (!m_UKeyVerify.GetSlotInfo(ulSlotId, ulFlags))
		{
			bRet = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyProc->szPromptText, _T("获取UKey设备Slot:%d信息失败, 请检查!"), ulSlotId);
			break;
		}
		else
		{
			if (ulFlags & CKF_PROTECTED_AUTHENTICATION_PATH)
			{
				pstcUKeyProc->emUKType = CK_UKFINGERTYPE;
			}
			else
			{
				pstcUKeyProc->emUKType = CK_UKNORMALTYPE;
			}
		}
		
		pstcUKeyProc->bFlags = TRUE;
		pstcUKeyProc->emOperateType = CK_OPERATEVERIFYTYPE;
		m_pfUKDataFunc(pstcUKeyProc, szUserPIN);

		if (!m_UKeyVerify.LoginUser(szUserPIN, ulFlags))
		{
			bRet = FALSE;
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyProc->szPromptText, _T("UKey设备Slot:%d口令校验失败, 请检查!"), ulSlotId);
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

BOOL CUKeyVerifyThread::SetObjectData(LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd, CK_UKPROCINFO *pstcUKeyProc)
{
	BOOL bRet = FALSE;
	DWORD dwCount = 0;
	
	char szUserData[1024] = {0};
	DWORD dwUserDataLen = sizeof(szUserData)-1;
	
	if (lpszUserNum == NULL || *lpszUserNum == '\0')
	{
		return FALSE;
	}
	
	if (lpszUserPasswd == NULL || *lpszUserPasswd == '\0')
	{
		return FALSE;
	}

	if (pstcUKeyProc == NULL)
	{
		return FALSE;
	}
	
	dwUserDataLen  = strlen(lpszUserNum);
	memcpy(szUserData, lpszUserNum, strlen(lpszUserNum));
	dwUserDataLen += 1;
	memcpy(szUserData+dwUserDataLen, lpszUserPasswd, strlen(lpszUserPasswd));
	dwUserDataLen += strlen(lpszUserPasswd);

	if (!m_UKeyVerify.FindObject(dwCount))
	{
		pstcUKeyProc->bFlags = FALSE;
		sprintf(pstcUKeyProc->szPromptText, _T("UKey设备查找对象失败, 请检查!"));
		return FALSE;
	}
	
	if (dwCount == 0)
	{
		if (!m_UKeyVerify.CreateObject(szUserData, dwUserDataLen))
		{
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyProc->szPromptText, _T("UKey设备创建对象失败, 请检查!"));
			return FALSE;
		}
	}
	else
	{
		if (!m_UKeyVerify.SetObjectValue(szUserData, dwUserDataLen))
		{
			pstcUKeyProc->bFlags = FALSE;
			sprintf(pstcUKeyProc->szPromptText, _T("UKey设备修改对象失败, 请检查!"));
			return FALSE;
		}
	}
	
	pstcUKeyProc->bFlags = TRUE;
	return TRUE;
}

BOOL CUKeyVerifyThread::GetObjectData(char *pszUserNum, char *pszUserPasswd, CK_UKPROCINFO *pstcUKeyProc)
{
	char szUserData[1024] = {0};
	DWORD dwUserDataLen = sizeof(szUserData)-1;
	
	char *pValue = NULL;
	CK_ULONG ulIndex = 0;
	
	if (pszUserNum == NULL || pszUserPasswd == NULL)
	{
		return FALSE;
	}

	if (pstcUKeyProc == NULL)
	{
		return FALSE;
	}
	
	if (!m_UKeyVerify.GetObjectValue((CK_BYTE*)szUserData, &dwUserDataLen))
	{
		pstcUKeyProc->bFlags = FALSE;
		sprintf(pstcUKeyProc->szPromptText, _T("UKey设备获取对象数据失败, 请检查!"));
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
BOOL CUKeyVerifyThread::CreateVerifyProc(BOOL bFlags, CK_UKDATA_CALLBACK_FUNC pfUKDataFunc)
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

void CUKeyVerifyThread::SetThreadProcTime(DWORD dwProcTime, DWORD dwCloseTime)
{
	m_dwProcTimeOver = dwProcTime;
	m_dwCloseTimeOver = dwCloseTime;
}

void CUKeyVerifyThread::SetUserData(LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd)
{
	m_strUserNum = lpszUserNum;
	m_strUserPasswd = lpszUserPasswd;
}

void CUKeyVerifyThread::SetUserPIN(LPCTSTR lpszUserPIN)
{
	m_strUserPIN = lpszUserPIN;
}