#ifndef __GLOBARINFO_H__
#define __GLOBARINFO_H__

#include "winioctl.h"

using namespace std;
class CGlobalInfo
{
public:
	CGlobalInfo(void);
	~CGlobalInfo(void);
	
public:
	static	CGlobalInfo*			CreateInstance(void);
	static  void					DestroyInstance(void);	

public:
	CString							GetAppPath();
	bool							IsFileExists(LPCTSTR lpszFilePath);

	int								StringToHexString(char* szDesc, const char* szSrc, int nLen, char chTag=0);
	int 							HexStringToBytes(unsigned char* szDesc, const char* szSrc,int nLen);
	int								BytesHexToString(char*  szDesc, const unsigned char* szSrc,int nLen);

	SYSTEMTIME						Int64ToSystemTime(const __int64& itime);
	__int64							SystemTimeToInt64(const SYSTEMTIME& itime);

	std::vector<char*>				SplitString1(const char pszSource[], const char* pszSeparator);
	std::vector<std::string>		SplitString2(const char* pszSource, const char* pszSeparator);	
	std::vector<CString>			SplitString3(const char* pszSource, const char* pszSeparator);

	bool							ConvertToInt(const double &val,int& i);
	bool							GetDiskInfo(unsigned int nDrvIndex, char szArrayModelNo[MAX_PATH], char szArraySerialNo[MAX_PATH]);

	bool							DNSLookupInfo(const char *pszDNSServerIp, const char *pszDomainName, char *pszDomainIp, unsigned int *puDomainIpLen);

protected:
	bool							DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP, BYTE btIDCmd, BYTE btDriveNum, PDWORD pdwBytesReturned);
	bool							ToLittleEndian(PDWORD pDiskData, int nFirstIndex, int nLastIndex, char* pResBuf, int &nResBufLen);

	bool							SendDNSRequest(SOCKET hSocket, const char *pszDNSServerAddr, const char *pszDomainName);
	bool							RecvDNSResponse(SOCKET hSocket, HANDLE hEvent, const char *pszDNSServerIp, std::vector<ULONG> *pvecIPList);

	bool							DNSEncodeString(const char *pszDomainName, char *pszDNSEncodeName, unsigned int *puDNSEncodeNameLen);
	bool							DNSDecodeString(const char *pszDNSEncodeName, char *pszDomainName, unsigned int *puDNSDomainNameLen, unsigned int *puDNSEncodeNameLen, char *pszPacketPos = NULL);

private:
	static  CGlobalInfo*			m_pGlobal;
};

#endif