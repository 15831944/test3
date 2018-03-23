#ifndef __FINGER_VERIFY_H__
#define __FINGER_VERIFY_H__

#include <wincrypt.h>
#include <cryptoki_ext.h>
#include <internal.h>

using namespace std;
class CUKeyVerify
{
public:
	CUKeyVerify();
	~CUKeyVerify();

	static CUKeyVerify&		Instance();
	
	BOOL					Initialize();
	void					Finalize();

public:
	BOOL					GetSlotList();
	BOOL					GetSlotId(std::vector<CK_UKEYPROCINFO*> &vecUKeyData);

	BOOL					CreateSession(CK_ULONG ulSlotId, CK_SESSION_HANDLE &hSession);
	BOOL					CloseSession(CK_SESSION_HANDLE hSession);
	
	BOOL					LoginUser(CK_SESSION_HANDLE hSession, LPCTSTR lpszUserPIN, CK_ULONG ulFlags);
	BOOL					LogoutUser(CK_SESSION_HANDLE hSession);

	BOOL					CreateObject(CK_SESSION_HANDLE hSession, LPCTSTR lpszUserData, CK_ULONG ulUserDataLen);
	BOOL					FindObject(CK_SESSION_HANDLE hSession, CK_ULONG &ulObjectCount);

	BOOL					SetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG ulUserDataLen);
	BOOL					GetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG *pUserDataLen);

private:
	std::map<CK_SLOT_ID, CK_UKEYPROCINFO*>	m_mapUKeyInfo;
	CRITICAL_SECTION		m_caUKeySection;
};

#endif