#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

typedef enum {
	CONFIG_EMPTYTYPE = 0,									//������
	CONFIG_ADDFILENAME_TYPE,								//�ļ�������ַ�����Ϣ
	CONFIG_DATEFILENAME_TYPE,								//�ļ����������ڸ�ʽ�޸�
	CONFIG_DELFILENAME_TYPE,								//�ļ���ɾ��ָ���ַ�����Ϣ
	CONFIG_EXTFILENAME_TYPE,								//�ļ����޸���չ����
	CONFIG_INDEXFILENAME_TYPE,								//�ļ�������ָ�������޸���Ϣ
	CONFIG_REPLACEFILENAME_TYPE,							//�ļ����滻Ϊָ���ַ�����Ϣ
}UPDATE_CONFIGTYPE;

typedef enum {
	STATE_EMPTYTYPE = 0,									//������
	STATE_UPDATESUCCED_TYPE,								//���³ɹ�	
	STATE_UPDATEFAILED_TYPE,								//����ʧ��
	STATE_UPDATEINPUTE_TYPE,								//��������
	STATE_UPDATEOUTPUT_TYPE,								//�������
	STATE_UPDATEMODIFY_TYPE,								//�����޸�
}UPDATE_STATETYPE;

typedef enum {
	DATE_EMPTYTYPE = 0,
	DATE_CREATETIME_TYPE,									//����ʱ��
	DATE_MODIFYTIME_TYPE,									//�޸�ʱ��
	DATE_ACCESSTIME_TYPE,									//����ʱ��
}UPDATE_DATETYPE;

typedef struct {
	unsigned int				uiFileSize;					//�ļ���С
	unsigned int				uiFileAttrib;				//�ļ�����
	__time64_t					time_create;				//�ļ�����ʱ��
	__time64_t					time_access;				//�ļ�����ʱ��
	__time64_t					time_write;					//�ļ�д��ʱ��
	char						szFileName[MAX_PATH];		//�ļ�����
	char						szParentPath[MAX_PATH];		//�ļ���·��
	char						szFilePath[MAX_PATH];		//�ļ�·��
	char						szFileExt[MAX_PATH];		//�ļ���չ��
}UPDATE_FILEINFO;

typedef struct {
	int							iIndex;						//�ļ�����ӵ�����, iIndex<0Ϊ��ĩβ��ʼ���;
	char						szFileName[MAX_PATH];		//�ļ�������ӵ��ַ���;
}UPDATE_ADDFILENAME;

typedef struct {
	char						szFileName[MAX_PATH];		//���޸�Ϊָ�����ڸ�ʽ���ļ���;
}UPDATE_DATEFILENAME;

typedef struct {
	int							iIndex;						//ɾ���ַ���ʼλ��, iIndex<0Ϊ��ĩβ��ʼɾ��;
	int							iCount;						//ɾ���ַ��ĸ���;
	char						szFileName[MAX_PATH];		//�ļ����д�ɾ�����ַ���;
}UPDATE_DELFILENAME;

typedef struct {
	BOOL						bIsUppercase;				//�Ƿ��д;
	char						szExtName[MAX_PATH];		//���滻���ļ���չ��;
}UPDATE_EXTFILENAME;

typedef struct {
	BOOL						bAutoAlign;					//�Զ�����;
	int							iBit;						//���λ��;
	int							iStartIndex;				//��ʼ���;
	int							iEndIndex;					//�������;
	char						szIndexFormat[MAX_PATH];	//��Ÿ�ʽ;
}UPDATE_INDEXFILENAME;

typedef struct {
	char						szFindName[MAX_PATH];		//ԭ�ļ����ַ�;
	char						szFileName[MAX_PATH];		//���滻���ļ����ַ�;
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