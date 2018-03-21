#ifndef __FINGER_VERIFY_H__
#define __FINGER_VERIFY_H__

#include <wincrypt.h>
#include <cryptoki_ext.h>

class CUKeyVerify
{
public:
	CUKeyVerify();
	~CUKeyVerify();
	
	BOOL				Initialize();
	void				Finalize();

public:
	BOOL				GetSlotList();
	BOOL				GetSlotId(CK_ULONG ulSlotIndex, CK_ULONG &ulSlotId);
	BOOL				GetSlotInfo(CK_ULONG ulSlotId, CK_ULONG &ulFlags);
	
	BOOL				CreateSession(CK_ULONG ulSlotId);
	BOOL				CloseSession();
	
	BOOL				LoginUser(LPCTSTR lpszUserPIN, CK_ULONG ulFlags);
	BOOL				LogoutUser();

	BOOL				CreateObject(LPCTSTR lpszUserData, CK_ULONG ulUserDataLen);
	BOOL				FindObject(CK_ULONG &ulObjectCount);

	BOOL				SetObjectValue(LPCTSTR lpszUserData, CK_ULONG ulUserDataLen);
	BOOL				GetObjectValue(CK_BYTE *pUserData, CK_ULONG *pUserDataLen);

private:
	CK_ULONG			m_ulSlotCount;
	CK_SLOT_ID_PTR		m_pSlotList;

	CK_SESSION_HANDLE 	m_hSession;
};

#endif