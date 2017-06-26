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

bool CGlobalInfo::GetFileTitle(const char *pszFileName, char *pszTitle, char *pszExt)
{
	char *ptr = NULL;
	int  nPos = 0;
	int  nLen = 0;

	if (pszFileName == NULL || pszTitle == NULL || pszExt == NULL)
	{
		return false;
	}

	nLen = strlen(pszFileName);

	ptr = strrchr((char*)pszFileName, '.');
	if (ptr == NULL)
	{
		memcpy(pszTitle, pszFileName, strlen(pszFileName));
		return true;
	}

	nPos = ptr - pszFileName;
	if (nPos == -1)
	{
		return false;
	}

	memcpy(pszTitle, pszFileName, nPos);
	memcpy(pszExt, pszFileName+nPos, nLen-nPos);

	return true;
}

int CGlobalInfo::EnumModifyName(const char *pszFilePath, const char *pszNewFileName, const char *pszSpanName)
{
	int  result;
	long file;

	char szFindPath[MAX_PATH+1]  = {0};
	char szFileName[MAX_PATH+1]  = {0};
	char szFileExt[MAX_PATH+1]   = {0};

	char szSrcFile[MAX_PATH+1] = {0};
	char szDstFile[MAX_PATH+1] = {0};

	char szSrcFileTitle[MAX_PATH+1] = {0};
	char szDstFileTitle[MAX_PATH+1] = {0};
	char szSpan[] = _T("\\");

	struct _stat	s_file;
	_finddata_t		c_file;

	if (pszFilePath == NULL)
	{
		return -1;
	}

	if (_stat(pszFilePath, &s_file) != 0)
	{
		return -1;
	}

	if ((s_file.st_mode&_S_IFDIR) != _S_IFDIR)
	{
		return -1;
	}

	_tcscpy(szFindPath, pszFilePath);
	_tcscat(szFindPath, _T("\\*.*"));

	file = _findfirst(szFindPath, &c_file);
	if (file == -1)
	{
		return 0;
	}

	do 
	{
		if ((_tcslen(c_file.name)==1 && c_file.name[0]==_T('.')) ||
			(_tcslen(c_file.name)==2 && c_file.name[0]==_T('.') && c_file.name[1]==_T('.')))
		{
			continue;
		}

		if ((c_file.attrib&_A_SUBDIR) == _A_SUBDIR)
		{
			continue;
		}
		else
		{
			memset(szFileName, 0x0, MAX_PATH+1);
			
			memset(szSrcFile, 0x0, MAX_PATH+1);
			memset(szDstFile, 0x0, MAX_PATH+1);

			if (strlen(c_file.name) == 0)
			{
				continue;
			}

			sprintf_s(szSrcFile, "%s\\%s", pszFilePath, c_file.name);

			if (!GetFileTitle(c_file.name, szSrcFileTitle, szFileExt))
			{
				continue;
			}

			if (!TFun1(szSrcFileTitle, pszNewFileName, pszSpanName,szDstFileTitle, 2))
			{
				continue;
			}

			sprintf_s(szFileName, "%s%s", szDstFileTitle, szFileExt);
			sprintf_s(szDstFile, "%s\\%s", pszFilePath, szFileName);	

			rename(szSrcFile, szDstFile);

			result = 1;
			continue;
		}
	} while (_tfindnext(file, &c_file) == 0);

	if (file)
	{
		_findclose(file);
	}

	return result;
}

