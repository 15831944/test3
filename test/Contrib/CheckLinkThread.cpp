#include "stdafx.h"
#include "CheckLinkThread.h"

#pragma comment ( lib, "iphlpapi.lib")
CCheckLinkThread::CCheckLinkThread()
{
	m_bExit = FALSE;
	
	m_dwThreadID = 0;
	m_dwProcTimeOver = 500;
	m_dwCloseTimeOver = 500;
	
	m_hThread = NULL;
	m_pIpAdapterInfo = NULL;
	m_pfCheckLinkFunc = NULL;

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CCheckLinkThread::~CCheckLinkThread()
{
	CloseCheckLinkProc();
	
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
}

DWORD CCheckLinkThread::CheckLinkProc(LPVOID lpParam)
{
	CCheckLinkThread *pCheckLinkProc = (CCheckLinkThread*)lpParam;
	if (pCheckLinkProc != NULL)
	{
		pCheckLinkProc->CheckLinkInfo();
	}
	
	return 0;
}

BOOL CCheckLinkThread::CreateCheckLinkProc(CHECK_LINK_CALLBACK_FUNC pfCheckLinkFunc)
{
	BOOL bRet = FALSE;
	
	do
	{
		if (m_hThread != NULL)
		{
			bRet = FALSE;
			break;
		}
		
		if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
		{
			SetEvent(m_hStartEvent);
			ResetEvent(m_hEndEvent);

			m_pfCheckLinkFunc = pfCheckLinkFunc;
			
			m_hThread = CreateThread(NULL, 0, CheckLinkProc, (LPVOID)this, 0, &m_dwThreadID);
			if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
			{
				bRet = FALSE;
				ResetEvent(m_hStartEvent);
				break;
			}
			
			bRet = TRUE;
			m_bExit = FALSE;
		}
		
	} while (FALSE);
	
	return bRet;
}

void CCheckLinkThread::CloseCheckLinkProc()
{
	m_bExit = TRUE;
	if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	WaitForSingleObject(m_hEndEvent, m_dwCloseTimeOver);
	ResetEvent(m_hStartEvent);

	if (m_pIpAdapterInfo != NULL)
	{
		delete m_pIpAdapterInfo;
		m_pIpAdapterInfo = NULL;
	}
	
	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

void CCheckLinkThread::CheckLinkInfo()
{
	BOOL bRet = FALSE;
	LINK_ADAPTER_DATA stcLinkAdapterData = {0};
	LINK_ADAPTER_INFO stcLinkAdapterInfo = {0};

	do 
	{
		if (!GetIpAdapterInfo())
		{
			bRet = FALSE;
			break;
		}

		if (!GetLinkAdapterInfo(&stcLinkAdapterData, &stcLinkAdapterInfo))
		{
			bRet = FALSE;
			break;
		}

		while(WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) != WAIT_OBJECT_0)
		{
			if (!m_bExit)
			{
				if (!CheckLinkAdapterInfo(&stcLinkAdapterData, &stcLinkAdapterInfo))
				{
					continue;
				}
			}
			else
			{
				SetEvent(m_hEndEvent);
			}
		};
	} while (FALSE);
}

BOOL CCheckLinkThread::GetIpAdapterInfo()
{
	BOOL bRet = FALSE;

	ULONG ulRet = 0;
	ULONG ulIpAdapterSize = 0;

	do 
	{
		if (m_pIpAdapterInfo != NULL)
		{
			delete m_pIpAdapterInfo;
			m_pIpAdapterInfo = NULL;
		}

		ulIpAdapterSize = sizeof(IP_ADAPTER_INFO);
		ulRet = GetAdaptersInfo(m_pIpAdapterInfo, &ulIpAdapterSize);
		if (ulRet == ERROR_BUFFER_OVERFLOW)
		{
			if (m_pIpAdapterInfo != NULL)
			{
				delete m_pIpAdapterInfo;
				m_pIpAdapterInfo = NULL;
			}

			m_pIpAdapterInfo = (IP_ADAPTER_INFO*)new BYTE[ulIpAdapterSize];
			if (m_pIpAdapterInfo == NULL)
			{
				bRet = FALSE;
				break;
			}
			memset(m_pIpAdapterInfo, 0x0, ulIpAdapterSize);

			ulRet = GetAdaptersInfo(m_pIpAdapterInfo, &ulIpAdapterSize);
		}
		if (ulRet != ERROR_SUCCESS)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CCheckLinkThread::GetLinkAdapterInfo(LINK_ADAPTER_DATA *pLinkAdapterData, LINK_ADAPTER_INFO *pLinkAdapterInfo)
{
	BOOL bRet = FALSE;
	ULONG ulAdapterIndex = 0;

	char *pIpString = NULL;
	char *pIpAdapterName = NULL;

	IP_ADDR_STRING *pIpAddrString = NULL;
	IP_ADAPTER_INFO *pIpAdapterInfo = NULL;

	do 
	{
		if (m_pIpAdapterInfo == NULL || m_pfCheckLinkFunc == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (pLinkAdapterData == NULL || pLinkAdapterInfo == NULL)
		{
			bRet = FALSE;
			break;
		}
	
		memset(pLinkAdapterInfo, 0x0, sizeof(LINK_ADAPTER_INFO));
		pLinkAdapterInfo->emLinkStateType = LINK_STATEINPUTTYPE;
		if (!m_pfCheckLinkFunc(pLinkAdapterInfo))
		{
			bRet = FALSE;
			break;
		}

		if (strcmp(pLinkAdapterInfo->szIpAddress, _T("")) == 0)
		{
			bRet = FALSE;
			break;
		}

		pIpAdapterInfo = m_pIpAdapterInfo;
		while(pIpAdapterInfo)
		{
			ulAdapterIndex = pIpAdapterInfo->Index;
			pIpAdapterName = pIpAdapterInfo->AdapterName;

			pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do 
			{
				pIpString = pIpAddrString->IpAddress.String;
				if (pIpString != NULL)
				{
					if (strcmp(pIpString, pLinkAdapterInfo->szIpAddress) == 0)
					{
						bRet = TRUE;

						pLinkAdapterData->ulAdapterIndex = ulAdapterIndex;
						
 						memcpy(pLinkAdapterData->szIpAddress, pIpString, strlen(pIpString));
 						memcpy(pLinkAdapterData->szAdapterName, pIpAdapterName, strlen(pIpAdapterName));

 						memcpy(&pLinkAdapterData->stcIpAdapterInfo, pIpAdapterInfo, sizeof(IP_ADAPTER_INFO));
						break;
					}
				}

				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);

			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	} while (FALSE);

	return bRet;
}

BOOL CCheckLinkThread::CheckLinkAdapterInfo(LINK_ADAPTER_DATA *pLinkAdapterData, LINK_ADAPTER_INFO *pLinkAdapterInfo)
{
	BOOL bRet = FALSE;
	MIB_IFROW stcMIBInfo = {0};

	do 
	{
		if (pLinkAdapterData == NULL || pLinkAdapterInfo == NULL || m_pfCheckLinkFunc == NULL)
		{
			bRet = FALSE;
			break;
		}

		stcMIBInfo.dwIndex = pLinkAdapterData->ulAdapterIndex;
		if (GetIfEntry(&stcMIBInfo) != NOERROR)
		{
			bRet = FALSE;
			break;
		}

		if (stcMIBInfo.dwOperStatus == IF_OPER_STATUS_NON_OPERATIONAL	//ÊÊÅäÆ÷±»½ûÓÃ
			|| stcMIBInfo.dwOperStatus == IF_OPER_STATUS_UNREACHABLE	//ÊÊÅäÆ÷Î´Á¬½Ó
			|| stcMIBInfo.dwOperStatus == IF_OPER_STATUS_DISCONNECTED	//ÊÊÅäÆ÷ÏßÀÂÎ´Á¬½Ó
			|| stcMIBInfo.dwOperStatus == IF_OPER_STATUS_CONNECTING)	//ÊÊÅäÆ÷Á¬½ÓÖÐ
		{
			pLinkAdapterInfo->bIsConnected = FALSE;
		}
		else if (stcMIBInfo.dwOperStatus == IF_OPER_STATUS_OPERATIONAL	//ÊÊÅäÆ÷Ä¬ÈÏÁ¬½Ó×´Ì¬
			|| stcMIBInfo.dwOperStatus == IF_OPER_STATUS_CONNECTED)		//ÊÊÅäÆ÷Õý³£Á¬½Ó×´Ì¬
		{
			pLinkAdapterInfo->bIsConnected = TRUE;
		}

		pLinkAdapterInfo->emLinkStateType = LINK_STATEOUTPUTTYPE;
		if (!m_pfCheckLinkFunc(pLinkAdapterInfo))
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}