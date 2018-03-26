#include "stdafx.h"
#include "UKeyVerify.h"

#include <wincrypt.h>
#include <cryptoki_ext.h>
#pragma comment(lib, "eps2003csp11_x86.lib")

CUKeyVerify::CUKeyVerify()
{
	InitializeCriticalSection(&m_caUKeySection);
}

CUKeyVerify::~CUKeyVerify()
{
	DeleteCriticalSection(&m_caUKeySection);
}

//
CUKeyVerify& CUKeyVerify::Instance()
{
	static CUKeyVerify inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CUKeyVerify::Initialize()
{
	CK_RV rv;
	rv = C_Initialize(NULL_PTR);
	if (rv != CKR_OK)
	{
		return FALSE;
	}

	return TRUE;
}

void CUKeyVerify::Finalize()
{
	C_Finalize(NULL_PTR);
}

BOOL CUKeyVerify::GetSlotList()
{
	BOOL bRet = FALSE;

	CK_RV rv;
	CK_FLAGS flags = 0;
	CK_ULONG ulSlotId = 0;
	
	CK_TOKEN_INFO tokenInfo = {0};
	CK_UKEYPROCINFO *pUKeyInfo = NULL;

	rv = C_WaitForSlotEvent(flags, &ulSlotId, NULL_PTR);
	if (rv != CKR_OK)
	{
		return FALSE;
	}

	EnterCriticalSection(&m_caUKeySection);

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
				bRet = FALSE;
				break;
			}
		}
		else
		{
			pUKeyInfo = new CK_UKEYPROCINFO;
			if (pUKeyInfo == NULL)
			{
				bRet = FALSE;
				break;
			}
			memset(pUKeyInfo, 0x0, sizeof(CK_UKEYPROCINFO));

			if (tokenInfo.flags & CKF_TOKEN_PRESENT)
			{
				pUKeyInfo->bFlags = FALSE;
				pUKeyInfo->bExist = TRUE;
				pUKeyInfo->ulSlotId = ulSlotId;
				pUKeyInfo->ulFlags  = tokenInfo.flags;

				pUKeyInfo->hSession = NULL;
				pUKeyInfo->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

				if (tokenInfo.flags & CKF_PROTECTED_AUTHENTICATION_PATH)
				{
					pUKeyInfo->emUKeyType = CK_UKEYDEVFINGERTYPE;
				}
				else
				{
					pUKeyInfo->emUKeyType = CK_UKEYDEVNORMALTYPE;
				}

				pUKeyInfo->emUKeyState = CK_UKEYSTATEINSERTTYPE;
				pUKeyInfo->emUKeyOperateType = CK_OPERATEENUMSLOTTYPE;

				m_mapUKeyInfo.insert(make_pair(ulSlotId, pUKeyInfo));
				
			}
			else
			{
				delete pUKeyInfo;
				pUKeyInfo = NULL;
			}
		}

		bRet = TRUE;
	} while (FALSE);
	
	LeaveCriticalSection(&m_caUKeySection);
	if (!bRet && pUKeyInfo != NULL)
	{
		delete pUKeyInfo;
		pUKeyInfo = NULL;
	}

	return bRet;
}

BOOL CUKeyVerify::GetSlotId(std::vector<CK_UKEYPROCINFO*> &vecUKeyData)
{
	CK_UKEYPROCINFO *pUKeyData = NULL;
	std::map<CK_SLOT_ID, CK_UKEYPROCINFO*>::iterator iterUKeyData;

	EnterCriticalSection(&m_caUKeySection);

	for (iterUKeyData=m_mapUKeyInfo.begin(); iterUKeyData!=m_mapUKeyInfo.end(); ++iterUKeyData)
	{
		pUKeyData = (CK_UKEYPROCINFO *)iterUKeyData->second;
		if (pUKeyData == NULL)
		{
			continue;
		}

		vecUKeyData.push_back(pUKeyData);
	}

	LeaveCriticalSection(&m_caUKeySection);
	return TRUE;
}

BOOL CUKeyVerify::CreateSession(CK_ULONG ulSlotId, CK_SESSION_HANDLE &hSession)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	
	do
	{
		rv = C_OpenSession(ulSlotId, CKF_RW_SESSION|CKF_SERIAL_SESSION, NULL_PTR, NULL_PTR, &hSession);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
		
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
		
		bRet = TRUE;
	} while(FALSE);
	
	return bRet;
}

BOOL CUKeyVerify::CloseSession(CK_SESSION_HANDLE hSession)
{
	CK_RV rv;
	if (hSession == NULL_PTR)
	{
		return FALSE;
	}

	rv = C_CloseSession(hSession);
	if (rv != CKR_OK)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CUKeyVerify::LoginUser(CK_SESSION_HANDLE hSession, LPCTSTR lpszUserPIN, CK_ULONG ulFlags)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	CK_ULONG ulPINLen = 0;
	CK_BYTE_PTR pPINBuffer = NULL_PTR;
	
	if (hSession == NULL_PTR)
	{
		return FALSE;
	}
	
	do
	{
		if (ulFlags & CKF_PROTECTED_AUTHENTICATION_PATH)
		{
			rv = C_Login(hSession, CKU_USER, 0, 0);
		}
		else
		{
			if (lpszUserPIN == NULL_PTR || *lpszUserPIN == '\0')
			{
				bRet = FALSE;
				break;
			}

			ulPINLen = strlen(lpszUserPIN);
			pPINBuffer = (CK_BYTE_PTR)lpszUserPIN;

			if (pPINBuffer == NULL_PTR)
			{
				bRet = FALSE;
				break;
			}

			rv = C_Login(hSession, CKU_USER, pPINBuffer, ulPINLen);
		}
		
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
	
		bRet = TRUE;
	} while (FALSE);

	if (!bRet)
	{
	}
	
	return bRet;
}

