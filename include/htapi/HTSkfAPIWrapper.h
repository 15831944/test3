#ifndef __HTSKFAPIWRAPPER_H__
#define __HTSKFAPIWRAPPER_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <windows.h>

#define HTSKFAPI_DLLNAME			"htfyedu.dll"
extern HMODULE	g_hSkfApiDll;

BOOL Function_Init();
BOOL Function_Finalize();

//////////////////////////////////////////////////////////////////////////
//
#pragma pack(1)

//返回值定义
#define SAR_OK							0x00000000
#define SAR_FAIL						0x0A000001
#define SAR_UNKNOWNERR					0x0A000002
#define SAR_NOTSUPPORTYETERR			0x0A000003
#define SAR_FILEERR						0x0A000004
#define SAR_INVALIDHANDLEERR			0x0A000005
#define SAR_INVALIDPARAMERR				0x0A000006
#define SAR_READFILEERR					0x0A000007
#define SAR_WRITEFILEERR				0x0A000008
#define SAR_NAMELENERR					0x0A000009
#define SAR_KEYUSAGEERR					0x0A00000A
#define SAR_MODULUSLENERR				0x0A00000B
#define SAR_NOTINITIALIZEERR			0x0A00000C
#define SAR_OBJERR						0x0A00000D
#define SAR_MEMORYERR					0x0A00000E
#define SAR_TIMEOUTERR					0x0A00000F
#define SAR_INDATALENERR				0x0A000010
#define SAR_INDATAERR					0x0A000011
#define SAR_GENRANDERR					0x0A000012
#define SAR_HASHOBJERR					0x0A000013
#define SAR_HASHERR						0x0A000014
#define SAR_GENRSAKEYERR				0x0A000015
#define SAR_RSAMODULUSLENERR			0x0A000016
#define SAR_CSPIMPRTPUBKEYERR			0x0A000017
#define SAR_RSAENCERR					0x0A000018
#define SAR_RSADECERR					0x0A000019
#define SAR_HASHNOTEQUALERR				0x0A00001A
#define SAR_KEYNOTFOUNTERR				0x0A00001B
#define SAR_CERTNOTFOUNTERR				0x0A00001C
#define SAR_NOTEXPORTERR				0x0A00001D
#define SAR_DECRYPTPADERR				0x0A00001E
#define SAR_MACLENERR					0x0A00001F
#define SAR_BUFFER_TOO_SMALL			0x0A000020
#define SAR_KEYINFOTYPEERR				0x0A000021
#define SAR_NOT_EVENTERR				0x0A000022
#define SAR_DEVICE_REMOVED				0x0A000023
#define SAR_PIN_INCORRECT				0x0A000024
#define SAR_PIN_LOCKED					0x0A000025
#define SAR_PIN_INVALID					0x0A000026
#define SAR_PIN_LEN_RANGE				0x0A000027
#define SAR_USER_ALREADY_LOGGED_IN		0x0A000028
#define SAR_USER_PIN_NOT_INITIALIZED	0x0A000029
#define SAR_USER_TYPE_INVALID			0x0A00002A
#define SAR_APPLICATION_NAME_INVALID	0x0A00002B
#define SAR_APPLICATION_EXISTS			0x0A00002C
#define SAR_USER_NOT_LOGGED_IN			0x0A00002D
#define SAR_APPLICATION_NOT_EXISTS		0x0A00002E
#define SAR_FILE_ALREADY_EXIST			0x0A00002F
#define SAR_NO_ROOM						0x0A000030

//Add extern return value
#define SAR_FILE_NOT_EXIST				0x0A000031		
#define SAR_GENERATESYMKEYERR			0x0A000032	

//生成对称密钥错误
#define SAR_CONTAINER_ALREADY_EXIST		0x0A000033	//容器已经存在
#define SAR_CONTAINER_NOT_EXIST			0x0A000034	//容器不存在
#define SAR_CONTAINER_NOT_OPEN			0x0A000035	//容器没有打开

//分组加密算法标识
#define SGD_SM1_ECB						0x00000101
#define SGD_SM1_CBC						0x00000102
#define SGD_SM1_CFB						0x00000104
#define SGD_SM1_OFB						0x00000108
#define SGD_SM1_MAC						0x00000110
#define SGD_SSF33_ECB					0x00000201
#define SGD_SSF33_CBC					0x00000202
#define SGD_SSF33_CFB					0x00000204
#define SGD_SSF33_OFB					0x00000208
#define SGD_SSF33_MAC					0x00000210
#define SGD_SMS4_ECB					0x00000401
#define SGD_SMS4_CBC					0x00000402
#define SGD_SMS4_CFB					0x00000404
#define SGD_SMS4_OFB					0x00000408
#define SGD_SMS4_MAC					0x00000410

