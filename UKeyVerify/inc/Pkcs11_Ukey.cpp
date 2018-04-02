#include "stdafx.h"
#include "Pkcs11_Ukey.h"

#include <string>
#include <map>
#include <vector>

#ifdef WIN32
	#ifdef _WIN64
		#pragma comment(lib, "eps2003csp11_x64.lib")
	#else
		#pragma comment(lib, "eps2003csp11_x86.lib")
	#endif
#endif

static CRITICAL_SECTION		g_caUKeySection;
static std::map<CK_SLOT_ID, CK_UKEYDEVICE*>	g_mapUKeyDevice;

bool PKCS11_Initialize()
{
	CK_RV rv;
	rv = C_Initialize(NULL_PTR);
	if (rv != CKR_OK)
	{
		return false;
	}
	
	InitializeCriticalSection(&g_caUKeySection);
	return true;
}

void PKCS11_Finalize()
{
	C_Finalize(NULL_PTR);
	DeleteCriticalSection(&g_caUKeySection);
}

bool PKCS11_GetSlotList()
{
	bool bRet = false;
	
	CK_RV rv;
	CK_FLAGS flags = 0;
	CK_ULONG ulSlotId = 0;
	
	CK_TOKEN_INFO tokenInfo = {0};
	CK_UKEYDEVICE *pUKeyDevice= NULL;
	
	rv = C_WaitForSlotEvent(flags, &ulSlotId, NULL_PTR);
	if (rv != CKR_OK)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	do
	{
		rv = C_GetTokenInfo(ulSlotId, &tokenInfo);
		if (rv != CKR_OK)
		{
			if (rv == CKR_TOKEN_NOT_PRESENT)
			{
				//m_mapUKeyInfo.
			}
			else
			{
				bRet = false;
				break;
			}
		}
		else
		{
			pUKeyDevice = new CK_UKEYDEVICE;
			if (pUKeyDevice == NULL)
			{
				bRet = FALSE;
				break;
			}
			memset(pUKeyDevice, 0x0, sizeof(CK_UKEYDEVICE));
			
			if (tokenInfo.flags & CKF_TOKEN_PRESENT)
			{
				pUKeyDevice->ulSlotId = ulSlotId;
				pUKeyDevice->ulFlags  = tokenInfo.flags;
				pUKeyDevice->hSession = NULL;

				pUKeyDevice->stcUKeyEnum.bExist = true;
				pUKeyDevice->stcUKeyEnum.ulSlotId = ulSlotId;
				pUKeyDevice->stcUKeyEnum.emUKeyState = CK_UKEYSTATEINSERTTYPE;

				if (tokenInfo.flags & CKF_PROTECTED_AUTHENTICATION_PATH)
				{
					pUKeyDevice->stcUKeyEnum.emUKeyType = CK_UKEYDEVFINGERTYPE;
				}
				else
				{
					pUKeyDevice->stcUKeyEnum.emUKeyType = CK_UKEYDEVNORMALTYPE;
				}
				
				g_mapUKeyDevice.insert(make_pair(ulSlotId, pUKeyDevice));
			}
			else
			{
				delete pUKeyDevice;
				pUKeyDevice = NULL;
			}
		}
		
		bRet = true;
	} while (false);
	
	LeaveCriticalSection(&g_caUKeySection);
	if (!bRet && pUKeyDevice != NULL)
	{
		delete pUKeyDevice;
		pUKeyDevice = NULL;
	}
	
	return bRet;
}

