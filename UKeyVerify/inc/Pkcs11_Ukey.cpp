#include "stdafx.h"
#include "Pkcs11_Ukey.h"

#ifdef WIN32
	#ifdef _WIN64
		#pragma comment(lib, "eps2003csp11_x64.lib")
	#else
		#pragma comment(lib, "eps2003csp11_x86.lib")
	#endif
#endif

bool PKCS11_Initialize(CK_UKEYHANDLE *pUKeyHandle, bool bFlags)
{
	CK_RV rv;
	bool bRet = false;

	do 
	{
		if (pUKeyHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (!bFlags)
		{
			rv = C_Initialize(NULL_PTR);
			if (rv != CKR_OK)
			{
				bRet = false;
				break;
			}

			E_GetAuxFunctionList(&pUKeyHandle->pAuxFunc);
		}
		
		bRet = true;
		InitializeCriticalSection(&pUKeyHandle->caUKeySection);
	} while (false);

	return bRet;
}

void PKCS11_Finalize(CK_UKEYHANDLE *pUKeyHandle, bool bFlags)
{
	bool bRet = false;
	CK_UKEYDEVICE *pUKeyDevice = NULL;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE *>::iterator iteUKeyDevice;

	do 
	{
		if (pUKeyHandle == NULL)
		{
			bRet = false;
			break;
		}

		for (iteUKeyDevice=pUKeyHandle->mapUKeyDevice.begin(); iteUKeyDevice!=pUKeyHandle->mapUKeyDevice.end(); )
		{
			pUKeyDevice = iteUKeyDevice->second;
			if (pUKeyDevice != NULL)
			{
				delete pUKeyDevice;
				pUKeyDevice = NULL;

				iteUKeyDevice = pUKeyHandle->mapUKeyDevice.erase(iteUKeyDevice);
			}
			else
			{
				++iteUKeyDevice;
			}
		}

		if (!bFlags)
		{
			C_Finalize(NULL_PTR);
		}
		
		bRet = true;
		DeleteCriticalSection(&pUKeyHandle->caUKeySection);
	} while (false);
}

bool PKCS11_GetSlotId(CK_UKEYHANDLE *pUKeyHandle)
{
	bool bRet = false;
	
	CK_RV rv;
	CK_ULONG ulSlotId = 0;
	
	CK_TOKEN_INFO tokenInfo = {0};
	CK_UKEYDEVICE *pUKeyDevice= NULL;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE *>::iterator iteUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}
	
	rv = C_WaitForSlotEvent(CKF_DONT_BLOCK, &ulSlotId, NULL_PTR);
	if (rv != CKR_OK)
	{
		return false;
	}
	
	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	do
	{
		rv = C_GetTokenInfo(ulSlotId, &tokenInfo);
		if (rv != CKR_OK)
		{
			if (rv == CKR_TOKEN_NOT_PRESENT)
			{
				iteUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
				if (iteUKeyDevice != pUKeyHandle->mapUKeyDevice.end())
				{
					pUKeyDevice = iteUKeyDevice->second;
					if (pUKeyDevice != NULL)
					{
						delete pUKeyDevice;
						pUKeyDevice = NULL;
					}

					pUKeyHandle->mapUKeyDevice.erase(iteUKeyDevice);
				}
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

				pUKeyDevice->bIsVerify = false;
				pUKeyDevice->bIsDefaultUserPIN = false;

				pUKeyDevice->stcUKeyEnum.bExist = true;
				pUKeyDevice->stcUKeyEnum.ulSlotId = ulSlotId;
				pUKeyDevice->stcUKeyEnum.emUKeyState = CK_UKEYSTATEINSERTTYPE;

				if (((EP_isBioActive)(pUKeyHandle->pAuxFunc->pFunc[EP_IS_BIOACTIVE]))(ulSlotId))
				{
					if (tokenInfo.flags & CKF_PROTECTED_AUTHENTICATION_PATH)
					{
						pUKeyDevice->bIsFinger = true;
						pUKeyDevice->stcUKeyEnum.emUKeyType = CK_UKEYDEVFINGERTYPE;
					}
					else
					{
						pUKeyDevice->bIsFinger = false;
						pUKeyDevice->stcUKeyEnum.emUKeyType = CK_UKEYDEVNORMALTYPE;
					}
				}
				else
				{
					pUKeyDevice->bIsFinger = false;
					pUKeyDevice->stcUKeyEnum.emUKeyType = CK_UKEYDEVNORMALTYPE;
				}
				
				pUKeyDevice->stcUKeyVerify.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				pUKeyDevice->stcUKeyRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				pUKeyDevice->stcUKeyWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

				pUKeyHandle->mapUKeyDevice.insert(make_pair(ulSlotId, pUKeyDevice));
			}
			else
			{
				delete pUKeyDevice;
				pUKeyDevice = NULL;
			}
		}
		
		bRet = true;
	} while (false);
	
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);

	if (!bRet && pUKeyDevice != NULL)
	{
		delete pUKeyDevice;
		pUKeyDevice = NULL;
	}
	
	return bRet;
}

