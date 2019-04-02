CString InterceptSubText(LPCTSTR lpszUserName, UINT uiLimitLen)
{
	BOOL bRet = FALSE;

	UINT uiBit = 0;
	UINT uiPos = 0;
	UINT uiNameLen = 0;

	UINT uiIndex = 0;
	UINT uiOffset = 0;
	UINT uiSpecIndex = 0;

	char *p = NULL;
	CString strUserName;
	char szNewUserName[256] = { 0 };

	do
	{
		if (lpszUserName == NULL || *lpszUserName == '\0')
		{
			bRet = FALSE;
			break;
		}

		p = (char*)lpszUserName;
		uiNameLen = strlen(lpszUserName);
		if (uiLimitLen > uiNameLen)
		{
			uiSpecIndex = uiNameLen;
		}
		else
		{
			uiSpecIndex = uiLimitLen;
		}

		while (*p != '\0')
		{
			if ((*p & 0x80) && (*(p + 1) & 0x80))
			{
				uiBit = 2;
			}
			else
			{
				uiBit = 1;
			}

			if (*(p + 1) != '\0' && uiIndex != uiSpecIndex)
			{
				memcpy(szNewUserName + uiOffset, p, uiBit);
				uiOffset += uiBit;

				p += uiBit;
				uiPos += uiBit;
			}
			else
			{
				if (uiIndex == uiSpecIndex)
				{
					if (uiPos == uiNameLen)
					{
						strUserName.Format(_T("%s"), szNewUserName);
					}
					else
					{
						strUserName.Format(_T("%s..."), szNewUserName);
					}
				}
				else
				{
					memcpy(szNewUserName + uiOffset, p, uiBit);
					uiOffset += uiBit;

					p += uiBit;
					uiPos += uiBit;

					strUserName.Format(_T("%s"), szNewUserName);
				}
				
				bRet = TRUE;
				break;
			}

			uiIndex++;
		}
	} while (FALSE);

	return strUserName;
}

void test1()
{
	if (*(p+uiBit) != '\0' && iIndex != iDelIndex)
	{
		memcpy(szDataBuffer+uiOffset, p, uiBit);
		uiOffset += uiBit;

		p += uiBit;
		uiPos += uiBit;
	}
	else
	{
		if (iDelIndex == iIndex)
		{
			p += uiBit;
			uiPos += uiBit;

			if (*(p+uiBit) == '\0')
			{
				memcpy(szDataBuffer+uiOffset, p, uiBit);
				uiOffset += uiBit;

				memcpy(szDataBuffer+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
				uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

				memcpy(szFileNewName, szDataBuffer, uiOffset);

				bRet = TRUE;
				break;
			}
		}
		else
		{
			memcpy(szDataBuffer+uiOffset, p, uiBit);
			uiOffset += uiBit;

			p += uiBit;
			uiPos += uiBit;

			memcpy(szDataBuffer+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
			uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

			memcpy(szFileNewName, szDataBuffer, uiOffset);

			bRet = TRUE;
			break;
		}
	}
}
