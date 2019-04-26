#include "stdafx.h"
#include "FILETest.h"

FILETest::FILETest()
{
}

FILETest::~FILETest()
{
}

void FILETest::Test1()
{
	int nRet   = -1;
	int nFileLen = -1;

	int nIndex = 0;
	int nPos   = 0;
	int nLen   = 0;

	int nLeft  = 0;
	int nRight = 0;

	char *pFileBuffer = NULL;
	char *pLine = NULL;
	char *p = NULL;

	CFile file;
	if (!file.Open("D:\\1.txt", CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone))
	{
		return;
	}

	nFileLen = file.GetLength();

	pFileBuffer = (char*)malloc(nFileLen+1);
	memset(pFileBuffer, 0x0, nFileLen+1);

	nRet = file.Read(pFileBuffer, nFileLen);
	if (nRet == -1 || nRet != nFileLen)
	{
		return;
	}

	p = pFileBuffer;

	nLeft  = nFileLen;
	nRight = nFileLen;

	while(*p != '\0')
	{
		if (*p == '\r')
		{
			nLen = nIndex - nPos;

			pLine = (char *)malloc(nLen+1);
			memset(pLine, 0x0, nLen+1);

			memcpy(pLine, pFileBuffer+nPos, nLen);

			free(pLine);
			pLine = NULL;

			nPos   = nIndex;
			nLeft -= nLen;
		}
		else if (nPos > 0 && (nIndex+1) == nRight)
		{
			nLen = nIndex -nPos;

			pLine = (char*)malloc(nLen+1);
			memset(pLine, 0x0, nLen+1);

			memcpy(pLine, pFileBuffer+nPos+1, nLen);

			free(pLine);
			pLine = NULL;

			nPos   = nIndex;
			nLeft -= nLen; 
		}

		nIndex++;
		p++;
	}
}