#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

#define ERROR_ENUM_FILEINFO					0x0F000001
#define ERROR_ENUM_EVALINFO					0x0F000002

#define UPDATE_FILENAME_SUCCESSUL_RESULT	0x08000001
#define UPDATE_FILENAME_FAILED_RESULT		0x08000002

#define	WM_UPDATEFILENAME_MSG				WM_USER	+ 0x1001

typedef enum tagConfigType
{
	CONFIG_EMPTYTYPE = 0,
	CONFIG_EXTTYPE,
	CONFIG_FILENAMETYPE
}ENUM_CONFIGTYPE;

typedef enum tagEvalType
{
	EVAL_EMPTYTYPE = 0,
	EVAL_ALLFILENAME,
	EVAL_SPECIFYNAME
}ENUM_EVALTYPE;

typedef struct tagUpdateResult
{
	unsigned long	ulError;
	unsigned long	ulResult;
}DATA_UPDATERESULT;

typedef struct tagEnumFileInfo
{
	DWORD		dwFileSize;
	DWORD		dwFileAttrib;
	__time64_t	time_create;
	__time64_t	time_access;
	__time64_t	time_write;
	char		szFileName[MAX_PATH];
	char		szParentPath[MAX_PATH];
	char		szFilePath[MAX_PATH];
	char		szFileExt[MAX_PATH];
}ENUM_FILEINFO;

typedef struct tagItemData
{
	ENUM_EVALTYPE	hEvalType;
	char			szItemName[MAX_PATH];
}EVAL_ITEMDATA;

typedef struct tagEvalFileInfo
{
	ENUM_CONFIGTYPE	hConfigType;
	ENUM_EVALTYPE	hEvalType;
	std::vector<std::string> vecString;

	tagEvalFileInfo::tagEvalFileInfo()
	{
		hConfigType = CONFIG_EMPTYTYPE;
		hEvalType   = EVAL_EMPTYTYPE;

		vecString.clear();
	}
}EVAL_FILEINFO;

class update_file_name
{
public:
	update_file_name();
	~update_file_name();
	
public:
	BOOL							CreateUpdateProc(HWND hWnd, const char* pszShellPath, const char* pszFindName, const char* pszSubName, ENUM_EVALTYPE hEvalType);
	BOOL							CloseUpdateProc();
	
	static 	update_file_name&		Instance();
	
protected:
	static  DWORD WINAPI			UpdateFileThreadProc(LPVOID lpParam);
	
protected:
	void							UpdateFileInfo();
	BOOL							EnumFileInfo();
	void							SetUpdateResult(BOOL bRet);

	BOOL							GetFileTitle(const char *pszFileName, char *pszTitle, char *pszExt);
	BOOL							GetEvalResult(EVAL_FILEINFO* pEvalTag);

	BOOL							SetFileExtInfo(EVAL_FILEINFO* pEvalTag);
	BOOL							SetFileNameInfo(EVAL_FILEINFO* pEvalTag);

	BOOL							SetAllFileName(const char* pszFilePath, const char* pszSrcName, const char* pszFindName, const char* pszSpecName, const char* pszFileExt, unsigned long ulIndex);
	BOOL							SetSpecifyName(const char* pszFilePath, const char* pszSrcName, const char* pszFindName, const char* pszSpecName, const char* pszFileExt);

protected:
	HANDLE							m_hThread;

	HANDLE							m_hStartEvent;
	HANDLE							m_hEndEvent;
	
	HWND							m_hWnd;
	ENUM_EVALTYPE					m_hEvalType;

private:
	BOOL							m_bExit;
	
	DWORD							m_dwThreadID;
	DWORD							m_dwWaitTime;
	DWORD							m_dwError;
	
	std::string						m_strShellPath;
	std::string						m_strFindName;
	std::string						m_strSubName;

	std::map<std::string, ENUM_FILEINFO*>	m_mapEnumInfo;
};

#endif