#include "stdafx.h"
#include "UKeyEnumThread.h"
#include "../inc/Pkcs11_Ukey.h"

static bool     g_bExit;

static ULONG 	g_ulThreadID;
static ULONG	g_ulProcTimeOver;
static ULONG	g_ulCloseTimeOver;

static HANDLE	g_hThread;
static HANDLE	g_hStartEvent;
static HANDLE	g_hEndEvent;

static CK_UKEYENUM_CALLBACK_FUNC g_pfUKeyEnum;
static bool		GetUKeyEnumInfo();

ULONG WINAPI UKeyEnumThreadProc(LPVOID lpParam)
{
	while(WaitForSingleObject(g_hEndEvent, g_ulProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!g_bExit)
		{
			if (!GetUKeyEnumInfo())
			{
				continue;
			}
		}
		else
		{
			SetEvent(g_hEndEvent);
		}
	};
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void UKeyEnumInitialize()
{
	g_bExit = false;
	
	g_ulThreadID = 0;
	g_ulProcTimeOver = 500;
	g_ulCloseTimeOver = 500;
	
	g_hThread = NULL;
	g_pfUKeyEnum = NULL;

	g_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void UKeyEnumFinalize()
{
	if (g_hStartEvent != NULL)
	{
		CloseHandle(g_hStartEvent);
		g_hStartEvent = NULL;
	}

	if (g_hEndEvent != NULL)
	{
		CloseHandle(g_hEndEvent);
		g_hEndEvent = NULL;
	}
}

bool openUKeyEnumProc(CK_UKEYENUM_CALLBACK_FUNC pfUKeyEnum, HANDLE &hEnumHandle)
{
	bool bRet = false;
	
	if (g_hThread != NULL || pfUKeyEnum == NULL)
	{
		return false;
	}
	
	if(WaitForSingleObject(g_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(g_hStartEvent);
		ResetEvent(g_hEndEvent);

		g_pfUKeyEnum = pfUKeyEnum;
		
		g_hThread = CreateThread(NULL, 0, UKeyEnumThreadProc, NULL, 0, &g_ulThreadID);
		if(g_hThread == NULL || g_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(g_hStartEvent);
			return false;
		}
	
		g_bExit = false;
		hEnumHandle = g_hThread;

		bRet = true;
	}
	
	return bRet;
}

bool closeUKeyEnumProc()
{
	g_bExit = true;
	if (g_hThread == NULL || g_hThread == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	WaitForSingleObject(g_hEndEvent, g_ulCloseTimeOver);
	ResetEvent(g_hStartEvent);
	
	if (g_hThread != NULL)
	{
		CloseHandle(g_hThread);
		g_hThread = NULL;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
bool GetUKeyEnumInfo()
{
	bool bRet = false;
	std::vector<CK_UKEYDEVICE*> vecUKeyDevice;

	do 
	{
		if (!PKCS11_GetSlotList())
		{
			bRet = false;
			break;
		}

		if (!PKCS11_GetSlotId(vecUKeyDevice))
		{
			bRet = false;
			break;
		}

		if (vecUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		if (g_pfUKeyEnum != NULL)
		{
			g_pfUKeyEnum(&vecUKeyDevice[0]->stcUKeyEnum);
		}

		bRet = true;
	} while (false);

	return bRet;
}