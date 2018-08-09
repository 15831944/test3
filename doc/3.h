
BOOL update_file_name::EnumFileInfo()
{
	BOOL bRet = FALSE;
	
	long file = 0;
	std::string strFileName;

	struct _stat	s_file ={0};
	_finddata_t		c_file;
	UPDATE_FILEINFO* pFileInfo = NULL;

	char szFindPath[MAX_PATH+1] = {0};
	char szFilePath[MAX_PATH+1] = {0};
	char szFileExt[MAX_PATH+1]  = {0};
	char szFileTitle[MAX_PATH+1] = {0};
	
/*
	if (_stat(m_strShellPath.c_str(), &s_file) != 0)
	{
		return FALSE;
	}

	if ((s_file.st_mode&_S_IFDIR) != _S_IFDIR)
	{
		return FALSE;
	}

	_tcscpy(szFindPath, m_strShellPath.c_str());
	_tcscat(szFindPath, _T("\\*.*"));

	file = _findfirst(szFindPath, &c_file);
	if (file == -1)
	{
		return FALSE;
	}

	do 
	{
		if (WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) == WAIT_OBJECT_0)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			if (!m_bExit)
			{
				m_dwProcTimeOver = 0;
			}
			else
			{

				SetEvent(m_hEndEvent);
				continue;
			}
		}

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
			if (strlen(c_file.name) == 0)
			{
				continue;
			}

			memset(szFileTitle, 0x0, MAX_PATH+1);
			memset(szFilePath, 0x0, MAX_PATH+1);
			memset(szFileExt, 0x0, MAX_PATH+1);

			sprintf(szFilePath, _T("%s\\%s"), m_strShellPath.c_str(), c_file.name);
			if (!GetFileTitle(c_file.name, szFileTitle, szFileExt))
			{
				continue;
			}

			pFileInfo = new UPDATE_FILEINFO;
			if (pFileInfo == NULL)
			{
				continue;
			}
			memset(pFileInfo, 0x0, sizeof(UPDATE_FILEINFO));

			pFileInfo->uiFileSize = c_file.size;
			pFileInfo->uiFileAttrib = c_file.attrib;
			pFileInfo->time_create = c_file.time_create;
			pFileInfo->time_access = c_file.time_access;
			pFileInfo->time_write  = c_file.time_write;
			strcpy(pFileInfo->szFileName, szFileTitle);
			strcpy(pFileInfo->szFilePath, szFilePath);
			strcpy(pFileInfo->szFileExt, szFileExt);
			strcpy(pFileInfo->szParentPath, m_strShellPath.c_str());

			strFileName = c_file.name;
			m_mapEnumInfo.insert(make_pair(strFileName, pFileInfo));

			bRet = TRUE;
		}
	} while (_tfindnext(file, &c_file) == 0);

	if (file)
	{
		_findclose(file);
	}
*/
	return bRet;
}



BOOL update_file_name::GetFileTitle(const char *pszFileName, char *pszTitle, char *pszExt)
{
	char *ptr = NULL;
	int  nPos = 0;
	int  nLen = 0;

	if (pszFileName == NULL || pszTitle == NULL || pszExt == NULL)
	{
		return FALSE;
	}

	nLen = strlen(pszFileName);

	ptr = strrchr((char*)pszFileName, '.');
	if (ptr == NULL)
	{
		memcpy(pszTitle, pszFileName, strlen(pszFileName));
		return TRUE;
	}

	nPos = ptr - pszFileName;
	if (nPos == -1)
	{
		return FALSE;
	}

	memcpy(pszTitle, pszFileName, nPos);
	memcpy(pszExt, pszFileName+nPos, nLen-nPos);

	return TRUE;
}

