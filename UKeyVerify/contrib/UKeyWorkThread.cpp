#include "stdafx.h"
#include "UKeyWorkThread.h"
#include "../inc/Pkcs11_Ukey.h"

static bool	SetUKeyWorkInfo(CK_UKEYHANDLE *pUKeyHandle);
static bool	ReadUKeyData(CK_UKEYHANDLE *pUKeyHandle, int iSlotIndex);
static bool	WriteUKeyData(CK_UKEYHANDLE *pUKeyHandle, int iSlotIndex);

ULONG WINAPI UKeyWorkThreadProc(LPVOID lpParam)
{
	CK_UKEYHANDLE *pUKeyHandle = (CK_UKEYHANDLE*)lpParam;
	if (pUKeyHandle == NULL)
	{
		return -1;
	}

	while(WaitForSingleObject(pUKeyHandle->strUKeyWorkThread.hEndEvent, pUKeyHandle->strUKeyWorkThread.ulProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!pUKeyHandle->strUKeyWorkThread.bExit)
		{
			if (!SetUKeyWorkInfo(pUKeyHandle))
			{
				continue;
			}
		}
		else
		{
			SetEvent(pUKeyHandle->strUKeyWorkThread.hEndEvent);
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void UKeyWorkInitialize(CK_UKEYHANDLE *pUKeyHandle)
{
	if (pUKeyHandle == NULL)
	{
		return;
	}

	pUKeyHandle->strUKeyWorkThread.bExit = false;

	pUKeyHandle->strUKeyWorkThread.ulThreadID = 0;
	pUKeyHandle->strUKeyWorkThread.ulProcTimeOver = 500;
	pUKeyHandle->strUKeyWorkThread.ulCloseTimeOver = 500;

	pUKeyHandle->strUKeyWorkThread.hThread = NULL;
	pUKeyHandle->strUKeyWorkThread.hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	pUKeyHandle->strUKeyWorkThread.hEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void UKeyWorkFinalize(CK_UKEYHANDLE *pUKeyHandle)
{
	if (pUKeyHandle == NULL)
	{
		return;
	}

	if (pUKeyHandle->strUKeyWorkThread.hStartEvent != NULL)
	{
		CloseHandle(pUKeyHandle->strUKeyWorkThread.hStartEvent);
		pUKeyHandle->strUKeyWorkThread.hStartEvent = NULL;
	}

	if (pUKeyHandle->strUKeyWorkThread.hEndEvent != NULL)
	{
		CloseHandle(pUKeyHandle->strUKeyWorkThread.hEndEvent);
		pUKeyHandle->strUKeyWorkThread.hEndEvent = NULL;
	}
}

bool openUKeyWorkProc(CK_UKEYHANDLE *pUKeyHandle)
{
	bool bRet = false;
	
	if (pUKeyHandle == NULL || pUKeyHandle->strUKeyWorkThread.hThread != NULL)
	{
		return false;
	}
	
	if(WaitForSingleObject(pUKeyHandle->strUKeyWorkThread.hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(pUKeyHandle->strUKeyWorkThread.hStartEvent);
		ResetEvent(pUKeyHandle->strUKeyWorkThread.hEndEvent);
		
		pUKeyHandle->strUKeyWorkThread.hThread = CreateThread(NULL, 0, UKeyWorkThreadProc, (LPVOID)pUKeyHandle, 0, &pUKeyHandle->strUKeyWorkThread.ulThreadID);
		if(pUKeyHandle->strUKeyWorkThread.hThread == NULL || pUKeyHandle->strUKeyWorkThread.hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(pUKeyHandle->strUKeyWorkThread.hStartEvent);
			return false;
		}
	
		pUKeyHandle->strUKeyWorkThread.bExit = false;
		pUKeyHandle->hWorkThread = pUKeyHandle->strUKeyWorkThread.hThread;

		bRet = true;
	}
	
	return bRet;
}

bool closeUKeyWorkProc(CK_UKEYHANDLE *pUKeyHandle)
{
	if (pUKeyHandle == NULL)
	{
		return false;
	}

	pUKeyHandle->strUKeyWorkThread.bExit = true;
	if (pUKeyHandle->strUKeyWorkThread.hThread == NULL || pUKeyHandle->strUKeyWorkThread.hThread == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	WaitForSingleObject(pUKeyHandle->strUKeyWorkThread.hEndEvent, pUKeyHandle->strUKeyWorkThread.ulCloseTimeOver);
	ResetEvent(pUKeyHandle->strUKeyWorkThread.hStartEvent);
	
	if (pUKeyHandle->strUKeyWorkThread.hThread != NULL)
	{
		CloseHandle(pUKeyHandle->strUKeyWorkThread.hThread);
		pUKeyHandle->strUKeyWorkThread.hThread = NULL;
	}

	pUKeyHandle->hWorkThread = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//
bool SetUKeyWorkInfo(CK_UKEYHANDLE *pUKeyHandle)
{
	bool bRet = false;

	do 
	{
		if (pUKeyHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (pUKeyHandle->pfUkeyReadData != NULL)
		{
			if (!ReadUKeyData(pUKeyHandle, 0))
			{
				bRet = false;
				break;
			}
		}
		else if (pUKeyHandle->pfUKeyWriteData != NULL)
		{
			if (!WriteUKeyData(pUKeyHandle, 0))
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	return bRet;
}

bool enumUKeyDevice(CK_UKEYHANDLE *pUKeyHandle, int iSlotIndex, CK_ULONG *pulSlotId, CK_UKEYDEVICETYPE &emUKeyType, HANDLE *pUKeyReadEvent, HANDLE *pUKeyWriteEvent)
{
	bool bRet = false;

	CK_UKEYENUM stcUKeyEnum = {0};
	std::vector<CK_UKEYDEVICE *> vecUKeyDevice;

	do 
	{
		if (pUKeyHandle == NULL || pulSlotId == NULL)
		{
			bRet = false;
			break;
		}

		if (!PKCS11_GetSlotList(pUKeyHandle, vecUKeyDevice))
		{
			bRet = false;
			break;
		}

		if (vecUKeyDevice.size() == 0 || vecUKeyDevice.size() < (iSlotIndex+1))
		{
			bRet = false;
			break;
		}

		*pulSlotId = vecUKeyDevice[iSlotIndex]->ulSlotId;
		emUKeyType = vecUKeyDevice[iSlotIndex]->stcUKeyEnum.emUKeyType;

		if (pUKeyReadEvent != NULL)
		{
			*pUKeyReadEvent = vecUKeyDevice[iSlotIndex]->stcUKeyRead.hEvent;
		}

		if (pUKeyWriteEvent != NULL)
		{
			*pUKeyWriteEvent = vecUKeyDevice[iSlotIndex]->stcUKeyWrite.hEvent;
		}

		bRet = true;
		memcpy(&stcUKeyEnum, &vecUKeyDevice[iSlotIndex]->stcUKeyEnum, sizeof(CK_UKEYENUM));
	} while (false);

	if (pUKeyHandle->pfUKeyEnum != NULL)
	{
		pUKeyHandle->pfUKeyEnum(&stcUKeyEnum);
	}

	return bRet;
}

bool verifyUKeyDevice(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYDEVICETYPE emUKeyType)
{
	bool bRet = false;

	CK_SESSION_HANDLE hSession;
	CK_UKEYVERIFY stcUKeyVerify = {0};

	do 
	{
		if (pUKeyHandle == NULL || ulSlotId == 0)
		{
			bRet = false;
			break;
		}

		if (!PKCS11_CreateSession(pUKeyHandle, ulSlotId))
		{
			bRet = false;
			break;
		}

		if (pUKeyHandle->pfUkeyVerify != NULL)
		{
			stcUKeyVerify.emUKeyType = emUKeyType;
			stcUKeyVerify.emUKeyState = CK_UKEYSTATEINPUTETYPE;
			pUKeyHandle->pfUkeyVerify(&stcUKeyVerify);
		}

		if (!PKCS11_LoginUser(pUKeyHandle, ulSlotId, stcUKeyVerify.szUserPIN))
		{
			bRet = false;
			stcUKeyVerify.emUKeyState = CK_UKEYSTATEFAILEDTYPE;
			break;
		}

		/*
		if (pUKeyHandle->pfUkeyVerify != NULL)
		{
		stcUKeyVerify.emUKeyState = CK_UKEYSTATEMODIFYTYPE;
		pUKeyHandle->pfUkeyVerify(&stcUKeyVerify);
		}

		if (!PKCS11_SetUserPin(pUKeyHandle, ulSlotId, stcUKeyVerify.szUserPIN, stcUKeyVerify.szNewUserPIN))
		{
		bRet = false;
		stcUKeyVerify.emUKeyState = CK_UKEYSTATEFAILEDTYPE;
		break;
		}

		//PKCS11_FingerEnroll(pUKeyHandle, ulSlotId);
		*/

		bRet = true;
		stcUKeyVerify.emUKeyState = CK_UKEYSTATESUCCEDTYPE;
	} while (false);

	if (pUKeyHandle->pfUkeyVerify != NULL)
	{
		pUKeyHandle->pfUkeyVerify(&stcUKeyVerify);
	}

	return bRet;
}

bool closeUKeySession(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId)
{
	bool bRet = false;

	do 
	{
		if (pUKeyHandle == NULL || ulSlotId == 0)
		{
			bRet = false;
			break;
		}

		if (!PKCS11_LogoutUser(pUKeyHandle, ulSlotId))
		{
			bRet = false;
			break;
		}

		if (!PKCS11_CloseSession(pUKeyHandle, ulSlotId))
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	return bRet;
}

bool readUKeyDeviceData(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYDEVICETYPE emUKeyType, HANDLE hUKeyReadEvent)
{
	bool bRet = false;
	CK_UKEYREADDATA stcUKeyReadData = {0};

	do 
	{
		if (pUKeyHandle == NULL || hUKeyReadEvent == NULL || ulSlotId == 0)
		{
			bRet = false;
			break;
		}

		if (pUKeyHandle->pfUkeyReadData)
		{
			stcUKeyReadData.emUKeyState = CK_UKEYSTATEINPUTETYPE;
			stcUKeyReadData.hEvent = hUKeyReadEvent;
			pUKeyHandle->pfUkeyReadData(&stcUKeyReadData);
		}

		if (WaitForSingleObject(hUKeyReadEvent, 0) != WAIT_OBJECT_0)
		{
			if (!verifyUKeyDevice(pUKeyHandle, ulSlotId, emUKeyType))
			{
				bRet = false;
				break;
			}

			if (!PKCS11_GetObjectValue(pUKeyHandle, ulSlotId, &stcUKeyReadData))
			{
				bRet = false;
				break;
			}

			if (!closeUKeySession(pUKeyHandle, ulSlotId))
			{
				bRet = false;
				break;
			}

			bRet = true;
		}
	} while (false);

	if (pUKeyHandle->pfUkeyReadData)
	{
		pUKeyHandle->pfUkeyReadData(&stcUKeyReadData);
	}

	return bRet;
}

bool writeUKeyDeviceData(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYDEVICETYPE emUKeyType, HANDLE hUKeyWriteEvent)
{
	bool bRet = false;

	CK_ULONG ulCount = 0;
	CK_UKEYWRITEDATA stcUKeyWriteData = {0};

	do 
	{
		if (pUKeyHandle == NULL || hUKeyWriteEvent == NULL || ulSlotId == 0)
		{
			bRet = false;
			break;
		}

		if (pUKeyHandle->pfUKeyWriteData)
		{
			stcUKeyWriteData.emUKeyState = CK_UKEYSTATEINPUTETYPE;
			stcUKeyWriteData.hEvent = hUKeyWriteEvent;
			pUKeyHandle->pfUKeyWriteData(&stcUKeyWriteData);
		}

		if (!PKCS11_FindObject(pUKeyHandle, ulSlotId, ulCount))
		{
			bRet = false;
			stcUKeyWriteData.emUKeyState = CK_UKEYSTATEFAILEDTYPE;
			break;
		}

		if (ulCount == 0)
		{
			if (!PKCS11_CreateObject(pUKeyHandle, ulSlotId, &stcUKeyWriteData))
			{
				bRet = false;
				break;
			}
		}
		else
		{
			if (!PKCS11_SetObjectValue(pUKeyHandle, ulSlotId, &stcUKeyWriteData))
			{
				bRet = false;
				break;
			}
		}

		bRet = true;
	} while (false);

	if (pUKeyHandle->pfUKeyWriteData)
	{
		pUKeyHandle->pfUKeyWriteData(&stcUKeyWriteData);
	}

	return bRet;
}

bool ReadUKeyData(CK_UKEYHANDLE *pUKeyHandle, int iSlotIndex)
{
	bool bRet = false;
	bool bIsVerify = false;

	CK_ULONG ulSlotId = 0;
	HANDLE hUKeyReadEvent;
	CK_UKEYDEVICETYPE emUKeyType;

	do 
	{
		if (pUKeyHandle == NULL || iSlotIndex < 0)
		{
			bRet = false;
			break;
		}

		if (!enumUKeyDevice(pUKeyHandle, iSlotIndex, &ulSlotId, emUKeyType, &hUKeyReadEvent, NULL))
		{
			bRet = false;
			break;
		}

		if (!readUKeyDeviceData(pUKeyHandle, ulSlotId, emUKeyType, hUKeyReadEvent))
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	return bRet;
}

bool WriteUKeyData(CK_UKEYHANDLE *pUKeyHandle, int iSlotIndex)
{
	bool bRet = false;
	bool bIsVerify = false;

	CK_ULONG ulSlotId = 0;
	HANDLE hUKeyWriteEvent;
	CK_UKEYDEVICETYPE emUKeyType;
/*
	do 
	{
		if (pUKeyHandle == NULL || iSlotIndex < 0)
		{
			bRet = false;
			break;
		}

		if (!enumUKeyDevice(pUKeyHandle, iSlotIndex, &ulSlotId, &bIsVerify, emUKeyType, NULL, &hUKeyWriteEvent))
		{
			bRet = false;
			break;
		}

		if (!bIsVerify)
		{
			if (!verifyUKeyDevice(pUKeyHandle, ulSlotId, emUKeyType))
			{
				bRet = false;
				break;
			}
		}

		if (!writeUKeyDeviceData(pUKeyHandle, ulSlotId, hUKeyWriteEvent))
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);
	*/
	return bRet;
}