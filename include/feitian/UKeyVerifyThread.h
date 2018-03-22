#ifndef __UKEYVERIFY_THREAD_H__
#define __UKEYVERIFY_THREAD_H__

#include "UKeyVerify.h"

typedef enum {
	CK_UKEMPTYTYPE = 0,							//空类型
	CK_UKNORMALTYPE,							//普通UKey设备
	CK_UKFINGERTYPE,							//指纹Key设备
}CK_UKEYDEVICETYPE;

typedef enum {
	CK_UKEYEMPTYTYPE = 0,						//空类型
	CK_UKEYINSERTTYPE,							//UKEY设备插入操作
	CK_UKEYREMOVETYPE,							//UKEY设备拔除操作
}CK_UKEYSTATETYPE;

typedef enum {
	CK_OPERATEEMPTYTYPE = 0,					//空类型	
	CK_OPERATEENUMSLOTTYPE,						//枚举设备操作;
	CK_OPERATEVERIFYTYPE,						//设备口令校验操作;
	CK_OPERATEWRITETYPE,						//设备写入操作;
	CK_OPERATEREADTYPE,							//设备读出操作;
}CK_UKEYOPERATETYPE;

typedef struct {
	BOOL				bFlags;					//操作成功,失败;
	BOOL				bExist;					//UKey设备是否存在;
	CK_UKEYDEVICETYPE	emUKeyType;				//UKey设备类型;
	CK_UKEYSTATETYPE	emUKeyState;			//UKey设备状态;
	CK_UKEYOPERATETYPE	emUKeyOperateType;		//UKey设备操作类型;
	CK_ULONG			ulSlotId;				//UKey设备ID;
	CK_ULONG			ulRemainCount;			//UKey设备PIN口令剩余次数;
	CK_BYTE*			pFingerData;			//UKey设备指纹数据;
	CK_ULONG			ulFingerDataLen;		//UKey设备指纹数据长度;
}CK_UKEYPROCINFO;

typedef struct {
	CK_UKEYOPERATETYPE	emUKeyOperateType;
	char				szUserNum[64];			//调度登录号码;
	char				szUserPasswd[64];		//调度登录密码;
	char				szUserPIN[64];			//UKey设备PIN码;
	char				szPromptText[64];		//设备提示信息;
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