BOOL update_file_name::GetEvalResult(EVAL_FILEINFO* pEvalTag)
{
	BOOL bRet = FALSE;

	std::string::size_type index = 0;
	std::string::size_type start = 0;
	const char* pSpecTag = _T(".");

	vector<std::string> vecString1;
	vector<std::string> vecString2;
	UPDATE_CONFIGTYPE hConfigType = CONFIG_EMPTYTYPE;
	
	if (pEvalTag == NULL)
	{
		return FALSE;
	}
/*
	if (m_strSubName == _T("") && m_strSubName.size() == 0)
	{
		return FALSE;
	}

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

	index = m_strFindName.find_first_of(pSpecTag, start);
	if (index != std::string::npos)
	{
		vecString1 = pGlobal->SplitString2(m_strFindName.c_str(), pSpecTag);
		if (vecString1.size() > 1)
		{
			hConfigType = CONFIG_EXTNAMETYPE;
		}
		else
		{
			hConfigType = CONFIG_EXTNAMETYPE;
		}
	}
	else
	{
		hConfigType = CONFIG_FILENAMETYPE;
	}

	index = 0;
	start = 0;
	index = m_strSubName.find_first_of(pSpecTag, start);
	if (index != std::string::npos)
	{
		vecString2 = pGlobal->SplitString2(m_strSubName.c_str(), pSpecTag);
		if (vecString2.size() > 1)
		{
			bRet = FALSE;
		}
		else
		{
			if (hConfigType == CONFIG_FILENAMETYPE)
			{
				bRet = FALSE;
			}
			else if (hConfigType == CONFIG_EXTNAMETYPE)
			{
				bRet = TRUE;
			}
			else
			{
				bRet = FALSE;
			}
		}
	}
	else
	{
		if (hConfigType == CONFIG_FILENAMETYPE)
		{
			bRet = TRUE;
		}
		else if (hConfigType == CONFIG_EXTNAMETYPE)
		{
			bRet = FALSE;
		}
		else
		{
			bRet = FALSE;
		}
	}

	if (bRet)
	{
		pEvalTag->emConfigType = hConfigType;
		pEvalTag->emEvalType = m_hEvalType;

		if (hConfigType == CONFIG_EXTNAMETYPE)
		{
			pEvalTag->vecString.assign(vecString1.begin(), vecString1.end());
		}
	}
*/
	return bRet;
}

BOOL update_file_name::SetFileExtInfo(EVAL_FILEINFO* pEvalTag)
{
	BOOL bRet = FALSE;
	BOOL bStatus = FALSE;

	char szSpecFileExt[MAX_PATH] = {0};
	char szSpecFilePath[MAX_PATH] = {0};

	UPDATE_FILEINFO* pFileInfo = NULL;
	std::vector<std::string>::iterator vecIter;
	std::map<std::string, UPDATE_FILEINFO*>::iterator mapIter;

	if (pEvalTag == NULL)
	{
		return FALSE;
	}
/*
	if (m_strFindName == _T("") && m_strFindName.size() == 0)
	{
		return FALSE;
	}

	if (m_strSubName == _T("") && m_strSubName.size() == 0)
	{
		return FALSE;
	}

	if (m_mapEnumInfo.size() == 0)
	{
		return FALSE;
	}

	for (mapIter=m_mapEnumInfo.begin(); mapIter!=m_mapEnumInfo.end(); mapIter++)
	{
		if (WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) == WAIT_OBJECT_0)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			if (!m_bExit)
			{
				m_dwProcTimeOver = 0;
			}
			else
			{

				SetEvent(m_hEndEvent);
				continue;
			}
		}

		pFileInfo = mapIter->second;
		if (pFileInfo == NULL)
		{
			continue;
		}

		bStatus = FALSE;
		for (vecIter=pEvalTag->vecString.begin(); vecIter!=pEvalTag->vecString.end(); vecIter++)
		{
			sprintf(szSpecFileExt, _T(".%s"), vecIter->c_str());
			if (strcmp(pFileInfo->szFileExt, szSpecFileExt) != 0)
			{
				bStatus = FALSE;
				continue;
			}
			else
			{
				bStatus = TRUE;
				break;
			}
		}

		if (bStatus)
		{
			bRet = TRUE;

			memset(szSpecFilePath, 0x0, MAX_PATH);
			sprintf(szSpecFilePath, _T("%s\\%s%s"), pFileInfo->szParentPath, pFileInfo->szFileName, m_strSubName.c_str());

			rename(pFileInfo->szFilePath, szSpecFilePath);
		}
		else
		{
			continue;
		}
	}
*/
	return bRet;
}

