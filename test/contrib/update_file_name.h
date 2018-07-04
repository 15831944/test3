#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

typedef enum{
	CONFIG_EMPTYTYPE	= 0,
	CONFIG_EXTNAMETYPE,
	CONFIG_FILENAMETYPE
}UPDATE_CONFIGTYPE;

typedef enum{
	EVAL_EMPTYTYPE		= 0,
	EVAL_ALLFILENAME,
	EVAL_SPECIFYNAME,
	EVAL_SPECIFYNUMINDEX
}UPDATE_EVALTYPE;

typedef enum {
	STATE_EMPTYTYPE		= 0,
	STATE_SUCCEDTYPE,
	STATE_FAILEDTYPE,
	STATE_INPUTETYPE,
	STATE_OUTPUTTYPE,
	STATE_MODIFYTYPE,
}UPDATE_STATETYPE;

typedef struct{
	unsigned int			uiFileSize;
	unsigned int			uiFileAttrib;
	__time64_t				time_create;
	__time64_t				time_access;
	__time64_t				time_write;
	char					szFileName[MAX_PATH];
	char					szParentPath[MAX_PATH];
	char					szFilePath[MAX_PATH];
	char					szFileExt[MAX_PATH];
}UPDATE_FILEINFO;

typedef struct {
	char					szFilePath[MAX_PATH];
	char					szFindName[MAX_PATH];
	char					szFileSubName[MAX_PATH];
	UPDATE_EVALTYPE			emEvalType;
	UPDATE_STATETYPE		emStateType;
	UPDATE_CONFIGTYPE		emConfigType;
}UPDATE_FILEDATA;

typedef struct {
	UPDATE_EVALTYPE			emEvalType;
	char					szItemName[MAX_PATH];
}EVAL_ITEMDATA;

typedef struct tagEvalFileInfo
{
	UPDATE_CONFIGTYPE		emConfigType;
	UPDATE_EVALTYPE			emEvalType;
	std::vector<std::string> vecString;

	tagEvalFileInfo::tagEvalFileInfo()
	{
		emConfigType = CONFIG_EMPTYTYPE;
		emEvalType   = EVAL_EMPTYTYPE;
		vecString.clear();
	}
}EVAL_FILEINFO;

typedef BOOL(*UPDATE_FILEDATA_CALLBACK_FUNC)(UPDATE_FILEDATA *pFileData);
//////////////////////////////////////////////////////////////////////////
//
using namespace std;
class update_file_name
{
public:
	update_file_name();
	~update_file_name();
	
public:
	BOOL							CreateUpdateProc(UPDATE_FILEDATA_CALLBACK_FUNC pfFileData);
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
	BOOL							SetNumIndexName(const char* pszFilePath, const char* pszSrcName, const char* pszFindName, const char* pszSpecName, const char* pszFileExt, unsigned long ulIndex);

protected:
	HANDLE							m_hThread;

	HANDLE							m_hStartEvent;
	HANDLE							m_hEndEvent;

private:
	BOOL							m_bExit;

	DWORD							m_dwThreadID;
	DWORD							m_dwProcTimeOver;
	DWORD							m_dwCloseTimeOver;

	UPDATE_FILEDATA_CALLBACK_FUNC	m_pfFileData;
	std::map<std::string, ENUM_FILEINFO*>	m_mapEnumInfo;
};

#endif