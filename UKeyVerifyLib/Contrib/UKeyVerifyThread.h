#ifndef __UKEYVERIFY_THREAD_H__
#define __UKEYVERIFY_THREAD_H__

#include <internal.h>

using namespace std;
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
	
	BOOL					SetUKeyData(int iSlotIndex);
	BOOL					GetUKeyData(int iSlotIndex);

	BOOL					UKeyEnum(std::vector<CK_UKEYPROCINFO *> &vecUKeyData);
	BOOL					GetUKeyInfo(int iIndex, int iSlotIndex, CK_UKEYPROCINFO *pUKeyData);

	BOOL					UKeyVerify(CK_UKEYPROCINFO *pUKeyData);
	BOOL					UKeyWriteObjectData(CK_UKEYPROCINFO *pUKeyData, LPCTSTR lpszUserNum, LPCTSTR lpszUserPasswd);
	BOOL					UKeyReadObjectData(CK_UKEYPROCINFO *pUKeyData);
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
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
	CK_UKDATA_CALLBACK_FUNC m_pfUKDataFunc;
};

#endif