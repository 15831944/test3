#include "HYCspAPIWrapper.h"
#include <intrin.h>

HMODULE  g_hCspApiDll = NULL;

//1
HSListReaders_ 				HSListReaders = NULL;
HSWaitForDevEvent_ 			HSWaitForDevEvent = NULL;
HSSetDevChgEvent_ 			HSSetDevChgEvent = NULL;
HSGetDevState_ 				HSGetDevState = NULL;

//2
HSConnectDev_ 				HSConnectDev = NULL;
HSDisconnectDev_ 			HSDisconnectDev = NULL;
HSBeginTransaction_ 		HSBeginTransaction = NULL;
HSEndTransaction_ 			HSEndTransaction = NULL;
HSGetDeviceInfo_ 			HSGetDeviceInfo = NULL;
HSGetSerial_				HSGetSerial = NULL;
HSWriteLabel_				HSWriteLabel = NULL;
HSGetTotalSize_ 			HSGetTotalSize = NULL;
HSGetFreeSize_				HSGetFreeSize = NULL;
HSErase_					HSErase = NULL;
HSFormat_					HSFormat = NULL;
HSFormatEx_					HSFormatEx = NULL;
HSFormatEx2_				HSFormatEx2 = NULL;
HSFormatEx3_				HSFormatEx3 = NULL;

//3
HSCreateFile_ 				HSCreateFile = NULL;
HSWriteFile_ 				HSWriteFile = NULL;
HSReadFile_ 				HSReadFile = NULL;
HSDeleteFile_ 				HSDeleteFile = NULL;
HSGetFileSize_ 				HSGetFileSize = NULL;
HSGetFileRight_ 			HSGetFileRight = NULL;
HSHasFileExist_ 			HSHasFileExist = NULL;

//4
HSListContainers_ 			HSListContainers = NULL;
HSGetContainerNum_ 			HSGetContainerNum = NULL;
HSGetContainerParam_ 		HSGetContainerParam = NULL;
HSGetContainerName_ 		HSGetContainerName = NULL;
HSCreateContainer_ 			HSCreateContainer = NULL;
HSDelContainer_ 			HSDelContainer = NULL;
HSDelContainerItem_			HSDelContainerItem = NULL;

//5
HSCreateApp_ 				HSCreateApp = NULL;
HSSelectApp_ 				HSSelectApp = NULL;
HSCloseApp_ 				HSCloseApp = NULL;
HSDeleteApp_ 				HSDeleteApp = NULL;
HSListApp_ 					HSListApp = NULL;
HSGetHWApp_ 				HSGetHWApp = NULL;

//6
HSReadCert_ 				HSReadCert = NULL;
HSWriteCert_ 				HSWriteCert = NULL;
HSGetCertInfo_ 				HSGetCertInfo = NULL;
HSGetCertExtInfo_ 			HSGetCertExtInfo = NULL;
HSRegistCertByReaderName_ 	HSRegistCertByReaderName = NULL;
HSRegistCertByData_ 		HSRegistCertByData = NULL;
HSUnRegistAllCert_ 			HSUnRegistAllCert = NULL;
HSUnRegistCertByData_ 		HSUnRegistCertByData = NULL;
HSRegistCertByHandle_ 		HSRegistCertByHandle = NULL;

//7
HSVerifyUserPin_ 			HSVerifyUserPin = NULL;
HSChangeUserPin_ 			HSChangeUserPin = NULL;
HSVerifySOPin_ 				HSVerifySOPin = NULL;
HSChangeSOPin_ 				HSChangeSOPin = NULL;
HSReWriteUserPin_ 			HSReWriteUserPin = NULL;
HSWriteCachedPin_ 			HSWriteCachedPin = NULL;
HSReadCachedPin_ 			HSReadCachedPin = NULL;
HSClearPin_ 				HSClearPin = NULL;
HSClearAllPin_ 				HSClearAllPin = NULL;
HSGetUserPinRetryNum_ 		HSGetUserPinRetryNum = NULL;
HSGetSOPinRetryNum_ 		HSGetSOPinRetryNum = NULL;
HSGetUserPinRetryNumEx_ 	HSGetUserPinRetryNumEx = NULL;
HSGetSOPinRetryNumEx_ 		HSGetSOPinRetryNumEx = NULL;
HSReset_ 					HSReset = NULL;
HSGetChangePINFlage_	 	HSGetChangePINFlage = NULL;
HSDevAuth_ 					HSDevAuth = NULL;
HSChangeDevAuth_ 			HSChangeDevAuth = NULL;

