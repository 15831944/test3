#ifndef __HYCSPAPIWRAPPER_H__
#define __HYCSPAPIWRAPPER_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <windows.h>
#include "HYErrorCode.h"

#define HTCSPAPI_DLLNAME			"GEA20003.dll"
extern HMODULE	g_hCspApiDll;

BOOL Function_Init();
BOOL Function_Finalize();

//////////////////////////////////////////////////////////////////////////
//
#define HS_HASH_SHA1					0x01
#define HS_HASH_SHA256					0x02
#define HS_HASH_MD2						0x03
#define HS_HASH_MD5						0x04
#define HS_SSL3_SHAMD5              	0x05
#define HS_HASH_SM3						0x06

#define SERIAL_LEN						0x08
#define MAX_RSA_MODULUS_BITS			2048	
#define MAX_RSA_MODULUS_LEN				((MAX_RSA_MODULUS_BITS + 7) / 8)	
#define MAX_RSA_PRIME_BITS				((MAX_RSA_MODULUS_BITS + 1) / 2)	
#define MAX_RSA_PRIME_LEN				((MAX_RSA_PRIME_BITS + 7) / 8)	
#define SO_PIN_LEN                  	0x10
#define USER_PIN_LEN                	0x10
#define HTCSP_APP_NUM               	0x0A

#define HT_NEEDNT_PIN					0x00
#define HT_SO_PIN						0x01
#define HT_USER_PIN						0x02
#define RSA_MODE_1024					0x01
#define RSA_MODE_2048					0x02
#define PIN_LEAD_LEN					0x06
#define HT_UNKNOWN_APP					0x00000000
#define HT_CSP_APP						0x00000001
#define HT_SEC_APP						0x00000004
#define HT_BLANK_HW						0x80000000
#define MAX_CON_NAME_LEN				0x40
#define HT_SSF33_KEY					0x03
#define HT_SCB2_KEY						0x04
#define HT_SM4_KEY						0x05
#define ENCRYPT_MODE					0x01
#define DECRYPT_MODE 					0x02
#define HT_RSA_PUB_KEY					0x01
#define HT_RSA_PRI_KEY					0x02
#define HT_FILE_NOT_EXIST				0x00
#define HT_FILE_EXIST					0x01
#define HAIKEY1000						0x01
#define HAIKEY2000						0x02
#define HAIKEY3000						0x03
#define HT_UNKNOW_TYPE					0x04
#define ECB_MODE						0x01
#define CBC_MODE						0x02
#define HT_DES_KEY						0x01
#define HT_3DES_112_KEY					0x02
#define HT_3DES_KEY						0x06
#define HT_SCB2							0x60
#define HS_STATE_EMPTY              	0x10
#define HS_STATE_PRESENT            	0x20
#define HGS_STATE_EMPTY					0x0010
#define HGS_STATE_PRESENT				0x0020
#define HS_DEVICE_INSERT            	0x01
#define HS_DEVICE_REMOVE            	0x02
#define HT_LABEL_LEN                	0x20

#define HS_READERNAME_MAX          		0x40
#define HS_SERIAL_LEN_MAX           	0x14
#define HS_PIN_LEN_MAX              	0x40

#define HS_EXCHANGE_KEY_EXISTING		0x00000001		// 加密密钥已存在
#define HS_EXCHANGE_CERT_EXISTING		0x00000002		// 加密证书已存在
#define HS_SIGN_KEY_EXISTING			0x00010000		// 签名密钥已存在
#define HS_SIGN_CERT_EXISTING			0x00020000		// 签名证书已存在

#define HS_EXCHANGE_1024				0x00000010		// 加密密钥 1024
#define HS_EXCHANGE_2048				0x00000020		// 加密密钥 2048
#define HS_SIGN_1024					0x00100000		// 签名密钥 1024
#define HS_SIGN_2048					0x00200000		// 签名密钥 2048


#define HS_SM2_EXCHANGE_256				0x00000040		// SM2 加密密钥
#define HS_SM2_SIGN_256					0x00400000		// SM2 签名密钥

#define HS_DELETE_RIGHT					0x01
#define HS_READ_RIGHT					0x02
#define HS_WRITE_RIGHT					0x03

#define HS_SSF33_KEY					0x03
#define HS_SM1_KEY						0x04
#define HS_SM4_KEY						0x05

#define RSA_ZERO_PADDING				0x00
// #define RSA_PKCS1_PADDING			0x01
// #define RSA_SSLV23_PADDING			0x02
// #define RSA_NO_PADDING				0x03
// #define RSA_PKCS1_OAEP_PADDING		0x04
// #define RSA_X931_PADDING				0x05

#define HS_MAX_FILE_NUM					0x32

#define HS_MIN_FILE_ID					0x0000
#define HS_MAX_FILE_ID					0xFFFF
#define HS_MIN_FILE_SIZE				0x0000
#define HS_MAX_FILE_SIZE				0x8000-3		// 文件的最大长度,32K-3,3字节为内部管理文件用
#define HS_MAX_FILES_NUM				50

#define HS_SM2_SEND                		0x01
#define HS_SM2_RECEIVE             		0x02

#define IV_LEN							0x20