bool PKCS11_GetSlotId(std::vector<CK_UKEYDEVICE*> &vecUKeyDevice)
{
	bool bRet = false;
	CK_UKEYDEVICE *pUKeyDevice = NULL;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	EnterCriticalSection(&g_caUKeySection);

	do
	{
		for (iterUKeyDevice=g_mapUKeyDevice.begin(); iterUKeyDevice!=g_mapUKeyDevice.end(); ++iterUKeyDevice)
		{
			pUKeyDevice = (CK_UKEYDEVICE *)iterUKeyDevice->second;
			if (pUKeyDevice == NULL)
			{
				continue;
			}

			vecUKeyDevice.push_back(pUKeyDevice);
		}
		
		bRet = true;
	} while (false);
	
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_CreateSession(CK_ULONG ulSlotId, CK_SESSION_HANDLE &hSession)
{
	CK_RV rv;
	bool bRet = false;
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		rv = C_OpenSession(ulSlotId, CKF_RW_SESSION|CKF_SERIAL_SESSION, NULL_PTR, NULL_PTR, &hSession);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while(false);
	
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_CloseSession(CK_SESSION_HANDLE hSession)
{
	CK_RV rv;
	bool bRet = false;
	
	if (hSession == NULL_PTR)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		rv = C_CloseSession(hSession);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while (false);
	
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_LoginUser(CK_SESSION_HANDLE hSession, const char *pszUserPIN, CK_ULONG ulFlags)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_ULONG ulPINLen = 0;
	CK_BYTE_PTR pPINBuffer = NULL_PTR;
	
	if (hSession == NULL_PTR)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		if (ulFlags & CKF_PROTECTED_AUTHENTICATION_PATH)
		{
			rv = C_Login(hSession, CKU_USER, 0, 0);
		}
		else
		{
			if (pszUserPIN == NULL_PTR || *pszUserPIN == '\0')
			{
				bRet = false;
				break;
			}

			ulPINLen = strlen(pszUserPIN);
			pPINBuffer = (CK_BYTE_PTR)pszUserPIN;

			if (pPINBuffer == NULL_PTR)
			{
				bRet = false;
				break;
			}

			rv = C_Login(hSession, CKU_USER, pPINBuffer, ulPINLen);
		}
		
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while (false);
	
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_LogoutUser(CK_SESSION_HANDLE hSession)
{
	CK_RV rv;
	bool bRet = false;
	
	if (hSession == NULL_PTR)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		rv = C_Logout(hSession);
		if (rv != CKR_OK)
		{
			return false;
		}
		
		bRet = true;
	} while (false);
	
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_CreateObject(CK_SESSION_HANDLE hSession, const char *pszUserData, CK_ULONG ulUserDataLen)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj;
	
	CK_BYTE truevalue  = true;
	CK_BYTE falsevalue = false;
	CK_BYTE datalabel[] = "dataLabel";
	
	CK_OBJECT_CLASS objectclass = CKO_DATA;
	
	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;
	
	if (hSession == NULL_PTR)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		if (ulUserDataLen > databuflen)
		{
			bRet = false;
			break;
		}
		memcpy(databuff, pszUserData, ulUserDataLen);
		
		CK_ATTRIBUTE dataAttr[] = {
				{ CKA_CLASS,	&objectclass,	sizeof(objectclass) },
				{ CKA_TOKEN,	&truevalue,		sizeof(truevalue) },
				//{ CKA_MODIFIABLE,&truevalue,	sizeof(truevalue) },
				//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
				{ CKA_LABEL,	datalabel,		sizeof(datalabel)-1 },
				{ CKA_VALUE,	databuff,		databuflen }
		};
		
//		rv = C_FindObjectsInit(m_hSession, dataAttr, 2);
//		if (rv != CKR_OK)
//		{
//			bRet = false;
//			break;
//		}
//		
//		rv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
//		{
//			bRet = false;
//			break;
//		}
		
		rv = C_CreateObject(hSession, dataAttr, sizeof(dataAttr)/sizeof(dataAttr[0]), &hCKObj);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while (false);
	
//	C_FindObjectsFinal(m_hSession);
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_FindObject(CK_SESSION_HANDLE hSession, CK_ULONG &ulObjectCount)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj;
	
	CK_BYTE truevalue  = true;
	CK_BYTE falsevalue = false;
	CK_BYTE datalabel[] = "dataLabel";
	
	CK_OBJECT_CLASS objectclass = CKO_DATA;

	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;
	
	if (hSession == NULL_PTR)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		CK_ATTRIBUTE dataAttr[] = {
				{ CKA_CLASS, 	&objectclass, 	sizeof(objectclass) },
				//{ CKA_TOKEN, 	&truevalue, 	sizeof(truevalue) },
				//{ CKA_MODIFIABLE, &truevalue,	sizeof(truevalue) },
				//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
				{ CKA_LABEL, 	datalabel, 		sizeof(datalabel)-1 },
				{ CKA_VALUE, 	databuff, 		databuflen }
		};
		
		rv = C_FindObjectsInit(hSession, dataAttr, 2);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		rv = C_FindObjects(hSession, &hCKObj, 1, &ulRetCount);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
		ulObjectCount = ulRetCount;
	} while (false);
	
	C_FindObjectsFinal(hSession);
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_SetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG ulUserDataLen)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj = NULL;
	
	CK_BYTE truevalue = true;
	CK_BYTE falsevalue = false;
	CK_BYTE datalabel[] = "dataLabel";
	
	CK_OBJECT_CLASS objectclass = CKO_DATA;
	
	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;
	
	if (hSession == NULL_PTR)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		if (ulUserDataLen > databuflen)
		{
			bRet = false;
			break;
		}
		memcpy(databuff, pUserData, ulUserDataLen);
		
		CK_ATTRIBUTE dataAttr[] = {
				{ CKA_CLASS, 	&objectclass, 	sizeof(objectclass) },
				//{ CKA_TOKEN, 	&truevalue, 	sizeof(truevalue) },
				//{ CKA_MODIFIABLE,&truevalue,	sizeof(truevalue) },
				//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
				{ CKA_LABEL, 	datalabel, 		sizeof(datalabel)-1 },
				{ CKA_VALUE, 	databuff, 		databuflen }
		};
		
		rv = C_FindObjectsInit(hSession, dataAttr, 2);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		rv = C_FindObjects(hSession, &hCKObj, 1, &ulRetCount);
 		if (rv != CKR_OK || ulRetCount != 1)
 		{
 			bRet = false;
 			break;
 		}

		rv = C_SetAttributeValue(hSession, hCKObj, dataAttr, 3);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while (false);
	
	C_FindObjectsFinal(hSession);
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}