BOOL update_file_name::SetFileNameInfo(EVAL_FILEINFO* pEvalTag)
{
	BOOL bRet = FALSE;
	unsigned long ulIndex = 0;

	UPDATE_FILEINFO* pFileInfo = NULL;
	std::vector<std::string>::iterator vecIter;
	std::map<std::string, UPDATE_FILEINFO*>::iterator mapIter;

	if (pEvalTag == NULL || pEvalTag->emEvalType == EVAL_EMPTYTYPE)
	{
		return FALSE;
	}
/*
	if (m_strSubName == _T("") && m_strSubName.size() == 0)
	{
		return FALSE;
	}

	if (m_mapEnumInfo.size() == 0)
	{
		return FALSE;
	}

	for (mapIter=m_mapEnumInfo.begin(); mapIter!=m_mapEnumInfo.end(); mapIter++, ulIndex++)
	{
		if (WaitForSingleObject(m_hEndEvent, m_dwProcTimeOver) == WAIT_OBJECT_0)
		{
			bRet = FALSE;
			break;
		}
		else
		{
			if (!m_bExit)
			{
				m_dwProcTimeOver = 0;
			}
			else
			{

				SetEvent(m_hEndEvent);
				continue;
			}
		}

		pFileInfo = mapIter->second;
		if (pFileInfo == NULL)
		{
			continue;
		}

		if (pEvalTag->emEvalType == EVAL_ALLFILENAME)
		{
			SetAllFileName(pFileInfo->szParentPath, pFileInfo->szFileName, NULL, m_strSubName.c_str(), pFileInfo->szFileExt, ulIndex);
		}
		else if (pEvalTag->emEvalType == EVAL_SPECIFYNAME)
		{
			SetSpecifyName(pFileInfo->szParentPath, pFileInfo->szFileName, m_strFindName.c_str(), m_strSubName.c_str(), pFileInfo->szFileExt);
		}
		else if (pEvalTag->emEvalType == EVAL_SPECIFYNUMINDEX)
		{
			SetNumIndexName(pFileInfo->szParentPath, pFileInfo->szFileName, m_strFindName.c_str(), m_strSubName.c_str(), pFileInfo->szFileExt, ulIndex);
		}

		bRet = TRUE;
	}
*/
	return bRet;
}

