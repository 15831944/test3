#include "stdafx.h"
#include "update_file_name.h"

update_file_data::update_file_data()
{
}

update_file_data::~update_file_data()
{

}

BOOL update_file_data::SetUpdateFileData(std::vector<UPDATE_FILEINFO*> &vecFileData, UPDATE_FILEDATA_CALLBACK_FUNC pfUpdateFileData)
{
	BOOL bRet = FALSE;
	UPDATE_FILEINFO *pFileInfo = NULL;
	UPDATE_FILEDATA stcUpdateFileData;
	std::vector<UPDATE_FILEINFO*>::iterator iterFileData;

	do 
	{
		if (vecFileData.size() == 0 || pfUpdateFileData == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (!pfUpdateFileData(&stcUpdateFileData))
		{
			bRet = FALSE;
			break;
		}

		m_vecFileData.clear();
		memset(&stcUpdateFileData, 0x0, sizeof(UPDATE_FILEDATA));

		for (iterFileData = vecFileData.begin(); iterFileData!=vecFileData.end(); ++iterFileData)
		{
			pFileInfo = (UPDATE_FILEINFO *)(*iterFileData);
			if (pFileInfo == NULL)
			{
				continue;
			}

			memcpy(&stcUpdateFileData.stcFileInfo, pFileInfo, sizeof(UPDATE_FILEINFO));
			m_vecFileData.push_back(&stcUpdateFileData);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL update_file_data::GetUpdateFileData(std::vector<UPDATE_FILEDATA *> &vecFileData)
{
	BOOL bRet = FALSE;

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

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
//
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
	} while (FALSE);
}

BOOL update_file_name::CreateUpdateProc(update_file_data fileData)
{
	BOOL bRet = FALSE;

	do
	{
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
			m_fileData = fileData;
		}		
	} while(FALSE);

	return bRet;
}

BOOL update_file_name::CloseUpdateProc()
{
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
	ResetEvent(m_hStartEvent);
	return TRUE;
}