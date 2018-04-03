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
	bool				bIsVerify;							//UKey设备是否已经校验;
	CK_UKEYENUM			stcUKeyEnum;						//UKey设备枚举信息;
	CK_UKEYVERIFY		stcUKeyVerify;						//UKey设备校验信息;
	CK_UKEYREADDATA		stcUKeyRead;						//UKey设备读取信息;
	CK_UKEYWRITEDATA	stcUKeyWrite;						//UKey设备写入信息;
}CK_UKEYDEVICE;

typedef struct {
	bool				bExit;								//线程是否关闭;
	ULONG 				ulThreadID;							//线程ID;
	ULONG				ulProcTimeOver;						//线程循环执行时间;
	ULONG				ulCloseTimeOver;					//线程关闭等待时间;
	HANDLE				hThread;							//线程句柄;
	HANDLE				hStartEvent;						//线程开始事件;
	HANDLE				hEndEvent;							//线程关闭事件;
}CK_UKEYTHREAD;

typedef struct {
	bool				bIsInited;							//PKCS#11库是否初始化;
	bool				bIsEnumThread;						//是否枚举设备;
	bool				bIsWorkThread;						//是否校验设备;
	HANDLE				hEnumThread;						//UKey设备枚举线程句柄;
	HANDLE				hWorkThread;						//UKey设备工作线程句柄;
	CK_UKEYTHREAD		stcUKeyEnumThread;					//UKey设备枚举线程;
	CK_UKEYTHREAD		strUKeyWorkThread;					//UKey设备工作线程;	
	CRITICAL_SECTION	caUKeySection;						//UKey设备临界区;
	CK_UKEYENUM_CALLBACK_FUNC	pfUKeyEnum;					//UKey设备枚举回调;
	CK_UKEYVERIFY_CALLBACK_FUNC	pfUkeyVerify;				//UKey设备校验回调;
	CK_UKEYREAD_CALLBACK_FUNC   pfUkeyReadData;				//UKey设备数据读取回调;
	CK_UKEYWRITE_CALLBACK_FUNC  pfUKeyWriteData;			//UKey设备数据写入回调;	
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*> mapUKeyDevice;		//UKey设备存储;
}CK_UKEYHANDLE;

#endif