//非对称密码算法标识
#define SGD_RSA							0x00010000
#define SGD_SM2_1						0x00020100
#define SGD_SM2_2						0x00020200
#define SGD_SM2_3						0x00020400

//密码杂凑算法标识
#define SGD_SM3							0x00000001
#define SGD_SHA1						0x00000002
#define SGD_SHA256						0x00000004

//not in gm spec
#define SGD_MD5							0x00000003

//权限类型
#define SECURE_NEVER_ACCOUNT			0x00000000	//不允许
#define SECURE_ADM_ACCOUNT				0x00000001	//管理员权限
#define SECURE_USER_ACCOUNT				0x00000010	//用户权限
#define SECURE_ANYONE_ACCOUNT			0x000000FF	//任何人

//PIN码类型
#define	ADMIN_TYPE						0x00000000	//管理员PIN
#define USER_TYPE						0x00000001	//用户PIN

//设备状态
#define DEV_ABSENT_STATE				0x00000000	//设备不存在	
#define DEV_PRESENT_STATE				0x00000001	//设备存在
#define DEV_UNKNOW_STATE				0x00000002

#define MAX_IV_LEN 						32
#define APPFILE 						0x7430
#define ALG_SSF33						1
#define ALG_SM1							2

#define TRANS_TYPE_PUBKEY_SKF2HS		1
#define TRANS_TYPE_PUBKEY_HS2SKF		2
#define TRANS_TYPE_PRIKEY_SKF2HS		3
#define TRANS_TYPE_PRIKEY_HS2SKF		4

#define ALG_SM3							0x40
#define ALG_SHA1						0x80
#define ALG_SHA256						0x100
#define ALG_MD5							0x8003

#define MAX_RSA_MODULUS_LEN 			256
#define MAX_RSA_EXPONENT_LEN 			4

#define ECC_MAX_XCOORDINATE_BITS_LEN 	512
#define ECC_MAX_YCOORDINATE_BITS_LEN 	512

#define ECC_MAX_MODULUS_BITS_LEN 		512

#define DEVAPI __stdcall
#define HAIKE__SKF_CHECK_RETURN_VALUE(rv) if (rv) throw rv

//////////////////////////////////////////////////////////////////////////
//
// 版本
typedef struct Struct_Version{
	BYTE 		major;		//主版本号
	BYTE 		minor;		//次版本号
}VERSION;

// 设备信息
typedef struct Struct_DEVINFO{
	VERSION 	Version;
	CHAR 		Manufacturer[64];
	CHAR 		Issuer[64];
	CHAR 		Label[32];
	CHAR 		SerialNumber[32];
	VERSION 	HWVersion;
	VERSION 	FirmwareVersion;
	ULONG 		AlgSymCap;
	ULONG 		AlgAsymCap;
	ULONG 		AlgHashCap;
	ULONG 		DevAuthAlgId;
	ULONG 		TotalSpace;
	ULONG 		FreeSpace;
	BYTE 		Reserved[64];
}DEVINFO, *PDEVINFO;

//
typedef struct IndexSt{
	DWORD 		dwID;
	char 		szName[32];
}INDEXFILETABLE, *PINDEXFILETABLE;

// 设备初始化信息设备类型：
typedef struct Struct_DEVINITINFO{
	VERSION 	AppVersion;
	CHAR 		Manufacturer[64];	//设备厂商信息,最长64个字符，不足64个字符以空白字符(ASCII码为0xFF)填充，不能以null（0x00）结束。
	CHAR 		Label[64];			//SIC/SZD标签,最长64个字符，不足64个字符以空白字符(ASCII码为0xFF)填充，不能以null（0x00）结束。
}DEVINITINFO,*PDEVINITINFO;

// RSA公钥交换数据块
typedef struct Struct_RSAPUBLICKEYBLOB{
	ULONG		AlgID;
	ULONG		BitLen;
	BYTE		Modulus[MAX_RSA_MODULUS_LEN];
	BYTE		PublicExponent[MAX_RSA_EXPONENT_LEN];
}RSAPUBLICKEYBLOB, *PRSAPUBLICKEYBLOB;

