#ifndef __UKEYVERIFY_THREAD_H__
#define __UKEYVERIFY_THREAD_H__

#include "UKeyVerify.h"

typedef enum {
	CK_UKEMPTYTYPE = 0,							//������
	CK_UKNORMALTYPE,							//��ͨUKey�豸
	CK_UKFINGERTYPE,							//ָ��Key�豸
}CK_UKEYDEVICETYPE;

typedef enum {
	CK_UKEYEMPTYTYPE = 0,						//������
	CK_UKEYINSERTTYPE,							//UKEY�豸�������
	CK_UKEYREMOVETYPE,							//UKEY�豸�γ�����
}CK_UKEYSTATETYPE;

typedef enum {
	CK_OPERATEEMPTYTYPE = 0,					//������	
	CK_OPERATEENUMSLOTTYPE,						//ö���豸����;
	CK_OPERATEVERIFYTYPE,						//�豸����У�����;
	CK_OPERATEWRITETYPE,						//�豸д�����;
	CK_OPERATEREADTYPE,							//�豸��������;
}CK_UKEYOPERATETYPE;

typedef struct {
	BOOL				bFlags;					//�����ɹ�,ʧ��;
	BOOL				bExist;					//UKey�豸�Ƿ����;
	CK_UKEYDEVICETYPE	emUKeyType;				//UKey�豸����;
	CK_UKEYSTATETYPE	emUKeyState;			//UKey�豸״̬;
	CK_UKEYOPERATETYPE	emUKeyOperateType;		//UKey�豸��������;
	CK_ULONG			ulSlotId;				//UKey�豸ID;
	CK_ULONG			ulRemainCount;			//UKey�豸PIN����ʣ�����;
	CK_BYTE*			pFingerData;			//UKey�豸ָ������;
	CK_ULONG			ulFingerDataLen;		//UKey�豸ָ�����ݳ���;
}CK_UKEYPROCINFO;

typedef struct {
	CK_UKEYOPERATETYPE	emUKeyOperateType;
	char				szUserNum[64];			//���ȵ�¼����;
	char				szUserPasswd[64];		//���ȵ�¼����;
	char				szUserPIN[64];			//UKey�豸PIN��;
	char				szPromptText[64];		//�豸��ʾ��Ϣ;
}CK_UKEYUSERDATA;

typedef BOOL (*CK_UKDATA_CALLBACK_FUNC)(const CK_UKEYPROCINFO *pUKeyInfo, CK_UKEYUSERDATA *pUKeyUserData, LPVOID lpParam);

class CUKeyVerifyThread
{
public:
	CUKeyVerifyThread();
	~CUKeyVerifyThread();

public:
	BOOL					CreateVerifyProc(BOOL bFlags, CK_UKDATA_CALLBACK_FUNC pfUKDataFunc, LPVOID lpParam);
	BOOL					CloseVerifyProc();

	BOOL					SetThreadProcState(BOOL bFlags);
	void					SetThreadProcTime(DWORD dwProcTime, DWORD dwCloseTime);

	void					SetUserData(LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd);
	void					SetUserPIN(LPCTSTR lpszUserPIN);
	
protected:
	static DWORD WINAPI		UKeyVerifyThreadProc(LPVOID lpParam);
	
protected:
	void					UKeyVerifyInfo();
	
	BOOL					GetUKeySlotInfo(CK_LONG ulSlotIndex, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData, CK_ULONG &ulSlotId, CK_ULONG &ulFlags);
	BOOL					UKeySlotVerify(CK_ULONG ulSlotId, CK_ULONG ulFlags, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData);
	void					ClearUKeyInfo();

	BOOL					SetObjectData(LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData);
	BOOL					GetObjectData(char *pszUserNum, char *pszUserPasswd, CK_UKEYPROCINFO *pstcUKeyProc, CK_UKEYUSERDATA *pstcUKeyUserData);
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
	CUKeyVerify				m_UKeyVerify;
	
private:
	BOOL					m_bExit;
	BOOL					m_bFlags;

	DWORD					m_dwThreadID;
	DWORD					m_dwProcTimeOver;
	DWORD					m_dwCloseTimeOver;
	
	CString					m_strUserNum;
	CString					m_strUserPasswd;
	CString					m_strUserPIN;

	LPVOID					m_lpParam;
	CK_UKEYPROCINFO			m_stcCurUKeyData;
	CK_UKDATA_CALLBACK_FUNC m_pfUKDataFunc;
};

#endif