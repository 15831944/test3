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

	do 
	{
		if (vecFileData.size() == 0 || pfUpdateFileData == NULL)
		{
			bRet = FALSE;
			break;
		}


	} while (FALSE);
	return bRet;
}

BOOL update_file_data::GetUpdateFileData(std::vector<UPDATE_FILEDATA *> &vecFileData)
{
	BOOL bRet = FALSE;
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
//
update_file_name::update_file_name()
{
	m_bExit = FALSE;
	
	m_dwThreadID = 0;
	m_dwProcTimeOver = 0;
	m_dwCloseTimeOver = 0;
	
	m_hThread = NULL;
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
			SetEvent(m_hStartEvent);
			ResetEvent(m_hEndEvent);

			m_hThread = CreateThread(NULL, 0, UpdateFileThreadProc, (LPVOID)this, 0, &m_dwThreadID);
			if (m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
			{
				bRet = FALSE;
				break;
			}
		}

		bRet = TRUE;
	} while(FALSE);

	if (!bRet)
	{
		ResetEvent(m_hStartEvent);
	}

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