BOOL update_file_name::SetAllFileName(const char* pszFilePath, const char* pszSrcName, const char* pszFindName, const char* pszSpecName, const char* pszFileExt, unsigned long ulIndex)
{
	int nRet = 0;
	int nPos = 0;

	BOOL bRet = FALSE;

	const char* pSubName = _T("_");

	char szSrcFilePath[MAX_PATH]  = {0};
	char szDescFilePath[MAX_PATH] = {0};

	if (pszFilePath == NULL || *pszFilePath == '\0')
	{
		return FALSE;
	}

	if (pszSpecName == NULL || *pszSpecName == '\0')
	{
		return FALSE;
	}

	if (pszFileExt == NULL || *pszFileExt == '\0')
	{
		return FALSE;
	}

	sprintf(szSrcFilePath,  _T("%s\\%s%s"), pszFilePath, pszSrcName,  pszFileExt);
	sprintf(szDescFilePath, _T("%s\\%s%s%d%s"), pszFilePath, pszSpecName, pSubName, ulIndex, pszFileExt);

	nRet = rename(szSrcFilePath, szDescFilePath);
	if (nRet == 0)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL update_file_name::SetSpecifyName(const char* pszFilePath, const char* pszSrcName, const char* pszFindName, const char* pszSpecName, const char* pszFileExt)
{
	int nRet = 0;
	int nPos = 0;
	int nIndex = 0;

	int nNameLen = 0;
	int nRemainLen = 0;

	BOOL bRet = FALSE;

	char* pChar = NULL;
	char* pSrcName = NULL;

	char szFileName[MAX_PATH] = {0};
	char szSrcFilePath[MAX_PATH]  = {0};
	char szDescFilePath[MAX_PATH] = {0};

	if (pszFilePath == NULL || *pszFilePath == '\0')
	{
		return FALSE;
	}

	if (pszFindName == NULL || *pszFindName == '\0')
	{
		return FALSE;
	}

	if (pszSpecName == NULL || *pszSpecName == '\0')
	{
		return FALSE;
	}

	if (pszFileExt == NULL || *pszFileExt == '\0')
	{
		return FALSE;
	}

	pSrcName = (char*)pszSrcName;
	nRemainLen = strlen(pszSrcName);

	while(nRemainLen>0)
	{
		pChar = strstr(pSrcName, pszFindName);
		if (pChar == NULL)
		{
			memcpy(szFileName+nNameLen, pSrcName, strlen(pSrcName));
			nNameLen += strlen(pSrcName);
			nRemainLen -= strlen(pSrcName);
			continue;
		}

		nPos = pChar - pSrcName;
		if (nPos <= -1)
		{
			nRemainLen -= strlen(pSrcName);
			continue;
		}

		if (nPos == 0)
		{
			nRet = 1;

			memcpy(szFileName+nNameLen, pszSpecName, strlen(pszSpecName));
			nNameLen += strlen(pszSpecName); 
			nRemainLen -= strlen(pszFindName);
		}
		else
		{
			nRet = 1;

			memcpy(szFileName+nNameLen, pSrcName, nPos);
			nNameLen += nPos;
			nRemainLen -= nPos;

			memcpy(szFileName+nNameLen, pszSpecName, strlen(pszSpecName));
			nNameLen += strlen(pszSpecName); 
			nRemainLen -= strlen(pszFindName);
		}
		
		pSrcName = pSrcName + nPos + strlen(pszFindName);
	}

	if (nRet != 1)
	{
		bRet = FALSE;
	}
	else
	{
		sprintf(szSrcFilePath,  _T("%s\\%s%s"), pszFilePath, pszSrcName, pszFileExt);
		sprintf(szDescFilePath, _T("%s\\%s%s"), pszFilePath, szFileName, pszFileExt);

		nRet = rename(szSrcFilePath, szDescFilePath);
		if (nRet == 0)
		{
			bRet = TRUE;
		}
		else
		{
			bRet = FALSE;
		}
	}

	return bRet;
}

BOOL update_file_name::SetNumIndexName(const char* pszFilePath, const char* pszSrcName, const char* pszFindName, const char* pszSpecName, const char* pszFileExt, unsigned long ulIndex)
{
	int nRet = 0;

	int nPos = 0;
	int nLeftPos = 0;
	int nRightPos = 0;

	int nPtrIndex = 0;
	int nLeftIndex = 0;
	int nRightIndex = 0;

	int nNameLen = 0;
	int nLeftLen = 0;
	int nRightLen = 0;
	int nRemainLen = 0;

	BOOL bRet = FALSE;
	BOOL bFirst = FALSE;
	BOOL bLeftFind = FALSE;
	BOOL bRightFind = FALSE;

	char* pChar = NULL;
	char* pValue = NULL;
	char* pSrcName = NULL;
	char* pPtrValue = NULL;

	const char* pSubName = _T("_");
	const char* pLeftSubName = _T("_");
	const char* pRightSubName = _T("#");

	char szLeftFileName[MAX_PATH] = {0};
	char szRightFileName[MAX_PATH] = {0};

	char szSrcFilePath[MAX_PATH]  = {0};
	char szDescFilePath[MAX_PATH] = {0};

	if (pszFilePath == NULL || *pszFilePath == '\0')
	{
		return FALSE;
	}

	if (pszFindName == NULL || *pszFindName == '\0')
	{
		return FALSE;
	}

	if (pszSpecName == NULL || *pszSpecName == '\0')
	{
		return FALSE;
	}

	if (pszFileExt == NULL || *pszFileExt == '\0')
	{
		return FALSE;
	}

	pSrcName = (char*)pszSrcName;
	nRemainLen = strlen(pszSrcName);

	pChar = strstr(pSrcName, pszFindName);
	if (pChar == NULL)
	{
		return FALSE;
	}

	nPos = pChar - pSrcName;
	if (nPos < 0)
	{
		return FALSE;
	}

	if (nPos == 0)
	{
		//Right //2			//(wl)_1#23#4	//(wl)_123
		nRightLen = nRemainLen - strlen(pszFindName);
		pValue = szRightFileName;

		nPtrIndex = 0;
		bFirst = FALSE;
		bLeftFind = FALSE;

		pPtrValue = (char*)pszSrcName + strlen(pszFindName);

		while(nRightLen > 0)
		{
			if (*pPtrValue >= '0' &&*pPtrValue <= '9')
			{
				if (!bFirst)
				{
					bFirst = TRUE;
					bRightFind = TRUE;

					memcpy(pValue+nRightPos, &(*pPtrValue), 1);
					nRightPos += 1;
				}
				else
				{
					if (nPtrIndex == (nRightIndex+1))
					{
						memcpy(pValue+nRightPos, &(*pPtrValue), 1);
						nRightPos += 1;
					}
					else
					{
						strncpy(pValue+nRightPos, pRightSubName, 1);
						nRightPos += 1;

						memcpy(pValue+nRightPos, &(*pPtrValue), 1);
						nRightPos += 1;
					}
				}

				nRightLen -= 1;
				nRightIndex = nPtrIndex;

				nPtrIndex++;
				pPtrValue = pPtrValue + 1;
				continue;
			}

			nRightLen -= 1;
			nPtrIndex ++;

			pPtrValue = pPtrValue + 1;
			continue;
		}
	}
	else
	{
		//Left //1			//$1$2$3s1(wl)		//123$4(wl)
		nLeftLen = nPos;
		pValue = szLeftFileName;

		pPtrValue = (char*)pszSrcName;

		while(nLeftLen > 0)
		{
			if (*pPtrValue >= '0' &&*pPtrValue <= '9')
			{
				if (!bFirst)
				{
					bFirst = TRUE;
					bLeftFind = TRUE;

					strncpy(pValue+nLeftPos, _T("("), 1);
					nLeftPos += 1;

					memcpy(pValue+nLeftPos, &(*pPtrValue), 1);
					nLeftPos += 1;
				}
				else
				{
					if (nPtrIndex == (nLeftIndex+1))
					{
						memcpy(pValue+nLeftPos, &(*pPtrValue), 1);
						nLeftPos += 1;
					}
					else
					{
						strncpy(pValue+nLeftPos, pLeftSubName, 1);
						nLeftPos += 1;

						memcpy(pValue+nLeftPos, &(*pPtrValue), 1);
						nLeftPos += 1;
					}
				}

				nLeftLen -= 1;
				nLeftIndex = nPtrIndex;
				nPtrIndex++;

				if (bFirst && nLeftLen == 0)
				{
					strncpy(pValue+nLeftPos, _T(")"), 1);
					nLeftPos += 1;
				}

				pPtrValue = pPtrValue + 1;
				continue;
			}

			nLeftLen -= 1;
			nPtrIndex++;

			if (bFirst && nLeftLen == 0)
			{
				strncpy(pValue+nLeftPos, _T(")"), 1);
				nLeftPos += 1;
			}

			pPtrValue = pPtrValue + 1;
			continue;
		}

		//Right //2			//$1$2$3s1(wl)_1#23#4		//123$4(wl)_123
		nRightLen = nRemainLen - nPos - strlen(pszFindName);
		pValue = szRightFileName;

		nPtrIndex = 0;
		bFirst = FALSE;
		
		pPtrValue = (char*)pszSrcName + nPos + strlen(pszFindName);

		while(nRightLen > 0)
		{
			if (*pPtrValue >= '0' &&*pPtrValue <= '9')
			{
				if (!bFirst)
				{
					bFirst = TRUE;
					bRightFind = TRUE;

					memcpy(pValue+nRightPos, &(*pPtrValue), 1);
					nRightPos += 1;
				}
				else
				{
					if (nPtrIndex == (nRightIndex+1))
					{
						memcpy(pValue+nRightPos, &(*pPtrValue), 1);
						nRightPos += 1;
					}
					else
					{
						strncpy(pValue+nRightPos, pRightSubName, 1);
						nRightPos += 1;

						memcpy(pValue+nRightPos, &(*pPtrValue), 1);
						nRightPos += 1;
					}
				}

				nRightLen -= 1;
				nRightIndex = nPtrIndex;

				nPtrIndex++;
				pPtrValue = pPtrValue + 1;
				continue;
			}

			nRightLen -= 1;
			nPtrIndex ++;

			pPtrValue = pPtrValue + 1;
			continue;
		}
	}

	if (!bLeftFind && !bRightFind)
	{
		sprintf(szDescFilePath, _T("%s\\%s%s%d%s"), pszFilePath, pszSpecName, pSubName, ulIndex, pszFileExt);
	}
	else if (bLeftFind && bRightFind)
	{
		sprintf(szDescFilePath, _T("%s\\%s%s%s%s%s"), pszFilePath, szLeftFileName, pszSpecName, pSubName, szRightFileName, pszFileExt);
	}
	else if (bLeftFind)
	{
		sprintf(szDescFilePath, _T("%s\\%s%s%s%d%s"), pszFilePath, szLeftFileName, pszSpecName, pSubName, ulIndex, pszFileExt);
	}
	else if (bRightFind)
	{
		sprintf(szDescFilePath, _T("%s\\%s%s%s%s"), pszFilePath, pszSpecName, pSubName, szRightFileName, pszFileExt);
	}

	sprintf(szSrcFilePath,  _T("%s\\%s%s"), pszFilePath, pszSrcName, pszFileExt);

	nRet = rename(szSrcFilePath, szDescFilePath);
	if (nRet == 0)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}