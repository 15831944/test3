#include "stdafx.h"
#include "update_file_name.h"

update_file_name::update_file_name()
{
	m_bExit = FALSE;
	
	m_dwThreadID = 0;
	m_dwWaitTime = 0;
	
	m_strShellPath = _T("");
	m_strNewFileName = _T("");
	m_strSpanName  = _T("");
	
	m_hThread = NULL;
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

update_file_name::~update_file_name()
{
	CloseUpdateProc();
}

update_file_name& update_file_name::Instance()
{
	static update_file_name inst;
	return inst;
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

BOOL update_file_name::CreateUpdateProc(const char* pszShellPath, const char* pszNewFileName, const char* pszSpanName)
{
	BOOL bRet = FALSE;
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);
		
		m_strShellPath = pszShellPath;
		m_strNewFileName = pszNewFileName;
		m_strSpanName  = pszSpanName;
		
		m_hThread = CreateThread(NULL, 0, UpdateFileThreadProc, (LPVOID)this, 0, &m_dwThreadID);
		if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}

		bRet = TRUE;
		m_bExit = FALSE;
		
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
	return bRet;
}

BOOL update_file_name::CloseUpdateProc()
{
	m_bExit = TRUE;
	WaitForSingleObject(m_hEndEvent, INFINITE);
	
	return TRUE;
}

void update_file_name::UpdateFileInfo()
{
}

BOOL update_file_name::EnumFileInfo()
{
	BOOL bRet = FALSE;
	
	long file = 0;

	char szFindPath[MAX_PATH+1]  = {0};
	
	struct _stat	s_file;
	_finddata_t		c_file;
	
	if(m_strShellPath == _T("") || m_strShellPath.size() == 0)
	{
		return FALSE;
	}
	
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
				
		}
	} while (_tfindnext(file, &c_file) == 0);
}