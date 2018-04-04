#include "stdafx.h"
#include "UKeyVerify.h"

#include "./inc/Pkcs11_Ukey.h"
#include "./contrib/UKeyEnumThread.h"
#include "./contrib/UKeyWorkThread.h"

static bool g_bIsInit = false;
static std::vector<CK_UKEYHANDLE*> g_vecUKeyHandle;

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

		pUKeyHandle = new CK_UKEYHANDLE;
		if (pUKeyHandle == NULL)
		{
			bRet = false;
			break;
		}

		if (!g_bIsInit)
		{
			if (!PKCS11_Initialize(pUKeyHandle, false))
			{
				bRet = false;
				g_bIsInit = false;
				break;
			}

			g_bIsInit = true;
		}
		else
		{
			if (!PKCS11_Initialize(pUKeyHandle, true))
			{
				bRet = false;
				break;
			}
		}

		pUKeyHandle->bIsInited = g_bIsInit;
		pUKeyHandle->pfUKeyEnum = pfUKeyEnum;
		pUKeyHandle->pfUkeyVerify = pfUkeyVerify;
		pUKeyHandle->pfUkeyReadData = pfUkeyReadData;
		pUKeyHandle->pfUKeyWriteData = pfUKeyWriteData;

		if (pfUKeyEnum != NULL)
		{
			UKeyEnumInitialize(pUKeyHandle);
			if (!openUKeyEnumProc(pUKeyHandle))
			{
				bRet = false;
				break;
			}

			pUKeyHandle->bIsEnumThread = true;
		}

		if (pfUkeyVerify != NULL)
		{
			UKeyWorkInitialize(pUKeyHandle);
			if (!openUKeyWorkProc(pUKeyHandle))
			{
				bRet = false;
				break;
			}

			pUKeyHandle->bIsWorkThread = true;
		}
	
		hUKeyProc = (HANDLE)pUKeyHandle;
		g_vecUKeyHandle.push_back(pUKeyHandle);

		bRet = true;
	} while (false);

	if (!bRet && pUKeyHandle != NULL)
	{
		delete pUKeyHandle;
		pUKeyHandle = NULL;
	}

	return bRet;
}

bool rzt_closeUKeyProc(HANDLE hUKeyProc)
{	
	bool bRet = false;
	CK_UKEYHANDLE *pUKeyHandle = NULL;
	std::vector<CK_UKEYHANDLE*>::iterator iterUKeyHandle;

	do 
	{
		if (hUKeyProc == NULL || hUKeyProc == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		for (iterUKeyHandle=g_vecUKeyHandle.begin(); iterUKeyHandle!=g_vecUKeyHandle.end(); )
		{
			pUKeyHandle = (CK_UKEYHANDLE *)(*iterUKeyHandle);
			if (pUKeyHandle == NULL)
			{
				++iterUKeyHandle;
				continue;
			}

			if (memcmp(hUKeyProc, pUKeyHandle, sizeof(CK_UKEYHANDLE)) == 0)
			{
				if (!closeUKeyWorkProc(pUKeyHandle))
				{
					++iterUKeyHandle;
					continue;
				}

				if (!closeUKeyEnumProc(pUKeyHandle))
				{
					++iterUKeyHandle;
					continue;
				}

				if (pUKeyHandle != NULL)
				{
					delete pUKeyHandle;
					pUKeyHandle = NULL;
				}

				iterUKeyHandle = g_vecUKeyHandle.erase(iterUKeyHandle);
				break;
			}
			else
			{
				++iterUKeyHandle;
			}
		}
		
		bRet = true;
	} while (false);

	return bRet;
}