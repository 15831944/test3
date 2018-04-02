#ifndef __PKCS11_UKEY_H__
#define __PKCS11_UKEY_H__

using namespace std;
bool		PKCS11_Initialize();
void		PKCS11_Finalize();

bool		PKCS11_GetSlotList();
bool		PKCS11_GetSlotId(std::vector<CK_UKEYDEVICE*> &vecUKeyDevice);

bool		PKCS11_CreateSession(CK_ULONG ulSlotId, CK_SESSION_HANDLE &hSession);
bool		PKCS11_CloseSession(CK_SESSION_HANDLE hSession);

bool		PKCS11_LoginUser(CK_SESSION_HANDLE hSession, const char *pszUserPIN, CK_ULONG ulFlags);
bool		PKCS11_LogoutUser(CK_SESSION_HANDLE hSession);

bool		PKCS11_CreateObject(CK_SESSION_HANDLE hSession, const char *pszUserData, CK_ULONG ulUserDataLen);
bool		PKCS11_FindObject(CK_SESSION_HANDLE hSession, CK_ULONG &ulObjectCount);

bool		PKCS11_SetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG ulUserDataLen);
bool		PKCS11_GetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG *pUserDataLen);

#endif