BOOL CUKeyVerify::LogoutUser(CK_SESSION_HANDLE hSession)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	if (hSession == NULL_PTR)
	{
		return FALSE;
	}
	
	rv = C_Logout(hSession);
	if (rv != CKR_OK)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CUKeyVerify::CreateObject(CK_SESSION_HANDLE hSession, LPCTSTR lpszUserData, CK_ULONG ulUserDataLen)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj;
	
	CK_BYTE truevalue  = TRUE;
	CK_BYTE falsevalue = FALSE;
	CK_BYTE datalabel[] = "dataLabel";

	CK_OBJECT_CLASS objectclass = CKO_DATA;

	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;

	if (hSession == NULL_PTR)
	{
		return FALSE;
	}
	
	do
	{
		if (ulUserDataLen > databuflen)
		{
			bRet = FALSE;
			break;
		}
		
		memcpy(databuff, lpszUserData, ulUserDataLen);

		CK_ATTRIBUTE dataAttr[] = {
				{ CKA_CLASS,	&objectclass,	sizeof(objectclass) },
				{ CKA_TOKEN,	&truevalue,		sizeof(truevalue) },
				//{ CKA_MODIFIABLE,&truevalue,	sizeof(truevalue) },
				//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
				{ CKA_LABEL,	datalabel,		sizeof(datalabel)-1 },
				{ CKA_VALUE,	databuff,		databuflen }
		};

// 		rv = C_FindObjectsInit(m_hSession, dataAttr, 2);
// 		if (rv != CKR_OK)
// 		{
// 			bRet = FALSE;
// 			break;
// 		}
// 		
// 		rv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
// 		if (rv != CKR_OK)
// 		{
// 			bRet = FALSE;
// 			break;
// 		}
		
		rv = C_CreateObject(hSession, dataAttr, sizeof(dataAttr)/sizeof(dataAttr[0]), &hCKObj);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while(FALSE);
	
	//C_FindObjectsFinal(m_hSession);
	return bRet;
}

BOOL CUKeyVerify::FindObject(CK_SESSION_HANDLE hSession, CK_ULONG &ulObjectCount)
{
	BOOL bRet = FALSE;

	CK_RV rv;
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj;

	CK_BYTE truevalue  = TRUE;
	CK_BYTE falsevalue = FALSE;
	CK_BYTE datalabel[] = "dataLabel";

	CK_OBJECT_CLASS objectclass = CKO_DATA;

	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;

	if (hSession == NULL_PTR)
	{
		return FALSE;
	}

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
			bRet = FALSE;
			break;
		}

		rv = C_FindObjects(hSession, &hCKObj, 1, &ulRetCount);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
		ulObjectCount = ulRetCount;
	} while (FALSE);

	C_FindObjectsFinal(hSession);
	return bRet;
}

BOOL CUKeyVerify::SetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG ulUserDataLen)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj = NULL;
	
	CK_BYTE truevalue = TRUE;
	CK_BYTE falsevalue = FALSE;
	CK_BYTE datalabel[] = "dataLabel";
	CK_OBJECT_CLASS objectclass = CKO_DATA;
	
	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;
	
	if (hSession == NULL_PTR)
	{
		return FALSE;
	}

	do
	{
		if (ulUserDataLen > databuflen)
		{
			bRet = FALSE;
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
			bRet = FALSE;
			break;
		}
		
		rv = C_FindObjects(hSession, &hCKObj, 1, &ulRetCount);
 		if (rv != CKR_OK || ulRetCount != 1)
 		{
 			bRet = FALSE;
 			break;
 		}

		rv = C_SetAttributeValue(hSession, hCKObj, dataAttr, 3);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
		
		bRet = TRUE;
	} while(FALSE);
	
	C_FindObjectsFinal(hSession);
	return bRet;
}

BOOL CUKeyVerify::GetObjectValue(CK_SESSION_HANDLE hSession, CK_BYTE *pUserData, CK_ULONG *pUserDataLen)
{
	BOOL bRet = FALSE;

	CK_RV rv;
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj = NULL;
	CK_OBJECT_CLASS objectclass = CKO_DATA;

	CK_BYTE truevalue = TRUE;
	CK_BYTE falsevalue = FALSE;
	CK_BYTE datalabel[] = "dataLabel";
	
	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;

	if (hSession == NULL_PTR)
	{
		return FALSE;
	}

	if (pUserData == NULL || pUserDataLen == NULL)
	{
		return FALSE;
	}

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
			bRet = FALSE;
			break;
		}

		rv = C_FindObjects(hSession, &hCKObj, 1, &ulRetCount);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}

		rv = C_GetAttributeValue(hSession, hCKObj, dataAttr, 3);
		if (rv != CKR_OK)
		{
			continue;
		}

		if (dataAttr[2].pValue == NULL || dataAttr[2].ulValueLen == 0)
		{
			bRet = FALSE;
			break;
		}

		if (*pUserDataLen < dataAttr[2].ulValueLen)
		{
			bRet = FALSE;
			break;
		}

		*pUserDataLen = dataAttr[2].ulValueLen;
		memcpy(pUserData, dataAttr[2].pValue, dataAttr[2].ulValueLen);

		bRet = TRUE;
	} while (FALSE);

	C_FindObjectsFinal(hSession);
	return bRet;
}