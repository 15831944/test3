#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include <string>
#include <map>
#include <vector>

typedef enum {
	CK_UKEYDEVEMPTYTYPE = 0,					//空类型;
	CK_UKEYDEVNORMALTYPE,						//普通UKey设备;
	CK_UKEYDEVFINGERTYPE,						//指纹Key设备;
}CK_UKEYDEVICETYPE;

typedef enum {
	CK_UKEYSTATEEMPTYTYPE = 0,					//空类型;
	CK_UKEYSTATESUCCEDTYPE,						//UKEY设备成功操作;
	CK_UKEYSTATEFAILEDTYPE,						//UKEY设备失败操作;
	CK_UKEYSTATEINSERTTYPE,						//UKEY设备插入操作;
	CK_UKEYSTATEREMOVETYPE,						//UKEY设备拔除操作;
	CK_UKEYSTATEINPUTETYPE,						//UKEY设备输入操作;
	CK_UKEYSTATEOUTPUTTYPE,						//UKEY设备输出操作;
}CK_UKEYSTATETYPE;

typedef enum {
	CK_OPERATEEMPTYTYPE = 0,					//空类型	
	CK_OPERATEENUMSLOTTYPE,						//枚举设备操作;
	CK_OPERATEVERIFYTYPE,						//设备口令校验操作;
	CK_OPERATEWRITETYPE,						//设备写入操作;
	CK_OPERATEREADTYPE,							//设备读出操作;
}CK_UKEYOPERATETYPE;

typedef struct {
	BOOL				bFlags;					//操作成功,失败;
	BOOL				bExist;					//UKey设备是否存在;
	CK_UKEYDEVICETYPE	emUKeyType;				//UKey设备类型;
	CK_UKEYSTATETYPE	emUKeyState;			//UKey设备状态;
	CK_UKEYOPERATETYPE	emUKeyOperateType;		//UKey设备操作类型;
	ULONG				ulSlotId;				//UKey设备ID;
	ULONG				ulFlags;				//UKEY设备Flags;
	HANDLE				hSession;				//UKey设备操作句柄;
	HANDLE				hEvent;					//UKey设备读写事件;
	ULONG				ulRemainCount;			//UKey设备PIN口令剩余次数;
	BYTE*				pFingerData;			//UKey设备指纹数据;
	ULONG				ulFingerDataLen;		//UKey设备指纹数据长度;
}CK_UKEYPROCINFO;

typedef struct {
	char				szUserNum[64];			//调度登录号码;
	char				szUserPasswd[64];		//调度登录密码;
	char				szUserPIN[64];			//UKey设备PIN码;
	char				szPromptText[64];		//设备提示信息;
}CK_UKEYUSERDATA;

typedef BOOL (*CK_UKDATA_CALLBACK_FUNC)(const CK_UKEYPROCINFO *pUKeyInfo, CK_UKEYUSERDATA *pUKeyUserData, LPVOID lpParam);

#endif