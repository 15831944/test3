#include "stdafx.h"
#include "UKeyWorkThread.h"

static bool     g_bExit;

static ULONG 	g_ulThreadID;
static ULONG	g_ulProcTimeOver;
static ULONG	g_ulCloseTimeOver;

static HANDLE	g_hThread;
static HANDLE	g_hStartEvent;
static HANDLE	g_hEndEvent;

static CK_UKEYVERIFY_CALLBACK_FUNC	g_pfUkeyVerify;
static CK_UKEYREAD_CALLBACK_FUNC    g_pfUkeyReadData;
static CK_UKEYWRITE_CALLBACK_FUNC   g_pfUKeyWriteData;

static bool		SetUKeyWorkInfo();
static bool		ReadUKeyData(int iSlotIndex);
static bool		WriteUKeyData(int iSlotIndex);

ULONG WINAPI UKeyWorkThreadProc(LPVOID lpParam)
{
	bool bRet = false;
	
	while(WaitForSingleObject(g_hEndEvent, g_ulProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!g_bExit)
		{
			if (!SetUKeyWorkInfo())
			{
				continue;
			}
		}
		else
		{
			SetEvent(g_hEndEvent);
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void UKeyWorkInitialize()
{
	g_bExit = false;
	
	g_ulThreadID = 0;
	g_ulProcTimeOver = 500;
	g_ulCloseTimeOver = 500;
	
	g_hThread = NULL;
	g_pfUkeyVerify = NULL;
	g_pfUkeyReadData = NULL;
	g_pfUKeyWriteData = NULL;
	
	g_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void UKeyWorkFinalize()
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

bool openUKeyWorkProc(CK_UKEYVERIFY_CALLBACK_FUNC pfUkeyVerify, CK_UKEYREAD_CALLBACK_FUNC pfUkeyReadData, CK_UKEYWRITE_CALLBACK_FUNC pfUKeyWriteData, HANDLE &hWorkHandle)
{
	bool bRet = false;
	
	if (g_hThread != NULL || pfUkeyVerify == NULL)
	{
		return false;
	}
	
	if(WaitForSingleObject(g_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(g_hStartEvent);
		ResetEvent(g_hEndEvent);

		g_pfUkeyVerify = pfUkeyVerify;
		g_pfUkeyReadData = pfUkeyReadData;
		g_pfUKeyWriteData = pfUKeyWriteData;
		
		g_hThread = CreateThread(NULL, 0, UKeyWorkThreadProc, NULL, 0, &g_ulThreadID);
		if(g_hThread == NULL || g_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(g_hStartEvent);
			return false;
		}
	
		g_bExit = false;
		hWorkHandle = g_hThread;

		bRet = true;
	}
	
	return bRet;
}

bool closeUKeyWorkProc()
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
}

//////////////////////////////////////////////////////////////////////////
//
bool SetUKeyWorkInfo()
{
	bool bRet = false;

	do 
	{
		if (g_pfUkeyReadData != NULL)
		{
			if (!ReadUKeyData(0))
			{
				bRet = false;
				break;
			}
		}
		else if (g_pfUKeyWriteData != NULL)
		{
			if (!WriteUKeyData(0))
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

bool ReadUKeyData(int iSlotIndex)
{
	return true;
}

bool WriteUKeyData(int iSlotIndex)
{
	return true;
}