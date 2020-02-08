#include "HTSkfAPIWrapper.h"

HMODULE	g_hSkfApiDll = NULL;

__SKF_WaitForDevEvent 			SKF_WaitForDevEvent = NULL;
__SKF_CancelWaitForDevEvent		SKF_CancelWaitForDevEvent = NULL;
__SKF_EnumDev					SKF_EnumDev = NULL;
__SKF_ConnectDev				SKF_ConnectDev = NULL;
__SKF_DisConnectDev				SKF_DisConnectDev = NULL;
__SKF_GetDevState				SKF_GetDevState = NULL;
__SKF_SetLabel					SKF_SetLabel = NULL;
__SKF_GetDevInfo				SKF_GetDevInfo = NULL;
__SKF_LockDev					SKF_LockDev = NULL;
__SKF_UnlockDev					SKF_UnlockDev = NULL;
__SKF_Transmit					SKF_Transmit = NULL;
__SKF_ChangeDevAuthKey			SKF_ChangeDevAuthKey = NULL;
__SKF_DevAuth					SKF_DevAuth = NULL;
__SKF_ChangePIN					SKF_ChangePIN = NULL;
__SKF_GetPINInfo				SKF_GetPINInfo = NULL;
__SKF_VerifyPIN					SKF_VerifyPIN = NULL;
__SKF_UnblockPIN				SKF_UnblockPIN = NULL;
__SKF_ClearSecureState			SKF_ClearSecureState = NULL;

__SKF_CreateApplication			SKF_CreateApplication = NULL;
__SKF_EnumApplication			SKF_EnumApplication = NULL;
__SKF_DeleteApplication			SKF_DeleteApplication = NULL;
__SKF_OpenApplication			SKF_OpenApplication = NULL;
__SKF_CloseApplication			SKF_CloseApplication = NULL;

__SKF_CreateFile				SKF_CreateFile = NULL;
__SKF_DeleteFile				SKF_DeleteFile = NULL;
__SKF_EnumFiles					SKF_EnumFiles = NULL;
__SKF_GetFileInfo				SKF_GetFileInfo = NULL;
__SKF_ReadFile					SKF_ReadFile = NULL;
__SKF_WriteFile					SKF_WriteFile = NULL;

__SKF_CreateContainer			SKF_CreateContainer = NULL;
__SKF_DeleteContainer			SKF_DeleteContainer = NULL;
__SKF_OpenContainer				SKF_OpenContainer = NULL;
__SKF_CloseContainer			SKF_CloseContainer = NULL;
__SKF_EnumContainer				SKF_EnumContainer = NULL;
__SKF_GetContainerType			SKF_GetContainerType = NULL;

__SKF_GenRandom					SKF_GenRandom = NULL;
__SKF_GenExtRSAKey				SKF_GenExtRSAKey = NULL;
__SKF_GenRSAKeyPair				SKF_GenRSAKeyPair = NULL;
__SKF_ImportRSAKeyPair			SKF_ImportRSAKeyPair = NULL;
__SKF_RSASignData				SKF_RSASignData = NULL;
__SKF_RSAVerify					SKF_RSAVerify = NULL;
__SKF_RSAExportSessionKey		SKF_RSAExportSessionKey = NULL;
__SKF_ExtRSAPubKeyOperation		SKF_ExtRSAPubKeyOperation = NULL;
__SKF_ExtRSAPriKeyOperation		SKF_ExtRSAPriKeyOperation = NULL;
__SKF_GenECCKeyPair				SKF_GenECCKeyPair = NULL;
__SKF_ImportECCKeyPair			SKF_ImportECCKeyPair = NULL;
__SKF_ECCSignData				SKF_ECCSignData = NULL;
__SKF_ECCVerify					SKF_ECCVerify = NULL;
__SKF_ECCExportSessionKey		SKF_ECCExportSessionKey = NULL;
__SKF_ExtECCEncrypt				SKF_ExtECCEncrypt = NULL;
__SKF_ExtECCDecrypt				SKF_ExtECCDecrypt = NULL;
__SKF_ECCDecrypt				SKF_ECCDecrypt = NULL;
__SKF_ExtECCSign				SKF_ExtECCSign = NULL;
__SKF_ExtECCVerify				SKF_ExtECCVerify = NULL;

__SKF_GenerateAgreementDataWithECC 			SKF_GenerateAgreementDataWithECC = NULL;
__SKF_GenerateAgreementDataAndKeyWithECC	SKF_GenerateAgreementDataAndKeyWithECC = NULL;

