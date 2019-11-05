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
	static	CGlobalInfo* CreateInstance(void);
	static  void		DestroyInstance(void);	

public:
	time_t				SystemTimeToTime(SYSTEMTIME st);
	time_t				SystemTimeToTimeEx(const SYSTEMTIME& st);

	time_t				FileTimeToTime(FILETIME fileTime);
	SYSTEMTIME			FileTimeToSystemTime(FILETIME fileTime);

	SYSTEMTIME			TimeToSystemTime(time_t t);
	SYSTEMTIME			TimeToSystemTimeEx(time_t t);
	
	bool				GetDiskInfo(unsigned int nDrvIndex, char szArrayModelNo[MAX_PATH], char szArraySerialNo[MAX_PATH]);

	bool				DNSLookupInfo(const char *pszDNSServerIp, const char *pszDomainName, char *pszDomainIp, unsigned int *puDomainIpLen);
	bool				OpenSysServer(const char *pszSvrName);

protected:
	bool				DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP, BYTE btIDCmd, BYTE btDriveNum, PDWORD pdwBytesReturned);
	bool				ToLittleEndian(PDWORD pDiskData, int nFirstIndex, int nLastIndex, char* pResBuf, int &nResBufLen);

	bool				SendDNSRequest(SOCKET hSocket, const char *pszDNSServerAddr, const char *pszDomainName);
	bool				RecvDNSResponse(SOCKET hSocket, HANDLE hEvent, const char *pszDNSServerIp, std::vector<ULONG> *pvecIPList);

	bool				DNSEncodeString(const char *pszDomainName, char *pszDNSEncodeName, unsigned int *puDNSEncodeNameLen);
	bool				DNSDecodeString(const char *pszDNSEncodeName, char *pszDomainName, unsigned int *puDNSDomainNameLen, unsigned int *puDNSEncodeNameLen, char *pszPacketPos = NULL);

	int					FindSubNum(char* pStr, char* pSubstr);

private:
	static  CGlobalInfo*	m_pGlobal;
};

#endif