#define HS_ALG_DES						0x71
#define HS_ALG_3DES						0x72
#define HS_ALG_3DES_112					0x73
#define HS_ALG_RC2						0x74
#define HS_ALG_RC4						0x75
#define HS_ALG_RC5						0x76
#define HS_ALG_SM1						0x91
#define HS_ALG_SSF33					0x92
#define HS_ALG_SM4_ECB					0x00000401
#define HS_ALG_SM4_CBC					0x00000402

#define AT_KEYEXCHANGE					1
#define AT_SIGNATURE					2

#define HS_SM2_SIGNATURE				0x00020100		// ECC签名算法
#define HS_SM2_EXCHANGE					0x00020200		// ECC密钥交换协议
#define HS_SM2_ENCRYPT	    			0x00020400		// ECC加密算法

#define ECC_MAX_XCOORDINATE_BITS_LEN 	512
#define ECC_MAX_YCOORDINATE_BITS_LEN 	512
#define ECC_MAX_MODULUS_BITS_LEN 		512

// 公钥结构
typedef struct _HTCSP_SM2_PUBLIC_ST
{
	DWORD 			dwAlgID;
	DWORD 			dwBits;
	BYTE 			XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE 			YCoordinate[ECC_MAX_YCOORDINATE_BITS_LEN/8];
} HTCSP_SM2_PUBLIC_ST;

// 私钥结构
typedef struct _HTCSP_SM2_PRIVATE_ST
{
	DWORD 			dwAlgID;
	DWORD 			dwBits;
	BYTE 			PrivateKey[ECC_MAX_MODULUS_BITS_LEN/8];
} HTCSP_SM2_PRIVATE_ST;