bool PKCS11_GetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG *pUserDataLen)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj = NULL;
	
	CK_BYTE truevalue = true;
	CK_BYTE falsevalue = false;
	CK_BYTE datalabel[] = "dataLabel";
	
	CK_OBJECT_CLASS objectclass = CKO_DATA;
	
	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;
	
	if (hSession == NULL_PTR)
	{
		return false;
	}
	
	EnterCriticalSection(&g_caUKeySection);
	
	do
	{
		CK_ATTRIBUTE dataAttr[] = {
				{ CKA_CLASS,	&objectclass,	sizeof(objectclass) },
				//{ CKA_TOKEN,	&truevalue,		sizeof(truevalue) },
				{ CKA_LABEL,	datalabel,		sizeof(datalabel)-1 },
				{ CKA_VALUE,	databuff,		databuflen }
		};
		
		rv = C_FindObjectsInit(hSession, dataAttr, 2);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		rv = C_FindObjects(hSession, &hCKObj, 1, &ulRetCount);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		rv = C_GetAttributeValue(hSession, hCKObj, dataAttr, 3);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		if (dataAttr[2].pValue == NULL || dataAttr[2].ulValueLen == 0)
		{
			bRet = false;
			break;
		}

		if (*pUserDataLen < dataAttr[2].ulValueLen)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
		*pUserDataLen = dataAttr[2].ulValueLen;
		memcpy(pUserData, dataAttr[2].pValue, dataAttr[2].ulValueLen);
	} while (false);
	
	C_FindObjectsFinal(hSession);
	LeaveCriticalSection(&g_caUKeySection);
	return bRet;
}