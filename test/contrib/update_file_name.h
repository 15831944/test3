#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

typedef enum {
	CONFIG_EMPTYTYPE = 0,						//������
	CONFIG_ADDFILENAME_TYPE,					//�ļ�������ַ�����Ϣ
	CONFIG_DATEFILENAME_TYPE,					//�ļ����������ڸ�ʽ�޸�
	CONFIG_DELFILENAME_TYPE,					//�ļ���ɾ��ָ���ַ�����Ϣ
	CONFIG_EXTFILENAME_TYPE,					//�ļ����޸���չ����
	CONFIG_INDEXFILENAME_TYPE,					//�ļ�������ָ�������޸���Ϣ
	CONFIG_REPLACEFILENAME_TYPE,				//�ļ����滻Ϊָ���ַ�����Ϣ
}UPDATE_CONFIGTYPE;

typedef enum {
	STATE_EMPTYTYPE = 0,						//������
	STATE_UPDATESUCCED_TYPE,					//���³ɹ�	
	STATE_UPDATEFAILED_TYPE,					//����ʧ��
	STATE_UPDATEINPUTE_TYPE,					//��������
	STATE_UPDATEOUTPUT_TYPE,					//�������
	STATE_UPDATEMODIFY_TYPE,					//�����޸�
}UPDATE_STATETYPE;

typedef struct {
	unsigned int		uiFileSize;				//�ļ���С
	unsigned int		uiFileAttrib;			//�ļ�����
	__time64_t			time_create;			//�ļ�����ʱ��
	__time64_t			time_access;			//�ļ�����ʱ��
	__time64_t			time_write;				//�ļ�д��ʱ��
	char				szFileName[MAX_PATH];	//�ļ�����
	char				szParentPath[MAX_PATH];	//�ļ���·��
	char				szFilePath[MAX_PATH];	//�ļ�·��
	char				szFileExt[MAX_PATH];	//�ļ���չ��
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