#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

typedef struct tagEnumFileInfo
{
	BOOL		bFileType;
	DWORD		dwFileSize;
	DWORD		dwFileAttrib;
	__time64_t	time_create;
	__time64_t	time_access;
	__time64_t	time_write;
	char		szFileName[MAX_PATH];
	char		szFilePath[MAX_PATH];
	char		szFileExt[MAX_PATH];
}ENUM_FILEINFO;

class update_file_name
{
public:
	update_file_name();
	~update_file_name();
	
public:
	BOOL							CreateUpdateProc(const char* pszShellPath, const char* pszNewFileName, const char* pszSpanName);
	BOOL							CloseUpdateProc();
	
	static 	update_file_name&		Instance();
	
protected:
	static  DWORD WINAPI			UpdateFileThreadProc(LPVOID lpParam);
	
protected:
	void							UpdateFileInfo();
	BOOL							EnumFileInfo();
	
protected:
	HANDLE							m_hThread;

	HANDLE							m_hStartEvent;
	HANDLE							m_hEndEvent;
	
private:
	BOOL							m_bExit;
	
	DWORD							m_dwThreadID;
	DWORD							m_dwWaitTime;
	
	std::string						m_strShellPath;
	std::string						m_strNewFileName;
	std::string						m_strSpanName;

	std::map<std::string, ENUM_FILEINFO*>	m_mapEnumInfo;
};

#endif