// 私钥运算密文数据
typedef struct _HTCSP_SM2_PRI_CRYPTO_ST
{
	BYTE 			r[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE 			s[ECC_MAX_XCOORDINATE_BITS_LEN/8];
} HTCSP_SM2_Pri_Crypto_ST;

// 公钥运算密文数据
typedef struct _HTCSP_SM2_PUB_CRYPTO_ST
{
	BYTE  			XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  			YCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  			Cipher[ECC_MAX_XCOORDINATE_BITS_LEN/2];
	BYTE  			Mac[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	DWORD 			dwCipherLen;
} HTCSP_SM2_Pub_Crypto_ST;

/*
*@brief 密钥结构，密钥结构体信息。
*@param dwKeyType 密钥类型。
*@param dwKeyID 密钥ID。
*@param pbKey 密钥数据。
*@param dwKeyLen 密钥数据长度。
*@param dwUseRight 密钥使用权限。
*@param dwReserved 保留。
*/
typedef struct _HS_KeyInfo_st
{
	DWORD 			dwKeyType;
	DWORD 			dwKeyID;
	BYTE 			*pbKey;
	DWORD 			dwKeyLen;
	DWORD 			dwUseRight;
	DWORD 			dwReserved;
}HS_KEYINFO, *PHS_KEYINFO;

/*
*@brief 设备基本信息结构，用于保存KEY的基本信息。
*@param dwPID 设备的PID。
*@param dwVID 设备的VID。
*@param pdwTotalSiz 芯片空间总大小。
*@param dwFreeSize 设备的剩余空间大小。
*@param dwCOSVer COS版本号。
*@param dwDevType 设备的型号，包括HAIKEY1000、HAIKEY2000、HAIKEY3000、HT_UNKNOW_TYPE等。
*@param dWKeyCategory 设备的种类，包括HAIKEY_COMMON、HAIKEY_BUTTON、HAIKEY_LCD_BUTTON、
                                      HAIKEY_AUDIO_BUTTON、HAIKEY_FINGERPRINT等。
*@param dwDevBufLen 设备缓冲区长度。
*@param dwChipSerial 芯片序列号（16进制的字符串）。
*/
typedef struct _HYC_DeviceInfo_st
{
	DWORD			dwPID;
	DWORD			dwVID;
	DWORD			dwTotalSize;
	DWORD			dwFreeSize;
	DWORD			dwCOSVer;
	DWORD			dwDevType;
	DWORD			dwKeyCategory;
	DWORD			dwDevBufLen;
	BYTE			szChipSerial[64]; 
	DWORD			dwReserved1;
} HYC_DEVICEINFO, *PHYC_DEVICEINFO;

/*
*@brief 公钥结构，公钥结构体信息。
*@param dwbits 公钥位数。
*@param byodulus n值。
*@param bypublicExponent e值。
*/
typedef struct _HTCSP_RSA_PUB_ST
{
	DWORD 			dwbits;
	BYTE 			byModulus[MAX_RSA_MODULUS_LEN];      //n              
	BYTE 			bypublicExponent[MAX_RSA_MODULUS_LEN];    //e
} HTCSP_RSA_PUBLIC_ST;

/*
*@brief 私钥结构，私钥结构体信息。
*@param dwbits 私钥位数。
*@param byodulus n值。
*@param bypublicExponent e值。
*@param byexponent d值。
*@param byprime p , q 。
*@param byprimeExponent p-1,q-1。
*@param bycoefficient iqmp。
*/
typedef struct _HTCSP_RSA_PRI_ST
{
	DWORD 			dwbits;
	BYTE 			byModulus[MAX_RSA_MODULUS_LEN];      //n              
	BYTE 			bypublicExponent[MAX_RSA_MODULUS_LEN];    //e
	BYTE 			byexponent[MAX_RSA_MODULUS_LEN];      //d
	BYTE 			byprime[2][MAX_RSA_PRIME_LEN];    // p, q
	BYTE 			byprimeExponent[2][MAX_RSA_PRIME_LEN]; //p-1,q-1
	BYTE 			bycoefficient[MAX_RSA_PRIME_LEN];  //iqmp
} HTCSP_RSA_PRIVATE_ST;

/*
*@brief RSA结构，RSA结构体信息。
*@param ht_RSA_pub_st 公钥结构体。
*@param ht_RSA_pri_st 私钥结构体。
*/
typedef struct _HTCSP_RSA_ST
{
	HTCSP_RSA_PUBLIC_ST 	ht_RSA_pub_st;
	HTCSP_RSA_PRIVATE_ST 	ht_RSA_pri_st;
} HTCSP_RSA_ST, *PHTCSP_RSA_ST;

/*
*@brief 证书结构，主要用于做RSA签名时传入传出签名相关数据的数据结构。
*@param chVer 版本号。
*@param szSerial 证书序列号。
*@param szC 国家信息。
*@param szOU 部门信息。
*@param szO 公司信息。
*@param szCN 用户名。
*@param szTimeFrom 有效起始日期。
*@param szTimeTo 有效到期日期。
*@param szIssuer 颁发者。
*@param szIssueTo 颁发给。
*@param szReserved1 电子邮件
*/
typedef struct _cert_info_st
{
	BYTE 			chVer;
	//modify		BYTE szSerial[SERIAL_LEN = 32];
	BYTE 			szC[MAX_PATH];
	BYTE 			szOU[MAX_PATH];
	BYTE 			szO[MAX_PATH];
	BYTE 			szCN[MAX_PATH];
	char 			szL[MAX_PATH];
	char 			szS[MAX_PATH];
	BYTE 			szTimeFrom[MAX_PATH];
	BYTE 			szTimeTo[MAX_PATH];
	BYTE 			szIssuer[MAX_PATH];
	BYTE 			szReserved1[MAX_PATH];
	BYTE 			szReserved2[MAX_PATH];
} HTCSP_CERT_ST, *PHTCSP_CERT_ST;

/*
*@brief 格式化结构，主要用于做格式化操作时传入传出格式化相关数据。
*@param szLabel 设备标识名称，长度为1-32字节。
*@param szSOPin 管理员口令，长度为1-16字节。
*@param dwSOPinRetry	管理员口令重试次数。
*@param szUserPin  用户口令，长度为1-16字节。
*@param dwUserPinRetry 用户口令重试次数。
*/
typedef struct _HS_FORMATINFO
{
	LPCSTR 			szLabel;
	LPCSTR 			szSOPin;
	DWORD 			dwSOPinRetry;
	LPCSTR 			szUserPin;
	DWORD 			dwUserPinRetry;
	DWORD 			dwReserved;
}HS_FORMATINFO, *PHS_FORMATINFO;

/*
*@brief 文件信息结构，主要用于创建文件操作时传入传出格式化相关数据。
*@param dwFileID 文件ID。
*@param dwDeleteRight 文件的删除权限。
*@param dwReadRight 文件的读权限。
*@param dwWriteRight 文件的写权限。
*@param dwFileSize 文件的大小，取值为 0<dwFileLen<=MAX_FILE_LEN。
*/
typedef struct _HS_FILEINFO
{
	DWORD 			dwFileID;
	DWORD 			dwDeleteRight;
	DWORD 			dwReadRight;
	DWORD 			dwWriteRight;
	DWORD 			dwFileSize;
	DWORD 			dwReserved1;
	DWORD 			dwReserved2;
}HS_FILEINFO, *PHS_FILEINFO;


/*
*@brief 缓存结构体
*@param hCard:设备句柄
*@param szReaderName:读卡器名
*@param szDevSerial:设备序列号
*@param szDevPin:用户口令
*@param dwOperationRecord:操作记录    CSP_SIGN_OPERATION：签名操作    CSP_RSA_OPERATION：生成RSA密钥对操作    CSP_CRYPT_OPERATION：加密操作     CSP_WRITECERT_OPERATION：写证书操作
*@param dwReserve:保留位
*/

typedef struct _HSPin_Cache_st
{
	char   			szReaderName[HS_READERNAME_MAX];
	char   			szDevSerial[HS_SERIAL_LEN_MAX];
	char   			szDevPin[HS_PIN_LEN_MAX];
	DWORD  			dwOperationRecord[32];
	DWORD  			dwReserve;
}HSPIN_CACHE, *PHSPIN_CACHE;

//////////////////////////////////////////////////////////////////////////////
typedef DWORD (__stdcall *DEVINSERTED)(IN void *pParam);
typedef DWORD (__stdcall *DEVREMOVED)(IN void *pParam);

//1		//Enum
typedef DWORD (_stdcall *HSListReaders_)(OUT LPSTR pszReaderName, IN OUT DWORD *pdwReaderNameLen, OUT DWORD *pdwReaderNum);
extern HSListReaders_ HSListReaders;

typedef DWORD (_stdcall *HSWaitForDevEvent_)(OUT char *szDevName, IN OUT DWORD *pdwDevNameLen, OUT DWORD *pdwEvent);
extern HSWaitForDevEvent_ HSWaitForDevEvent;

typedef DWORD (_stdcall *HSSetDevChgEvent_)(IN LPCSTR szReaderName, IN DEVINSERTED InsertProc, IN DEVREMOVED RemoveProc, IN void *pProcParam, OUT HANDLE *phEvtHandler);
extern HSSetDevChgEvent_ HSSetDevChgEvent;

typedef DWORD (_stdcall *HSGetDevState_)(IN LPCSTR szReaderName, OUT DWORD *pdwState);
extern HSGetDevState_ HSGetDevState;

//2		//Device
typedef DWORD (_stdcall *HSConnectDev_)(IN LPCSTR szDevName, OUT HANDLE *phCard);
extern HSConnectDev_ HSConnectDev;

typedef DWORD (_stdcall *HSDisconnectDev_)(IN HANDLE hCard);
extern HSDisconnectDev_ HSDisconnectDev;

typedef DWORD (_stdcall *HSBeginTransaction_)(IN HANDLE hCard);
extern HSBeginTransaction_ HSBeginTransaction;

typedef DWORD (_stdcall *HSEndTransaction_)(IN HANDLE hCard);
extern HSEndTransaction_ HSEndTransaction;

typedef DWORD (_stdcall *HSGetDeviceInfo_)(IN HANDLE hCard, OUT HYC_DEVICEINFO *pDeviceSt);
extern HSGetDeviceInfo_ HSGetDeviceInfo;

typedef DWORD (_stdcall *HSGetSerial_)(IN HANDLE hCard, OUT BYTE bSerial[SERIAL_LEN]);
extern HSGetSerial_	HSGetSerial;

typedef DWORD (_stdcall *HSWriteLabel_)(IN HANDLE hCard, IN LPCSTR szLabel);
extern HSWriteLabel_ HSWriteLabel;

typedef DWORD (_stdcall *HSGetTotalSize_)(IN HANDLE hCard, OUT DWORD *pdwTotalSize);
extern HSGetTotalSize_ HSGetTotalSize;

typedef DWORD (_stdcall *HSGetFreeSize_)(IN HANDLE hCard, OUT DWORD *pdwFreeSize);
extern HSGetFreeSize_ HSGetFreeSize;

typedef DWORD (_stdcall *HSErase_)(IN HANDLE hCard);
extern HSErase_ HSErase;

typedef DWORD (_stdcall *HSFormat_)(IN HANDLE hCard, IN HS_FORMATINFO hsFormatInfo);
extern HSFormat_ HSFormat;

typedef DWORD (_stdcall *HSFormatEx_)(IN HANDLE hCard);
extern HSFormatEx_ HSFormatEx;

typedef DWORD (_stdcall *HSFormatEx2_)(IN HANDLE hCard, IN HS_FORMATINFO hsFormatInfo, IN DWORD dwRight);
extern HSFormatEx2_ HSFormatEx2;

typedef DWORD (_stdcall *HSFormatEx3_)(IN HANDLE hCard, IN HS_FORMATINFO hsFormatInfo, IN DWORD dwRight, IN char *pszDevAuth, IN DWORD dwDevAuthLen);
extern HSFormatEx3_ HSFormatEx3;

//3		//File
typedef DWORD (_stdcall *HSCreateFile_)(IN HANDLE hCard, IN HS_FILEINFO FileInfo);
extern HSCreateFile_ HSCreateFile;

typedef DWORD (_stdcall *HSWriteFile_)(IN HANDLE hCard, IN DWORD dwFileIndex, IN DWORD dwOffset, IN BYTE *pbData, IN DWORD dwDataLen);
extern HSWriteFile_ HSWriteFile;

typedef DWORD (_stdcall *HSReadFile_)(IN HANDLE hCard, IN DWORD dwFileIndex, IN DWORD dwOffset, OUT BYTE *pbData, IN OUT DWORD *pdwReadLen);
extern HSReadFile_ HSReadFile;

typedef DWORD (_stdcall *HSDeleteFile_)(IN HANDLE hCard, IN DWORD dwFileIndex);
extern HSDeleteFile_ HSDeleteFile;

typedef DWORD (_stdcall *HSGetFileSize_)(IN HANDLE hCard, IN DWORD dwFileIndex, OUT DWORD *pdwFileSize);
extern HSGetFileSize_ HSGetFileSize;

typedef DWORD (_stdcall *HSGetFileRight_)(IN HANDLE hCard, IN DWORD dwFileIndex, IN DWORD dwRightType, OUT DWORD *pdwPinType);
extern HSGetFileRight_ HSGetFileRight;

typedef DWORD (_stdcall *HSHasFileExist_)(IN HANDLE hCard, IN DWORD dwFileIndex, OUT DWORD *pdwFileState);
extern HSHasFileExist_ HSHasFileExist;

//4		//Container
typedef DWORD (_stdcall *HSListContainers_)(IN HANDLE hCard, OUT LPSTR pszContainersName, IN OUT DWORD *pdwContainersNameLen, OUT DWORD *pdwContainerNum);
extern HSListContainers_ HSListContainers;

typedef DWORD (_stdcall *HSGetContainerNum_)(IN HANDLE hCard, OUT DWORD *pdwContainerNum);
extern HSGetContainerNum_ HSGetContainerNum;

typedef DWORD (_stdcall *HSGetContainerParam_)(IN HANDLE hCard, IN LPCSTR pszContainerName, OUT DWORD *pdwContainerParam);
extern HSGetContainerParam_ HSGetContainerParam;

typedef DWORD (_stdcall *HSGetContainerName_)(IN HANDLE hCard, IN DWORD dwContainerIndex, OUT LPSTR pszContainerName, IN OUT DWORD *pdwContainerNameLen, OUT DWORD *pdwContainerParam);
extern HSGetContainerName_ HSGetContainerName;

typedef DWORD (_stdcall *HSCreateContainer_)(IN HANDLE hCard, IN LPCSTR szContainerName);
extern HSCreateContainer_ HSCreateContainer;

typedef DWORD (_stdcall *HSDelContainer_)(IN HANDLE hCard, IN LPCSTR szContainerName);
extern HSDelContainer_ HSDelContainer;

typedef DWORD (_stdcall *HSDelContainerItem_)(IN HANDLE hCard, IN char *pszContainerName, IN DWORD dwKeySpec);
extern HSDelContainerItem_ HSDelContainerItem;

//5		//Application
typedef DWORD (_stdcall *HSCreateApp_)(IN HANDLE hCard, IN DWORD dwAppID, IN DWORD dwEFCreateRight, IN DWORD dwEFDeleteRight, IN char szSOPIN[SO_PIN_LEN], IN DWORD dwSOPINRetries, IN char szUserPIN[USER_PIN_LEN], IN DWORD dwUserPINRetries, OUT HANDLE *phApp);
extern HSCreateApp_ HSCreateApp;

typedef DWORD (_stdcall *HSSelectApp_)(IN HANDLE hCard, IN DWORD dwAppID, OUT HANDLE *hApp);
extern HSSelectApp_ HSSelectApp;

typedef DWORD (_stdcall *HSCloseApp_)(IN HANDLE hApp);
extern HSCloseApp_ HSCloseApp;

typedef DWORD (_stdcall *HSDeleteApp_)(IN HANDLE hCard, IN DWORD dwAppID);
extern HSDeleteApp_ HSDeleteApp;

typedef DWORD (_stdcall *HSListApp_)(IN HANDLE hCard, OUT DWORD dwAppID[HTCSP_APP_NUM], OUT DWORD *pdwAppNum);
extern HSListApp_ HSListApp;

typedef DWORD (_stdcall *HSGetHWApp_)(IN HANDLE hCard, OUT DWORD *pdwHWApp);
extern HSGetHWApp_ HSGetHWApp;

//6		//Cert
typedef DWORD (_stdcall *HSReadCert_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, OUT BYTE *pbCert, IN OUT DWORD *pdwCertLen);
extern HSReadCert_ HSReadCert;

typedef DWORD (_stdcall *HSWriteCert_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN BYTE *pbCert, IN DWORD dwCertLen);
extern HSWriteCert_ HSWriteCert;

typedef DWORD (_stdcall *HSGetCertInfo_)(IN BYTE *pbCert, IN DWORD dwCertLen, OUT HTCSP_CERT_ST *pHtcs);
extern HSGetCertInfo_ HSGetCertInfo;

typedef DWORD (_stdcall *HSGetCertExtInfo_)(IN HANDLE hCard, IN BYTE *pbCert, IN DWORD dwCertLen, IN char *szName, OUT char *szExt, IN OUT DWORD *dwExtLen);
extern HSGetCertExtInfo_ HSGetCertExtInfo;

typedef DWORD (_stdcall *HSRegistCertByReaderName_)(IN LPCSTR szReaderName, IN LPCSTR szCSPName);
extern HSRegistCertByReaderName_ HSRegistCertByReaderName;

typedef DWORD (_stdcall *HSRegistCertByData_)(IN BYTE *pbCert, IN DWORD dwCertLen, IN LPCSTR szContainer, IN LPCSTR szCSPName);
extern HSRegistCertByData_ HSRegistCertByData;

typedef DWORD (_stdcall *HSUnRegistAllCert_)(IN LPCSTR szCSPName);
extern HSUnRegistAllCert_ HSUnRegistAllCert;

typedef DWORD (_stdcall *HSUnRegistCertByData_)(IN LPCSTR szCSPName, IN LPCSTR szContainer);
extern HSUnRegistCertByData_ HSUnRegistCertByData;

typedef DWORD (_stdcall *HSRegistCertByHandle_)(IN HANDLE hCard, IN LPCSTR szCSPName);
extern HSRegistCertByHandle_ HSRegistCertByHandle;

//7		//PIN
typedef DWORD (_stdcall *HSVerifyUserPin_)(IN HANDLE hCard, IN LPCSTR szUserPin, OUT DWORD *pdwRetryNum);
extern HSVerifyUserPin_ HSVerifyUserPin;

typedef DWORD (_stdcall *HSChangeUserPin_)(IN HANDLE hCard, IN LPCSTR szOldPin, IN LPCSTR szNewPin, OUT DWORD *pdwRetryNum);
extern HSChangeUserPin_ HSChangeUserPin;

typedef DWORD (_stdcall *HSVerifySOPin_)(IN HANDLE hCard, IN LPCSTR szSOPin, OUT DWORD *pdwRetryNum);
extern HSVerifySOPin_ HSVerifySOPin;

typedef DWORD (_stdcall *HSChangeSOPin_)(IN HANDLE hCard, IN LPCSTR szOldPin, IN LPCSTR szNewPin,OUT DWORD *pdwRetryNum);
extern HSChangeSOPin_ HSChangeSOPin;

typedef DWORD (_stdcall *HSReWriteUserPin_)(IN HANDLE hCard, IN LPCSTR szSOPin, IN LPCSTR szNewUserPin, OUT DWORD *pdwRetryNum);
extern HSReWriteUserPin_ HSReWriteUserPin;

typedef DWORD (_stdcall *HSWriteCachedPin_)(IN char *pszCachedData, IN DWORD dwCachedDataLen);
extern HSWriteCachedPin_ HSWriteCachedPin;

typedef DWORD (_stdcall *HSReadCachedPin_)(IN OUT char *pszCachedData, IN DWORD dwCachedDataLen);
extern HSReadCachedPin_ HSReadCachedPin;

typedef DWORD (_stdcall *HSClearPin_)(IN char *pszDeviceSerial);
extern HSClearPin_ HSClearPin;

typedef DWORD (_stdcall *HSClearAllPin_)();
extern HSClearAllPin_ HSClearAllPin;

typedef DWORD (_stdcall *HSGetUserPinRetryNum_)(IN HANDLE hCard, OUT DWORD *pdwRetryNum);
extern HSGetUserPinRetryNum_ HSGetUserPinRetryNum;

typedef DWORD (_stdcall *HSGetSOPinRetryNum_)(IN HANDLE hCard, OUT DWORD *pdwRetryNum);
extern HSGetSOPinRetryNum_ HSGetSOPinRetryNum;

typedef DWORD (_stdcall *HSGetUserPinRetryNumEx_)(IN HANDLE hCard, OUT DWORD *pdwRetryNum);
extern HSGetUserPinRetryNumEx_ HSGetUserPinRetryNumEx;

typedef DWORD (_stdcall *HSGetSOPinRetryNumEx_)(IN HANDLE hCard, OUT DWORD *pdwRetryNum);
extern HSGetSOPinRetryNumEx_ HSGetSOPinRetryNumEx;

typedef DWORD (_stdcall *HSReset_)(IN HANDLE hCard);
extern HSReset_ HSReset;

typedef DWORD (_stdcall *HSGetChangePINFlage_)(IN HANDLE hCard, OUT DWORD *pdwChangePINFlage);
extern HSGetChangePINFlage_ HSGetChangePINFlage;

typedef DWORD (_stdcall *HSDevAuth_)(IN HANDLE hCard, IN char *pszAuth, IN DWORD dwAuthLen);
extern HSDevAuth_ HSDevAuth;

typedef DWORD (_stdcall *HSChangeDevAuth_)(IN HANDLE hCard, IN char *pszAuth, IN DWORD dwAuthLen);
extern HSChangeDevAuth_ HSChangeDevAuth;

//8		//KeyPair
typedef DWORD (_stdcall *HSGenRSA_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec);
extern HSGenRSA_ HSGenRSA;

typedef DWORD (_stdcall *HSGenRSAEx_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN DWORD dwRSAKeyLen);
extern HSGenRSAEx_ HSGenRSAEx;

typedef DWORD (_stdcall *HSGenRSASoft_)(OUT HTCSP_RSA_ST *phtcsp_rsa_st, IN DWORD dwRSALen);
extern HSGenRSASoft_ HSGenRSASoft;

typedef DWORD (_stdcall *HSExportRSAPubKey_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, OUT BYTE *pbPubKey, IN OUT DWORD *pdwPubKeyLen);
extern HSExportRSAPubKey_ HSExportRSAPubKey;	

typedef DWORD (_stdcall *HSImportRSA_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN DWORD dwRSAKeyType, IN BYTE *pbRSAKey, IN DWORD dwRSAKeyLen);
extern HSImportRSA_ HSImportRSA;	

typedef DWORD (_stdcall *HSRSAEncrypt_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSAEncrypt_ HSRSAEncrypt;
								
typedef DWORD (_stdcall *HSRSADecrypt_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSADecrypt_ HSRSADecrypt;	

typedef DWORD (_stdcall *HSRSAEncryptSoft_)(IN HTCSP_RSA_PUBLIC_ST ht_RSA_pub_st, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSAEncryptSoft_ HSRSAEncryptSoft;	

typedef DWORD (_stdcall *HSRSADecryptSoft_)(IN HTCSP_RSA_PRIVATE_ST ht_RSA_pri_st, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern 	HSRSADecryptSoft_ HSRSADecryptSoft;

typedef DWORD (_stdcall *HSHashPadding_)(IN HANDLE hCard, IN DWORD dwHashAlgID, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSHashPadding_ HSHashPadding;	

typedef DWORD (_stdcall *HSRSASign_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSASign_ HSRSASign;

typedef DWORD (_stdcall *HSRSAVerify_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSAVerify_ HSRSAVerify;

typedef DWORD (_stdcall *HSRSASignSoft_)(IN HTCSP_RSA_PRIVATE_ST ht_RSA_pri_st, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSASignSoft_ HSRSASignSoft;

typedef DWORD (_stdcall *HSRSAVerifySoft_)(IN HTCSP_RSA_PUBLIC_ST ht_RSA_pub_st, IN DWORD dwPadMode, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSAVerifySoft_ HSRSAVerifySoft;

//9		//SymCrypt
typedef DWORD (_stdcall *HSWriteKey_)(IN HANDLE hCard, IN HS_KEYINFO KeyInfo);				
extern HSWriteKey_ HSWriteKey;

typedef DWORD (_stdcall *HSSymEcb_)(IN HANDLE hCard, IN DWORD dwKeyType, IN DWORD dwMode, IN BYTE *pbInData, IN DWORD dwDataLen, OUT BYTE *pbOutData);
extern HSSymEcb_ HSSymEcb;

typedef DWORD (_stdcall *HSSymSessionEcb_)(IN HANDLE hCard, IN HS_KEYINFO KeyInfo, IN DWORD dwMode, IN BYTE *pbInData, IN DWORD dwDataLen, OUT BYTE *pbOutData);
extern HSSymSessionEcb_ HSSymSessionEcb;

typedef DWORD (_stdcall *HSGenRandom_)(IN HANDLE hCard, IN DWORD dwRandomLen, OUT BYTE *pbRandom);
extern HSGenRandom_ HSGenRandom;

typedef DWORD (_stdcall *HSRSAPad_)(IN DWORD dwRSAOpMode, IN DWORD dwPadMode, IN DWORD dwNLen, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSRSAPad_ HSRSAPad;

typedef DWORD (_stdcall *HSDESEcb_)(IN CONST BYTE *pbKey, IN DWORD dwKeyLen, IN DWORD dwMode, IN CONST BYTE *pbInData, IN DWORD dwDataLen, OUT BYTE *pbOutData);
extern HSDESEcb_ HSDESEcb;

typedef DWORD (_stdcall *HSDESCbc_)(IN CONST BYTE *pbKey, IN DWORD dwKeyLen, IN BYTE bIV[8], IN DWORD dwMode, IN CONST BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSDESCbc_ HSDESCbc;

typedef DWORD (_stdcall *HSSCB2Cbc_)(IN HANDLE hCard, IN HS_KEYINFO *pKey, IN BYTE bIV[16], IN DWORD dwMode, IN CONST BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSSCB2Cbc_ HSSCB2Cbc;

typedef DWORD (_stdcall *HSSymInit_)(IN HANDLE hCard, IN DWORD dwAlgID, IN BYTE *pbKey, IN DWORD dwKeyBitLen, IN BYTE bIV[IV_LEN], IN DWORD dwIVLen, IN BYTE *pbSalt, IN DWORD dwSaltLen, IN DWORD dwModeOperation, IN DWORD dwEncrypt, OUT HANDLE *hSymCtx);
extern HSSymInit_ HSSymInit;

typedef DWORD (_stdcall *HSSymUpdate_)(IN HANDLE hCard, IN HANDLE hSymCtx, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSSymUpdate_ HSSymUpdate;

typedef DWORD (_stdcall *HSSymFinal_)(IN HANDLE hCard, IN HANDLE hSymCtx, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSSymFinal_ HSSymFinal;

//10	//Hash
typedef DWORD (_stdcall *HSSHA1_)(IN CONST BYTE *pbData, IN DWORD dwDataLen, OUT BYTE bHash[20]);
extern HSSHA1_ HSSHA1;

typedef DWORD (_stdcall *HSMD5_)(IN CONST BYTE *pbData, IN DWORD dwDataLen, OUT BYTE bHash[16]);
extern HSMD5_ HSMD5;

typedef DWORD (_stdcall *HSHashData_)(IN DWORD dwAlgID, IN CONST BYTE *pbData, IN DWORD dwDataLen, OUT BYTE bHash[64], IN OUT DWORD *pdwHashLen);
extern HSHashData_ HSHashData;

typedef DWORD (_stdcall *HSHashInit_)(IN DWORD dwAlgID, OUT HANDLE *phHash);
extern HSHashInit_ HSHashInit;

typedef DWORD (_stdcall *HSHashUpdate_)(IN HANDLE hHash, IN BYTE *pbData, IN DWORD dwDataLen);
extern HSHashUpdate_ HSHashUpdate;

typedef DWORD (_stdcall *HSHashFinal_)(IN HANDLE hHash, OUT BYTE *pHashData, IN OUT DWORD *pdwHashLen);
extern HSHashFinal_ HSHashFinal;

//11	//Encryption 
typedef DWORD (_stdcall *HSExportRSA_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec,
									   OUT HTCSP_RSA_ST *phtcsp_rsa_st);
extern HSExportRSA_ HSExportRSA;

typedef DWORD (_stdcall *HSImportRSAEx_)(IN HANDLE hCard, IN LPCSTR szContainerName, IN DWORD dwKeySpec, IN HTCSP_RSA_ST htcsp_rsa_st);
extern HSImportRSAEx_ HSImportRSAEx;

typedef DWORD (_stdcall *HSPreSetSymKeyID_)(IN DWORD dwKeyID);
extern HSPreSetSymKeyID_ HSPreSetSymKeyID;

typedef DWORD (_stdcall *HSCancelSetSymKeyID_)();
extern HSCancelSetSymKeyID_ HSCancelSetSymKeyID;

typedef DWORD (_stdcall *HSRSASignLCD_)(IN HANDLE hCard, IN char *pszContainerName, IN char *pszAccounts, IN DWORD dwAccountsLen, IN char *pszNum,  IN DWORD dwNumLen, OUT BYTE *pbSignature, IN OUT DWORD *pdwSignatureLen);
extern HSRSASignLCD_ HSRSASignLCD;

typedef DWORD (_stdcall *HS33ImprotRSA_)(IN HANDLE hCard, IN char *pszContainerName,IN DWORD dwKeySpec, IN BYTE *pbData, IN DWORD dwDataLen, IN BYTE *pbKey);
extern HS33ImprotRSA_ HS33ImprotRSA;

typedef DWORD (_stdcall *HSWriteCharacter_)(IN HANDLE hCard, IN char *pszData, IN DWORD dwCharacterLen, IN DWORD dwCharLen);
extern HSWriteCharacter_ HSWriteCharacter;

typedef DWORD (_stdcall *HSRSASignBD_)(IN HANDLE hCard, IN char *pszContainerName, IN char *pszData, IN DWORD dwDataLen, OUT BYTE *pbSignature, IN OUT DWORD *pdwSignatureLen);
extern HSRSASignBD_ HSRSASignBD;

typedef DWORD (_stdcall *BH_GetSignature_)(IN HANDLE hCard, IN char *pszContainerName, OUT BYTE *pbSignature, IN OUT DWORD *pdwSignatureLen);
extern BH_GetSignature_ BH_GetSignature;

typedef DWORD (_stdcall *BH_ShowData_)(IN HANDLE hCard, IN BYTE *pbData, IN DWORD dwDataLen);
extern BH_ShowData_ BH_ShowData;

//12	//
typedef DWORD (_stdcall *HSGenSM2KeyPair_)(IN HANDLE hCard, IN char *pszContainerName, IN DWORD dwAlgID, OUT HTCSP_SM2_PUBLIC_ST *pht_SM2_pub_st);
extern HSGenSM2KeyPair_ HSGenSM2KeyPair;

typedef DWORD (_stdcall *HSSM2SignByCon_)(IN HANDLE hCard, IN char *pszContainerName, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT HTCSP_SM2_Pri_Crypto_ST *pht_SM2_Pri_Crypto_st);
extern HSSM2SignByCon_ HSSM2SignByCon;

typedef DWORD (_stdcall *HSSM2DecryptByCon_)(IN HANDLE hCard, IN char *pszContainerName, IN DWORD dwKeySpec, IN HTCSP_SM2_Pub_Crypto_ST *pht_SM2_Pub_Crypto_st, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSSM2DecryptByCon_ HSSM2DecryptByCon;

typedef DWORD (_stdcall *HSSM2Sign_)(IN HANDLE hCard, IN HTCSP_SM2_PRIVATE_ST *pht_SM2_pri_st,
 IN BYTE *pbInData, IN DWORD dwInDataLen, OUT HTCSP_SM2_Pri_Crypto_ST *pht_SM2_Pri_Crypto_st);
extern HSSM2Sign_ HSSM2Sign;

typedef DWORD (_stdcall *HSSM2Verify_)(IN HANDLE hCard, IN HTCSP_SM2_PUBLIC_ST *pht_SM2_pub_st,
 IN BYTE *pbInData, IN DWORD dwInDataLen, IN HTCSP_SM2_Pri_Crypto_ST *pht_SM2_Pri_Crypto_st);
extern HSSM2Verify_ HSSM2Verify;

typedef DWORD (_stdcall *HSSM2Encrypt_)(IN HANDLE hCard, IN HTCSP_SM2_PUBLIC_ST *pht_SM2_pub_st, IN BYTE *pbInData, IN DWORD dwInDataLen, OUT HTCSP_SM2_Pub_Crypto_ST *pht_SM2_Pub_Crypto_st);
extern HSSM2Encrypt_ HSSM2Encrypt;

typedef DWORD (_stdcall *HSSM2Decrypt_)(IN HANDLE hCard, IN HTCSP_SM2_PRIVATE_ST *pht_SM2_pri_st, IN HTCSP_SM2_Pub_Crypto_ST *pht_SM2_Pub_Crypto_st, OUT BYTE *pbOutData, IN OUT DWORD *pdwOutDataLen);
extern HSSM2Decrypt_ HSSM2Decrypt;

//13	//
typedef DWORD (_stdcall *HSSetTimepiece_)(IN HANDLE hCard, IN SYSTEMTIME *pSystime);
extern HSSetTimepiece_	HSSetTimepiece;

typedef DWORD (_stdcall *HSGetTimepiece_)(IN HANDLE hCard, OUT SYSTEMTIME *pSystime);
extern HSGetTimepiece_	HSGetTimepiece;

typedef DWORD (_stdcall *HSGetTimepieceOfCipher_)(IN HANDLE hCard,IN BYTE pbRandom[8],OUT BYTE * pSystime);
extern HSGetTimepieceOfCipher_	HSGetTimepieceOfCipher;


//
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS_) (HANDLE, PBOOL);
extern LPFN_ISWOW64PROCESS_	fnIsWow64Process;

typedef UINT (WINAPI *LPFN_GetSystemWow64DirectoryA_)(LPSTR ,UINT);
extern LPFN_GetSystemWow64DirectoryA_ fnGetSystemWow64DirectoryA;

#ifdef __cplusplus
}
#endif

#endif