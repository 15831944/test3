#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include <string>
#include <vector>
#include <map>

#include <wincrypt.h>
#include <cryptoki_ext.h>

#include "../UKeyVerify.h"

typedef struct {
	ULONG				ulSlotId;							//UKey设备ID;
	ULONG				ulFlags;							//UKEY设备Flags;
	HANDLE				hSession;							//UKey设备操作句柄;
	CK_UKEYENUM			stcUKeyEnum;						//UKey设备枚举信息
	CK_UKEYVERIFY		stcUKeyVerify;						//UKey设备校验信息
	CK_UKEYREADDATA		stcUKeyRead;						//UKey设备读取信息
	CK_UKEYWRITEDATA	stcUKeyWrite;						//UKey设备写入信息
}CK_UKEYDEVICE;

typedef struct {
	bool				bIsInited;							//PKCS#11库是否初始化
	bool				bIsEnumThread;						//是否枚举设备
	bool				bIsWorkThread;						//是否校验设备
	HANDLE				hEnumThread;						//UKey设备枚举线程句柄
	HANDLE				hWorkThread;						//UKey设备工作线程句柄
	CK_UKEYDEVICE		stcUKeyDevice;						//UKey设备信息
}CK_UKEYHANDLE;

#endif