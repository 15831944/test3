#include "stdafx.h"
#include "GlobalInfo.h"



CGlobalInfo* CGlobalInfo::m_pGlobal = NULL;
CGlobalInfo::CGlobalInfo(void)
{
}

CGlobalInfo::~CGlobalInfo(void)
{
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

CString CGlobalInfo::GetAppPath()
{
	CString  strAppPath;
	TCHAR    szModuleFileName[MAX_PATH +1];
	ZeroMemory(szModuleFileName, MAX_PATH +1);

	::GetModuleFileName(NULL, szModuleFileName, sizeof(szModuleFileName)/sizeof(TCHAR));
	strAppPath = CString(szModuleFileName);

	int nPos   = strAppPath.ReverseFind('\\');
	strAppPath = strAppPath.Left(nPos);

	return strAppPath;
}

void CGlobalInfo::test1()
{
	unsigned int uDNSEncode = 0;
	char* pszDomain = _T("www.baidu.com");

	uDNSEncode = strlen(pszDomain) + 2;

// 	char* pszDNSEncode = new char[uDNSEncode];
// 	if (pszDNSEncode == NULL)
// 	{
// 		return;
// 	}
// 	memset(pszDNSEncode, 0x0, uDNSEncode);

	SendDNSRequest(NULL, "www.baidu.com");
}

int CGlobalInfo::StringToHexString(char* szDesc, const char* szSrc, int nLen, char chTag)
{
	unsigned char* pSrc = (unsigned char*)szSrc;
	assert(szDesc != NULL && pSrc != NULL);

	int nRet = 0, i = 0;
	char szHex[2]={0};
	unsigned char c1, c2;

	if (szDesc != NULL && pSrc != NULL)
	{
		for (i=0; i < nLen; ++i)
		{
			memset(szHex, 0, 2);
			c1 = *pSrc >>4;		////将取得的字符分为高低四位，再次判断高位
			if (c1 >= 0x0 && c1 <= 0x9)
			{
				szHex[0] = c1 - 0 + '0';
			}
			else
			{
				szHex[0] = c1 - 10 + 'A';
			}
			nRet++;

			c2 = *pSrc++ & 0x0F;
			if (c2 >= 0x0 && c2 <= 0x9)
			{
				szHex[1] = c2 - 0 + '0';
			}
			else
			{
				szHex[1] = c2 - 10 + 'A';
			}
			nRet++;

			*szDesc++ = szHex[0];
			*szDesc++ = szHex[1];

			if (0 != chTag)
			{
				*szDesc++ = chTag;
				nRet++;
			}
		}
	}

	return nRet;
}

int CGlobalInfo::HexStringToBytes(unsigned char* szDesc, const char* szSrc,int nLen)
{
	unsigned char* pSrc = (unsigned char*)szSrc;
	assert(pSrc != NULL && szDesc != NULL);

	int nRet = 0, i = 0;
	for (i=0; i<nLen; i+=2)
	{
		// 输出高4位
		if(*pSrc>='0' && *pSrc<='9')
		{
			*szDesc = ((*pSrc - '0') << 4);
		}
		else if (*pSrc>='A' && *pSrc<='F')
		{
			*szDesc = ((*pSrc - 'A' + 10) << 4);
		}
		else if (*pSrc>='a' && *pSrc<='f')
		{
			*szDesc = ((*pSrc - 'a' + 10) << 4);
		}
		else
		{
			nRet = -1;
			break;
		}
		pSrc++;

		// 输出低4位
		if(*pSrc>='0' && *pSrc<='9')
		{
			*szDesc |= (*pSrc - '0');
		}
		else if (*pSrc>='A' && *pSrc<='F')
		{
			*szDesc |= (*pSrc - 'A' + 10);
		}
		else if (*pSrc>='a' && *pSrc<='f')
		{
			*szDesc |= (*pSrc - 'a' + 10);
		}
		else
		{
			nRet = -1;
			break;
		}

		pSrc++;
		szDesc++;
		nRet++;
	}

	return nRet;
}

int CGlobalInfo::BytesHexToString(char*  szDesc, const unsigned char* szSrc,int nLen)
{
	unsigned char* pSrc = (unsigned char*)szSrc;
	assert(pSrc != NULL && szDesc != NULL);

	int i = 0;
	const char tab[]="0123456789ABCDEF";

	for(i=0; i<nLen; i++)
	{
		// 输出低4位
		*szDesc++ = tab[*pSrc >> 4];

		// 输出高4位
		*szDesc++ = tab[*pSrc & 0x0f];
		pSrc++;
	}
	*szDesc = '\0';
	return nLen*2;
}

SYSTEMTIME CGlobalInfo::Int64ToSystemTime(const __int64& itime)
{
	FILETIME ft;
	SYSTEMTIME st;
	ULARGE_INTEGER ularge;
	__int64 tmptimeA, tmptimeB;
	tmptimeA = itime;
	tmptimeB = itime;
	ularge.HighPart = (DWORD)(tmptimeA>>32);
	ularge.LowPart = (DWORD)((tmptimeB<<32)>>32);
	ft.dwLowDateTime = ularge.LowPart;
	ft.dwHighDateTime = ularge.HighPart;
	FileTimeToSystemTime(&ft, &st);

	return st;
}

__int64 CGlobalInfo::SystemTimeToInt64(const SYSTEMTIME& itime)
{
	FILETIME ft;
	SystemTimeToFileTime(&itime, &ft);
	ULARGE_INTEGER ularge;
	ularge.LowPart = ft.dwLowDateTime;
	ularge.HighPart = ft.dwHighDateTime;
	__int64 int64 = ularge.QuadPart;
	return int64;
}

/*
 * C语言函数字符串分割
 * char转换CString, strtok函数分割字符串;
*/
std::vector<char*> CGlobalInfo::SplitString1(const char pszSource[], const char* pszSeparator)
{
	BOOL bRet = FALSE;
	int npos = 0;
	unsigned long ulStrLen = 0;

	char* pString = NULL;
	char* pSrcString = NULL;
	char* pSubString = NULL;

	std::vector<char*> vecString;

	if (pszSource == NULL || pszSeparator == NULL)
	{
		bRet = FALSE;
		goto part1;
	}

	pString = (char*)pszSource;

#define MACRO_STRTOK_TAG 1
#define MACRO_STRSTR_TAG 0

#if (MACRO_STRTOK_TAG == 1)	
	pSubString = strtok(pString, pszSeparator);
	if (pSubString == NULL)
	{
		bRet = FALSE;
		goto part1;
	}
	else
	{
		vecString.push_back(pSubString);
	}

	while(1)
	{
		pSubString = strtok(NULL, pszSeparator);
		if (pSubString == NULL)
		{
			break;
		}
		else
		{
			vecString.push_back(pSubString);
		}
	}
#endif

#if (MACRO_STRSTR_TAG == 1)
	while(1)
	{
		pSubString = strstr(pString, pszSeparator);
		if (pSubString != NULL)
		{
			memset(pSubString, '\0', strlen(pszSeparator));
			vecString.push_back(pString);
			
			pSubString = pSubString + strlen(pszSeparator);
			pString = pSubString;
		}
		else
		{
			vecString.push_back(pString);
			break;
		}
	}
#endif

part1:
	return vecString;
}

/*
 * C++函数字符串分割
 * char转换string, substr函数分割字符串;
*/
std::vector<std::string> CGlobalInfo::SplitString2(const char* pszSource, const char* pszSeparator)
{
	std::string::size_type start = 0;
	std::string::size_type index = 0;

	std::string strString(pszSource);
	std::string strSepString(pszSeparator);

	std::vector<std::string> vecString;

#define MACRO_STRING1_TAG 0
#define MACRO_STRING2_TAG 1

#if (MACRO_STRING1_TAG == 1)
	std::string strSubString;
	do 
	{
		index = strString.find_first_of(pszSeparator, start);
		if (index != std::string::npos)
		{
			strSubString = strString.substr(start, index-start);
			if (strSubString != _T(""))
			{
				vecString.push_back(strSubString);
			}

			start = strString.find_first_not_of(pszSeparator, index);

			if (start == std::string::npos)
			{
				return vecString;
			}
		}
	} while (index != std::string::npos);

	strSubString = strString.substr(start);
	vecString.push_back(strSubString);
#endif
	
#if (MACRO_STRING2_TAG == 1)
	string::const_iterator substart = strString.begin();
	
	string::const_iterator delimstart = strSepString.begin();
	string::const_iterator delimend   = strSepString.end();
	
	while(1)
	{
		string::const_iterator subend = strString.end();

		subend = search(substart, subend, delimstart, delimend);
		if (subend == strString.end())
		{
			std::string strSubString(substart, subend);
			if (!strSubString.empty())
			{
				vecString.push_back(strSubString);
			}
			break;
		}
		else
		{
			std::string strSubString(substart, subend);
			if (!strSubString.empty())
			{
				vecString.push_back(strSubString);
			}

			substart = subend + strSepString.size();
		}
	}
#endif

	return vecString;
}

/*
 * MFC函数字符串分割
 * char转换CString, MFC函数分割字符串;
*/
std::vector<CString> CGlobalInfo::SplitString3(const char* pszSource, const char* pszSeparator)
{
	int nPos    = 0;
	int nPrePos = 0;

	CString strSubString;
	CString strString = pszSource;

	std::vector<CString> vecString;

#define MACRO_CSTRING1_TAG 1
#define MACRO_CSTRING2_TAG 0
#define MACRO_CSTRING3_TAG 0
#define MACRO_CSTRING4_TAG 0

#if (MACRO_CSTRING1_TAG == 1)
	while(nPos != -1)
	{
		nPrePos = nPos;

		nPos = strString.Find(pszSeparator, (nPos+1));
		vecString.push_back(strString.Mid(nPrePos, (nPos-nPrePos)));
	}
#endif

#if (MACRO_CSTRING2_TAG == 1)
	nPos = strString.Find(pszSeparator);

	while(nPos != -1)
	{
		strSubString = strString.Left(nPos);
		if (!strSubString.IsEmpty())
		{
			vecString.push_back(strSubString);
		}

		strString = strString.Right(strString.GetLength()-nPos-1);
		nPos = strString.Find(pszSeparator);
	}

	if (!strString.IsEmpty())
	{
		vecString.push_back(strString);
	}
#endif

#if (MACRO_CSTRING3_TAG == 1)
	strSubString = strString.Tokenize(pszSeparator, nPos);
	while(strSubString.Trim() != _T(""))
	{
		vecString.push_back(strSubString);
		strSubString = strString.Tokenize(pszSeparator, nPos);
	}
#endif

#if (MACRO_CSTRING4_TAG == 1)
	while(AfxExtractSubString(strSubString, pszSource, nPos, *pszSeparator))
	{
		nPos++;
		vecString.push_back(strSubString);
	}
#endif

	return vecString;
}

//////////////////////////////////////////////////////////////////////////
//
bool CGlobalInfo::ConvertToInt(const double &val,int& i)
{
	int num[2] ={0};  
	memcpy(num,&val,8);  
	int high =num[1];  
	int nExp =((high>>20)&0x7ff) - 1023;  
	if(nExp<= 20)  
	{  
		i = ( high&0xfffff |0x100000)>>(20 - nExp);  
	}  
	else if(nExp > 20 && nExp <= 30)  
	{  
		int low= num[0];  
		i = ( ( high&0xfffff |0x100000)<<(nExp - 20) )+ ( low >>(52 - nExp));  
	}  
	else  
		return false;  

	if(high&0x80000000)  
		i = ~i + 1;  
	return true; 
}

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

bool CGlobalInfo::DNSLookupInfo()
{
	return true;
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

bool CGlobalInfo::SendDNSRequest(const char* pszDNSServerAddr, const char* pszDomainName)
{
	bool bRet = false;

	unsigned int uQType = 0;
	unsigned int uQClass = 0;
	
	unsigned int uCurProcId = 0;
	unsigned int uDNSPacketSize = 0;
	unsigned int uDomainNameLen = 0;
	unsigned int uDNSEncodeNameLen = 0;

	const short nDNS_TYPE_LEN = 2;
	const short nDNS_CLASS_LEN = 2;
	const short nMAX_DOMAINNAME_LEN = MAX_PATH;
	const short nDNS_PACKET_LEN = (sizeof(HT_DNS_HEADER) + nMAX_DOMAINNAME_LEN + nDNS_TYPE_LEN + nDNS_CLASS_LEN);

	char *pszDNSPacket = NULL;
	char *pszDNSEncodeName = NULL;
	HT_DNS_HEADER *pDNSHeader = NULL;

	if (pszDNSServerAddr == NULL || *pszDNSServerAddr == '\0')
	{
		return false;
	}

	if (pszDomainName == NULL || *pszDomainName == '\0')
	{
		return false;
	}

	do 
	{
		//
		uDomainNameLen = strlen(pszDomainName);
		uDNSEncodeNameLen = uDomainNameLen + 2;

		//DNS组包
		pszDNSPacket = new(std::nothrow)char(nDNS_PACKET_LEN);
		if (pszDNSPacket == NULL)
		{
			bRet = false;
			break;
		}
		memset(pszDNSPacket, 0x0, nDNS_PACKET_LEN);

		pDNSHeader = (HT_DNS_HEADER*)pszDNSPacket;
		uCurProcId = (unsigned int)GetCurrentProcessId();
		pDNSHeader->usTransId = uCurProcId;
		pDNSHeader->usFlag = htons(0x0100);
		pDNSHeader->usQuestionCount = htons(0x0001);
		pDNSHeader->usAnswerCount = 0x0000;
		pDNSHeader->usAuthorityCount = 0x0000;
		pDNSHeader->usAdditionalCount = 0x0000;

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

		memcpy(pszDNSPacket += sizeof(HT_DNS_HEADER), pszDNSEncodeName, uDNSEncodeNameLen);
		memcpy(pszDNSPacket += uDNSEncodeNameLen, (char*)(&uQType), nDNS_TYPE_LEN);
		memcpy(pszDNSPacket += nDNS_TYPE_LEN, (char*)(&uQClass), nDNS_CLASS_LEN);

		uDNSPacketSize = sizeof(HT_DNS_HEADER) + uDNSEncodeNameLen + nDNS_TYPE_LEN + nDNS_CLASS_LEN;

	} while (false);

	if (pszDNSEncodeName != NULL)
	{
		delete[] pszDNSEncodeName;
		pszDNSEncodeName = NULL;
	}

	if (pszDNSPacket != NULL)
	{
		delete pszDNSPacket;
		pszDNSPacket = NULL;
	}

	return bRet;
}

bool CGlobalInfo::RecvDNSResponse()
{
	return true;
}

bool CGlobalInfo::DNSEncodeString(const char* pszDomainName, char* pszDNSEncode, unsigned int *puDNSEncodeLen)
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

	if (pszDNSEncode == NULL || *puDNSEncodeLen == NULL)
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

	szDNSEncode[uDNSEncodeLen+1] = '\0';
	uDNSEncodeLen += 1;

	if (*puDNSEncodeLen < uDNSEncodeLen)
	{
		return false;
	}

	*puDNSEncodeLen = uDNSEncodeLen;
	memcpy(pszDNSEncode, szDNSEncode, uDNSEncodeLen);
	return true;
}