// RSA私钥交换数据块
typedef struct Struct_RSAPRIVATEKEYBLOB{
	ULONG		AlgID;
	ULONG		BitLen;
	BYTE		Modulus[MAX_RSA_MODULUS_LEN];
	BYTE		PublicExponent[MAX_RSA_EXPONENT_LEN];
	BYTE		PrivateExponent[MAX_RSA_MODULUS_LEN];
	BYTE		Prime1[MAX_RSA_MODULUS_LEN/2];
	BYTE		Prime2[MAX_RSA_MODULUS_LEN/2];
	BYTE		Prime1Exponent[MAX_RSA_MODULUS_LEN/2];
	BYTE		Prime2Exponent[MAX_RSA_MODULUS_LEN/2];
	BYTE		Coefficient[MAX_RSA_MODULUS_LEN/2];
}RSAPRIVATEKEYBLOB, *PRSAPRIVATEKEYBLOB;

// ECC公钥交换数据块
typedef struct Struct_ECCPUBLICKEYBLOB{
	ULONG		BitLen;
	BYTE		XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE		YCoordinate[ECC_MAX_YCOORDINATE_BITS_LEN/8];
}ECCPUBLICKEYBLOB, *PECCPUBLICKEYBLOB;

// ECC私钥交换数据块
typedef struct Struct_ECCPRIVATEKEYBLOB{
	ULONG		BitLen;
	BYTE		PrivateKey[ECC_MAX_MODULUS_BITS_LEN/8];
}ECCPRIVATEKEYBLOB, *PECCPRIVATEKEYBLOB;

// ECC密文数据结构
typedef struct Struct_ECCCIPHERBLOB{
	BYTE  		XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  		YCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  		Hash[32];
	ULONG 		CipherLen;
	BYTE  		Cipher[1];
}ECCCIPHERBLOB, *PECCCIPHERBLOB;

