#include "stdafx.h"
#include "update_file_name.h"

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2018.09.01 17:39
 */
/************************************************************************/

//update_file_data
update_file_data::update_file_data()
{
	InitializeCriticalSection(&m_csLockData);
}

update_file_data::~update_file_data()
{
	ClearFileData();
	DeleteCriticalSection(&m_csLockData);
}

update_file_data& update_file_data::Instance()
{
	static update_file_data inst;
	return inst;
}

BOOL update_file_data::SetUpdateFileData(std::vector<UPDATE_FILEINFO*> &vecFileData, UPDATE_FILEDATA_CALLBACK_FUNC pfUpdateFileData, void *pParentObject)
{
	BOOL bRet = FALSE;
	
	UPDATE_FILEINFO *pFileInfo = NULL;
	UPDATE_FILEDATA *pFileData = NULL;

	UPDATE_FILEDATA stcUpdateFileData;
	std::vector<UPDATE_FILEINFO*>::iterator iterFileData;

	EnterCriticalSection(&m_csLockData);
	do 
	{
		memset(&stcUpdateFileData, 0x0, sizeof(UPDATE_FILEDATA));
		if (vecFileData.size() == 0 || pfUpdateFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		stcUpdateFileData.emUpdateStatus = STATE_UPDATEINPUTE_TYPE;
		if (!pfUpdateFileData(&stcUpdateFileData, pParentObject))
		{
			bRet = FALSE;
			break;
		}

		ClearFileData();
		for (iterFileData = vecFileData.begin(); iterFileData!=vecFileData.end(); ++iterFileData)
		{
			pFileInfo = (UPDATE_FILEINFO *)(*iterFileData);
			if (pFileInfo == NULL)
			{
				continue;
			}

			pFileData = new UPDATE_FILEDATA;
			if (pFileData == NULL)
			{
				continue;
			}
			memset(pFileData, 0x0, sizeof(UPDATE_FILEDATA));

			memcpy(pFileData, &stcUpdateFileData, sizeof(UPDATE_FILEDATA));
			memcpy(&pFileData->stcFileInfo, pFileInfo, sizeof(UPDATE_FILEINFO));
			pFileData->pfUpdateFunc = pfUpdateFileData;

			m_vecFileData.push_back(pFileData);
		}

		bRet = TRUE;
	} while (FALSE);

	LeaveCriticalSection(&m_csLockData);
	return bRet;
}

BOOL update_file_data::GetUpdateFileData(std::vector<UPDATE_FILEDATA *> &vecFileData)
{
	BOOL bRet = FALSE;

	EnterCriticalSection(&m_csLockData);
	do 
	{
		if (m_vecFileData.size() == 0)
		{
			bRet = FALSE;
			break;
		}

		vecFileData.clear();
		vecFileData.assign(m_vecFileData.begin(), m_vecFileData.end());

		bRet = TRUE;
	} while (FALSE);

	LeaveCriticalSection(&m_csLockData);
	return bRet;
}

void update_file_data::ClearFileData()
{
	BOOL bRet = FALSE;
	UPDATE_FILEDATA *pFileData = NULL;
	std::vector<UPDATE_FILEDATA *>::iterator iterFileData;

	EnterCriticalSection(&m_csLockData);
	do 
	{
		for (iterFileData=m_vecFileData.begin(); iterFileData!=m_vecFileData.end();)
		{
			pFileData = (UPDATE_FILEDATA *)(*iterFileData);
			if (pFileData != NULL)
			{
				delete pFileData;
				pFileData = NULL;

				iterFileData = m_vecFileData.erase(iterFileData);
			}
			else
			{
				++iterFileData;
			}
		}

		m_vecFileData.clear();
		bRet = TRUE;
	} while (FALSE);

	LeaveCriticalSection(&m_csLockData);
}

BOOL update_file_data::EnumFileInfo(const char *pszShellPath, std::vector<UPDATE_FILEINFO*> &vecFileData)
{
	BOOL bRet = FALSE;

	char *p = NULL;
	char *ptr = NULL;

	unsigned int uiPos = 0;
	std::string strFindPath;
	std::string strFilePath;

	WIN32_FIND_DATA fd;
	HANDLE hFind = NULL;
	UPDATE_FILEINFO *pFileInfo = NULL;

	do 
	{
		if (pszShellPath == NULL || *pszShellPath == '\0')
		{
			bRet = FALSE;
			break;
		}

		strFindPath  = pszShellPath;
		strFindPath += _T("\\");
		strFindPath += _T("*");

		hFind = FindFirstFile(strFindPath.c_str(), &fd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			bRet = FALSE;
			break;
		}

		do 
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}
			else
			{
				pFileInfo = new UPDATE_FILEINFO;
				if (pFileInfo == NULL)
				{
					continue;
				}
				memset(pFileInfo, 0x0, sizeof(UPDATE_FILEINFO));

				pFileInfo->uiFileSize = (fd.nFileSizeHigh*(MAXDWORD+1)) + fd.nFileSizeLow;
				pFileInfo->uiFileAttrib = fd.dwFileAttributes;
				pFileInfo->time_create = static_cast<__int64>(fd.ftCreationTime.dwHighDateTime)<<32 | fd.ftCreationTime.dwLowDateTime;
				pFileInfo->time_access = static_cast<__int64>(fd.ftLastAccessTime.dwHighDateTime)<<32 | fd.ftLastAccessTime.dwLowDateTime;
				pFileInfo->time_write  = static_cast<__int64>(fd.ftLastWriteTime.dwHighDateTime)<<32 | fd.ftLastWriteTime.dwLowDateTime;

				sprintf(pFileInfo->szFileName, _T("%s"), fd.cFileName);
				sprintf(pFileInfo->szParentPath, _T("%s"), pszShellPath);
				sprintf(pFileInfo->szFilePath, _T("%s\\%s"), pszShellPath, fd.cFileName);

				ptr = strrchr(fd.cFileName, '.');
				if (ptr == NULL)
				{
				}
				else
				{
					uiPos = ptr - fd.cFileName;
					memcpy(pFileInfo->szFileExt, fd.cFileName+uiPos, strlen(fd.cFileName)-uiPos);
				}

				vecFileData.push_back(pFileInfo);
			}
		} while (FindNextFile(hFind, &fd));

		bRet = TRUE;
	} while (FALSE);

	if (hFind != NULL)
	{
		FindClose(hFind);
		hFind = NULL;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
update_file_func::update_file_func()
{
}

update_file_func::~update_file_func()
{
}

update_file_func& update_file_func::Instance()
{
	static update_file_func inst;
	return inst;
}

BOOL update_file_func::SetUpdateFileFunc(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	do 
	{
		switch (emConfigType)
		{
		case CONFIG_ADDFILENAME_TYPE:
			{
				if (!SetAddFileName(emConfigType, pFileData))
				{
					bRet = FALSE;
					break;
				}

				bRet = TRUE;
			}
			break;

		case CONFIG_DATEFILENAME_TYPE:
			{
				if (!SetDateFileName(emConfigType, pFileData))
				{
					bRet = FALSE;
					break;
				}

				bRet = TRUE;
			}
			break;

		case CONFIG_DELFILENAME_TYPE:
			{
				if (!SetDelFileName(emConfigType, pFileData))
				{
					bRet = FALSE;
					break;
				}

				bRet = TRUE;
			}
			break;

		case CONFIG_EXTFILENAME_TYPE:
			{
				if (!SetExtFileName(emConfigType, pFileData))
				{
					bRet = FALSE;
					break;
				}

				bRet = TRUE;
			}
			break;

		case CONFIG_INDEXFILENAME_TYPE:
			{
				if (!SetIndexFileName(emConfigType, pFileData))
				{
					bRet = FALSE;
					break;
				}

				bRet = TRUE;
			}
			break;

		case CONFIG_REPLACEFILENAME_TYPE:
			{
				if (!SetReplaceFileName(emConfigType, pFileData))
				{
					bRet = FALSE;
					break;
				}

				bRet = TRUE;
			}
			break;

		default:
			break;
		}

		if (!bRet)
		{
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetAddFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	int iAddIndex = -1;
	int iIndex = -1;

	unsigned int uiPos = 0;
	unsigned int uiLen = 0;
	unsigned int uiBit = 0;
	unsigned int uiOffset = 0;

	char *p = NULL;
	char *ptr = NULL;
	char *pFileName = NULL;

	char szDataBuffer[MAX_PATH] = {0};
	char szFileOldName[MAX_PATH] = {0};
	char szFileNewName[MAX_PATH] = {0};

	char szOldFilePath[MAX_PATH] = {0};
	char szNewFilePath[MAX_PATH] = {0};

	do 
	{
		if (emConfigType != pFileData->emConfigType || pFileData == NULL)
		{//判断文件名称修改类型
			bRet = FALSE;
			break;
		}

		if (strcmp(pFileData->stcAddFileName.szFileName, _T("")) == 0)
		{//判断规则名称是否为空
			bRet = FALSE;
			break;
		}

		ptr = strrchr(pFileData->stcFileInfo.szFileName, '.');	//strtok
		if (ptr == NULL)
		{
			uiPos = strlen(pFileData->stcFileInfo.szFileName);
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);
			pFileName = pFileData->stcFileInfo.szFileName;
		}
		else
		{
			uiPos = ptr - pFileData->stcFileInfo.szFileName;
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);
			pFileName = szFileOldName;
		}

		uiLen = strlen(pFileName);	//名称长度
		iAddIndex = pFileData->stcAddFileName.iIndex;	//待添加的位置
		
		if (iAddIndex >= 0)
		{//字符正向处理
			uiPos = 0;
			iIndex = 0;
			
			p = pFileName;
			while (*p != '\0')
			{
				if ((*p&0x80) && (*(p+1)&0x80))
				{//汉字
					uiBit = 2;
				}
				else
				{//ASCII码
					uiBit = 1;
				}

				if (iAddIndex != iIndex && *(p+uiBit) != '\0')
				{
					memcpy(szDataBuffer+uiOffset, p, uiBit);
					uiOffset += uiBit;

					p += uiBit;
					uiPos += uiBit;
				}
				else
				{
					memcpy(szDataBuffer+uiOffset, pFileData->stcAddFileName.szFileName, strlen(pFileData->stcAddFileName.szFileName));
					uiOffset += strlen(pFileData->stcAddFileName.szFileName);

					if (iAddIndex == iIndex)
					{
						memcpy(szDataBuffer+uiOffset, p, uiLen-uiPos);
						uiOffset += (uiLen-uiPos);

						p += (uiLen-uiPos);
						uiPos += (uiLen-uiPos);
					}
					else
					{
						memcpy(szDataBuffer+uiOffset, p, uiBit);
						uiOffset += uiBit;

						p += uiBit;
						uiPos += uiBit;
					}

					memcpy(szDataBuffer+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
					uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

					memcpy(szFileNewName, szDataBuffer, uiOffset);
					bRet = TRUE;
					break;
				}

				iIndex++;
			}
		}
		else
		{//字符反向处理
			uiPos = 0;
			iIndex = uiLen;

			p = pFileName + uiLen;
			while (iIndex > 0)
			{
				if (iIndex >= 2)
				{
					if ((*(p-1)&0x80) && (*(p-2)&0x80))
					{//汉字
						uiBit = 2;
					}
					else
					{//ASCII码
						uiBit = 1;
					}
				}
				else
				{//ASCII码
					uiBit = 1;
				}

				p -= uiBit;
				uiPos += uiBit;

				iIndex--;
				if (iAddIndex == iIndex-uiLen)	//正数转负数:~num+1;	负数转正数:~(num-1)
				{
					memcpy(szDataBuffer+uiOffset, pFileName, uiLen-uiPos);
					uiOffset += uiLen-uiPos;

					memcpy(szDataBuffer+uiOffset, pFileData->stcAddFileName.szFileName, strlen(pFileData->stcAddFileName.szFileName));
					uiOffset += strlen(pFileData->stcAddFileName.szFileName);

					if (p != NULL && uiPos != 0)
					{
						memcpy(szDataBuffer+uiOffset, p, uiPos);
						uiOffset += uiPos;
					}

					memcpy(szDataBuffer+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
					uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

					memcpy(szFileNewName, szDataBuffer, uiOffset);
					bRet = TRUE;
					break;
				}
			}
		}

		if (bRet)
		{
			sprintf(szOldFilePath, _T("%s\\%s%s"), pFileData->stcFileInfo.szParentPath, szFileOldName, pFileData->stcFileInfo.szFileExt);
			sprintf(szNewFilePath, _T("%s\\%s"), pFileData->stcFileInfo.szParentPath, szFileNewName);

			rename(szOldFilePath, szNewFilePath);
		}

	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetDateFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	unsigned int uiPos = 0;
	unsigned int uiLen = 0;

	unsigned int uiIndex = 0;
	unsigned int uiOffset = 0;

	char *p = NULL;
	char *ptr = NULL;
	char *pFileName = NULL;

	char szFileOldName[MAX_PATH] = {0};
	char szFileNewName[MAX_PATH] = {0};

	do 
	{
		if (emConfigType != pFileData->emConfigType || pFileData == NULL)
		{//判断文件名称修改类型
			bRet = FALSE;
			break;
		}

		if (strcmp(pFileData->stcDateFileName.szFileName, _T("")) == 0)
		{
			bRet = FALSE;
			break;
		}

		ptr = strrchr(pFileData->stcFileInfo.szFileName, '.');	//strtok
		if (ptr == NULL)
		{
			uiPos = strlen(pFileData->stcFileInfo.szFileName);
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = pFileData->stcFileInfo.szFileName;
		}
		else
		{
			uiPos = ptr - pFileData->stcFileInfo.szFileName;
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = szFileOldName;
		}

		uiLen = strlen(pFileName);	//名称长度 

		memcpy(szFileNewName+uiOffset, pFileData->stcDateFileName.szFileName, strlen(pFileData->stcDateFileName.szFileName));
		uiOffset += strlen(pFileData->stcDateFileName.szFileName);

		memcpy(szFileNewName+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
		uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetDelFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	int iIndex = -1;
	int iSpecIndex = -1;

	int iDelIndex = -1;
	int iDelCount = -1;

	unsigned int uiPos = 0;
	unsigned int uiLen = 0;
	unsigned int uiBit = 0;
	unsigned int uiOffset = 0;

	char *p = NULL;
	char *ptr = NULL;
	char *pFileName = NULL;

	char szDataBuffer[MAX_PATH] = {0};
	char szFileOldName[MAX_PATH] = {0};
	char szFileNewName[MAX_PATH] = {0};

	char szOldFilePath[MAX_PATH] = {0};
	char szNewFilePath[MAX_PATH] = {0};

	do 
	{
		if (emConfigType != pFileData->emConfigType || pFileData == NULL)
		{//判断文件名称修改类型
			bRet = FALSE;
			break;
		}

		//
		ptr = strrchr(pFileData->stcFileInfo.szFileName, '.');	//strtok
		if (ptr == NULL)
		{
			uiPos = strlen(pFileData->stcFileInfo.szFileName);
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);
			pFileName = pFileData->stcFileInfo.szFileName;
		}
		else
		{
			uiPos = ptr - pFileData->stcFileInfo.szFileName;
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);
			pFileName = szFileOldName;
		}

		//
		if (strcmp(pFileData->stcDelFileName.szFileName, _T("")) != 0)
		{
			ptr = strtok(pFileName, pFileData->stcDelFileName.szFileName);
			while(ptr)
			{
				memcpy(szDataBuffer+uiOffset, ptr, strlen(ptr));
				uiOffset += strlen(ptr);

				ptr = strtok(NULL, pFileData->stcDelFileName.szFileName);
			}

			if (strcmp(szDataBuffer, _T("")) == 0)
			{
				bRet = FALSE;
				break;
			}

			memcpy(szDataBuffer+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
			uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

			memcpy(szFileNewName, szDataBuffer, uiOffset);
			bRet = TRUE;
		}
		else
		{
			uiLen = strlen(pFileName);
			iDelIndex = pFileData->stcDelFileName.iIndex;	//待删除的位置
			if (pFileData->stcDelFileName.iCount > uiLen)
			{
				iDelCount = uiLen;
			}
			else
			{
				iDelCount = pFileData->stcDelFileName.iCount;
			}

			if (iDelIndex >= 0)
			{//字符正向处理
				iIndex = 0;
				p = pFileName;

				while (*p != '\0')
				{
					if ((*p&0x80) && (*(p+1)&0x80))
					{//汉字
						uiBit = 2;
					}
					else
					{//ASCII码
						uiBit = 1;
					}

					if (iIndex == 0)
					{
						uiPos = 0;
					}
					else
					{
						uiPos += uiBit;
						p += uiBit;
					}

					if (iIndex == iDelIndex)
					{
						iSpecIndex = iIndex;
					}
					else
					{
						if (iSpecIndex >= 0)
						{
							if (iSpecIndex == 0 || (iIndex == iSpecIndex+iDelCount))
							{
								memcpy(szDataBuffer+uiOffset, pFileName+uiPos, (uiLen-uiPos));
								uiOffset += (uiLen-uiPos);

								memcpy(szDataBuffer+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
								uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

								memcpy(szFileNewName, szDataBuffer, uiOffset);
								bRet = TRUE;
								break;
							}
							else
							{
								if (iIndex >= uiLen)
								{
									bRet = FALSE;
									break;
								}
							}
						}
						else
						{
							memcpy(szDataBuffer+uiOffset, p, uiBit);
							uiOffset += uiBit;
						}
					}

					iIndex++;
				}
			}
			else
			{//字符反向处理
				iIndex = uiLen;
				p = pFileName + uiLen;

				do 
				{
					if ((*p&0x80) && (*(p-1)&0x80))
					{//汉字
						uiBit = 2;
					}
					else
					{//ASCII码
						uiBit = 1;
					}

					if (iIndex == uiLen)
					{
						uiPos = uiLen;
						p -= uiBit;
					}
					else
					{
						uiPos -= uiBit;
						p -= uiBit;
					}

					if ((iIndex-uiLen) == iDelIndex)	
					{//正数转负数:~num+1;	负数转正数:~(num-1)
						iSpecIndex = iIndex;
					}
					else
					{
						if (iSpecIndex >= 0)
						{
							if (iIndex == (iSpecIndex+iDelCount))
							{
								memcpy(szDataBuffer+uiOffset, pFileName+uiPos, (uiLen-uiPos));
								uiOffset += (uiLen-uiPos);

								memcpy(szDataBuffer+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
								uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

								memcpy(szFileNewName, szDataBuffer, uiOffset);
								bRet = TRUE;
								break;
							}
							else
							{
								if (iIndex >= uiLen)
								{
									bRet = FALSE;
									break;
								}
							}
						}
						else
						{
							memcpy(szDataBuffer+uiOffset, p, uiBit);
							uiOffset += uiBit;
						}
					}

					iIndex--;
				} while (*p != '\0');
			}
		}

		if (bRet)
		{
			sprintf(szOldFilePath, _T("%s\\%s%s"), pFileData->stcFileInfo.szParentPath, szFileOldName, pFileData->stcFileInfo.szFileExt);
			sprintf(szNewFilePath, _T("%s\\%s"), pFileData->stcFileInfo.szParentPath, szFileNewName);

			rename(szOldFilePath, szNewFilePath);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetExtFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	unsigned int uiPos = 0;
	unsigned int uiLen = 0;
	unsigned int uiOffset = 0;

	char *p = NULL;
	char *ptr = NULL;
	char *pFileExt = NULL;
	char *pFileName = NULL;

	char szExtName[MAX_PATH] = {0};
	char szFileOldName[MAX_PATH] = {0};
	char szFileNewName[MAX_PATH] = {0};

	do 
	{
		if (emConfigType != pFileData->emConfigType || pFileData == NULL)
		{//判断文件名称修改类型
			bRet = FALSE;
			break;
		}

		ptr = strrchr(pFileData->stcFileInfo.szFileName, '.');	//strtok
		if (ptr == NULL)
		{
			uiPos = strlen(pFileData->stcFileInfo.szFileName);
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = pFileData->stcFileInfo.szFileName;
			pFileExt = NULL;
		}
		else
		{
			uiPos = ptr - pFileData->stcFileInfo.szFileName;
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = szFileOldName;
			pFileExt = ptr;
		}

		uiLen = strlen(pFileName);	//名称长度
		if (strcmp(pFileData->stcExtFileName.szExtName, _T("")) != 0)
		{
			ptr = strrchr(pFileData->stcExtFileName.szExtName, '.');
			if (ptr == NULL)
			{
				sprintf(szExtName, _T(".%s"), pFileData->stcExtFileName.szExtName);
			}
			else
			{
				strcpy(szExtName, pFileData->stcExtFileName.szExtName);
			}

			if (pFileData->stcExtFileName.bIsUppercase)
			{//转换大写
				pFileExt = strupr(szExtName);
			}
			else
			{//转换小写
				pFileExt = strlwr(szExtName);
			}
		}
		else
		{
			if (pFileData->stcExtFileName.bIsUppercase)
			{
				pFileExt = strupr(pFileExt);
			}
			else
			{
				pFileExt = strlwr(pFileExt);
			}
		}

		memcpy(szFileNewName+uiOffset, pFileName, uiLen);
		uiOffset += uiLen;

		memcpy(szFileNewName+uiOffset, pFileExt, strlen(pFileExt));
		uiOffset += strlen(pFileExt);

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetIndexFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	unsigned int uiPos = 0;
	unsigned int uiLen = 0;
	unsigned int uiOffset = 0;

	char *p = NULL;
	char *ptr = NULL;
	char *pFileName = NULL;

	char szFileOldName[MAX_PATH] = {0};
	char szFileNewName[MAX_PATH] = {0};

	do 
	{
		if (emConfigType != pFileData->emConfigType || pFileData == NULL)
		{//判断文件名称修改类型
			bRet = FALSE;
			break;
		}

		ptr = strrchr(pFileData->stcFileInfo.szFileName, '.');	//strtok
		if (ptr == NULL)
		{
			uiPos = strlen(pFileData->stcFileInfo.szFileName);
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = pFileData->stcFileInfo.szFileName;
		}
		else
		{
			uiPos = ptr - pFileData->stcFileInfo.szFileName;
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = szFileOldName;
		}

		uiLen = strlen(pFileName);	//名称长度

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetReplaceFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	unsigned int uiPos = 0;
	unsigned int uiLen = 0;
	unsigned int uiOffset = 0;

	char *p = NULL;
	char *ptr = NULL;
	char *pFileName = NULL;

	char szFileOldName[MAX_PATH] = {0};
	char szFileNewName[MAX_PATH] = {0};

	do 
	{
		if (emConfigType != pFileData->emConfigType || pFileData == NULL)
		{//判断文件名称修改类型
			bRet = FALSE;
			break;
		}

		if (strcmp(pFileData->stcReplaceFileName.szFindName, _T("")) == 0 || strcmp(pFileData->stcReplaceFileName.szFileName, _T("")) == 0)
		{
			bRet = FALSE;
			break;
		}

		ptr = strrchr(pFileData->stcFileInfo.szFileName, '.');	//strtok
		if (ptr == NULL)
		{
			uiPos = strlen(pFileData->stcFileInfo.szFileName);
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = pFileData->stcFileInfo.szFileName;
		}
		else
		{
			uiPos = ptr - pFileData->stcFileInfo.szFileName;
			memcpy(szFileOldName, pFileData->stcFileInfo.szFileName, uiPos);

			pFileName = szFileOldName;
		}

		uiLen = strlen(pFileName);	//名称长度
		ptr = strstr(pFileName, pFileData->stcReplaceFileName.szFindName);
		if (ptr == NULL)
		{
			bRet = FALSE;
			break;
		}

		uiPos = ptr - pFileName;

		memcpy(szFileNewName+uiOffset, pFileName, uiPos);
		uiOffset += uiPos;

		memcpy(szFileNewName+uiOffset, pFileData->stcReplaceFileName.szFileName, strlen(pFileData->stcReplaceFileName.szFileName));
		uiOffset += strlen(pFileData->stcReplaceFileName.szFileName);

		memcpy(szFileNewName+uiOffset, pFileName+uiPos, uiLen-uiPos);
		uiOffset += (uiLen-uiPos);

		memcpy(szFileNewName+uiOffset, pFileData->stcFileInfo.szFileExt, strlen(pFileData->stcFileInfo.szFileExt));
		uiOffset += strlen(pFileData->stcFileInfo.szFileExt);

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//update_file_name
update_file_name::update_file_name()
{
	m_bExit = FALSE;
	
	m_dwProcTimeOver = 500;
	m_dwCloseTimeOver = INFINITE;
	
	m_hThread = NULL;
	m_dwThreadID = 0;

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

update_file_name::~update_file_name()
{
	if (m_hStartEvent != NULL)
	{
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
	}

	if (m_hEndEvent != NULL)
	{
		CloseHandle(m_hEndEvent);
		m_hEndEvent = NULL;
	}
}

DWORD update_file_name::UpdateFileThreadProc(LPVOID lpParam)
{
	update_file_name* pUpdateFileProc = (update_file_name*)lpParam;
	if(pUpdateFileProc != NULL)
	{
		pUpdateFileProc->UpdateFileInfo();
	}

	return 0;
}

void update_file_name::UpdateFileInfo()
{
	BOOL bRet = FALSE;

	UPDATE_FILEDATA *pUpdateFileData;
	std::vector<UPDATE_FILEDATA*> vecFileData;
	std::vector<UPDATE_FILEDATA*>::iterator iterFileData;

	do 
	{
		if (!update_file_data::Instance().GetUpdateFileData(vecFileData))
		{
			bRet = FALSE;
			break;
		}

		for (iterFileData=vecFileData.begin(); iterFileData!=vecFileData.end(); ++iterFileData)
		{
			pUpdateFileData = (UPDATE_FILEDATA *)(*iterFileData);
			if (pUpdateFileData == NULL)
			{
				continue;
			}

			if (!update_file_func::Instance().SetUpdateFileFunc(pUpdateFileData->emConfigType, pUpdateFileData))
			{
				continue;
			}
		}

		bRet = TRUE;
	} while (FALSE);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL update_file_name::CreateUpdateProc()
{
	BOOL bRet = FALSE;

	do
	{
		CloseUpdateProc();
		if (WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
		{
			m_hThread = CreateThread(NULL, 0, UpdateFileThreadProc, (LPVOID)this, 0, &m_dwThreadID);
			if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
			{
				bRet = FALSE;
				break;
			}

			SetEvent(m_hStartEvent);
			ResetEvent(m_hEndEvent);
			
			bRet = TRUE;
		}	
	} while(FALSE);

	return bRet;
}

BOOL update_file_name::CloseUpdateProc()
{
	BOOL bRet = FALSE;

	do 
	{
		if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			bRet = FALSE;
			break;
		}

		m_bExit = TRUE;
		WaitForSingleObject(m_hEndEvent, m_dwCloseTimeOver);

/*
		if (m_mapEnumInfo.size() > 0)
		{
			for (std::map<std::string, UPDATE_FILEINFO*>::iterator mapIter mapIter = m_mapEnumInfo.begin(); mapIter != m_mapEnumInfo.end();)
			{
				if (mapIter->second != NULL)
				{
					delete mapIter->second;
					mapIter->second = NULL;
				}

				mapIter = m_mapEnumInfo.erase(mapIter);
			}

			m_mapEnumInfo.clear();
		}
*/
		if (m_hThread != NULL)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
		
		ResetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}