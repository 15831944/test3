#include "stdafx.h"
#include "UKeyVerify.h"

#include <wincrypt.h>
#include <cryptoki_ext.h>
#pragma comment(lib, "eps2003csp11_x86.lib")

CUKeyVerify::CUKeyVerify()
{
	m_pSlotList = NULL_PTR;
	m_ulSlotCount = 0;
}

CUKeyVerify::~CUKeyVerify()
{
	if (m_pSlotList != NULL_PTR)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	m_ulSlotCount = 0;
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
	CK_ULONG ulCount = 0;
	
	do 
	{
		rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
		
		if (ulCount <= 0)
		{
			bRet = FALSE;
			break;
		}
		
		if (m_pSlotList == NULL_PTR)
		{
			m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
			if (m_pSlotList == NULL_PTR)
			{
				bRet = FALSE;
				break;
			}
			memset(m_pSlotList, 0x0, ulCount*sizeof(CK_SLOT_ID));
		}
		else
		{
			memset(m_pSlotList, 0x0, ulCount*sizeof(CK_SLOT_ID));
		}
		
		rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
		
		if (ulCount <= 0)
		{
			bRet = FALSE;
			break;
		}
		
		bRet = TRUE;
		m_ulSlotCount = ulCount;
	} while(FALSE);
	
	if (!bRet && m_pSlotList != NULL_PTR)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	
	return bRet;
}

BOOL CUKeyVerify::GetSlotId(CK_LONG SlotIndex, CK_ULONG &ulSlotId)
{
	CK_RV rv;
	CK_FLAGS flags = 0;

	if (SlotIndex == -1)
	{
		rv = C_WaitForSlotEvent(flags, &ulSlotId, NULL_PTR);
		if (rv != CKR_OK)
		{
			return FALSE;
		}
	}
	else
	{
		if (m_pSlotList == NULL_PTR || m_ulSlotCount == 0)
		{
			return FALSE;
		}

		if (SlotIndex < m_ulSlotCount-1)
		{
			return FALSE;
		}

		ulSlotId = m_pSlotList[SlotIndex];
	}

	return TRUE;
}

BOOL CUKeyVerify::GetSlotInfo(CK_ULONG ulSlotId, CK_ULONG &ulFlags)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	CK_TOKEN_INFO tokenInfo = {0};
	
	do
	{
		rv = C_GetTokenInfo(ulSlotId, &tokenInfo);
		if (rv != CKR_OK)
		{
			bRet = FALSE;

			if (rv == CKR_TOKEN_NOT_PRESENT)
			{
				ulFlags = CKR_TOKEN_NOT_PRESENT;
			}		
			break;
		}
		
		if (tokenInfo.flags & CKF_PROTECTED_AUTHENTICATION_PATH)
		{
			ulFlags = CKF_PROTECTED_AUTHENTICATION_PATH;
		}
		else
		{
			ulFlags = tokenInfo.flags;
		}
		
		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

BOOL CUKeyVerify::CreateSession(CK_ULONG ulSlotId)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	
	do
	{
		rv = C_OpenSession(ulSlotId, CKF_RW_SESSION|CKF_SERIAL_SESSION, NULL_PTR, NULL_PTR, &m_hSession);
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

BOOL CUKeyVerify::CloseSession()
{
	CK_RV rv;
	if (m_hSession == NULL_PTR)
	{
		return FALSE;
	}

	rv = C_CloseSession(m_hSession);
	if (rv != CKR_OK)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CUKeyVerify::LoginUser(LPCTSTR lpszUserPIN, CK_ULONG ulFlags)
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	CK_ULONG ulPINLen = 0;
	CK_BYTE_PTR pPINBuffer = NULL_PTR;
	
	if (m_hSession == NULL_PTR)
	{
		return FALSE;
	}
	
	do
	{
		if (ulFlags & CKF_PROTECTED_AUTHENTICATION_PATH)
		{
			rv = C_Login(m_hSession, CKU_USER, 0, 0);
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

			rv = C_Login(m_hSession, CKU_USER, pPINBuffer, ulPINLen);
		}
		
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
	
		bRet = TRUE;
	} while (FALSE);
	
	return bRet;
}

BOOL CUKeyVerify::LogoutUser()
{
	BOOL bRet = FALSE;
	
	CK_RV rv;
	if (m_hSession == NULL_PTR)
	{
		return FALSE;
	}
	
	rv = C_Logout(m_hSession);
	if (rv != CKR_OK)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CUKeyVerify::CreateObject(LPCTSTR lpszUserData, CK_ULONG ulUserDataLen)
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

	if (m_hSession == NULL_PTR)
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
		
		rv = C_CreateObject(m_hSession, dataAttr, sizeof(dataAttr)/sizeof(dataAttr[0]), &hCKObj);
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

BOOL CUKeyVerify::FindObject(CK_ULONG &ulObjectCount)
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

	if (m_hSession == NULL_PTR)
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

		rv = C_FindObjectsInit(m_hSession, dataAttr, 2);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}

		rv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
		ulObjectCount = ulRetCount;
	} while (FALSE);

	C_FindObjectsFinal(m_hSession);
	return bRet;
}

BOOL CUKeyVerify::SetObjectValue(LPCTSTR lpszUserData, CK_ULONG ulUserDataLen)
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
	
	if (m_hSession == NULL_PTR)
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
				{ CKA_CLASS, 	&objectclass, 	sizeof(objectclass) },
				//{ CKA_TOKEN, 	&truevalue, 	sizeof(truevalue) },
				//{ CKA_MODIFIABLE,&truevalue,	sizeof(truevalue) },
				//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
				{ CKA_LABEL, 	datalabel, 		sizeof(datalabel)-1 },
				{ CKA_VALUE, 	databuff, 		databuflen }
		};
	
		rv = C_FindObjectsInit(m_hSession, dataAttr, 2);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
		
		rv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
 		if (rv != CKR_OK || ulRetCount != 1)
 		{
 			bRet = FALSE;
 			break;
 		}

		rv = C_SetAttributeValue(m_hSession, hCKObj, dataAttr, 3);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}
		
		bRet = TRUE;
	} while(FALSE);
	
	C_FindObjectsFinal(m_hSession);
	return bRet;
}

BOOL CUKeyVerify::GetObjectValue(CK_BYTE *pUserData, CK_ULONG *pUserDataLen)
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

	if (m_hSession == NULL_PTR)
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

		rv = C_FindObjectsInit(m_hSession, dataAttr, 2);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}

		rv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if (rv != CKR_OK)
		{
			bRet = FALSE;
			break;
		}

		rv = C_GetAttributeValue(m_hSession, hCKObj, dataAttr, 3);
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

	C_FindObjectsFinal(m_hSession);
	return bRet;
}