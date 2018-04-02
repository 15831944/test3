#include "stdafx.h"
#include "UKeyVerify.h"

#include "./inc/Pkcs11_Ukey.h"
#include "./contrib/UKeyEnumThread.h"
#include "./contrib/UKeyWorkThread.h"

static bool g_bIsInit = false;
bool rzt_openUKeyProc(CK_UKEYENUM_CALLBACK_FUNC pfUKeyEnum, CK_UKEYVERIFY_CALLBACK_FUNC pfUkeyVerify, CK_UKEYREAD_CALLBACK_FUNC pfUkeyReadData, CK_UKEYWRITE_CALLBACK_FUNC pfUKeyWriteData, HANDLE &hUKeyProc)
{
	bool bRet = false;
	CK_UKEYHANDLE *pUKeyHandle = NULL;
	CK_UKEYDEVICE stcUKeyDevice = {0};

	do 
	{
		if (pfUKeyEnum == NULL || pfUkeyVerify == NULL)
		{
			bRet = false;
			break;
		}

		if (pfUkeyReadData != NULL && pfUKeyWriteData != NULL)
		{
			bRet = false;
			break;
		}

		if (g_bIsInit)
		{
			if (!PKCS11_Initialize())
			{
				bRet = false;
				g_bIsInit = false;
				break;
			}
		}

		pUKeyHandle = new CK_UKEYHANDLE;
		if (pUKeyHandle == NULL)
		{
			bRet = false;
			break;
		}
		memset(pUKeyHandle, 0x0, sizeof(CK_UKEYHANDLE));

		pUKeyHandle->bIsInited = g_bIsInit;

		if (pfUKeyEnum != NULL)
		{
			UKeyEnumInitialize();

			if (!openUKeyEnumProc(pfUKeyEnum, pUKeyHandle->hEnumThread))
			{
				bRet = FALSE;
				break;
			}

			pUKeyHandle->bIsEnumThread = true;
		}

		if (pfUkeyVerify != NULL)
		{
			UKeyWorkInitialize();

			if (!openUKeyWorkProc(pfUkeyVerify, pfUkeyReadData, pfUKeyWriteData, pUKeyHandle->hWorkThread))
			{
				bRet = FALSE;
				break;
			}

			pUKeyHandle->bIsWorkThread = true;
		}
		
		bRet = TRUE;
	} while (false);

	return bRet;
}

bool rzt_closeUKeyProc()
{	
	bool bRet = false;

	do 
	{
		int i = 0;
	} while (false);

	return bRet;
}