bool PKCS11_GetSlotList(CK_UKEYHANDLE *pUKeyHandle, std::vector<CK_UKEYDEVICE*> &vecUKeyDevice)
{
	bool bRet = false;

	CK_UKEYDEVICE *pUKeyDevice = NULL;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);

	do
	{
		for (iterUKeyDevice=pUKeyHandle->mapUKeyDevice.begin(); iterUKeyDevice!=pUKeyHandle->mapUKeyDevice.end(); ++iterUKeyDevice)
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
	
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_CreateSession(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId)
{
	CK_RV rv;
	bool bRet = false;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	
	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		if (iterUKeyDevice->second->hSession == NULL)
		{
			rv = C_OpenSession(ulSlotId, CKF_RW_SESSION | CKF_SERIAL_SESSION, NULL_PTR, NULL_PTR, &hSession);
			if (rv != CKR_OK)
			{
				bRet = false;
				break;
			}

			iterUKeyDevice->second->hSession = (HANDLE)hSession;
		}
		
		bRet = true;
	} while(false);
	
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_CloseSession(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId)
{
	CK_RV rv;
	bool bRet = false;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}
	
	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	
	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		rv = C_CloseSession(hSession);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}
		
		bRet = true;
		iterUKeyDevice->second->hSession = NULL;
	} while (false);
	
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_LoginUser(CK_UKEYHANDLE *pUKeyHandle,  CK_ULONG ulSlotId, const char *pszUserPIN)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_ULONG ulFlags = 0;
	CK_ULONG ulPINLen = 0;
	CK_BYTE_PTR pPINBuffer = NULL_PTR;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;
	
	if (pUKeyHandle == NULL)
	{
		return false;
	}
	
	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	
	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		if (!iterUKeyDevice->second->bIsVerify)
		{
			if (((EP_isBioActive)(pUKeyHandle->pAuxFunc->pFunc[EP_IS_BIOACTIVE]))(ulSlotId))
			{
				ulFlags = iterUKeyDevice->second->ulFlags;
				if (ulFlags & CKF_PROTECTED_AUTHENTICATION_PATH)
				{
					rv = C_Login(hSession, CKU_USER, 0, 0);
				}
			}
			else
			{
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

			if (strcmp(pszUserPIN, DEFAULT_USERPIN) == 0)
			{
				iterUKeyDevice->second->bIsDefaultUserPIN = true;
			}
			else
			{
				iterUKeyDevice->second->bIsDefaultUserPIN = false;
			}

			iterUKeyDevice->second->bIsVerify = true;
			iterUKeyDevice->second->stcUKeyVerify.emUKeyState = CK_UKEYSTATESUCCEDTYPE;
			memcpy(iterUKeyDevice->second->stcUKeyVerify.szUserPIN, pszUserPIN, ulPINLen);
		}
		
		bRet = true;
	} while (false);

	if (!bRet)
	{
		iterUKeyDevice->second->bIsVerify = false;
		iterUKeyDevice->second->stcUKeyVerify.emUKeyState = CK_UKEYSTATEFAILEDTYPE;
		memset(iterUKeyDevice->second->stcUKeyVerify.szUserPIN, 0x0, UKEYPIN_MAX_LEN);
	}
	
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_LogoutUser(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;
	
	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	
	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		rv = C_Logout(hSession);
		if (rv != CKR_OK)
		{
			return false;
		}
		
		iterUKeyDevice->second->bIsVerify = false;
		iterUKeyDevice->second->stcUKeyVerify.emUKeyState = CK_UKEYSTATEEMPTYTYPE;

		memset(iterUKeyDevice->second->stcUKeyVerify.szUserPIN, 0x0, sizeof(UKEYPIN_MAX_LEN));
		memset(iterUKeyDevice->second->stcUKeyVerify.szNewUserPIN, 0x0, sizeof(UKEYPIN_MAX_LEN));

		bRet = true;
	} while (false);
	
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_SetUserPin(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, const char *pszOldUserPIN, const char *pszNewUserPIN)
{
	CK_RV rv;
	bool bRet = false;

	CK_ULONG ulFlags = 0;
	CK_ULONG ulOldPINLen = 0;
	CK_BYTE_PTR pOldPINBuffer = NULL_PTR;

	CK_ULONG ulNewPINLen = 0;
	CK_BYTE_PTR pNewPINBuffer = NULL_PTR;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);

	do 
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		ulFlags = iterUKeyDevice->second->ulFlags;
		if (ulFlags & CKF_PROTECTED_AUTHENTICATION_PATH)
		{
			bRet = true;
			break;
		}
		else
		{
			if (pszOldUserPIN == NULL || *pszOldUserPIN == '\0'
				|| pszNewUserPIN == NULL || *pszNewUserPIN == '\0')
			{
				bRet = false;
				break;
			}

			ulOldPINLen = strlen(pszOldUserPIN);
			pOldPINBuffer = (CK_BYTE_PTR)pszOldUserPIN;

			ulNewPINLen = strlen(pszNewUserPIN);
			pNewPINBuffer = (CK_BYTE_PTR)pszNewUserPIN;

			rv = C_SetPIN(hSession, pOldPINBuffer, ulOldPINLen, pNewPINBuffer, ulNewPINLen);
			if (rv != CKR_OK)
			{
				bRet = false;
				break;
			}

			iterUKeyDevice->second->bIsVerify = true;
			iterUKeyDevice->second->stcUKeyVerify.emUKeyState = CK_UKEYSTATESUCCEDTYPE;
			memcpy(iterUKeyDevice->second->stcUKeyVerify.szUserPIN, pszNewUserPIN, ulNewPINLen);

			bRet = true;
		}
	} while (false);

	if (!bRet)
	{
		iterUKeyDevice->second->bIsVerify = false;
		iterUKeyDevice->second->stcUKeyVerify.emUKeyState = CK_UKEYSTATEFAILEDTYPE;
		memset(iterUKeyDevice->second->stcUKeyVerify.szUserPIN, 0x0, UKEYPIN_MAX_LEN);
	}

	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_FindObject(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_ULONG &ulObjectCount)
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

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	CK_ATTRIBUTE dataAttr[] = {
		{ CKA_CLASS, 	&objectclass, 	sizeof(objectclass) },
		//{ CKA_TOKEN, 	&truevalue, 	sizeof(truevalue) },
		//{ CKA_MODIFIABLE, &truevalue,	sizeof(truevalue) },
		//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
		{ CKA_LABEL, 	datalabel, 		sizeof(datalabel)-1 },
		{ CKA_VALUE, 	databuff, 		databuflen }
	};

	EnterCriticalSection(&pUKeyHandle->caUKeySection);

	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

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
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_CreateObject(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYWRITEDATA *pUKeyWriteData)
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

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;
	
	if (pUKeyHandle == NULL || pUKeyWriteData == NULL)
	{
		return false;
	}

	CK_ATTRIBUTE dataAttr[] = {
		{ CKA_CLASS,	&objectclass,	sizeof(objectclass) },
		{ CKA_TOKEN,	&truevalue,		sizeof(truevalue) },
		//{ CKA_MODIFIABLE,&truevalue,	sizeof(truevalue) },
		//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
		{ CKA_LABEL,	datalabel,		sizeof(datalabel)-1 },
		{ CKA_VALUE,	databuff,		databuflen }
	};
	
	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	
	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		memcpy(databuff, pUKeyWriteData->szUserNum, strlen(pUKeyWriteData->szUserNum));
		databuflen = strlen(pUKeyWriteData->szUserNum);
		databuflen += 1;
		memcpy(databuff + databuflen, pUKeyWriteData->szUserPasswd, strlen(pUKeyWriteData->szUserPasswd));
		databuflen += strlen(pUKeyWriteData->szUserPasswd);

		rv = C_CreateObject(hSession, dataAttr, sizeof(dataAttr) / sizeof(dataAttr[0]), &hCKObj);
		if (rv != CKR_OK)
		{
			bRet = false;
			break;
		}

		pUKeyWriteData->emUKeyState = CK_UKEYSTATESUCCEDTYPE;
		iterUKeyDevice->second->stcUKeyWrite.emUKeyState = CK_UKEYSTATESUCCEDTYPE;

		memcpy(iterUKeyDevice->second->stcUKeyWrite.szUserNum, pUKeyWriteData->szUserNum, strlen(pUKeyWriteData->szUserNum));
		memcpy(iterUKeyDevice->second->stcUKeyWrite.szUserPasswd, pUKeyWriteData->szUserPasswd, strlen(pUKeyWriteData->szUserPasswd));

		bRet = true;
		SetEvent(iterUKeyDevice->second->stcUKeyWrite.hEvent);
	} while (false);
	
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_SetObjectValue(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYWRITEDATA *pUKeyWriteData)
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
	
	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL || pUKeyWriteData == NULL)
	{
		return false;
	}

	CK_ATTRIBUTE dataAttr[] = {
		{ CKA_CLASS, 	&objectclass, 	sizeof(objectclass) },
		//{ CKA_TOKEN, 	&truevalue, 	sizeof(truevalue) },
		//{ CKA_MODIFIABLE,&truevalue,	sizeof(truevalue) },
		//{ CKA_PRIVATE,	&truevalue,	sizeof(truevalue) },
		{ CKA_LABEL, 	datalabel, 		sizeof(datalabel)-1 },
		{ CKA_VALUE, 	databuff, 		databuflen }
	};
	
	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	
	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		memcpy(databuff, pUKeyWriteData->szUserNum, strlen(pUKeyWriteData->szUserNum));
		databuflen = strlen(pUKeyWriteData->szUserNum);
		databuflen += 1;
		memcpy(databuff + databuflen, pUKeyWriteData->szUserPasswd, strlen(pUKeyWriteData->szUserPasswd));
		databuflen += strlen(pUKeyWriteData->szUserPasswd);

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

		pUKeyWriteData->emUKeyState = CK_UKEYSTATESUCCEDTYPE;
		iterUKeyDevice->second->stcUKeyWrite.emUKeyState = CK_UKEYSTATESUCCEDTYPE;

		memcpy(iterUKeyDevice->second->stcUKeyWrite.szUserNum, pUKeyWriteData->szUserNum, strlen(pUKeyWriteData->szUserNum));
		memcpy(iterUKeyDevice->second->stcUKeyWrite.szUserPasswd, pUKeyWriteData->szUserPasswd, strlen(pUKeyWriteData->szUserPasswd));

		bRet = true;
		SetEvent(iterUKeyDevice->second->stcUKeyWrite.hEvent);
	} while (false);

	if (!bRet)
	{
		pUKeyWriteData->emUKeyState = CK_UKEYSTATEFAILEDTYPE;
		iterUKeyDevice->second->stcUKeyWrite.emUKeyState = CK_UKEYSTATEFAILEDTYPE;
	}
	
	C_FindObjectsFinal(hSession);
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_GetObjectValue(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYREADDATA *pUKeyReadData)
{
	CK_RV rv;
	bool bRet = false;
	
	CK_ULONG ulIndex = 0;
	CK_ULONG ulRetCount = 0;
	CK_OBJECT_HANDLE hCKObj = NULL;
	
	CK_BYTE truevalue = true;
	CK_BYTE falsevalue = false;
	CK_BYTE datalabel[] = "dataLabel";
	
	char *pValue = NULL;
	CK_OBJECT_CLASS objectclass = CKO_DATA;
	
	CK_BYTE databuff[512] = {0};
	CK_ULONG databuflen = sizeof(databuff)-1;
	
	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	CK_ATTRIBUTE dataAttr[] = {
		{ CKA_CLASS,	&objectclass,	sizeof(objectclass) },
		//{ CKA_TOKEN,	&truevalue,		sizeof(truevalue) },
		{ CKA_LABEL,	datalabel,		sizeof(datalabel) - 1 },
		{ CKA_VALUE,	databuff,		databuflen }
	};

	if (pUKeyHandle == NULL || pUKeyReadData == NULL)
	{
		return false;
	}
	
	EnterCriticalSection(&pUKeyHandle->caUKeySection);
	
	do
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

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

		pValue = (char*)dataAttr[2].pValue;
		while (*pValue != '\0')
		{
			if (ulIndex == 0)
			{
				memcpy(pUKeyReadData->szUserNum, pValue, strlen(pValue));
			}
			else if (ulIndex == 1)
			{
				memcpy(pUKeyReadData->szUserPasswd, pValue, strlen(pValue));
			}
			else
			{
				break;
			}

			ulIndex++;
			pValue = pValue + strlen(pValue) + 1;
		}

		pUKeyReadData->emUKeyState = CK_UKEYSTATESUCCEDTYPE;
		iterUKeyDevice->second->stcUKeyRead.emUKeyState = CK_UKEYSTATESUCCEDTYPE;

		memcpy(iterUKeyDevice->second->stcUKeyRead.szUserNum, pUKeyReadData->szUserNum, strlen(pUKeyReadData->szUserNum));
		memcpy(iterUKeyDevice->second->stcUKeyRead.szUserPasswd, pUKeyReadData->szUserPasswd, strlen(pUKeyReadData->szUserPasswd));

		bRet = true;
		SetEvent(iterUKeyDevice->second->stcUKeyRead.hEvent);
	} while (false);

	if (!bRet)
	{
		pUKeyReadData->emUKeyState = CK_UKEYSTATEFAILEDTYPE;
		iterUKeyDevice->second->stcUKeyRead.emUKeyState = CK_UKEYSTATEFAILEDTYPE;
	}
	
	C_FindObjectsFinal(hSession);
	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_GetFingerCount(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId)
{
	CK_RV rv;
	bool bRet = false;

	CK_ULONG ulRetCount = 0;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);

	do 
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		if (!((EP_isBioActive)(pUKeyHandle->pAuxFunc->pFunc[EP_IS_BIOACTIVE]))(ulSlotId))
		{
			
		}
		else
		{

		}
	} while (false);

	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_FingerEnroll(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_ULONG ulFingerId)
{
	CK_RV rv;
	bool bRet = false;

	CK_ULONG ulNumber = 0;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);

	do 
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}

		ulNumber = 1;
		while (WaitForSingleObject(iterUKeyDevice->second->stcUKeyVerify.hEvent, 0) != WAIT_OBJECT_0)
		{
			if (ulNumber < USERFINGER_ENROLL_NUMBER)
			{
				rv = ((EP_EnrollFinger)pUKeyHandle->pAuxFunc->pFunc[EP_ENROLL_FINGER])(ulSlotId, ulFingerId, ulNumber);
				if (rv != CKR_OK)
				{
					if (rv == CKR_BIO_ENROLL_TIMEOUT)
					{
						Sleep(100);
						continue;
					}
					else
					{
						bRet = false;
						break;
					}
				}

				ulNumber++;
			}
			else
			{
				bRet = true;
				break;
			}
		}
	} while (false);

	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_FingerDelete(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId)
{
	CK_RV rv;
	bool bRet = false;

	CK_ULONG ulRetCount = 0;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);

	do 
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}


	} while (false);

	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}

bool PKCS11_FingerClear(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId)
{
	CK_RV rv;
	bool bRet = false;

	CK_ULONG ulRetCount = 0;

	CK_SESSION_HANDLE hSession;
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*>::iterator iterUKeyDevice;

	if (pUKeyHandle == NULL)
	{
		return false;
	}

	EnterCriticalSection(&pUKeyHandle->caUKeySection);

	do 
	{
		if (ulSlotId == 0 || pUKeyHandle->mapUKeyDevice.size() == 0)
		{
			bRet = false;
			break;
		}

		iterUKeyDevice = pUKeyHandle->mapUKeyDevice.find(ulSlotId);
		if (iterUKeyDevice == pUKeyHandle->mapUKeyDevice.end())
		{
			bRet = false;
			break;
		}

		hSession = (CK_SESSION_HANDLE)iterUKeyDevice->second->hSession;
		if (hSession == NULL_PTR)
		{
			bRet = false;
			break;
		}


	} while (false);

	LeaveCriticalSection(&pUKeyHandle->caUKeySection);
	return bRet;
}