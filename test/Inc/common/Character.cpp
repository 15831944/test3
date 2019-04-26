#include "stdafx.h"
#include "Character.h"

#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//
CControl::CControl()
{

}

CControl::~CControl()
{

}

int CControl::IsStringANSI(const char* pszString)
{
	return 0;
}

int CControl::IsStringUtf8(const char* pszString)
{
	bool bRet = false;
	bool bIsAllAscii = false;

	unsigned int uiIndex = 0;
	unsigned int uiBytes = 0;		//UTF-8可用1-6个字节编码, ASCII用一个字节
	unsigned char uChar = '\0';

	uChar = *pszString;
	for (uiIndex=0; pszString[uiIndex]!='\0'; ++uiIndex)
	{
		uChar = *(pszString + uiIndex);
		if ((uChar&0x80) != 0 && uiBytes == 0)	//判断是否ASCII编码, 如果不是则为UTF8; ASCII用7位编码, 最高位标记为0, 0xxxxxxx
		{
			bIsAllAscii = false;
		}
		else
		{
			bIsAllAscii = true;
		}

		if (uiBytes == 0)
		{//
			if (uChar >= 0x80)			//如果不是ASCII码, 则是多字节符, 计算字节数
			{
				if (uChar >= 0xFC && uChar <= 0xFD)
				{
					uiBytes = 6;
				}
				else if (uChar >= 0xF8)
				{
					uiBytes = 5;
				}
				else if (uChar >= 0xF0)
				{
					uiBytes = 4;
				}
				else if (uChar >= 0xE0)
				{
					uiBytes = 3;
				}
				else if (uChar >= 0xC0)
				{
					uiBytes = 2;
				}
				else
				{
					bRet = false;
					break;
				}

				uiBytes--;
			}
		}
		else
		{
			if ((uChar&0xC0) != 0x80)	//多字节符的非首字节, 应为10xxxxxx
			{
				bRet = false;
				break;
			}

			uiBytes--;
		}
	}

	if (uiBytes != 0)
	{
		bRet = false;
	}
	else
	{
		if (bIsAllAscii)
		{
			bRet = true;
		}
		else
		{
			bRet = true;
		}
	}

	return bRet ? 1 : 0;
}

int CControl::IsStringGBK(const char* pszString)
{
	bool bRet = false;
	bool bIsAllAscii = false;

	unsigned int uiIndex = 0;
	unsigned int uiBytes = 0;
	unsigned char uChar = '\0';

	uChar = *pszString;
	for (uiIndex=0; pszString[uiIndex]!='\0'; ++uiIndex)
	{
		uChar = *(pszString+uiIndex);
		if ((uChar&0x80) != 0 && uiBytes == 0)	//判断是否ASCII编码, 如果不是则是GBK编码
		{
			bIsAllAscii = false;
		}
		else
		{
			bIsAllAscii = true;
		}

		if (uiBytes == 0)
		{
			if (uChar >= 0x80)
			{
				if (uChar >= 0x81 && uChar <= 0xFE)
				{
					uiBytes = 2;
				}
				else
				{
					bRet = false;
					break;
				}

				uiBytes--;
			}
		}
		else
		{
			if (uChar < 0x40 || uChar > 0xFE)
			{
				bRet = false;
				break;
			}

			uiBytes--;
		}
	}

	if (uiBytes != 0)
	{
		bRet = false;
	}
	else
	{
		if (bIsAllAscii)
		{
			bRet = true;
		}
		else
		{
			bRet = true;
		}
	}

	return bRet ? 1 : 0;
}

int CControl::IsStringUnicode(const char* pszString)
{
	return 0;
}

/*
 * C语言函数字符串分割
 * char转换CString, strtok函数分割字符串;
*/
std::vector<char*> CControl::SplitString1(const char pszSource[], const char* pszSeparator)
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
std::vector<std::string> CControl::SplitString2(const char* pszSource, const char* pszSeparator)
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
std::vector<CString> CControl::SplitString3(const char* pszSource, const char* pszSeparator)
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
CConvert::CConvert()
{
}

CConvert::~CConvert()
{

}

//////////////////////////////////////////////////////////////////////////
////将wchar_t* 转成char*的实现函数(wcstombs,mbstowcs)
int CConvert::UnicodeToANSI(char* szDesc, const wchar_t* szSrc)
{
#ifdef WIN32
	int nLength = wcslen(szSrc);
	//获取转换后的长度
	int nbytes = WideCharToMultiByte(CP_ACP,        //指定执行转换的代码页; CP_ACP : ANSI代码页;CP_UTF7：使用UTF-7转换；CP_UTF8：使用UTF-8转换;
									 0,
									 szSrc,			//指定的宽字节字符串的缓冲区 
									 -1,
									 NULL,			//指向接收被转换字符串的缓冲区
									 0,				//指定由参数lpMultiByteStr指向的缓冲区最大值 
									 NULL,
									 NULL);
	
	// 通过以上得到的结果，转换unicode 字符为ascii 字符
	nbytes = WideCharToMultiByte(CP_ACP,
								 0,
								 szSrc,
								 -1,
								 szDesc,
								 nbytes,
								 NULL,
								 NULL);
	return nbytes;
#else
#endif
}

////将char* 转成wchar_t*的实现函数
int CConvert::ANSIToUnicode(wchar_t* szDesc, const char* szSrc)
{
#ifdef WIN32
	int nLength = strlen(szSrc);
	int nbytes  = MultiByteToWideChar(CP_ACP,
									  0,
									  (LPCSTR)szSrc,
									  -1,
									  NULL,
									  0);

	nbytes = MultiByteToWideChar(CP_ACP,
								 0,
								 (LPCSTR)szSrc,
								 -1,
								 szDesc,
								 nbytes);
	return nbytes;
#else
#endif
}

int CConvert::UTF8ToUnicode(wchar_t* szDesc, const char* szSrc)
{
#ifdef WIN32
	int nLength= strlen(szSrc);
	int nbytes = MultiByteToWideChar(CP_UTF8,
								     0,
									(LPCSTR)szSrc,
									 -1,						//为-1时，表示该字符串是以NULL结尾的字符串，其长度会自动计入字符串长度之内
									 NULL,
									 NULL);

	nbytes = MultiByteToWideChar(CP_UTF8,
								 0,
								(LPCSTR)szSrc,
								-1,
								szDesc,
								nbytes);
	
	return nbytes;
#else
#endif
}

int CConvert::UnicodeToUTF8(char* szDesc, const wchar_t* szSrc)
{
#ifdef WIN32
	int nLength = wcslen(szSrc);
	int nbytes  = WideCharToMultiByte(CP_UTF8,
									  0,
									  (LPCWSTR)szSrc,
									  -1,
									  NULL,
									  0,
									  NULL,
									  NULL);

	
	nbytes = WideCharToMultiByte(CP_UTF8,
								 0,
								(LPCWSTR)szSrc,
								-1,
								szDesc,
								nbytes,
								NULL,
								NULL);
	return nbytes;
#else
#endif
}


int CConvert::StringToHexString(char* szDesc, const char* szSrc, int nLen, char chTag)
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


int CConvert::HexStringToBytes(unsigned char* szDesc, const char* szSrc,int nLen)
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


int CConvert::BytesHexToString(char*  szDesc, const unsigned char* szSrc,int nLen)
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

bool CConvert::ConvertToInt(const double &val,int& i)
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