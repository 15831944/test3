#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include <string>
#include <map>
#include <vector>

typedef enum {
	CK_UKEYDEVEMPTYTYPE = 0,					//������;
	CK_UKEYDEVNORMALTYPE,						//��ͨUKey�豸;
	CK_UKEYDEVFINGERTYPE,						//ָ��Key�豸;
}CK_UKEYDEVICETYPE;

typedef enum {
	CK_UKEYSTATEEMPTYTYPE = 0,					//������;
	CK_UKEYSTATESUCCEDTYPE,						//UKEY�豸�ɹ�����;
	CK_UKEYSTATEFAILEDTYPE,						//UKEY�豸ʧ�ܲ���;
	CK_UKEYSTATEINSERTTYPE,						//UKEY�豸�������;
	CK_UKEYSTATEREMOVETYPE,						//UKEY�豸�γ�����;
	CK_UKEYSTATEINPUTETYPE,						//UKEY�豸�������;
	CK_UKEYSTATEOUTPUTTYPE,						//UKEY�豸�������;
}CK_UKEYSTATETYPE;

typedef enum {
	CK_OPERATEEMPTYTYPE = 0,					//������	
	CK_OPERATEENUMSLOTTYPE,						//ö���豸����;
	CK_OPERATEVERIFYTYPE,						//�豸����У�����;
	CK_OPERATEWRITETYPE,						//�豸д�����;
	CK_OPERATEREADTYPE,							//�豸��������;
}CK_UKEYOPERATETYPE;

typedef struct {
	BOOL				bFlags;					//�����ɹ�,ʧ��;
	BOOL				bExist;					//UKey�豸�Ƿ����;
	CK_UKEYDEVICETYPE	emUKeyType;				//UKey�豸����;
	CK_UKEYSTATETYPE	emUKeyState;			//UKey�豸״̬;
	CK_UKEYOPERATETYPE	emUKeyOperateType;		//UKey�豸��������;
	ULONG				ulSlotId;				//UKey�豸ID;
	ULONG				ulFlags;				//UKEY�豸Flags;
	HANDLE				hSession;				//UKey�豸�������;
	HANDLE				hEvent;					//UKey�豸��д�¼�;
	ULONG				ulRemainCount;			//UKey�豸PIN����ʣ�����;
	BYTE*				pFingerData;			//UKey�豸ָ������;
	ULONG				ulFingerDataLen;		//UKey�豸ָ�����ݳ���;
}CK_UKEYPROCINFO;

typedef struct {
	char				szUserNum[64];			//���ȵ�¼����;
	char				szUserPasswd[64];		//���ȵ�¼����;
	char				szUserPIN[64];			//UKey�豸PIN��;
	char				szPromptText[64];		//�豸��ʾ��Ϣ;
}CK_UKEYUSERDATA;

typedef BOOL (*CK_UKDATA_CALLBACK_FUNC)(const CK_UKEYPROCINFO *pUKeyInfo, CK_UKEYUSERDATA *pUKeyUserData, LPVOID lpParam);

#endif