// ECC签名数据结构
typedef struct Struct_ECCSIGNATUREBLOB{
	BYTE 		r[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE 		s[ECC_MAX_XCOORDINATE_BITS_LEN/8];
}ECCSIGNATUREBLOB, *PECCSIGNATUREBLOB;

typedef struct SKF_ENVELOPEDKEYBLOB{
	ULONG 		Version;                  	// 当前版本为 1
	ULONG 		ulSymmAlgID;              	// 规范中的算法标识，限定ECB模式
	ULONG 		ulBits;						// 加密密钥对的密钥位长度
	BYTE 		cbEncryptedPriKey[64];     	// 加密保护的加密私钥
	ECCPUBLICKEYBLOB PubKey;        		// 加密公钥
	ECCCIPHERBLOB 		ECCCipherBlob;  	// SM2 公钥加密的密钥加密密钥
}ENVELOPEDKEYBLOB, *PENVELOPEDKEYBLOB;

// 分组密码参数
typedef struct Struct_BLOCKCIPHERPARAM{
	BYTE		IV[MAX_IV_LEN];
	ULONG		IVLen;
	ULONG		PaddingType;
	ULONG		FeedBitLen;
} BLOCKCIPHERPARAM, *PBLOCKCIPHERPARAM;

// 文件属性
typedef struct Struct_FILEATTRIBUTE{
	CHAR		FileName[32];
	ULONG		FileSize;
	ULONG		ReadRights;
	ULONG		WriteRights; 
}FILEATTRIBUTE, *PFILEATTRIBUTE;

typedef struct KeySt{
	DWORD		dwHandleType;
	DWORD		dwAlgID;
	BYTE		bKey[32];
	BYTE		bIV[32];
	BLOCKCIPHERPARAM BlockCipherParam; 
	HANDLE		hCard;
	HANDLE 		hHSSymHandle;
	DWORD 		dwPaddingFlag;
	BYTE 		bRemainData[128];
	DWORD 		dwRemainDataLen;
	DWORD 		dwFlag;
}KEY_HANDLE, *PKEY_HANDLE;

typedef struct AsymKeySt{
	DWORD		dwHandleType; 				// HANDLE_TYPE_ASYMKEY
	HANDLE		hContainer;
	HANDLE		hDev;
	RSAPUBLICKEYBLOB pPubBlob;
	RSAPRIVATEKEYBLOB pPriBlob;
	char 		szConName[0x40];
	DWORD 		dwKeySpec;
}ASYMKEY_HANDLE, *PASYMKEY_HANDLE;

typedef struct _SKF_APPINFO_ST_ {
	char 		szAppName[0x40];
	HANDLE 		hCard;
	char 		szReserved[0x40];
}SKF_APPINFO, *PSKF_APPINFO;

typedef struct _SKF_CONINFO_ST_ {
	char 		szConName[0x40];
	HANDLE 		hCard;
	HANDLE 		hApp;
	char 		szReserved[0x40];
}SKF_CONINFO, *PSKF_CONINFO;

typedef struct _SKF_HASH_ST_ {
	HANDLE 		hCard;
	HANDLE 		hHash;
	DWORD 		dwAlgID;
}SKF_HASHINFO, *PSKF_HASHINFO;

typedef HANDLE DEVHANDLE;
typedef HANDLE HAPPLICATION;
typedef HANDLE HCONTAINER;


typedef ULONG (DEVAPI *__SKF_WaitForDevEvent)(LPSTR szDevName, ULONG *pulDevNameLen, ULONG *pulEvent);
extern __SKF_WaitForDevEvent 		SKF_WaitForDevEvent;

typedef ULONG (DEVAPI *__SKF_CancelWaitForDevEvent)();
extern __SKF_CancelWaitForDevEvent	SKF_CancelWaitForDevEvent;

typedef ULONG (DEVAPI *__SKF_EnumDev)(BOOL bPresent, LPSTR szNameList, ULONG *pulSize);
extern __SKF_EnumDev				SKF_EnumDev;

typedef ULONG (DEVAPI *__SKF_ConnectDev)(LPSTR szName, DEVHANDLE *phDev);
extern __SKF_ConnectDev				SKF_ConnectDev;

typedef ULONG (DEVAPI *__SKF_DisConnectDev)(DEVHANDLE hDev);
extern __SKF_DisConnectDev			SKF_DisConnectDev;

typedef ULONG (DEVAPI *__SKF_GetDevState)(LPSTR szDevName, ULONG *pulDevState);
extern __SKF_GetDevState			SKF_GetDevState;

typedef ULONG (DEVAPI *__SKF_SetLabel)(DEVHANDLE hDev, LPSTR szLabel);
extern __SKF_SetLabel				SKF_SetLabel;

typedef ULONG (DEVAPI *__SKF_GetDevInfo)(DEVHANDLE hDev, DEVINFO *pDevInfo);
extern __SKF_GetDevInfo				SKF_GetDevInfo;

typedef ULONG (DEVAPI *__SKF_LockDev)(DEVHANDLE hDev, ULONG ulTimeOut);
extern __SKF_LockDev				SKF_LockDev;

typedef ULONG (DEVAPI *__SKF_UnlockDev)(DEVHANDLE hDev);
extern __SKF_UnlockDev				SKF_UnlockDev;

typedef ULONG (DEVAPI *__SKF_Transmit)(DEVHANDLE hDev, BYTE* pbCommand, ULONG ulCommandLen, BYTE* pbData, ULONG* pulDataLen);
extern __SKF_Transmit				SKF_Transmit;

/* 访问控制 */
typedef ULONG (DEVAPI *__SKF_ChangeDevAuthKey)(DEVHANDLE hDev, BYTE *pbKeyValue, ULONG ulKeyLen);
extern __SKF_ChangeDevAuthKey		SKF_ChangeDevAuthKey;

typedef ULONG (DEVAPI *__SKF_DevAuth)(DEVHANDLE hDev, BYTE *pbAuthData, ULONG ulLen);
extern __SKF_DevAuth				SKF_DevAuth;

typedef ULONG (DEVAPI *__SKF_ChangePIN)(HAPPLICATION hApplication, ULONG ulPINType, LPSTR szOldPin, LPSTR szNewPin, ULONG *pulRetryCount);
extern __SKF_ChangePIN				SKF_ChangePIN;

typedef ULONG (DEVAPI *__SKF_GetPINInfo)(HAPPLICATION hApplication, ULONG ulPINType, ULONG *pulMaxRetryCount, ULONG *pulRemainRetryCount, BOOL *pbDefaultPin);
extern __SKF_GetPINInfo				SKF_GetPINInfo;

typedef ULONG (DEVAPI *__SKF_VerifyPIN)(HAPPLICATION hApplication, ULONG ulPINType, LPSTR szPIN, ULONG *pulRetryCount);
extern __SKF_VerifyPIN				SKF_VerifyPIN;

typedef ULONG (DEVAPI *__SKF_UnblockPIN)(HAPPLICATION hApplication, LPSTR szAdminPIN, LPSTR szNewUserPIN, ULONG *pulRetryCount);
extern __SKF_UnblockPIN				SKF_UnblockPIN;

typedef ULONG (DEVAPI *__SKF_ClearSecureState)(HAPPLICATION hApplication);
extern __SKF_ClearSecureState		SKF_ClearSecureState;

/* 应用管理 */
typedef ULONG (DEVAPI *__SKF_CreateApplication)(DEVHANDLE hDev, LPSTR szAppName, LPSTR szAdminPin, DWORD dwAdminPinRetryCount, LPSTR szUserPin, DWORD dwUserPinRetryCount, DWORD dwCreateFileRights, HAPPLICATION *phApplication);
extern __SKF_CreateApplication		SKF_CreateApplication;

typedef ULONG (DEVAPI *__SKF_EnumApplication)(DEVHANDLE hDev, LPSTR szAppName, ULONG *pulSize);
extern __SKF_EnumApplication		SKF_EnumApplication;

typedef ULONG (DEVAPI *__SKF_DeleteApplication)(DEVHANDLE hDev, LPSTR szAppName);
extern __SKF_DeleteApplication		SKF_DeleteApplication;

typedef ULONG (DEVAPI *__SKF_OpenApplication)(DEVHANDLE hDev, LPSTR szAppName, HAPPLICATION *phApplication);
extern __SKF_OpenApplication		SKF_OpenApplication;

typedef ULONG (DEVAPI *__SKF_CloseApplication)(HAPPLICATION hApplication);
extern __SKF_CloseApplication		SKF_CloseApplication;

/* 文件管理 */
typedef ULONG (DEVAPI *__SKF_CreateFile)(HAPPLICATION hApplication, LPSTR szFileName, ULONG ulFileSize, ULONG ulReadRights, ULONG ulWriteRights);
extern __SKF_CreateFile				SKF_CreateFile;

typedef ULONG (DEVAPI *__SKF_DeleteFile)(HAPPLICATION hApplication, LPSTR szFileName);
extern __SKF_DeleteFile				SKF_DeleteFile;

typedef ULONG (DEVAPI *__SKF_EnumFiles)(HAPPLICATION hApplication, LPSTR szFileList, ULONG *pulSize);
extern __SKF_EnumFiles				SKF_EnumFiles;

typedef ULONG (DEVAPI *__SKF_GetFileInfo)(HAPPLICATION hApplication, LPSTR szFileName, FILEATTRIBUTE *pFileInfo);
extern __SKF_GetFileInfo			SKF_GetFileInfo;

typedef ULONG (DEVAPI *__SKF_ReadFile)(HAPPLICATION hApplication, LPSTR szFileName, ULONG ulOffset, ULONG ulSize, BYTE * pbOutData, ULONG *pulOutLen);
extern __SKF_ReadFile				SKF_ReadFile;

typedef ULONG (DEVAPI *__SKF_WriteFile)(HAPPLICATION hApplication, LPSTR szFileName, ULONG ulOffset, BYTE *pbData, ULONG ulSize);
extern __SKF_WriteFile				SKF_WriteFile;

/* 容器管理 */
typedef ULONG (DEVAPI *__SKF_CreateContainer)(HAPPLICATION hApplication, LPSTR szContainerName, HCONTAINER *phContainer);
extern __SKF_CreateContainer		SKF_CreateContainer;

typedef ULONG (DEVAPI *__SKF_DeleteContainer)(HAPPLICATION hApplication, LPSTR szContainerName);
extern __SKF_DeleteContainer		SKF_DeleteContainer;

typedef ULONG (DEVAPI *__SKF_OpenContainer)(HAPPLICATION hApplication, LPSTR szContainerName, HCONTAINER *phContainer);
extern __SKF_OpenContainer			SKF_OpenContainer;

typedef ULONG (DEVAPI *__SKF_CloseContainer)(HCONTAINER hContainer);
extern __SKF_CloseContainer			SKF_CloseContainer;

typedef ULONG (DEVAPI *__SKF_EnumContainer)(HAPPLICATION hApplication, LPSTR szContainerName, ULONG *pulSize);
extern __SKF_EnumContainer			SKF_EnumContainer;

typedef ULONG (DEVAPI *__SKF_GetContainerType)(IN HCONTAINER hContainer, OUT ULONG *pulConProperty);
extern __SKF_GetContainerType		SKF_GetContainerType;

//typedef ULONG (DEVAPI *__SKF_GetConProperty)(IN HCONTAINER hContainer, OUT ULONG *pulConProperty);
//extern __SKF_GetConProperty			SKF_GetConProperty;

/* 密码服务 */
typedef ULONG (DEVAPI *__SKF_GenRandom)(DEVHANDLE hDev, BYTE *pbRandom, ULONG ulRandomLen);
extern __SKF_GenRandom				SKF_GenRandom;

typedef ULONG (DEVAPI *__SKF_GenExtRSAKey)(DEVHANDLE hDev, ULONG ulBitsLen, RSAPRIVATEKEYBLOB *pBlob);
extern __SKF_GenExtRSAKey			SKF_GenExtRSAKey;

typedef ULONG (DEVAPI *__SKF_GenRSAKeyPair)(HCONTAINER hContainer, ULONG ulBitsLen, RSAPUBLICKEYBLOB *pBlob);
extern __SKF_GenRSAKeyPair			SKF_GenRSAKeyPair;

typedef ULONG (DEVAPI *__SKF_ImportRSAKeyPair)(HCONTAINER hContainer, ULONG ulSymAlgId, BYTE *pbWrappedKey, ULONG ulWrappedKeyLen, BYTE *pbEncryptedData, ULONG ulEncryptedDataLen);
extern __SKF_ImportRSAKeyPair		SKF_ImportRSAKeyPair;

typedef ULONG (DEVAPI *__SKF_RSASignData)(HCONTAINER hContainer, BYTE *pbData, ULONG ulDataLen, BYTE *pbSignature, ULONG *pulSignLen);
extern __SKF_RSASignData			SKF_RSASignData;

typedef ULONG (DEVAPI *__SKF_RSAVerify)(DEVHANDLE hDev , RSAPUBLICKEYBLOB* pRSAPubKeyBlob, BYTE *pbData, ULONG ulDataLen, BYTE *pbSignature, ULONG ulSignLen);
extern __SKF_RSAVerify				SKF_RSAVerify;

typedef ULONG (DEVAPI *__SKF_RSAExportSessionKey)(HCONTAINER hContainer, ULONG ulAlgId, RSAPUBLICKEYBLOB *pPubKey, BYTE *pbData, ULONG *pulDataLen, HANDLE *phSessionKey);
extern __SKF_RSAExportSessionKey	SKF_RSAExportSessionKey;

typedef ULONG (DEVAPI *__SKF_ExtRSAPubKeyOperation)(DEVHANDLE hDev, RSAPUBLICKEYBLOB* pRSAPubKeyBlob, BYTE* pbInput, ULONG ulInputLen, BYTE* pbOutput, ULONG* pulOutputLen);
extern __SKF_ExtRSAPubKeyOperation	SKF_ExtRSAPubKeyOperation;

typedef ULONG (DEVAPI *__SKF_ExtRSAPriKeyOperation)(DEVHANDLE hDev, RSAPRIVATEKEYBLOB* pRSAPriKeyBlob, BYTE* pbInput, ULONG ulInputLen, BYTE* pbOutput, ULONG* pulOutputLen);
extern __SKF_ExtRSAPriKeyOperation	SKF_ExtRSAPriKeyOperation;

typedef ULONG (DEVAPI *__SKF_GenECCKeyPair)(HCONTAINER hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB *pBlob);
extern __SKF_GenECCKeyPair			SKF_GenECCKeyPair;

//typedef ULONG (DEVAPI *__SKF_ImportECCKeyPair)(HCONTAINER hContainer, ULONG ulSymAlgId, BYTE *pbWrappedKey, ULONG ulWrappedKeyLen, BYTE *pbEncryptedData, ULONG ulEncryptedDataLen);
//extern __SKF_ImportECCKeyPair		SKF_ImportECCKeyPair;

typedef ULONG (DEVAPI *__SKF_ImportECCKeyPair)(IN HCONTAINER hContainer, PENVELOPEDKEYBLOB pEnvelopedKeyBlob);
extern __SKF_ImportECCKeyPair		SKF_ImportECCKeyPair;

typedef ULONG (DEVAPI *__SKF_ECCSignData)(HCONTAINER hContainer, BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);
extern __SKF_ECCSignData			SKF_ECCSignData;

typedef ULONG (DEVAPI *__SKF_ECCVerify)(DEVHANDLE hDev , ECCPUBLICKEYBLOB* pECCPubKeyBlob, BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);
extern __SKF_ECCVerify				SKF_ECCVerify;

typedef ULONG (DEVAPI *__SKF_ECCExportSessionKey)(HCONTAINER hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB *pPubKey, PECCCIPHERBLOB pData, HANDLE *phSessionKey);
extern __SKF_ECCExportSessionKey	SKF_ECCExportSessionKey;

typedef ULONG (DEVAPI *__SKF_ExtECCEncrypt)(DEVHANDLE hDev, ECCPUBLICKEYBLOB* pECCPubKeyBlob, BYTE* pbPlainText, ULONG ulPlainTextLen, PECCCIPHERBLOB pCipherText);
extern __SKF_ExtECCEncrypt			SKF_ExtECCEncrypt;

typedef ULONG (DEVAPI *__SKF_ExtECCDecrypt)(DEVHANDLE hDev, ECCPRIVATEKEYBLOB* pECCPriKeyBlob, PECCCIPHERBLOB pCipherText, BYTE* pbPlainText, ULONG* pulPlainTextLen);
extern __SKF_ExtECCDecrypt			SKF_ExtECCDecrypt;

typedef ULONG (DEVAPI *__SKF_ECCDecrypt)(HCONTAINER hContainer, PECCCIPHERBLOB pCipherText, BYTE *pbPlainText, ULONG *pulPlainTextLength);
extern __SKF_ECCDecrypt				SKF_ECCDecrypt;

typedef ULONG (DEVAPI *__SKF_ExtECCSign)(DEVHANDLE hDev, ECCPRIVATEKEYBLOB* pECCPriKeyBlob, BYTE* pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);
extern __SKF_ExtECCSign				SKF_ExtECCSign;

typedef ULONG (DEVAPI *__SKF_ExtECCVerify)(DEVHANDLE hDev, ECCPUBLICKEYBLOB* pECCPubKeyBlob, BYTE* pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);
extern __SKF_ExtECCVerify			SKF_ExtECCVerify;

typedef ULONG (DEVAPI *__SKF_GenerateAgreementDataWithECC)(HCONTAINER hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB* pTempECCPubKeyBlob, BYTE* pbID, ULONG ulIDLen, HANDLE *phAgreementHandle);
extern	__SKF_GenerateAgreementDataWithECC 			SKF_GenerateAgreementDataWithECC;

typedef ULONG (DEVAPI *__SKF_GenerateAgreementDataAndKeyWithECC)(HANDLE hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB* pSponsorECCPubKeyBlob, ECCPUBLICKEYBLOB* pSponsorTempECCPubKeyBlob, ECCPUBLICKEYBLOB* pTempECCPubKeyBlob, BYTE* pbID, ULONG ulIDLen, BYTE *pbSponsorID, ULONG ulSponsorIDLen, HANDLE *phKeyHandle);
extern __SKF_GenerateAgreementDataAndKeyWithECC		SKF_GenerateAgreementDataAndKeyWithECC;

typedef ULONG (DEVAPI *__SKF_GenerateKeyWithECC)(HANDLE hAgreementHandle, ECCPUBLICKEYBLOB* pECCPubKeyBlob, ECCPUBLICKEYBLOB* pTempECCPubKeyBlob, BYTE* pbID, ULONG ulIDLen, HANDLE *phKeyHandle);
extern __SKF_GenerateKeyWithECC		SKF_GenerateKeyWithECC;

typedef ULONG (DEVAPI *__SKF_ExportPublicKey)(HCONTAINER hContainer, BOOL bSignFlag, BYTE* pbBlob, ULONG* pulBlobLen);
extern __SKF_ExportPublicKey		SKF_ExportPublicKey;

typedef ULONG (DEVAPI *__SKF_ImportSessionKey)(HCONTAINER hContainer, ULONG ulAlgId, BYTE *pbWrapedData, ULONG ulWrapedLen, HANDLE *phKey);
extern __SKF_ImportSessionKey		SKF_ImportSessionKey;

typedef ULONG (DEVAPI *__SKF_SetSymmKey)(DEVHANDLE hDev, BYTE* pbKey, ULONG ulAlgID, HANDLE* phKey);
extern __SKF_SetSymmKey				SKF_SetSymmKey;

typedef ULONG (DEVAPI *__SKF_EncryptInit)(HANDLE hKey, BLOCKCIPHERPARAM EncryptParam);
extern __SKF_EncryptInit			SKF_EncryptInit;

typedef ULONG (DEVAPI *__SKF_Encrypt)(HANDLE hKey, BYTE * pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);
extern __SKF_Encrypt				SKF_Encrypt;

typedef ULONG (DEVAPI *__SKF_EncryptUpdate)(HANDLE hKey, BYTE * pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);
extern __SKF_EncryptUpdate			SKF_EncryptUpdate;

typedef ULONG (DEVAPI *__SKF_EncryptFinal)(HANDLE hKey, BYTE *pbEncryptedData, ULONG *ulEncryptedDataLen );
extern __SKF_EncryptFinal			SKF_EncryptFinal;

typedef ULONG (DEVAPI *__SKF_DecryptInit)(HANDLE hKey, BLOCKCIPHERPARAM DecryptParam);
extern __SKF_DecryptInit			SKF_DecryptInit;

typedef ULONG (DEVAPI *__SKF_Decrypt)(HANDLE hKey, BYTE * pbEncryptedData, ULONG ulEncryptedLen, BYTE * pbData, ULONG * pulDataLen);
extern __SKF_Decrypt				SKF_Decrypt;

typedef ULONG (DEVAPI *__SKF_DecryptUpdate)(HANDLE hKey, BYTE * pbEncryptedData, ULONG ulEncryptedLen, BYTE * pbData, ULONG * pulDataLen);
extern __SKF_DecryptUpdate			SKF_DecryptUpdate;

typedef ULONG (DEVAPI *__SKF_DecryptFinal)(HANDLE hKey, BYTE *pbDecryptedData, ULONG *pulDecryptedDataLen);
extern __SKF_DecryptFinal			SKF_DecryptFinal;

//typedef ULONG (DEVAPI *__SKF_DigestInit)(DEVHANDLE hDev, ULONG ulAlgID, HANDLE *phHash);
//extern __SKF_DigestInit				SKF_DigestInit;

typedef ULONG (DEVAPI *__SKF_DigestInit)(DEVHANDLE hDev, ULONG ulAlgID, ECCPUBLICKEYBLOB *pPubKey, unsigned char *pucID, ULONG ulIDLen, HANDLE *phHash);
extern __SKF_DigestInit				SKF_DigestInit;

typedef ULONG (DEVAPI *__SKF_Digest)(HANDLE hHash, BYTE *pbData, ULONG ulDataLen, BYTE *pbHashData, ULONG *pulHashLen);
extern __SKF_Digest					SKF_Digest;

typedef ULONG (DEVAPI *__SKF_DigestUpdate)(HANDLE hHash, BYTE *pbData, ULONG ulDataLen);
extern __SKF_DigestUpdate			SKF_DigestUpdate;

typedef ULONG (DEVAPI *__SKF_DigestFinal)(HANDLE hHash, BYTE *pHashData, ULONG *pulHashLen);
extern __SKF_DigestFinal			SKF_DigestFinal;

typedef ULONG (DEVAPI *__SKF_MacInit)(HANDLE hKey, BLOCKCIPHERPARAM* pMacParam, HANDLE *phMac);
extern __SKF_MacInit				SKF_MacInit;

typedef ULONG (DEVAPI *__SKF_Mac)(HANDLE hMac, BYTE *pbData, ULONG ulDataLen, BYTE *pbMacData, ULONG *pulMacLen);
extern __SKF_Mac					SKF_Mac;

typedef ULONG (DEVAPI *__SKF_MacUpdate)(HANDLE hMac, BYTE *pbData, ULONG ulDataLen);
extern	__SKF_MacUpdate				SKF_MacUpdate;

typedef ULONG (DEVAPI *__SKF_MacFinal)(HANDLE hMac, BYTE *pbMacData, ULONG *pulMacDataLen);
extern 	__SKF_MacFinal				SKF_MacFinal;

typedef ULONG (DEVAPI *__SKF_CloseHandle)(HANDLE hHandle);
extern __SKF_CloseHandle			SKF_CloseHandle;

//
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS_) (HANDLE, PBOOL);
extern LPFN_ISWOW64PROCESS_	fnIsWow64Process;

typedef UINT (WINAPI *LPFN_GetSystemWow64DirectoryA_)(LPSTR ,UINT);
extern LPFN_GetSystemWow64DirectoryA_ fnGetSystemWow64DirectoryA;

#ifdef __cplusplus
}
#endif

#endif