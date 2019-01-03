#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

typedef enum {
	CONFIG_EMPTYTYPE = 0,									//空类型
	CONFIG_ADDFILENAME_TYPE,								//文件名添加字符串信息
	CONFIG_DATEFILENAME_TYPE,								//文件名按照日期格式修改
	CONFIG_DELFILENAME_TYPE,								//文件名删除指定字符串信息
	CONFIG_EXTFILENAME_TYPE,								//文件名修改扩展名称
	CONFIG_INDEXFILENAME_TYPE,								//文件名按照指定索引修改信息
	CONFIG_REPLACEFILENAME_TYPE,							//文件名替换为指定字符串信息
}UPDATE_CONFIGTYPE;

typedef enum {
	STATE_EMPTYTYPE = 0,									//空类型
	STATE_UPDATESUCCED_TYPE,								//更新成功	
	STATE_UPDATEFAILED_TYPE,								//更新失败
	STATE_UPDATEINPUTE_TYPE,								//更新输入
	STATE_UPDATEOUTPUT_TYPE,								//更新输出
	STATE_UPDATEMODIFY_TYPE,								//更新修改
}UPDATE_STATETYPE;

typedef enum {
	DATE_EMPTYTYPE = 0,
	DATE_CREATETIME_TYPE,									//创建时间
	DATE_MODIFYTIME_TYPE,									//修改时间
	DATE_ACCESSTIME_TYPE,									//访问时间
}UPDATE_DATETYPE;

typedef struct {
	unsigned int				uiFileSize;					//文件大小
	unsigned int				uiFileAttrib;				//文件属性
	__time64_t					time_create;				//文件创建时间
	__time64_t					time_access;				//文件访问时间
	__time64_t					time_write;					//文件写入时间
	char						szFileName[MAX_PATH];		//文件名称
	char						szParentPath[MAX_PATH];		//文件父路径
	char						szFilePath[MAX_PATH];		//文件路径
	char						szFileExt[MAX_PATH];		//文件扩展名
}UPDATE_FILEINFO;

typedef struct {
	int							iIndex;						//文件名添加的索引, iIndex<0为从末尾开始添加;
	char						szFileName[MAX_PATH];		//文件名新添加的字符名;
}UPDATE_ADDFILENAME;

typedef struct {
	char						szFileName[MAX_PATH];		//待修改为指定日期格式的文件名;
}UPDATE_DATEFILENAME;

typedef struct {
	int							iIndex;						//删除字符开始位置, iIndex<0为从末尾开始删除;
	int							iCount;						//删除字符的个数;
	char						szFileName[MAX_PATH];		//文件名中待删除的字符名;
}UPDATE_DELFILENAME;

typedef struct {
	BOOL						bIsUppercase;				//是否大写;
	char						szExtName[MAX_PATH];		//待替换的文件扩展名;
}UPDATE_EXTFILENAME;

typedef struct {
	BOOL						bAutoAlign;					//自动对齐;
	int							iBit;						//序号位数;
	int							iStartIndex;				//开始序号;
	int							iEndIndex;					//结束序号;
	char						szIndexFormat[MAX_PATH];	//序号格式;
}UPDATE_INDEXFILENAME;

typedef struct {
	char						szFindName[MAX_PATH];		//原文件名字符;
	char						szFileName[MAX_PATH];		//待替换的文件名字符;
}UPDATE_REPLACEFILENAME;

typedef BOOL(*UPDATE_FILEDATA_CALLBACK_FUNC)(void *pUpdateData, void *pParentObject);
typedef struct {
	UPDATE_CONFIGTYPE			emConfigType;
	UPDATE_STATETYPE			emUpdateStatus;
	UPDATE_FILEINFO				stcFileInfo;
	union {
		UPDATE_ADDFILENAME		stcAddFileName;
		UPDATE_DATEFILENAME		stcDateFileName;
		UPDATE_DELFILENAME		stcDelFileName;
		UPDATE_EXTFILENAME		stcExtFileName;
		UPDATE_INDEXFILENAME	stcIndexFileName;
		UPDATE_REPLACEFILENAME	stcReplaceFileName;
	};
	UPDATE_FILEDATA_CALLBACK_FUNC pfUpdateFunc;
}UPDATE_FILEDATA;

using namespace std;
class update_file_data
{
public:
	update_file_data();
	~update_file_data();

public:
	static update_file_data& Instance();

public:
	BOOL				SetUpdateFileData(std::vector<UPDATE_FILEINFO*> &vecFileData, UPDATE_FILEDATA_CALLBACK_FUNC pfUpdateFileData);
	BOOL				GetUpdateFileData(std::vector<UPDATE_FILEDATA*> &vecFileData);

	void				ClearFileData();

private:
	CRITICAL_SECTION	m_csLockData;
	std::vector<UPDATE_FILEDATA*> m_vecFileData;
};

class update_file_func
{
public:
	update_file_func();
	~update_file_func();

public:
	static update_file_func& Instance();

public:
	BOOL				SetUpdateFileFunc(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData);

protected:
	BOOL				SetAddFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData);
	BOOL				SetDateFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData);
	BOOL				SetDelFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData);
	BOOL				SetExtFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData);
	BOOL				SetIndexFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData);
	BOOL				SetReplaceFileName(UPDATE_CONFIGTYPE emConfigType, UPDATE_FILEDATA *pFileData);
};

class update_file_name
{
public:
	update_file_name();
	~update_file_name();
	
public:
	BOOL				CreateUpdateProc();
	BOOL				CloseUpdateProc();
	
protected:
	static  DWORD WINAPI UpdateFileThreadProc(LPVOID lpParam);
	
protected:
	void				UpdateFileInfo();
	BOOL				UpdateFileName();

protected:
	HANDLE				m_hThread;

	HANDLE				m_hStartEvent;
	HANDLE				m_hEndEvent;

private:
	BOOL				m_bExit;
	
	DWORD				m_dwThreadID;
	DWORD				m_dwProcTimeOver;
	DWORD				m_dwCloseTimeOver;
};

#endif