__SKF_GenerateKeyWithECC		SKF_GenerateKeyWithECC = NULL;
__SKF_ExportPublicKey			SKF_ExportPublicKey = NULL;
__SKF_ImportSessionKey			SKF_ImportSessionKey = NULL;
__SKF_SetSymmKey				SKF_SetSymmKey = NULL;
__SKF_EncryptInit				SKF_EncryptInit = NULL;
__SKF_Encrypt					SKF_Encrypt = NULL;
__SKF_EncryptUpdate				SKF_EncryptUpdate = NULL;
__SKF_EncryptFinal				SKF_EncryptFinal = NULL;
__SKF_DecryptInit				SKF_DecryptInit = NULL;
__SKF_Decrypt					SKF_Decrypt = NULL;
__SKF_DecryptUpdate				SKF_DecryptUpdate = NULL;
__SKF_DecryptFinal				SKF_DecryptFinal = NULL;
__SKF_DigestInit				SKF_DigestInit = NULL;
__SKF_Digest					SKF_Digest = NULL;
__SKF_DigestUpdate				SKF_DigestUpdate = NULL;
__SKF_DigestFinal				SKF_DigestFinal = NULL;
__SKF_MacInit					SKF_MacInit = NULL;
__SKF_Mac						SKF_Mac = NULL;
__SKF_MacUpdate					SKF_MacUpdate = NULL;
__SKF_MacFinal					SKF_MacFinal = NULL;
__SKF_CloseHandle				SKF_CloseHandle = NULL;

//
LPFN_ISWOW64PROCESS_			fnIsWow64Process = NULL;
LPFN_GetSystemWow64DirectoryA_	fnGetSystemWow64DirectoryA = NULL;

//
BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS_)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
	fnGetSystemWow64DirectoryA = (LPFN_GetSystemWow64DirectoryA_)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"GetSystemWow64DirectoryA");

	if ( (NULL != fnIsWow64Process)&& (NULL != fnGetSystemWow64DirectoryA))
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
		}
	}

	return bIsWow64;
}

//
BOOL GetPathInsideDLL(char *szOutDllPath, DWORD dwOutDllPathLen)
{
	DWORD dwPathLen = 0;
	char* p = NULL;

	char szFullPath[MAX_PATH] = {0};
	char szDirPath[MAX_PATH] = {0};

	if (szOutDllPath == NULL)
	{
		return FALSE;
	}

	if (!GetModuleFileName(NULL, szFullPath, MAX_PATH))
	{
		return FALSE;
	}

	p = strrchr(szFullPath, '\\');
	if (p == NULL)
	{
		return FALSE;
	}

	dwPathLen = p - szFullPath;
	strncpy(szDirPath, szFullPath, dwPathLen);

	if (dwOutDllPathLen < dwPathLen)
	{
		return FALSE;
	}

	strcpy(szOutDllPath, szDirPath);
	return TRUE;
}

