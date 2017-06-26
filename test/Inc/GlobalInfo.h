#ifndef __GLOBARINFO_H__
#define __GLOBARINFO_H__

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
	bool							ConvertToInt(const double &val,int& i);
	bool							GetFileTitle(const char *pszFileName, char *pszTitle, char *pszExt);

	int								StringToHexString(char* szDesc, const char* szSrc, int nLen, char chTag=0);
	int 							HexStringToBytes(unsigned char* szDesc, const char* szSrc,int nLen);
	int								BytesHexToString(char*  szDesc, const unsigned char* szSrc,int nLen);

	SYSTEMTIME						Int64ToSystemTime(const __int64& itime);
	__int64							SystemTimeToInt64(const SYSTEMTIME& itime);

	int								EnumModifyName(const char *pszFilePath, const char *pszNewFileName, const char *pszSpanName);
	bool							TFun1(const char *pszSrcFileName, const char *pszReqName, const char *pszSpanName, char *pszDstFileName, const int nBit);
	
	std::vector<CString>			SplitString1(LPCTSTR lpszSource, LPCTSTR lpszSeparator);
	std::vector<CString>			SplitString2(LPCTSTR lpszSource, LPCTSTR lpszSeparator);	
	std::vector<CString>			SplitString3(LPCTSTR lpszSource, LPCTSTR lpszSeparator);
	std::vector<CString>			SplitString4(LPCTSTR lpszSource, LPCTSTR lpszSeparator);
	std::vector<CString>			SplitString5(LPCTSTR lpszSource, LPCTSTR lpszSeparator);
	std::vector<std::string>		SplitString6(LPCTSTR lpszSource, LPCTSTR lpszSeparator);
	
private:
	static  CGlobalInfo*			m_pGlobal;
};

#endif