//8
HSGenRSA_ 					HSGenRSA = NULL;
HSGenRSAEx_ 				HSGenRSAEx = NULL;
HSGenRSASoft_ 				HSGenRSASoft = NULL;
HSExportRSAPubKey_ 			HSExportRSAPubKey = NULL;
HSImportRSA_ 				HSImportRSA = NULL;
HSRSAEncrypt_ 				HSRSAEncrypt = NULL;
HSRSADecrypt_ 				HSRSADecrypt = NULL;	
HSRSAEncryptSoft_ 			HSRSAEncryptSoft = NULL;	
HSRSADecryptSoft_ 			HSRSADecryptSoft = NULL;
HSHashPadding_ 				HSHashPadding = NULL;
HSRSASign_ 					HSRSASign = NULL;
HSRSAVerify_ 				HSRSAVerify = NULL;
HSRSASignSoft_ 				HSRSASignSoft = NULL;
HSRSAVerifySoft_ 			HSRSAVerifySoft = NULL;

//9
HSWriteKey_ 				HSWriteKey = NULL;
HSSymEcb_ 					HSSymEcb = NULL;
HSSymSessionEcb_ 			HSSymSessionEcb = NULL;
HSGenRandom_ 				HSGenRandom = NULL;
HSRSAPad_ 					HSRSAPad = NULL;
HSDESEcb_ 					HSDESEcb = NULL;
HSDESCbc_ 					HSDESCbc = NULL;
HSSCB2Cbc_ 					HSSCB2Cbc = NULL;
HSSymInit_ 					HSSymInit = NULL;
HSSymUpdate_ 				HSSymUpdate = NULL;
HSSymFinal_ 				HSSymFinal = NULL;

//10
HSSHA1_ 					HSSHA1 = NULL;
HSMD5_ 						HSMD5 = NULL;
HSHashData_ 				HSHashData = NULL;
HSHashInit_ 				HSHashInit = NULL;
HSHashUpdate_ 				HSHashUpdate = NULL;
HSHashFinal_ 				HSHashFinal = NULL;

//11
HSExportRSA_ 				HSExportRSA = NULL;
HSImportRSAEx_ 				HSImportRSAEx = NULL;
HSPreSetSymKeyID_ 			HSPreSetSymKeyID = NULL;
HSCancelSetSymKeyID_ 		HSCancelSetSymKeyID = NULL;
HSRSASignLCD_ 				HSRSASignLCD = NULL;
HS33ImprotRSA_ 				HS33ImprotRSA = NULL;
HSWriteCharacter_ 			HSWriteCharacter = NULL;
HSRSASignBD_ 				HSRSASignBD = NULL;
BH_GetSignature_ 			BH_GetSignature = NULL;
BH_ShowData_ 				BH_ShowData = NULL;

//12
HSGenSM2KeyPair_ 			HSGenSM2KeyPair = NULL;
HSSM2SignByCon_ 			HSSM2SignByCon = NULL;
HSSM2DecryptByCon_ 			HSSM2DecryptByCon = NULL;
HSSM2Sign_ 					HSSM2Sign = NULL;
HSSM2Verify_ 				HSSM2Verify = NULL;
HSSM2Encrypt_ 				HSSM2Encrypt = NULL;
HSSM2Decrypt_ 				HSSM2Decrypt = NULL;

//13
HSSetTimepiece_				HSSetTimepiece = NULL;
HSGetTimepiece_				HSGetTimepiece = NULL;
HSGetTimepieceOfCipher_		HSGetTimepieceOfCipher = NULL;

