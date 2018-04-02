#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include <string>
#include <vector>
#include <map>

#include <wincrypt.h>
#include <cryptoki_ext.h>

#include "../UKeyVerify.h"

typedef struct {
	ULONG				ulSlotId;							//UKey�豸ID;
	ULONG				ulFlags;							//UKEY�豸Flags;
	HANDLE				hSession;							//UKey�豸�������;
	CK_UKEYENUM			stcUKeyEnum;						//UKey�豸ö����Ϣ
	CK_UKEYVERIFY		stcUKeyVerify;						//UKey�豸У����Ϣ
	CK_UKEYREADDATA		stcUKeyRead;						//UKey�豸��ȡ��Ϣ
	CK_UKEYWRITEDATA	stcUKeyWrite;						//UKey�豸д����Ϣ
}CK_UKEYDEVICE;

typedef struct {
	bool				bIsInited;							//PKCS#11���Ƿ��ʼ��
	bool				bIsEnumThread;						//�Ƿ�ö���豸
	bool				bIsWorkThread;						//�Ƿ�У���豸
	HANDLE				hEnumThread;						//UKey�豸ö���߳̾��
	HANDLE				hWorkThread;						//UKey�豸�����߳̾��
	CK_UKEYDEVICE		stcUKeyDevice;						//UKey�豸��Ϣ
}CK_UKEYHANDLE;

#endif