bool CGlobalInfo::TFun1(const char *pszSrcFileName, const char *pszReqName, const char *pszSpanName, char *pszDstFileName, const int nBit)
{
	bool bRet  = false;
	bool bFind = false;

	char *ptr  = NULL;
	char *pVal = NULL;

	static int nIndex = 0;
	char szIndexVal[32];
	char szFileName[32];

	int  nPos1 = 0;
	int  nPos2 = 0;
	int  nPos3 = 0;

	int  nLen      = 0;
	int  nSrcLen   = 0;
	int  nReqLen   = 0;	
	int  nSpanLen  = 0;
	
	int  nLeftLen  = 0;
	int  nRightLen = 0;	

	if (pszSrcFileName == NULL || pszReqName == NULL || pszDstFileName == NULL)
	{
		return false;
	}

	nSrcLen  = strlen(pszSrcFileName);
	nReqLen  = strlen(pszReqName);

	memset(szIndexVal, 0x0, 32);
	memset(szFileName, 0x0, 32);

	if (pszSpanName != "")
	{
		nSpanLen = strlen(pszSpanName);

		ptr = strstr((char*)pszSrcFileName, pszSpanName);
		if (ptr == NULL)
		{
			return false;
		}

		nPos1 = ptr - pszSrcFileName;
		if (nPos1 == -1)
		{
			return false;
		}
		
		if (nPos1 != 0)
		{//查找span分割的左右部分的字符串
			for (int i=0; i<3; i++)
			{
				if (bFind)
				{
					memcpy(szIndexVal, pVal, nLen);
					break;
				}

				if (i == 0)
				{//
					nLeftLen = nPos1+1; 

					pVal = new char[nLeftLen];
					memset(pVal, 0x0, nLeftLen);

					ptr = (char*)pszSrcFileName;

					while(nPos2 < nPos1)
					{
						if (*ptr >= '0' && *ptr <= '9')
						{
							memcpy(pVal+nPos3, &(*ptr), 1);
							nPos3 += 1;

							nLen++;

							ptr++;
							nPos2++;							
							continue;
						}
						
						if (nLen >= nBit)
						{
							bFind = true;
							break;
						}
						else
						{
							nLen  = 0;
							nPos3 = 0;

							memset(pVal, 0x0, nLeftLen);
						}
							
						ptr++;
						nPos2++;
					}

					if (nLen >= nBit)
					{
						bFind = true;
						memcpy(szIndexVal, pVal, nLen);
					}

					if (pVal)
					{
						delete[] pVal;
						pVal = NULL;
					}

					if (bFind)
					{
						break;
					}
				}
				else if (i == 1)
				{//
					nPos3 = 0;
					nPos2 += nSpanLen;

					nLen  = 0;
					nRightLen = (nSrcLen-nPos2) +1;
					
					ptr = (char*)(pszSrcFileName + nPos2);
	
					pVal = new char[nRightLen];
					memset(pVal, 0x0, nRightLen);

					while(nPos2 < nSrcLen)
					{
						if (*ptr >= '0' && *ptr <= '9')
						{
							memcpy(pVal+nPos3, &(*ptr), 1);
							nPos3 += 1;

							nLen++;

							ptr++;
							nPos2++;
							continue;
						}

						if (nLen >= nBit)
						{
							bFind = true;
							break;
						}
						else
						{
							nLen  = 0;
							nPos3 = 0;

							memset(pVal, 0x0, nRightLen);
						}

						ptr++;
						nPos2++;
					}

					if (nLen >= nBit)
					{
						bFind = true;
						memcpy(szIndexVal, pVal, nLen);
					}

					if (pVal)
					{
						delete[] pVal;
						pVal = NULL;
					}

					if (bFind)
					{
						break;
					}
				}
				else
				{
					sprintf_s(szIndexVal, "%d", nIndex++);
				}
			}
		}
		else
		{//查找span右半部分的字符串
			nPos3  = 0;
			nPos2 += nSpanLen;

			nRightLen = (nSrcLen-nPos2) + 1;
			
			ptr = (char*)(pszSrcFileName + nPos2);

			pVal = new char[nRightLen];
			memset(pVal, 0x0, nRightLen);

			while(nPos2 < nSrcLen)
			{
				if (*ptr >= '0' && *ptr <= '9')
				{
					memcpy(pVal+nPos3, &(*ptr), 1);
					nPos3 += 1;

					nLen++;

					ptr++;
					nPos2++;
					continue;
				}

				if (nLen >= nBit)
				{
					bFind = true;
					break;
				}
				else
				{
					nLen  = 0;
					nPos3 = 0;

					memset(pVal, 0x0, nRightLen);
				}

				ptr++;
				nPos2++;
			}

			if (nLen >= nBit)
			{
				bFind = true;
				memcpy(szIndexVal, pVal, nLen);
			}

			if (pVal)
			{
				delete[] pVal;
				pVal = NULL;
			}
		}

		if (bFind)
		{
			bRet = true;

			sprintf_s(szFileName, "%s%s", pszReqName, szIndexVal);	
			memcpy(pszDstFileName, szFileName, (nReqLen+nLen));
		}
		else
		{
			bRet = false;
		}
		
	}
	else
	{
		nLeftLen =  nSrcLen + 1;

		ptr = (char*)pszSrcFileName;

		pVal = new char[nLeftLen];
		memset(pVal, 0x0, nLeftLen);

		while(nPos2 < nSrcLen)
		{
			if (*ptr >= '0' && *ptr <= '9')
			{
				memcpy(pVal+nPos3, &(*ptr), 1);
				nPos3 += 1;

				nLen++;

				ptr++;
				nPos2++;
				continue;
			}

			if (nLen >= nBit)
			{
				bFind = true;
				break;
			}
			else
			{
				nLen  = 0;
				nPos3 = 0;

				memset(pVal, 0x0, nLeftLen);
			}

			ptr++;
			nPos2++;
		}

		if (nLen >= nBit)
		{
			bFind = true;
			memcpy(szIndexVal, pVal, nLen);
		}

		if (pVal)
		{
			delete[] pVal;
			pVal = NULL;
		}

		if (bFind)
		{
			bRet = true;

			sprintf_s(szFileName, "%s%s", pszReqName, szIndexVal);	
			memcpy(pszDstFileName, szFileName, (nReqLen+nLen));
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}

std::vector<CString> CGlobalInfo::SplitString1(LPCTSTR lpszSource, LPCTSTR lpszSeparator)
{
	int nPos    = 0;
	int nPrePos = 0;

	CString strSubString;
	CString strString = lpszSource;

	std::vector<CString> vecString;

	while(nPos != -1)
	{
		nPrePos = nPos;

		nPos = strString.Find(lpszSeparator, (nPos+1));
		vecString.push_back(strString.Mid(nPrePos, (nPos-nPrePos)));
	}

	return vecString;
}

std::vector<CString> CGlobalInfo::SplitString2(LPCTSTR lpszSource, LPCTSTR lpszSeparator)
{
	CString strSubString;
	CString strString = lpszSource;
	
	std::vector<CString> vecString;

	int nPos = strString.Find(lpszSeparator);

	while(nPos != -1)
	{
		strSubString = strString.Left(nPos);
		if (!strSubString.IsEmpty())
		{
			vecString.push_back(strSubString);
		}

		strString = strString.Right(strString.GetLength()-nPos-1);
		nPos = strString.Find(lpszSeparator);
	}

	if (!strString.IsEmpty())
	{
		vecString.push_back(strString);
	}

	return vecString;
}

std::vector<CString> CGlobalInfo::SplitString3(LPCTSTR lpszSource, LPCTSTR lpszSeparator)
{
	int nPos = 0;

	CString strSubString;

	std::vector<CString> vecString;

	while(AfxExtractSubString(strSubString, lpszSource, nPos, *lpszSeparator))
	{
		nPos++;
		vecString.push_back(strSubString);
	}

	return vecString;
}

std::vector<CString> CGlobalInfo::SplitString4(LPCTSTR lpszSource, LPCTSTR lpszSeparator)
{
	CString strSubString;
	CString strString(lpszSource);
	CString strSeparator(lpszSeparator);
	
	std::vector<CString> vecString;

	strSubString = strtok(strString.GetBuffer(0), strSeparator.GetBuffer(0));
	if (!strSubString.IsEmpty())
	{
		vecString.push_back(strSubString);
	}

	while (1)
	{
		strSubString = strtok(NULL, strSeparator.GetBuffer(0));
		if (strSubString == _T(""))
		{
			break;
		}

		strSubString.TrimLeft();
		if (!strSubString.IsEmpty())
		{
			vecString.push_back(strSubString);
		}
	}

	return vecString;
}

std::vector<CString> CGlobalInfo::SplitString5(LPCTSTR lpszSource, LPCTSTR lpszSeparator)
{
	int nPos = 0;

	CString strSubString;
	CString strString(lpszSource);

	std::vector<CString> vecString;

	strSubString = strString.Tokenize(lpszSeparator, nPos);
	while(strSubString.Trim() != _T(""))
	{
		vecString.push_back(strSubString);
		strSubString = strString.Tokenize(lpszSeparator, nPos);
	}

	return vecString;
}

std::vector<std::string> CGlobalInfo::SplitString6(LPCTSTR lpszSource, LPCTSTR lpszSeparator)
{
	std::string::size_type start = 0;
	std::string::size_type index = 0;

	std::string strSubString;
	std::string strString(lpszSource);

	std::vector<std::string> vecString;

	do 
	{
		index = strString.find_first_of(lpszSeparator, start);
		if (index != std::string::npos)
		{
			strSubString = strString.substr(start, index-start);
			if (strSubString != _T(""))
			{
				vecString.push_back(strSubString);
			}

			start = strString.find_first_not_of(lpszSeparator, index);

			if (start == std::string::npos)
			{
				return vecString;
			}
		}
	} while (index != std::string::npos);

	strSubString = strString.substr(start);
	vecString.push_back(strSubString);

	return vecString;
}