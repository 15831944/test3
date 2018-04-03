#include "stdafx.h"
#include "UKeyEnumThread.h"
#include "../inc/Pkcs11_Ukey.h"

static bool	GetUKeyEnumInfo(CK_UKEYHANDLE *pUKeyHandle);

ULONG WINAPI UKeyEnumThreadProc(LPVOID lpParam)
{
	CK_UKEYHANDLE *pUKeyHandle = (CK_UKEYHANDLE*)lpParam;
	if (pUKeyHandle == NULL)
	{
		return -1;
	}

	while(WaitForSingleObject(pUKeyHandle->stcUKeyEnumThread.hEndEvent, pUKeyHandle->stcUKeyEnumThread.ulProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!pUKeyHandle->stcUKeyEnumThread.bExit)
		{
			if (!GetUKeyEnumInfo(pUKeyHandle))
			{
				continue;
			}
		}
		else
		{
			SetEvent(pUKeyHandle->stcUKeyEnumThread.hEndEvent);
		}
	};
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void UKeyEnumInitialize(CK_UKEYHANDLE *pUKeyHandle)
{
	if (pUKeyHandle == NULL)
	{
		return;
	}

	pUKeyHandle->stcUKeyEnumThread.bExit = false;

	pUKeyHandle->stcUKeyEnumThread.ulThreadID = 0;
	pUKeyHandle->stcUKeyEnumThread.ulProcTimeOver = 500;
	pUKeyHandle->stcUKeyEnumThread.ulCloseTimeOver = 500;
	
	pUKeyHandle->stcUKeyEnumThread.hThread = NULL;
	pUKeyHandle->stcUKeyEnumThread.hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	pUKeyHandle->stcUKeyEnumThread.hEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void UKeyEnumFinalize(CK_UKEYHANDLE *pUKeyHandle)
{
	if (pUKeyHandle == NULL)
	{
		return;
	}

	if (pUKeyHandle->stcUKeyEnumThread.hStartEvent != NULL)
	{
		CloseHandle(pUKeyHandle->stcUKeyEnumThread.hStartEvent);
		pUKeyHandle->stcUKeyEnumThread.hStartEvent = NULL;
	}

	if (pUKeyHandle->stcUKeyEnumThread.hEndEvent != NULL)
	{
		CloseHandle(pUKeyHandle->stcUKeyEnumThread.hEndEvent);
		pUKeyHandle->stcUKeyEnumThread.hEndEvent = NULL;
	}
}

bool openUKeyEnumProc(CK_UKEYHANDLE *pUKeyHandle)
{
	bool bRet = false;
	
	if (pUKeyHandle == NULL || pUKeyHandle->stcUKeyEnumThread.hThread != NULL)
	{
		return false;
	}
	
	if(WaitForSingleObject(pUKeyHandle->stcUKeyEnumThread.hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(pUKeyHandle->stcUKeyEnumThread.hStartEvent);
		ResetEvent(pUKeyHandle->stcUKeyEnumThread.hEndEvent);

		pUKeyHandle->stcUKeyEnumThread.hThread = CreateThread(NULL, 0, UKeyEnumThreadProc, (LPVOID)pUKeyHandle, 0, &pUKeyHandle->stcUKeyEnumThread.ulThreadID);
		if(pUKeyHandle->stcUKeyEnumThread.hThread == NULL || pUKeyHandle->stcUKeyEnumThread.hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(pUKeyHandle->stcUKeyEnumThread.hStartEvent);
			return false;
		}
	
		pUKeyHandle->stcUKeyEnumThread.bExit = false;
		pUKeyHandle->hEnumThread = pUKeyHandle->stcUKeyEnumThread.hThread;

		bRet = true;
	}
	
	return bRet;
}

bool closeUKeyEnumProc(CK_UKEYHANDLE *pUKeyHandle)
{
	if (pUKeyHandle = NULL)
	{
		return false;
	}

	pUKeyHandle->stcUKeyEnumThread.bExit = true;
	if (pUKeyHandle->stcUKeyEnumThread.hThread == NULL || pUKeyHandle->stcUKeyEnumThread.hThread == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	WaitForSingleObject(pUKeyHandle->stcUKeyEnumThread.hEndEvent, pUKeyHandle->stcUKeyEnumThread.ulCloseTimeOver);
	ResetEvent(pUKeyHandle->stcUKeyEnumThread.hStartEvent);
	
	if (pUKeyHandle->stcUKeyEnumThread.hThread != NULL)
	{
		CloseHandle(pUKeyHandle->stcUKeyEnumThread.hThread);
		pUKeyHandle->stcUKeyEnumThread.hThread = NULL;
	}

	pUKeyHandle->hEnumThread = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//
bool GetUKeyEnumInfo(CK_UKEYHANDLE *pUKeyHandle)
{
	bool bRet = false;

	do 
	{
		if (pUKeyHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (!PKCS11_GetSlotId(pUKeyHandle))
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	return bRet;
}