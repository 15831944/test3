#include "stdafx.h"
#include "GlobalInfo.h"

CGlobalInfo* CGlobalInfo::m_pGlobal = NULL;
CGlobalInfo::CGlobalInfo(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

CGlobalInfo::~CGlobalInfo(void)
{
	WSACleanup();
}

CGlobalInfo* CGlobalInfo::CreateInstance()
{
	if (CGlobalInfo::m_pGlobal == NULL)
	{
		CGlobalInfo::m_pGlobal = new CGlobalInfo;
	}

	return CGlobalInfo::m_pGlobal;
}

void CGlobalInfo::DestroyInstance()
{
	if (CGlobalInfo::m_pGlobal != NULL)
	{
		delete CGlobalInfo::m_pGlobal;
		CGlobalInfo::m_pGlobal = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//
time_t CGlobalInfo::SystemTimeToTime(SYSTEMTIME st)
{
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);

	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	LONGLONG nLL;
	nLL = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;

	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	return pt;
}

time_t CGlobalInfo::SystemTimeToTimeEx(const SYSTEMTIME& st)
{
	tm temptm = {st.wSecond, 
				 st.wMinute, 
				 st.wHour, 
				 st.wDay, 
				 st.wMonth - 1, 
				 st.wYear - 1900, 
				 st.wDayOfWeek, 
				 0, 
				 0};

	return mktime(&temptm);
}

SYSTEMTIME CGlobalInfo::TimeToSystemTime(time_t t)
{
	LONGLONG nLL = Int32x32To64(t, 10000000) + 116444736000000000;

	FILETIME ft;
	ft.dwLowDateTime = (DWORD)nLL;
	ft.dwHighDateTime = (DWORD)(nLL >> 32);

	SYSTEMTIME pst;
	FileTimeToSystemTime(&ft, &pst);

	return pst;
}

SYSTEMTIME CGlobalInfo::TimeToSystemTimeEx(time_t t)
{
	tm temptm = *localtime(&t);

	SYSTEMTIME st = {1900 + temptm.tm_year, 
					 1 + temptm.tm_mon, 
					 temptm.tm_wday, 
					 temptm.tm_mday, 
					 temptm.tm_hour, 
					 temptm.tm_min, 
					 temptm.tm_sec, 
					 0};
	return st;
}

//////////////////////////////////////////////////////////////////////////
//
bool CGlobalInfo::GetDiskInfo(unsigned int nDrvIndex, char szArrayModelNo[MAX_PATH], char szArraySerialNo[MAX_PATH])
{
	HANDLE hDev;

	int nDrive =0;
	int btIDCmd = 0;

	int nModelNoLen = 0;
	int nSerialNoLen = 0;

	DWORD dwBytesReturned = 0;
	DWORD dwDiskData[256] = {0};

	SENDCMDINPARAMS InParams;
	GETVERSIONINPARAMS gvopVersionParams = {0};

	const WORD IDE_ATAPI_IDENTIFY = 0xA1;   // 读取ATAPI设备的命令
	const WORD IDE_ATA_IDENTIFY   = 0xEC;   // 读取ATA设备的命令

	USHORT* pIDSector = NULL;
	BYTE btIDOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];

	char szModelNo[MAX_PATH]   = {0};
	char szSerialNo[MAX_PATH]  = {0};
	char szDriveName[MAX_PATH] = {0};
	
	sprintf(szDriveName, _T("\\\\.\\PHYSICALDRIVE%d"), nDrvIndex);

	hDev = ::CreateFile(szDriveName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDev == NULL || hDev == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DeviceIoControl(hDev, SMART_GET_VERSION, NULL, 0, &gvopVersionParams, sizeof(gvopVersionParams), &dwBytesReturned, NULL);

// 	if (gvopVersionParams.bIDEDeviceMap <= 0)
// 	{
// 		if (hDev)
// 		{
// 			::CloseHandle(hDev);
// 			hDev = NULL;
// 		}
// 
// 		return false;
// 	}

	btIDCmd = (gvopVersionParams.bIDEDeviceMap >> nDrive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

	if(!DoIdentify(hDev, &InParams, (PSENDCMDOUTPARAMS)btIDOutCmd, (BYTE)btIDCmd, (BYTE)nDrive, &dwBytesReturned))
	{
		if (hDev)
		{
			::CloseHandle(hDev);
			hDev = NULL;
		}
		return false;
	}

	if (hDev)
	{
		::CloseHandle(hDev);
		hDev = NULL;
	}

	pIDSector = (USHORT*)((SENDCMDOUTPARAMS*)btIDOutCmd)->bBuffer;
	for(int i=0; i < 256; i++)	dwDiskData[i] = pIDSector[i];

	nSerialNoLen = MAX_PATH;
	ToLittleEndian(dwDiskData, 10, 19, szSerialNo, nSerialNoLen);

	nModelNoLen = MAX_PATH;
	ToLittleEndian(dwDiskData, 27, 46, szModelNo, nModelNoLen);

	strcpy(szArrayModelNo,  szModelNo);
	strcpy(szArraySerialNo, szSerialNo);
	return true;
}

bool CGlobalInfo::DNSLookupInfo(const char *pszDNSServerIp, const char *pszDomainName, char *pszDomainIp, unsigned int *puDomainIpLen)
{
	SOCKET hSocket;
	HANDLE hEvent;

	char *pIpAddr = NULL;
	struct in_addr ipaddr; 
	std::vector<ULONG> vecIPList;

	if (pszDNSServerIp == NULL  || *pszDNSServerIp == '\0')
	{
		return false;
	}

	if (pszDomainName == NULL || *pszDomainName == '\0')
	{
		return false;
	}

	if (pszDomainIp == NULL || puDomainIpLen == NULL)
	{
		return false;
	}

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	do 
	{
		hSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (hSocket == INVALID_SOCKET)
		{
			break;
		}

		if (!SendDNSRequest(hSocket, pszDNSServerIp, pszDomainName))
		{
			break;
		}

		if (!RecvDNSResponse(hSocket, hEvent, pszDNSServerIp, &vecIPList))
		{
			break;
		}

		if (vecIPList.size() == 0)
		{
			break;
		}
	} while (false);

	if (hSocket != INVALID_SOCKET)
	{
		closesocket(hSocket);
		hSocket = NULL;
	}

	ipaddr.s_addr = vecIPList[0];
	pIpAddr = inet_ntoa(ipaddr);
	if (pIpAddr == NULL)
	{
		return false;
	}

	if (*puDomainIpLen < strlen(pIpAddr))
	{
		return false;
	}

	*puDomainIpLen = strlen(pIpAddr);
	strcpy(pszDomainIp, pIpAddr);
	return true;
}

bool CGlobalInfo::OpenSysServer(const char *pszSvrName)
{
	bool bRet = false;
	DWORD dwCount = 0;
	const DWORD dwMaxRetryCount = 3;

	SC_HANDLE scHandle;
	SC_HANDLE svcHandle;

	SERVICE_STATUS svcStatus;

	do 
	{
		if (pszSvrName == NULL || *pszSvrName == '\0')
		{
			bRet = false;
			break;
		}

		scHandle = ::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
		if (scHandle == NULL)
		{
			bRet = false;
			break;
		}

		svcHandle = ::OpenService(scHandle, pszSvrName, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
		if (svcHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (!::QueryServiceStatus(svcHandle, &svcStatus))
		{
			bRet = false;
			break;
		}

		if (svcStatus.dwCurrentState == SERVICE_RUNNING)
		{
			bRet = true;
		}
		else
		{
			if (!::StartService(svcHandle, NULL, NULL))
			{
				bRet = false;
				break;
			}

			while (::QueryServiceStatus(svcHandle, &svcStatus))
			{
				if (dwCount >= dwMaxRetryCount)
				{
					bRet = false;
					break;
				}

				::Sleep( svcStatus.dwWaitHint);
				if (svcStatus.dwCurrentState == SERVICE_RUNNING)
				{
					bRet = true;
					break;
				}

				dwCount++;
			}
		}
	} while (false);

	if (svcHandle != NULL && svcHandle != INVALID_HANDLE_VALUE)
	{
		::CloseServiceHandle(svcHandle);
		svcHandle = NULL;
	}

	if (scHandle != NULL && scHandle != INVALID_HANDLE_VALUE)
	{
		::CloseServiceHandle(scHandle);
		scHandle = NULL;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
bool CGlobalInfo::DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP, BYTE btIDCmd, BYTE btDriveNum, PDWORD pdwBytesReturned)
{
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg = 0;
	pSCIP->irDriveRegs.bSectorCountReg  = 1;
	pSCIP->irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg  = 0;
	pSCIP->irDriveRegs.bCylHighReg = 0;

	pSCIP->irDriveRegs.bDriveHeadReg = (btDriveNum & 1) ? 0xB0 : 0xA0;
	pSCIP->irDriveRegs.bCommandReg = btIDCmd;
	pSCIP->bDriveNumber = btDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return DeviceIoControl(	hPhysicalDriveIOCTL, 
							SMART_RCV_DRIVE_DATA,
							(LPVOID)pSCIP,
							sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)pSCOP,
							sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
							pdwBytesReturned, NULL);
}

bool CGlobalInfo::ToLittleEndian(PDWORD pDiskData, int nFirstIndex, int nLastIndex, char* pResBuf, int &nResBufLen)
{
	int nIndex = 0;
	int nBufLen = 0;

	char* pDest = NULL;
	char szBuf[MAX_PATH] = {0};

	if (pDiskData == NULL)
	{
		return false;
	}

	if (pResBuf == NULL)
	{
		return false;
	}

	pDest = szBuf;

	for (nIndex=nFirstIndex; nIndex<nLastIndex; nIndex++)
	{
		pDest[0] = pDiskData[nIndex] >> 8;
		pDest[1] = pDiskData[nIndex] & 0xff;

		pDest += 2;
	}
	*pDest = 0;

	--pDest;
	while(*pDest == 0x20)
	{
		*pDest = 0;
		--pDest;
	}

	nBufLen = strlen(szBuf);
	if (nBufLen <= 0)
	{
		return false;
	}
	else
	{
		if (nBufLen > nResBufLen)
		{
			return false;
		}

		nResBufLen = nBufLen;
		strcpy(pResBuf, szBuf);
	}

	return true;
}

bool CGlobalInfo::SendDNSRequest(SOCKET hSocket, const char* pszDNSServerIp, const char* pszDomainName)
{
	bool bRet = false;

	unsigned int uQType = 0;
	unsigned int uQClass = 0;
	
	unsigned int uDNSPacketSize = 0;
	unsigned int uDomainNameLen = 0;
	unsigned int uDNSEncodeNameLen = 0;

	const short nDNS_TYPE_LEN = 2;
	const short nDNS_CLASS_LEN = 2;
	const short nMAX_DOMAINNAME_LEN = MAX_PATH;
	const short nDNS_PACKET_LEN = (sizeof(HT_DNS_HEADER) + nMAX_DOMAINNAME_LEN + nDNS_TYPE_LEN + nDNS_CLASS_LEN);

	HT_DNS_HEADER stDNSHeader;
	char *pszDNSPacket = NULL;
	char *pszDNSEncodeName = NULL;

	if (hSocket == INVALID_SOCKET)
	{
		return false;
	}
	
	if (pszDNSServerIp == NULL || *pszDNSServerIp == '\0')
	{
		return false;
	}

	if (pszDomainName == NULL || *pszDomainName == '\0')
	{
		return false;
	}

	sockaddr_in sockAddrDNSServer;
	sockAddrDNSServer.sin_family = AF_INET; 
	sockAddrDNSServer.sin_port = htons( 53 );
	sockAddrDNSServer.sin_addr.s_addr = inet_addr(pszDNSServerIp);

	do 
	{
		//
		uDomainNameLen = strlen(pszDomainName);
		uDNSEncodeNameLen = uDomainNameLen + 2;

		//DNS组包
		pszDNSPacket = new char[nDNS_PACKET_LEN];	//(std::nothrow)
		if (pszDNSPacket == NULL)
		{
			bRet = false;
			break;
		}
		memset(pszDNSPacket, 0x0, nDNS_PACKET_LEN);

		memset(&stDNSHeader, 0x0, sizeof(HT_DNS_HEADER));
		stDNSHeader.usTransId = GetCurrentProcessId();
		stDNSHeader.usFlag = htons(0x0100);
		stDNSHeader.usQuestionCount = htons(0x0001);
		stDNSHeader.usAnswerCount = 0x0000;
		stDNSHeader.usAuthorityCount = 0x0000;
		stDNSHeader.usAdditionalCount = 0x0000;

		uQType = htons(0x0001);
		uQClass = htons(0x0001);

		//域名编码
		pszDNSEncodeName = new char[uDNSEncodeNameLen];
		if (pszDNSEncodeName == NULL)
		{
			bRet = false;
			break;
		}
		memset(pszDNSEncodeName, 0x0, uDNSEncodeNameLen);

		if (!DNSEncodeString(pszDomainName, pszDNSEncodeName, &uDNSEncodeNameLen))
		{
			bRet = false;
			break;
		}

		uDNSPacketSize = 0;
		memcpy(pszDNSPacket+uDNSPacketSize, &stDNSHeader, sizeof(HT_DNS_HEADER));
		uDNSPacketSize += sizeof(HT_DNS_HEADER);
		memcpy(pszDNSPacket+uDNSPacketSize, pszDNSEncodeName, uDNSEncodeNameLen+1);
		uDNSPacketSize += uDNSEncodeNameLen+1;
		memcpy(pszDNSPacket+uDNSPacketSize, (char*)(&uQType),  nDNS_TYPE_LEN);
		uDNSPacketSize += nDNS_TYPE_LEN;
		memcpy(pszDNSPacket+uDNSPacketSize, (char*)(&uQClass), nDNS_CLASS_LEN);
		uDNSPacketSize += nDNS_CLASS_LEN;

		if (sendto(hSocket, pszDNSPacket, uDNSPacketSize, 0, (sockaddr*)&sockAddrDNSServer, sizeof(sockAddrDNSServer)) == SOCKET_ERROR)
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	if (pszDNSEncodeName != NULL)
	{
		delete[] pszDNSEncodeName;
		pszDNSEncodeName = NULL;
	}

	if (pszDNSPacket != NULL)
	{
		delete[] pszDNSPacket;
		pszDNSPacket = NULL;
	}

	return bRet;
}

bool CGlobalInfo::RecvDNSResponse(SOCKET hSocket, HANDLE hEvent, const char *pszDNSServerIp, std::vector<ULONG> *pvecIPList)
{
	bool bRet = false;

	unsigned int uTimeOut = 50;
	unsigned int uDomainNameLen = 0;
	unsigned int uDNSEncodeNameLen = 0;

	const short nDNS_TTL_LEN  = 4;
	const short nDNS_TYPE_LEN = 2;
	const short nDNS_DATA_LEN = 2;
	const short nDNS_CLASS_LEN = 2;
	
	const short nDNS_DOMAIN_LEN = MAX_PATH;
	const short nDNS_BUFFER_LEN = 1024;

	const short nDNS_TYPE_A = 0x0001;		//1 a host address
	const short nDNS_TYPE_NAME = 0x0005;	//5 the canonical name for an alias

	char recvbuf[nDNS_BUFFER_LEN] = {0};
	char szDomainName[nDNS_DOMAIN_LEN] = {0};

	if (hSocket == INVALID_SOCKET)
	{
		return false;
	}

	if (hEvent == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	if (pszDNSServerIp == NULL || *pszDNSServerIp == '\0')
	{
		return false;
	}

	if (pvecIPList == NULL)
	{
		return false;
	}

	sockaddr_in sockAddrDNSServer;
	sockAddrDNSServer.sin_family = AF_INET; 
	sockAddrDNSServer.sin_port = htons( 53 );
	sockAddrDNSServer.sin_addr.s_addr = inet_addr(pszDNSServerIp);

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 500;

	ULONG usAnswerTTL = 0;
	USHORT usAnswerType = 0;
	USHORT usAnswerCount = 0;
	USHORT usAnswerClass = 0;
	USHORT usQuestionCount = 0;
	USHORT usAnswerDataLen = 0;
	USHORT nsEncodedNameLen = 0;

	fd_set fdRead;
	USHORT usTransId = GetCurrentProcessId();

	while (WaitForSingleObject(hEvent, uTimeOut) != WAIT_OBJECT_0)
	{
		FD_ZERO(&fdRead);
		FD_SET(hSocket, &fdRead);

		if (select(hSocket+1, &fdRead, NULL, NULL, &tv) <= 0)
		{
			uTimeOut = 500;
			continue;
		}

		if (!FD_ISSET(hSocket, &fdRead))
		{
			uTimeOut = 500;
			continue;
		}

		int nSockaddrDestSize = sizeof(sockAddrDNSServer);
		memset(recvbuf, 0x0, nDNS_BUFFER_LEN);

		if (recvfrom(hSocket, recvbuf, nDNS_BUFFER_LEN, 0, (struct sockaddr*)&sockAddrDNSServer, &nSockaddrDestSize) == SOCKET_ERROR)
		{
			uTimeOut = 500;
			continue;
		}

		HT_DNS_HEADER *pDNSHeader = (HT_DNS_HEADER*)recvbuf;
		if (pDNSHeader == NULL)
		{
			uTimeOut = 500;
			continue;
		}

		if ( pDNSHeader->usTransId != usTransId 
			|| (ntohs(pDNSHeader->usFlag) & 0xfb7f) != 0x8100
			|| (usQuestionCount = ntohs(pDNSHeader->usQuestionCount)) < 0
			|| (usAnswerCount = ntohs(pDNSHeader->usAnswerCount)) <= 0)
		{
			uTimeOut = 500;
			continue;
		}

		char *pDNSData = recvbuf + sizeof(HT_DNS_HEADER);
		if (pDNSData == NULL)
		{
			uTimeOut = 500;
			continue;
		}

		//解析Question字段
		for (int i = 0; i != usQuestionCount; ++i)
		{
			uDNSEncodeNameLen = 0;

			uDomainNameLen = nDNS_DOMAIN_LEN;
			memset(szDomainName, 0x0, nDNS_DOMAIN_LEN);

			if (!DNSDecodeString(pDNSData, szDomainName, &uDomainNameLen, &uDNSEncodeNameLen))
			{
				return false;
			}

			pDNSData += (uDNSEncodeNameLen + nDNS_TYPE_LEN + nDNS_CLASS_LEN);
		}

		//解析Answer字段
		for (int i = 0; i != usAnswerCount; ++i)
		{
			uDNSEncodeNameLen = 0;

			uDomainNameLen = nDNS_DOMAIN_LEN;
			memset(szDomainName, 0x0, nDNS_DOMAIN_LEN);

			if (!DNSDecodeString(pDNSData, szDomainName, &uDomainNameLen, &uDNSEncodeNameLen, recvbuf))
			{
				return false;
			}

			pDNSData += uDNSEncodeNameLen;
			usAnswerType = ntohs(*(USHORT*)(pDNSData));
			usAnswerClass = ntohs(*(USHORT*)(pDNSData + nDNS_TYPE_LEN));
			usAnswerTTL = ntohl(*(ULONG*)(pDNSData + nDNS_TYPE_LEN + nDNS_CLASS_LEN));
			usAnswerDataLen = ntohs(*(USHORT*)(pDNSData + nDNS_TYPE_LEN + nDNS_CLASS_LEN + nDNS_TTL_LEN));

			pDNSData += (nDNS_TYPE_LEN + nDNS_CLASS_LEN + nDNS_TTL_LEN + nDNS_DATA_LEN);
			if (usAnswerType == nDNS_TYPE_A)
			{
				 ULONG ulIP = *(ULONG*)(pDNSData);
				pvecIPList->push_back(ulIP);
			}

/*
			if (usAnswerType == nDNS_TYPE_NAME)
			{
				uDNSEncodeNameLen = 0;

				uDomainNameLen = nDNS_DOMAIN_LEN;
				memset(szDomainName, 0x0, nDNS_DOMAIN_LEN);

				if (!DNSDecodeString(pDNSData, szDomainName, &uDomainNameLen, &uDNSEncodeNameLen, recvbuf))
				{
					return false;
				}

				pvecCNameList->push_back(szDomainName);
			}
*/

			pDNSData += (usAnswerDataLen);
		}

		bRet = true;
		break;
	}

	return bRet;
}

bool CGlobalInfo::DNSEncodeString(const char* pszDomainName, char* pszDNSEncodeName, unsigned int *puDNSEncodeNameLen)
{
	unsigned int uBufferLen = 0;
	unsigned int uDNSLabelLen = 0;
	unsigned int uDNSEncodeLen = 0;
	unsigned int uDomainNameLen = 0;

	const short nMAX_STRING_LEN = 64;
	const short nMAX_LABEL_LEN  = MAX_PATH;
	const short nMAX_ENCODE_LEN = MAX_PATH;

	char szBuffer[nMAX_STRING_LEN]  = {0};
	char szDNSLabel[nMAX_LABEL_LEN] = {0};
	char szDNSEncode[nMAX_ENCODE_LEN] = {0};
	
	char szSeps[] = _T(".");

	LPTSTR pszDNSToken = NULL;
	LPTSTR pszDNSNextToken = NULL;

	if (pszDomainName == NULL || *pszDomainName == '\0')
	{
		return false;
	}

	if (pszDNSEncodeName == NULL || *puDNSEncodeNameLen == NULL)
	{
		return false;
	}

	uDomainNameLen = strlen(pszDomainName);
	StringCchCopy(szDNSLabel, nMAX_LABEL_LEN-1, pszDomainName);

	pszDNSToken = _tcstok_s(szDNSLabel, szSeps, &pszDNSNextToken);
	if (*pszDNSNextToken == 32)
	{
		pszDNSNextToken++;
	}

	while (pszDNSToken != NULL)
	{	
		if (*pszDNSToken != '\0')
		{
			memset(szBuffer, 0x0, nMAX_STRING_LEN);

			uDNSLabelLen = strlen(pszDNSToken);
			sprintf(szBuffer, _T("%c%s"), uDNSLabelLen, pszDNSToken);
			
			uBufferLen = strlen(szBuffer);
			memcpy(szDNSEncode+uDNSEncodeLen, szBuffer, uBufferLen);
			uDNSEncodeLen += uBufferLen;
		}

		if (*pszDNSNextToken == 32)
		{
			pszDNSNextToken++;
		}

		pszDNSToken  = _tcstok_s(NULL, szSeps, &pszDNSNextToken);
	}

 	szDNSEncode[uDNSEncodeLen] = '\0';
	if (*puDNSEncodeNameLen < uDNSEncodeLen)
	{
		return false;
	}

	*puDNSEncodeNameLen = uDNSEncodeLen;
	memcpy(pszDNSEncodeName, szDNSEncode, uDNSEncodeLen);
	return true;
}

bool CGlobalInfo::DNSDecodeString(const char *pszDNSEncodeName, char *pszDomainName, unsigned int *puDNSDomainNameLen, unsigned int *puDNSEncodeNameLen, char *pszPacketPos)
{
	unsigned int uDNSTokenPos = 0;
	unsigned int uDNSTokenLen = 0;
	unsigned int uDNSLabelLen = 0;
	unsigned int uDNSEncodeLen = 0;
	unsigned int uDomainNameLen = 0;

	const short nMAX_STRING_LEN = 64;
	const short nMAX_LABEL_LEN  = MAX_PATH;

	char szBuffer[nMAX_STRING_LEN]  = {0};
	char szDNSLabel[nMAX_LABEL_LEN];// = {0};

	char* pszDecodeString = NULL;

	if (pszDNSEncodeName == NULL || *pszDNSEncodeName == '\0')
	{
		return false;
	}

	if (pszDomainName == NULL || puDNSDomainNameLen == NULL)
	{
		return false;
	}

	if (puDNSEncodeNameLen == NULL)
	{
		return false;
	}

	pszDecodeString = (char*)pszDNSEncodeName;
	while(*pszDecodeString != 0x0)
	{
		if ((*pszDecodeString & 0xc0) == 0)	//普通格式:
		{
			uDNSTokenPos = 0;
			uDNSTokenLen = *pszDecodeString;

			memcpy(szDNSLabel+uDNSLabelLen, pszDecodeString+1, uDNSTokenLen);
			uDNSLabelLen += uDNSTokenLen;
			uDNSTokenPos += uDNSTokenLen;
			uDNSEncodeLen += uDNSTokenLen;

			memcpy(szDNSLabel+uDNSLabelLen, _T("."), 1);
			uDNSLabelLen += 1;
			uDNSTokenPos += 1;
			uDNSEncodeLen += 1;

			pszDecodeString += uDNSTokenPos;
		}
		else
		{
			if (pszPacketPos == NULL || pszPacketPos == '\0')
			{
				return false;
			}
	
			uDomainNameLen = MAX_PATH;
			USHORT usJumpPos = ntohs(*(USHORT*)(pszDecodeString)) & 0x3fff;
			if (!DNSDecodeString(pszPacketPos+usJumpPos, pszDomainName+uDNSLabelLen, &uDomainNameLen, &uDNSEncodeLen, pszPacketPos))
			{
				return false;
			}
			else
			{
				*puDNSEncodeNameLen += 2;
				return true;
			}
		}
	}

	szDNSLabel[uDNSLabelLen-1] = '\0';
	uDNSLabelLen -= 1;

	if (*puDNSDomainNameLen < uDNSLabelLen)
	{
		return false;
	}
	*puDNSDomainNameLen = uDNSLabelLen;
	strcpy(pszDomainName, szDNSLabel);

	*puDNSEncodeNameLen = uDNSEncodeLen +1;
	return true;
}