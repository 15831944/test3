#ifndef __PKCS11_UKEY_H__
#define __PKCS11_UKEY_H__

using namespace std;
bool		PKCS11_Initialize(CK_UKEYHANDLE *pUKeyHandle, bool bFlags);
void		PKCS11_Finalize(CK_UKEYHANDLE *pUKeyHandle, bool bFlags);

bool		PKCS11_GetSlotId(CK_UKEYHANDLE *pUKeyHandle);
bool		PKCS11_GetSlotList(CK_UKEYHANDLE *pUKeyHandle, std::vector<CK_UKEYDEVICE*> &vecUKeyDevice);

bool		PKCS11_CreateSession(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId);
bool		PKCS11_CloseSession(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId);

bool		PKCS11_LoginUser(CK_UKEYHANDLE *pUKeyHandle,  CK_ULONG ulSlotId, const char *pszUserPIN);
bool		PKCS11_LogoutUser(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId);
bool		PKCS11_SetUserPin(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, const char *pszOldUserPIN, const char *pszNewUserPIN);

bool		PKCS11_FindObject(CK_UKEYHANDLE *pUKeyHandle,   CK_ULONG ulSlotId, CK_ULONG &ulObjectCount);
bool		PKCS11_CreateObject(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYWRITEDATA *pUKeyWriteData);
bool		PKCS11_SetObjectValue(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYWRITEDATA *pUKeyWriteData);

bool		PKCS11_GetObjectValue(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYREADDATA *pUKeyReadData);

#endif