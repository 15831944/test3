CString CUtiliy::InterceptSubText(LPCTSTR lpszUserName, UINT uiLimitLen)
{
	BOOL bRet = FALSE;

	UINT uiBit = 0;
	UINT uiPos = 0;

	UINT uiIndex = 0;
	UINT uiOffset = 0;
	UINT uiSpecIndex = 0;

	UINT uitest = 0;
	UINT uiNameLen = 0;
	

	char *p = NULL;
	CString strUserName;
	char szNewUserName[256] = {0};

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

		while(*p != '\0')
		{
			if ((*p&0x80) && (*(p+1)&0x80))
			{
				uiBit = 2;
			}
			else
			{
				uiBit = 1;
			}

			if (*(p+1) != '\0' && uiIndex != uiSpecIndex)
			{
				memcpy(szNewUserName+uiOffset, p, uiBit);
				uiOffset += uiBit;

				p += uiBit;
				uiPos += uiBit;
			}
			else
			{

			}

			uiIndex++;
		}
	} while (FALSE);

	return strUserName;
}