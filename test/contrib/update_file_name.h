#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

typedef enum {
	CONFIG_EMPTYTYPE = 0,						//空类型
	CONFIG_ADDFILENAME_TYPE,					//文件名添加字符串信息
	CONFIG_DATEFILENAME_TYPE,					//文件名按照日期格式修改
	CONFIG_DELFILENAME_TYPE,					//文件名删除指定字符串信息
	CONFIG_EXTFILENAME_TYPE,					//文件名修改扩展名称
	CONFIG_INDEXFILENAME_TYPE,					//文件名按照指定索引修改信息
	CONFIG_REPLACEFILENAME_TYPE,				//文件名替换为指定字符串信息
}UPDATE_CONFIGTYPE;

typedef enum {
	STATE_EMPTYTYPE = 0,						//空类型
	STATE_UPDATESUCCED_TYPE,					//更新成功	
	STATE_UPDATEFAILED_TYPE,					//更新失败
	STATE_UPDATEINPUTE_TYPE,					//更新输入
	STATE_UPDATEOUTPUT_TYPE,					//更新输出
	STATE_UPDATEMODIFY_TYPE,					//更新修改
}UPDATE_STATETYPE;

typedef struct {
	unsigned int		uiFileSize;				//文件大小
	unsigned int		uiFileAttrib;			//文件属性
	__time64_t			time_create;			//文件创建时间
	__time64_t			time_access;			//文件访问时间
	__time64_t			time_write;				//文件写入时间
	char				szFileName[MAX_PATH];	//文件名称
	char				szParentPath[MAX_PATH];	//文件父路径
	char				szFilePath[MAX_PATH];	//文件路径
	char				szFileExt[MAX_PATH];	//文件扩展名
}UPDATE_FILEINFO;

typedef struct {
}UPDATE_ADDFILENAME;

typedef struct {
}UPDATE_DATEFILENAME;

typedef struct {
}UPDATE_DELFILENAME;

typedef struct {
}UPDATE_EXTFILENAME;

typedef struct {
}UPDATE_INDEXFILENAME;

typedef struct {
}UPDATE_REPLACEFILENAME;

typedef struct {
	UPDATE_CONFIGTYPE		emConfigType;
	UPDATE_STATETYPE		emUpdateStatus;
	UPDATE_FILEINFO			stcFileInfo;
	union {
		UPDATE_ADDFILENAME	stcAddFileName;
		UPDATE_DATEFILENAME	stcDateFileName;
		UPDATE_DELFILENAME	stcDelFileName;
		UPDATE_EXTFILENAME	stcExtFileName;
		UPDATE_INDEXFILENAME stcIndexFileName;
		UPDATE_REPLACEFILENAME stcReplaceFileName;
	};
}UPDATE_FILEDATA;

typedef BOOL(*UPDATE_FILEDATA_CALLBACK_FUNC)(UPDATE_FILEDATA *pUpdateFileData);

using namespace std;
class update_file_data
{
public:
	update_file_data();
	~update_file_data();

public:
	BOOL				SetUpdateFileData(std::vector<UPDATE_FILEINFO*> &vecFileData, UPDATE_FILEDATA_CALLBACK_FUNC pfUpdateFileData);
	BOOL				GetUpdateFileData(std::vector<UPDATE_FILEDATA*> &vecFileData);

private:
	std::vector<UPDATE_FILEDATA*> m_vecFileData;
};

class update_file_name
{
public:
	update_file_name();
	~update_file_name();
	
public:
	BOOL				CreateUpdateProc(update_file_data fileData);
	BOOL				CloseUpdateProc();
	
protected:
	static  DWORD WINAPI UpdateFileThreadProc(LPVOID lpParam);
	
protected:
	void				UpdateFileInfo();

protected:
	HANDLE				m_hThread;

	HANDLE				m_hStartEvent;
	HANDLE				m_hEndEvent;

private:
	BOOL				m_bExit;
	update_file_data	m_fileData;

	DWORD				m_dwThreadID;
	DWORD				m_dwProcTimeOver;
	DWORD				m_dwCloseTimeOver;
};

#endif