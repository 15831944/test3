#ifndef __UKEYVERIFY_THREAD_H__
#define __UKEYVERIFY_THREAD_H__

#include "../Inc/algorithm/UKeyVerify.h"

typedef enum {
	CK_UKEMPTYTYPE = 0,
	CK_UKNORMALTYPE,
	CK_UKFINGERTYPE,
}CK_UKTYPE;

typedef enum {
	CK_OPERATEEMPTYTYPE = 0,
	CK_OPERATEENUMSLOTTYPE,
	CK_OPERATEVERIFYTYPE,
	CK_OPERATEWRITETYPE,
	CK_OPERATEREADTYPE,
}CK_OPERATETYPE;

typedef struct {
	BOOL			bFlags;				//
	BOOL			bExist;				//UKey�豸�Ƿ����;
	CK_UKTYPE		emUKType;			//UKey�豸����;
	CK_OPERATETYPE	emOperateType;		//UKey�豸��������;
	CK_ULONG		ulSlotId;			//UKey�豸ID;
	CK_ULONG		ulRemainCount;		//UKey�豸PIN����ʣ�����;
	CK_BYTE*		pFingerData;		//UKey�豸ָ������;
	CK_ULONG		ulFingerDataLen;	//UKey�豸ָ�����ݳ���;
	char			szUserNum[64];		//���ȵ�¼����;
	char			szUserPasswd[64];	//���ȵ�¼����;
	char			szPromptText[64];	//�豸��ʾ��Ϣ;
}CK_UKPROCINFO;

typedef BOOL (*CK_UKDATA_CALLBACK_FUNC)(CK_UKPROCINFO *pUKeyInfo, char *pszUserPIN);

class CUKeyVerifyThread
{
public:
	CUKeyVerifyThread();
	~CUKeyVerifyThread();

public:
	BOOL					CreateVerifyProc(BOOL bFlags, CK_UKDATA_CALLBACK_FUNC pfUKDataFunc);
	BOOL					CloseVerifyProc();

	BOOL					SetThreadProcState(BOOL bFlags);
	void					SetThreadProcTime(DWORD dwProcTime, DWORD dwCloseTime);

	void					SetUserData(LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd);
	void					SetUserPIN(LPCTSTR lpszUserPIN);
	
protected:
	static DWORD WINAPI		UKeyVerifyThreadProc(LPVOID lpParam);
	
protected:
	void					UKeyVerifyInfo();
	
	BOOL					UKeySlotVerify(CK_UKPROCINFO *pstcUKeyProc);
	void					ClearUKeyInfo();

	BOOL					SetObjectData();
	BOOL					GetObjectData();
	
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

	CK_UKDATA_CALLBACK_FUNC m_pfUKDataFunc;
};

#endif