#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include <string>
#include <vector>
#include <map>

#include <wincrypt.h>
#include <cryptoki_ext.h>
#include <auxiliary.h>

#include "../UKeyVerify.h"

typedef struct{
	ULONG				ulSlotId;							//UKey�豸ID;
	ULONG				ulFlags;							//UKEY�豸Flags;
	HANDLE				hSession;							//UKey�豸�������;
	bool				bIsVerify;							//UKey�豸�Ƿ��Ѿ�У��;
	bool				bIsFinger;							//UKey�豸�Ƿ���ָ��;
	bool				bIsDefaultUserPIN;					//UKey�豸�Ƿ�ȱʡ�û�PIN;
	CK_UKEYENUM			stcUKeyEnum;						//UKey�豸ö����Ϣ;
	CK_UKEYVERIFY		stcUKeyVerify;						//UKey�豸У����Ϣ;
	CK_UKEYREADDATA		stcUKeyRead;						//UKey�豸��ȡ��Ϣ;
	CK_UKEYWRITEDATA	stcUKeyWrite;						//UKey�豸д����Ϣ;
}CK_UKEYDEVICE;

typedef struct{
	bool				bExit;								//�߳��Ƿ�ر�;
	ULONG 				ulThreadID;							//�߳�ID;
	ULONG				ulProcTimeOver;						//�߳�ѭ��ִ��ʱ��;
	ULONG				ulCloseTimeOver;					//�̹߳رյȴ�ʱ��;
	HANDLE				hThread;							//�߳̾��;
	HANDLE				hStartEvent;						//�߳̿�ʼ�¼�;
	HANDLE				hEndEvent;							//�̹߳ر��¼�;
}CK_UKEYTHREAD;

typedef struct tagUKeyHandle{
	bool				bIsInited;							//PKCS#11���Ƿ��ʼ��;
	bool				bIsEnumThread;						//�Ƿ�ö���豸;
	bool				bIsWorkThread;						//�Ƿ�У���豸;
	HANDLE				hEnumThread;						//UKey�豸ö���߳̾��;
	HANDLE				hWorkThread;						//UKey�豸�����߳̾��;
	CK_UKEYTHREAD		stcUKeyEnumThread;					//UKey�豸ö���߳�;
	CK_UKEYTHREAD		strUKeyWorkThread;					//UKey�豸�����߳�;	
	CRITICAL_SECTION	caUKeySection;						//UKey�豸�ٽ���;
	AUX_FUNC_LIST_PTR	pAuxFunc;							//UKey�豸�ڲ��ص�����;
	CK_UKEYENUM_CALLBACK_FUNC	pfUKeyEnum;					//UKey�豸ö�ٻص�;
	CK_UKEYVERIFY_CALLBACK_FUNC	pfUkeyVerify;				//UKey�豸У��ص�;
	CK_UKEYREAD_CALLBACK_FUNC   pfUkeyReadData;				//UKey�豸���ݶ�ȡ�ص�;
	CK_UKEYWRITE_CALLBACK_FUNC  pfUKeyWriteData;			//UKey�豸����д��ص�;	
	std::map<CK_SLOT_ID, CK_UKEYDEVICE*> mapUKeyDevice;		//UKey�豸�洢;

	tagUKeyHandle::tagUKeyHandle()
	{
		bIsInited = false;
		bIsEnumThread = false;
		bIsWorkThread = false;
		hEnumThread = NULL;
		hWorkThread = NULL;
		pAuxFunc = NULL;
		pfUKeyEnum = NULL;
		pfUkeyVerify = NULL;
		pfUkeyReadData = NULL;
		pfUKeyWriteData = NULL;

		mapUKeyDevice.clear();
		memset(&stcUKeyEnumThread, 0x0, sizeof(CK_UKEYTHREAD));
		memset(&strUKeyWorkThread, 0x0, sizeof(CK_UKEYTHREAD));
	}
}CK_UKEYHANDLE;

#endif