BOOL Function_Init()
{
	BOOL bIsWOW64 = FALSE;

	char szFileDir[MAX_PATH] = {0};
	char szApiPath[MAX_PATH] = {0};

	GetPathInsideDLL(szFileDir, MAX_PATH);

	strcpy_s(szApiPath, sizeof(szApiPath), szFileDir);
	strcat_s(szApiPath, sizeof(szApiPath), "\\");
	strcat_s(szApiPath, sizeof(szApiPath), HTSKFAPI_DLLNAME);

	g_hSkfApiDll = ::LoadLibraryEx(szApiPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(g_hSkfApiDll == NULL)
	{
		memset(szFileDir, 0x0, MAX_PATH);
		memset(szApiPath, 0x0, MAX_PATH);

		bIsWOW64 = IsWow64();
		if (bIsWOW64)
		{
			fnGetSystemWow64DirectoryA(szFileDir, sizeof(szFileDir));
		}
		else
		{
			GetSystemDirectory(szFileDir, sizeof(szFileDir));
		}

		strcpy_s(szApiPath, sizeof(szApiPath), szFileDir);
		strcat_s(szApiPath, sizeof(szApiPath), "\\");
		strcat_s(szApiPath, sizeof(szApiPath), HTSKFAPI_DLLNAME);

		g_hSkfApiDll = ::LoadLibraryEx(szApiPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		if(g_hSkfApiDll == NULL)
		{
			return FALSE;
		}
	}
	
	SKF_WaitForDevEvent                    	= (__SKF_WaitForDevEvent)                    	GetProcAddress(g_hSkfApiDll, "SKF_WaitForDevEvent");
	SKF_CancelWaitForDevEvent              	= (__SKF_CancelWaitForDevEvent)              	GetProcAddress(g_hSkfApiDll, "SKF_CancelWaitForDevEvent");
	SKF_EnumDev                            	= (__SKF_EnumDev)                            	GetProcAddress(g_hSkfApiDll, "SKF_EnumDev");
	SKF_ConnectDev                         	= (__SKF_ConnectDev)                         	GetProcAddress(g_hSkfApiDll, "SKF_ConnectDev");
	SKF_DisConnectDev                      	= (__SKF_DisConnectDev)                     	GetProcAddress(g_hSkfApiDll, "SKF_DisConnectDev");
	SKF_GetDevState                        	= (__SKF_GetDevState)                        	GetProcAddress(g_hSkfApiDll, "SKF_GetDevState");
	SKF_SetLabel                           	= (__SKF_SetLabel)                           	GetProcAddress(g_hSkfApiDll, "SKF_SetLabel");
	SKF_GetDevInfo                         	= (__SKF_GetDevInfo)                         	GetProcAddress(g_hSkfApiDll, "SKF_GetDevInfo");
	SKF_LockDev                            	= (__SKF_LockDev)                            	GetProcAddress(g_hSkfApiDll, "SKF_LockDev");
	SKF_UnlockDev                          	= (__SKF_UnlockDev)                          	GetProcAddress(g_hSkfApiDll, "SKF_UnlockDev");
	SKF_Transmit                           	= (__SKF_Transmit)                           	GetProcAddress(g_hSkfApiDll, "SKF_Transmit");

	SKF_ChangeDevAuthKey                   	= (__SKF_ChangeDevAuthKey)                   	GetProcAddress(g_hSkfApiDll, "SKF_ChangeDevAuthKey");
	SKF_DevAuth                            	= (__SKF_DevAuth)                            	GetProcAddress(g_hSkfApiDll, "SKF_DevAuth");
	SKF_ChangePIN                          	= (__SKF_ChangePIN)                          	GetProcAddress(g_hSkfApiDll, "SKF_ChangePIN");
	SKF_GetPINInfo                         	= (__SKF_GetPINInfo)                         	GetProcAddress(g_hSkfApiDll, "SKF_GetPINInfo");
	SKF_VerifyPIN                          	= (__SKF_VerifyPIN)                          	GetProcAddress(g_hSkfApiDll, "SKF_VerifyPIN");
	SKF_UnblockPIN                         	= (__SKF_UnblockPIN)                         	GetProcAddress(g_hSkfApiDll, "SKF_UnblockPIN");
	SKF_ClearSecureState                   	= (__SKF_ClearSecureState)                   	GetProcAddress(g_hSkfApiDll, "SKF_ClearSecureState");

	SKF_CreateApplication                  	= (__SKF_CreateApplication)                  	GetProcAddress(g_hSkfApiDll, "SKF_CreateApplication");
	SKF_EnumApplication                    	= (__SKF_EnumApplication)                    	GetProcAddress(g_hSkfApiDll, "SKF_EnumApplication");
	SKF_DeleteApplication                 	= (__SKF_DeleteApplication)                  	GetProcAddress(g_hSkfApiDll, "SKF_DeleteApplication");
	SKF_OpenApplication                    	= (__SKF_OpenApplication)                    	GetProcAddress(g_hSkfApiDll, "SKF_OpenApplication");
	SKF_CloseApplication                   	= (__SKF_CloseApplication)                   	GetProcAddress(g_hSkfApiDll, "SKF_CloseApplication");

	SKF_CreateFile                         	= (__SKF_CreateFile)                         	GetProcAddress(g_hSkfApiDll, "SKF_CreateFile");
	SKF_DeleteFile                         	= (__SKF_DeleteFile)                         	GetProcAddress(g_hSkfApiDll, "SKF_DeleteFile");
	SKF_EnumFiles                          	= (__SKF_EnumFiles)                          	GetProcAddress(g_hSkfApiDll, "SKF_EnumFiles");
	SKF_GetFileInfo                        	= (__SKF_GetFileInfo)                        	GetProcAddress(g_hSkfApiDll, "SKF_GetFileInfo");
	SKF_ReadFile                           	= (__SKF_ReadFile)                           	GetProcAddress(g_hSkfApiDll, "SKF_ReadFile");
	SKF_WriteFile                          	= (__SKF_WriteFile)                          	GetProcAddress(g_hSkfApiDll, "SKF_WriteFile");

	SKF_CreateContainer                    	= (__SKF_CreateContainer)                    	GetProcAddress(g_hSkfApiDll, "SKF_CreateContainer");
	SKF_DeleteContainer                    	= (__SKF_DeleteContainer)                    	GetProcAddress(g_hSkfApiDll, "SKF_DeleteContainer");
	SKF_OpenContainer                      	= (__SKF_OpenContainer)                      	GetProcAddress(g_hSkfApiDll, "SKF_OpenContainer");
	SKF_CloseContainer                     	= (__SKF_CloseContainer)                     	GetProcAddress(g_hSkfApiDll, "SKF_CloseContainer");
	SKF_EnumContainer                      	= (__SKF_EnumContainer)                      	GetProcAddress(g_hSkfApiDll, "SKF_EnumContainer");
	SKF_GetContainerType                   	= (__SKF_GetContainerType)                   	GetProcAddress(g_hSkfApiDll, "SKF_GetContainerType");

	SKF_GenRandom                          	= (__SKF_GenRandom)                          	GetProcAddress(g_hSkfApiDll, "SKF_GenRandom");
	SKF_GenExtRSAKey                       	= (__SKF_GenExtRSAKey)                       	GetProcAddress(g_hSkfApiDll, "SKF_GenExtRSAKey");
	SKF_GenRSAKeyPair                      	= (__SKF_GenRSAKeyPair)                      	GetProcAddress(g_hSkfApiDll, "SKF_GenRSAKeyPair");
	SKF_ImportRSAKeyPair                   	= (__SKF_ImportRSAKeyPair)                   	GetProcAddress(g_hSkfApiDll, "SKF_ImportRSAKeyPair");
	SKF_RSASignData                        	= (__SKF_RSASignData)                        	GetProcAddress(g_hSkfApiDll, "SKF_RSASignData");
	SKF_RSAVerify                          	= (__SKF_RSAVerify)                          	GetProcAddress(g_hSkfApiDll, "SKF_RSAVerify");
	SKF_RSAExportSessionKey                	= (__SKF_RSAExportSessionKey)                	GetProcAddress(g_hSkfApiDll, "SKF_RSAExportSessionKey");
	SKF_ExtRSAPubKeyOperation              	= (__SKF_ExtRSAPubKeyOperation)              	GetProcAddress(g_hSkfApiDll, "SKF_ExtRSAPubKeyOperation");
	SKF_ExtRSAPriKeyOperation              	= (__SKF_ExtRSAPriKeyOperation)              	GetProcAddress(g_hSkfApiDll, "SKF_ExtRSAPriKeyOperation");
	SKF_GenECCKeyPair                      	= (__SKF_GenECCKeyPair)                      	GetProcAddress(g_hSkfApiDll, "SKF_GenECCKeyPair");
	SKF_ImportECCKeyPair                   	= (__SKF_ImportECCKeyPair)                   	GetProcAddress(g_hSkfApiDll, "SKF_ImportECCKeyPair");
	SKF_ECCSignData                       	= (__SKF_ECCSignData)                        	GetProcAddress(g_hSkfApiDll, "SKF_ECCSignData");
	SKF_ECCVerify                          	= (__SKF_ECCVerify)                          	GetProcAddress(g_hSkfApiDll, "SKF_ECCVerify");
	SKF_ECCExportSessionKey                	= (__SKF_ECCExportSessionKey)                	GetProcAddress(g_hSkfApiDll, "SKF_ECCExportSessionKey");
	SKF_ExtECCEncrypt                      	= (__SKF_ExtECCEncrypt)                      	GetProcAddress(g_hSkfApiDll, "SKF_ExtECCEncrypt");
	SKF_ExtECCDecrypt                      	= (__SKF_ExtECCDecrypt)                      	GetProcAddress(g_hSkfApiDll, "SKF_ExtECCDecrypt");
	SKF_ECCDecrypt                         	= (__SKF_ECCDecrypt)                         	GetProcAddress(g_hSkfApiDll, "SKF_ECCDecrypt");
	SKF_ExtECCSign                         	= (__SKF_ExtECCSign)                         	GetProcAddress(g_hSkfApiDll, "SKF_ExtECCSign");
	SKF_ExtECCVerify                       	= (__SKF_ExtECCVerify)                       	GetProcAddress(g_hSkfApiDll, "SKF_ExtECCVerify");
	SKF_GenerateAgreementDataWithECC       	= (__SKF_GenerateAgreementDataWithECC)       	GetProcAddress(g_hSkfApiDll, "SKF_GenerateAgreementDataWithECC");
	SKF_GenerateAgreementDataAndKeyWithECC 	= (__SKF_GenerateAgreementDataAndKeyWithECC) 	GetProcAddress(g_hSkfApiDll, "SKF_GenerateAgreementDataAndKeyWithECC");
	SKF_GenerateKeyWithECC                 	= (__SKF_GenerateKeyWithECC)                 	GetProcAddress(g_hSkfApiDll, "SKF_GenerateKeyWithECC");
	SKF_ExportPublicKey                    	= (__SKF_ExportPublicKey)                    	GetProcAddress(g_hSkfApiDll, "SKF_ExportPublicKey");
	SKF_ImportSessionKey                   	= (__SKF_ImportSessionKey)                   	GetProcAddress(g_hSkfApiDll, "SKF_ImportSessionKey");
	SKF_SetSymmKey                         	= (__SKF_SetSymmKey)                         	GetProcAddress(g_hSkfApiDll, "SKF_SetSymmKey");
	SKF_EncryptInit                        	= (__SKF_EncryptInit)                        	GetProcAddress(g_hSkfApiDll, "SKF_EncryptInit");
	SKF_Encrypt                            	= (__SKF_Encrypt)                            	GetProcAddress(g_hSkfApiDll, "SKF_Encrypt");
	SKF_EncryptUpdate                      	= (__SKF_EncryptUpdate)                      	GetProcAddress(g_hSkfApiDll, "SKF_EncryptUpdate");
	SKF_EncryptFinal                       	= (__SKF_EncryptFinal)                       	GetProcAddress(g_hSkfApiDll, "SKF_EncryptFinal");
	SKF_DecryptInit                        	= (__SKF_DecryptInit)                        	GetProcAddress(g_hSkfApiDll, "SKF_DecryptInit");
	SKF_Decrypt                            	= (__SKF_Decrypt)                            	GetProcAddress(g_hSkfApiDll, "SKF_Decrypt");
	SKF_DecryptUpdate                      	= (__SKF_DecryptUpdate)                      	GetProcAddress(g_hSkfApiDll, "SKF_DecryptUpdate");
	SKF_DecryptFinal                       	= (__SKF_DecryptFinal)                       	GetProcAddress(g_hSkfApiDll, "SKF_DecryptFinal");
	SKF_DigestInit                         	= (__SKF_DigestInit)                         	GetProcAddress(g_hSkfApiDll, "SKF_DigestInit");
	SKF_Digest                             	= (__SKF_Digest)                             	GetProcAddress(g_hSkfApiDll, "SKF_Digest");
	SKF_DigestUpdate                       	= (__SKF_DigestUpdate)                       	GetProcAddress(g_hSkfApiDll, "SKF_DigestUpdate");
	SKF_DigestFinal                        	= (__SKF_DigestFinal)                        	GetProcAddress(g_hSkfApiDll, "SKF_DigestFinal");
	SKF_MacInit                            	= (__SKF_MacInit)                            	GetProcAddress(g_hSkfApiDll, "SKF_MacInit");
	SKF_Mac                                	= (__SKF_Mac)                                	GetProcAddress(g_hSkfApiDll, "SKF_Mac");
	SKF_MacUpdate                          	= (__SKF_MacUpdate)                          	GetProcAddress(g_hSkfApiDll, "SKF_MacUpdate");
	SKF_MacFinal                           	= (__SKF_MacFinal)                           	GetProcAddress(g_hSkfApiDll, "SKF_MacFinal");
	SKF_CloseHandle                        	= (__SKF_CloseHandle)                        	GetProcAddress(g_hSkfApiDll, "SKF_CloseHandle");
	
	return TRUE;
}

BOOL Function_Finalize()
{
	if(g_hSkfApiDll != NULL)
	{
		FreeLibrary(g_hSkfApiDll);
		g_hSkfApiDll = NULL;
	}
	
	return TRUE;
}