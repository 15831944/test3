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
	DeleteCriticalSection(&m_csLockData);
}

update_file_data& update_file_data::Instance()
{
	static update_file_data inst;
	return inst;
}

BOOL update_file_data::SetUpdateFileData(std::vector<UPDATE_FILEINFO*> &vecFileData, UPDATE_FILEDATA_CALLBACK_FUNC pfUpdateFileData)
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
		if (!pfUpdateFileData(&stcUpdateFileData))
		{
			bRet = FALSE;
			break;
		}

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
	BOOL bAscii = FALSE;

	unsigned int uiPos = 0;
	unsigned int uiLen = 0;

	unsigned int uiIndex = 0;
	unsigned int uiOffset = 0;

	char *pFileName = NULL;
	char szFileName[MAX_PATH] = {0};
	UPDATE_ADDFILENAME stcAddFileName = {0};

	do 
	{
		if (emConfigType == CONFIG_EMPTYTYPE || pFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (emConfigType != pFileData->emConfigType)
		{
			bRet = FALSE;
			break;
		}

		if (strcmp(pFileData->stcAddFileName.szFileName, _T("")) == 0)
		{
			bRet = FALSE;
			break;
		}

		memcpy(&stcAddFileName, &pFileData->stcAddFileName, sizeof(UPDATE_ADDFILENAME));

		pFileName = pFileData->stcFileInfo.szFileName;
		uiLen = strlen(pFileData->stcFileInfo.szFileName);

		if (stcAddFileName.iPos > -1)
		{
			while (*pFileName != '\0')
			{
				uiIndex ++;
				if ((*pFileName&0x80) && (*pFileName&0x80))
				{
					bAscii = FALSE;
					uiPos += 2;
				}
				else
				{
					bAscii = TRUE;
					uiPos += 1;
				}

				if (uiIndex == stcAddFileName.iPos)
				{
					memcpy(szFileName+uiOffset, pFileData->stcFileInfo.szFileName, uiPos);
					uiOffset += uiPos;

					memcpy(szFileName+uiOffset, stcAddFileName.szFileName, strlen(stcAddFileName.szFileName));
					uiOffset += strlen(stcAddFileName.szFileName);
					
					memcpy(szFileName+uiOffset, pFileData->stcFileInfo.szFileName+uiPos, uiLen-uiPos);
					uiOffset += (uiLen-uiPos);
					break;
				}

				pFileName+=uiPos;
			}
		}
		else
		{
			uiPos = (strlen(pFileData->stcFileInfo.szFileName) - strlen(pFileData->stcFileInfo.szFileExt) - 1);
			pFileName += uiPos;

			while (*pFileName != '\0')
			{
				if ((*pFileName&0x80) && (*pFileName&0x80))
				{
					pFileName -= 2;
					uiPos -= 2;
				}
				else
				{
					pFileName--;
					uiPos--;
				}
			}
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetDateFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;
	
	UPDATE_DATEFILENAME stcDateFileName = {0};

	do 
	{
		if (emConfigType == CONFIG_EMPTYTYPE || pFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (emConfigType != pFileData->emConfigType)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetDelFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	UPDATE_DELFILENAME stcDelFileName  = {0};

	do 
	{
		if (emConfigType == CONFIG_EMPTYTYPE || pFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (emConfigType != pFileData->emConfigType)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetExtFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	UPDATE_EXTFILENAME stcExtFileName = {0};

	do 
	{
		if (emConfigType == CONFIG_EMPTYTYPE || pFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (emConfigType != pFileData->emConfigType)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetIndexFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	UPDATE_INDEXFILENAME stcIndexFileName = {0};

	do 
	{
		if (emConfigType == CONFIG_EMPTYTYPE || pFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (emConfigType != pFileData->emConfigType)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_func::SetReplaceFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData)
{
	BOOL bRet = FALSE;

	UPDATE_REPLACEFILENAME stcReplaceFileName = {0};

	do 
	{
		if (emConfigType == CONFIG_EMPTYTYPE || pFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (emConfigType != pFileData->emConfigType)
		{
			bRet = FALSE;
			break;
		}

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

	do 
	{
		if (!UpdateFileName())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);
}

BOOL update_file_name::UpdateFileName()
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

	return bRet;
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