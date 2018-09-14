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
	unsigned int uiBytes = 0;		//UTF-8����1-6���ֽڱ���, ASCII��һ���ֽ�
	unsigned char uChar = '\0';

	uChar = *pszString;
	for (uiIndex=0; pszString[uiIndex]!='\0'; ++uiIndex)
	{
		uChar = *(pszString + uiIndex);
		if ((uChar&0x80) != 0 && uiBytes == 0)	//�ж��Ƿ�ASCII����, ���������ΪUTF8; ASCII��7λ����, ���λ���Ϊ0, 0xxxxxxx
		{
			bIsAllAscii = false;
		}
		else
		{
			bIsAllAscii = true;
		}

		if (uiBytes == 0)
		{//
			if (uChar >= 0x80)			//�������ASCII��, ���Ƕ��ֽڷ�, �����ֽ���
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
			if ((uChar&0xC0) != 0x80)	//���ֽڷ��ķ����ֽ�, ӦΪ10xxxxxx
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
		if ((uChar&0x80) != 0 && uiBytes == 0)	//�ж��Ƿ�ASCII����, �����������GBK����
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
//////////////////////////////////////////////////////////////////////////
//
CConvert::CConvert()
{
}

CConvert::~CConvert()
{

}

//////////////////////////////////////////////////////////////////////////
////��wchar_t* ת��char*��ʵ�ֺ���(wcstombs,mbstowcs)
int CConvert::UnicodeToANSI(char* szDesc, const wchar_t* szSrc)
{
#ifdef WIN32
	int nLength = wcslen(szSrc);
	//��ȡת����ĳ���
	int nbytes = WideCharToMultiByte(CP_ACP,        //ָ��ִ��ת���Ĵ���ҳ; CP_ACP : ANSI����ҳ;CP_UTF7��ʹ��UTF-7ת����CP_UTF8��ʹ��UTF-8ת��;
									 0,
									 szSrc,			//ָ���Ŀ��ֽ��ַ����Ļ����� 
									 -1,
									 NULL,			//ָ����ձ�ת���ַ����Ļ�����
									 0,				//ָ���ɲ���lpMultiByteStrָ��Ļ��������ֵ 
									 NULL,
									 NULL);
	
	// ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
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

////��char* ת��wchar_t*��ʵ�ֺ���
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
									 -1,						//Ϊ-1ʱ����ʾ���ַ�������NULL��β���ַ������䳤�Ȼ��Զ������ַ�������֮��
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
			c1 = *pSrc >>4;		////��ȡ�õ��ַ���Ϊ�ߵ���λ���ٴ��жϸ�λ
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
		// �����4λ
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

		// �����4λ
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
		// �����4λ
		*szDesc++ = tab[*pSrc >> 4];

		// �����4λ
		*szDesc++ = tab[*pSrc & 0x0f];
		pSrc++;
	}
	*szDesc = '\0';
	return nLen*2;
}