//
LPFN_ISWOW64PROCESS_		fnIsWow64Process = NULL;
LPFN_GetSystemWow64DirectoryA_ fnGetSystemWow64DirectoryA = NULL;

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS_)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
	fnGetSystemWow64DirectoryA = (LPFN_GetSystemWow64DirectoryA_)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"GetSystemWow64DirectoryA");

	if ((fnIsWow64Process != NULL) && (fnGetSystemWow64DirectoryA != NULL))
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
		}
	}

	return bIsWow64;
}

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
	HMODULE hModule = NULL;

	char szFileDir[MAX_PATH] = {0};
	char szApiPath[MAX_PATH] = {0};

	MEMORY_BASIC_INFORMATION stModuleMemInfo = {0};
	VirtualQueryEx(GetCurrentProcess(), _ReturnAddress(), &stModuleMemInfo, sizeof(MEMORY_BASIC_INFORMATION));

	GetPathInsideDLL(szFileDir, MAX_PATH);

	strcpy_s(szApiPath, sizeof(szApiPath), szFileDir);
	strcat_s(szApiPath, sizeof(szApiPath), "\\");
	strcat_s(szApiPath, sizeof(szApiPath), HTCSPAPI_DLLNAME);

	g_hCspApiDll = ::LoadLibraryEx(szApiPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(g_hCspApiDll == NULL)
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
		strcat_s(szApiPath, sizeof(szApiPath), HTCSPAPI_DLLNAME);

		g_hCspApiDll = ::LoadLibraryEx(szApiPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		if(g_hCspApiDll == NULL)
		{
			return FALSE;
		}
	}
	
	//1
	HSListReaders = (HSListReaders_)GetProcAddress(g_hCspApiDll, "HSListReaders");
	HSWaitForDevEvent = (HSWaitForDevEvent_)GetProcAddress(g_hCspApiDll, "HSWaitForDevEvent");
	HSSetDevChgEvent = (HSSetDevChgEvent_)GetProcAddress(g_hCspApiDll, "HSSetDevChgEvent");
	HSGetDevState = (HSGetDevState_)GetProcAddress(g_hCspApiDll, "HSGetDevState");
	
	//2
	HSConnectDev = (HSConnectDev_)GetProcAddress(g_hCspApiDll, "HSConnectDev");
	HSDisconnectDev = (HSDisconnectDev_)GetProcAddress(g_hCspApiDll, "HSDisconnectDev");
	HSBeginTransaction = (HSBeginTransaction_)GetProcAddress(g_hCspApiDll, "HSBeginTransaction");
	HSEndTransaction = (HSEndTransaction_)GetProcAddress(g_hCspApiDll, "HSEndTransaction");
	HSGetDeviceInfo = (HSGetDeviceInfo_)GetProcAddress(g_hCspApiDll, "HSGetDeviceInfo");
	HSGetSerial	= (HSGetSerial_)GetProcAddress(g_hCspApiDll, "HSGetSerial");
	HSWriteLabel = (HSWriteLabel_)GetProcAddress(g_hCspApiDll, "HSWriteLabel");
	HSGetTotalSize = (HSGetTotalSize_)GetProcAddress(g_hCspApiDll, "HSGetTotalSize") ;
	HSGetFreeSize = (HSGetFreeSize_)GetProcAddress(g_hCspApiDll, "HSGetFreeSize");
	HSErase = (HSErase_)GetProcAddress(g_hCspApiDll, "HSErase");
	HSFormat = (HSFormat_)GetProcAddress(g_hCspApiDll, "HSFormat");
	HSFormatEx = (HSFormatEx_)GetProcAddress(g_hCspApiDll, "HSFormatEx");
	HSFormatEx2 = (HSFormatEx2_)GetProcAddress(g_hCspApiDll, "HSFormatEx2");
	HSFormatEx3 = (HSFormatEx3_)GetProcAddress(g_hCspApiDll, "HSFormatEx3");
	
	//3
	HSCreateFile = (HSCreateFile_)GetProcAddress(g_hCspApiDll, "HSCreateFile");
	HSWriteFile = (HSWriteFile_)GetProcAddress(g_hCspApiDll, "HSWriteFile");
	HSReadFile = (HSReadFile_)GetProcAddress(g_hCspApiDll, "HSReadFile");
	HSDeleteFile = (HSDeleteFile_)GetProcAddress(g_hCspApiDll, "HSDeleteFile");
	HSGetFileSize = (HSGetFileSize_)GetProcAddress(g_hCspApiDll, "HSGetFileSize");
	HSGetFileRight = (HSGetFileRight_)GetProcAddress(g_hCspApiDll, "HSGetFileRight");
	HSHasFileExist = (HSHasFileExist_)GetProcAddress(g_hCspApiDll, "HSHasFileExist");
	
	//4
	HSListContainers = (HSListContainers_)GetProcAddress(g_hCspApiDll, "HSListContainers");
	HSGetContainerNum = (HSGetContainerNum_)GetProcAddress(g_hCspApiDll, "HSGetContainerNum");
	HSGetContainerParam = (HSGetContainerParam_)GetProcAddress(g_hCspApiDll, "HSGetContainerParam");
	HSGetContainerName = (HSGetContainerName_)GetProcAddress(g_hCspApiDll, "HSGetContainerName");
	HSCreateContainer = (HSCreateContainer_)GetProcAddress(g_hCspApiDll, "HSCreateContainer");
	HSDelContainer = (HSDelContainer_)GetProcAddress(g_hCspApiDll, "HSDelContainer");
	HSDelContainerItem = (HSDelContainerItem_)GetProcAddress(g_hCspApiDll, "HSDelContainerItem");
	
	//5
	HSCreateApp = (HSCreateApp_)GetProcAddress(g_hCspApiDll, "HSCreateApp");
	HSSelectApp = (HSSelectApp_)GetProcAddress(g_hCspApiDll, "HSSelectApp");
	HSCloseApp = (HSCloseApp_)GetProcAddress(g_hCspApiDll, "HSCloseApp");
	HSDeleteApp = (HSDeleteApp_)GetProcAddress(g_hCspApiDll, "HSDeleteApp");
	HSListApp = (HSListApp_)GetProcAddress(g_hCspApiDll, "HSListApp");
	HSGetHWApp = (HSGetHWApp_)GetProcAddress(g_hCspApiDll, "HSGetHWApp");
	
	//6
	HSReadCert = (HSReadCert_)GetProcAddress(g_hCspApiDll, "HSReadCert");
	HSWriteCert = (HSWriteCert_)GetProcAddress(g_hCspApiDll, "HSWriteCert");
	HSGetCertInfo = (HSGetCertInfo_)GetProcAddress(g_hCspApiDll, "HSGetCertInfo");
	HSGetCertExtInfo = (HSGetCertExtInfo_)GetProcAddress(g_hCspApiDll, "HSGetCertExtInfo");
	HSRegistCertByReaderName = (HSRegistCertByReaderName_)GetProcAddress(g_hCspApiDll, "HSRegistCertByReaderName");
	HSRegistCertByData = (HSRegistCertByData_)GetProcAddress(g_hCspApiDll, "HSRegistCertByData");
	HSUnRegistAllCert = (HSUnRegistAllCert_)GetProcAddress(g_hCspApiDll, "HSUnRegistAllCert");
	HSUnRegistCertByData = (HSUnRegistCertByData_)GetProcAddress(g_hCspApiDll, "HSUnRegistCertByData");
	HSRegistCertByHandle = (HSRegistCertByHandle_)GetProcAddress(g_hCspApiDll, "HSRegistCertByHandle");
	
	//7
	HSVerifyUserPin = (HSVerifyUserPin_)GetProcAddress(g_hCspApiDll, "HSVerifyUserPin");
	HSChangeUserPin = (HSChangeUserPin_)GetProcAddress(g_hCspApiDll, "HSChangeUserPin");
	HSVerifySOPin = (HSVerifySOPin_)GetProcAddress(g_hCspApiDll, "HSVerifySOPin");
	HSChangeSOPin = (HSChangeSOPin_)GetProcAddress(g_hCspApiDll, "HSChangeSOPin");
	HSReWriteUserPin = (HSReWriteUserPin_)GetProcAddress(g_hCspApiDll, "HSReWriteUserPin");
	HSWriteCachedPin = (HSWriteCachedPin_)GetProcAddress(g_hCspApiDll, "HSWriteCachedPin");
	HSReadCachedPin = (HSReadCachedPin_)GetProcAddress(g_hCspApiDll, "HSReadCachedPin");
	HSClearPin = (HSClearPin_)GetProcAddress(g_hCspApiDll, "HSClearPin");
	HSClearAllPin = (HSClearAllPin_)GetProcAddress(g_hCspApiDll, "HSClearAllPin");
	HSGetUserPinRetryNum = (HSGetUserPinRetryNum_)GetProcAddress(g_hCspApiDll, "HSGetUserPinRetryNum");
	HSGetSOPinRetryNum = (HSGetSOPinRetryNum_)GetProcAddress(g_hCspApiDll, "HSGetSOPinRetryNum");
	HSGetUserPinRetryNumEx = (HSGetUserPinRetryNumEx_)GetProcAddress(g_hCspApiDll, "HSGetUserPinRetryNumEx");
	HSGetSOPinRetryNumEx = (HSGetSOPinRetryNumEx_)GetProcAddress(g_hCspApiDll, "HSGetSOPinRetryNumEx");
	HSReset = (HSReset_)GetProcAddress(g_hCspApiDll, "HSReset");
	HSGetChangePINFlage = (HSGetChangePINFlage_)GetProcAddress(g_hCspApiDll, "HSGetChangePINFlage");
	HSDevAuth = (HSDevAuth_)GetProcAddress(g_hCspApiDll, "HSDevAuth");
	HSChangeDevAuth = (HSChangeDevAuth_)GetProcAddress(g_hCspApiDll, "HSChangeDevAuth");
	
	//8
	HSGenRSA = (HSGenRSA_)GetProcAddress(g_hCspApiDll, "HSGenRSA");
	HSGenRSAEx = (HSGenRSAEx_)GetProcAddress(g_hCspApiDll, "HSGenRSAEx");
	HSGenRSASoft = (HSGenRSASoft_)GetProcAddress(g_hCspApiDll, "HSGenRSASoft");
	HSExportRSAPubKey = (HSExportRSAPubKey_)GetProcAddress(g_hCspApiDll, "HSExportRSAPubKey");
	HSImportRSA = (HSImportRSA_)GetProcAddress(g_hCspApiDll, "HSImportRSA");
	HSRSAEncrypt = (HSRSAEncrypt_)GetProcAddress(g_hCspApiDll, "HSRSAEncrypt");
	HSRSADecrypt = (HSRSADecrypt_)GetProcAddress(g_hCspApiDll, "HSRSADecrypt");
	HSRSAEncryptSoft = (HSRSAEncryptSoft_)GetProcAddress(g_hCspApiDll, "HSRSAEncryptSoft");
	HSRSADecryptSoft = (HSRSADecryptSoft_)GetProcAddress(g_hCspApiDll, "HSRSADecryptSoft");
	HSHashPadding = (HSHashPadding_)GetProcAddress(g_hCspApiDll, "HSHashPadding");
	HSRSASign = (HSRSASign_)GetProcAddress(g_hCspApiDll, "HSRSASign");
	HSRSAVerify = (HSRSAVerify_)GetProcAddress(g_hCspApiDll, "HSRSAVerify");
	HSRSASignSoft = (HSRSASignSoft_)GetProcAddress(g_hCspApiDll, "HSRSASignSoft");
	HSRSAVerifySoft = (HSRSAVerifySoft_)GetProcAddress(g_hCspApiDll, "HSRSAVerifySoft");
	
	//9
	HSWriteKey = (HSWriteKey_)GetProcAddress(g_hCspApiDll, "HSWriteKey");
	HSSymEcb = (HSSymEcb_)GetProcAddress(g_hCspApiDll, "HSSymEcb");
	HSSymSessionEcb = (HSSymSessionEcb_)GetProcAddress(g_hCspApiDll, "HSSymSessionEcb");
	HSGenRandom = (HSGenRandom_)GetProcAddress(g_hCspApiDll, "HSGenRandom");
	HSRSAPad = (HSRSAPad_)GetProcAddress(g_hCspApiDll, "HSRSAPad");
	HSDESEcb = (HSDESEcb_)GetProcAddress(g_hCspApiDll, "HSDESEcb");
	HSDESCbc = (HSDESCbc_)GetProcAddress(g_hCspApiDll, "HSDESCbc");
	HSSCB2Cbc = (HSSCB2Cbc_)GetProcAddress(g_hCspApiDll, "HSSCB2Cbc");
	HSSymInit = (HSSymInit_)GetProcAddress(g_hCspApiDll, "HSSymInit");
	HSSymUpdate = (HSSymUpdate_)GetProcAddress(g_hCspApiDll, "HSSymUpdate");
	HSSymFinal = (HSSymFinal_)GetProcAddress(g_hCspApiDll, "HSSymFinal");
	
	//10
	HSSHA1 = (HSSHA1_)GetProcAddress(g_hCspApiDll, "HSSHA1");
	HSMD5 = (HSMD5_)GetProcAddress(g_hCspApiDll, "HSMD5");
	HSHashData = (HSHashData_)GetProcAddress(g_hCspApiDll, "HSHashData");
	HSHashInit = (HSHashInit_)GetProcAddress(g_hCspApiDll, "HSHashInit");
	HSHashUpdate = (HSHashUpdate_)GetProcAddress(g_hCspApiDll, "HSHashUpdate");
	HSHashFinal = (HSHashFinal_)GetProcAddress(g_hCspApiDll, "HSHashFinal");
	
	//11
	HSExportRSA = (HSExportRSA_)GetProcAddress(g_hCspApiDll, "HSExportRSA");
	HSImportRSAEx = (HSImportRSAEx_)GetProcAddress(g_hCspApiDll, "HSImportRSAEx");
	HSPreSetSymKeyID = (HSPreSetSymKeyID_)GetProcAddress(g_hCspApiDll, "HSPreSetSymKeyID");
	HSCancelSetSymKeyID = (HSCancelSetSymKeyID_)GetProcAddress(g_hCspApiDll, "HSCancelSetSymKeyID");
	HSRSASignLCD = (HSRSASignLCD_)GetProcAddress(g_hCspApiDll, "HSRSASignLCD");
	HS33ImprotRSA = (HS33ImprotRSA_)GetProcAddress(g_hCspApiDll, "HS33ImprotRSA");
	HSWriteCharacter = (HSWriteCharacter_)GetProcAddress(g_hCspApiDll, "HSWriteCharacter");
	HSRSASignBD = (HSRSASignBD_)GetProcAddress(g_hCspApiDll, "HSRSASignBD");
	BH_GetSignature = (BH_GetSignature_)GetProcAddress(g_hCspApiDll, "BH_GetSignature");
	BH_ShowData = (BH_ShowData_)GetProcAddress(g_hCspApiDll, "BH_ShowData");
	
	//12
	HSGenSM2KeyPair = (HSGenSM2KeyPair_)GetProcAddress(g_hCspApiDll, "HSGenSM2KeyPair");
	HSSM2SignByCon = (HSSM2SignByCon_)GetProcAddress(g_hCspApiDll, "HSSM2SignByCon");
	HSSM2DecryptByCon = (HSSM2DecryptByCon_)GetProcAddress(g_hCspApiDll, "HSSM2DecryptByCon");
	HSSM2Sign = (HSSM2Sign_)GetProcAddress(g_hCspApiDll, "HSSM2Sign");
	HSSM2Verify = (HSSM2Verify_)GetProcAddress(g_hCspApiDll, "HSSM2Verify");
	HSSM2Encrypt = (HSSM2Encrypt_)GetProcAddress(g_hCspApiDll, "HSSM2Encrypt");
	HSSM2Decrypt = (HSSM2Decrypt_)GetProcAddress(g_hCspApiDll, "HSSM2Decrypt");
	
	//13
	HSSetTimepiece = (HSSetTimepiece_)GetProcAddress(g_hCspApiDll, "HSSetTimepiece");
	HSGetTimepiece = (HSGetTimepiece_)GetProcAddress(g_hCspApiDll, "HSGetTimepiece");
	HSGetTimepieceOfCipher = (HSGetTimepieceOfCipher_)GetProcAddress(g_hCspApiDll, "HSGetTimepieceOfCipher");

	return TRUE;
}

BOOL Function_Finalize()
{
	if(g_hCspApiDll != NULL)
	{
		FreeLibrary(g_hCspApiDll);
		g_hCspApiDll = NULL;
	}
	
	return TRUE;
}