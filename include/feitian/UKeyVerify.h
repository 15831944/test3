#ifndef __UKEY_VERIFY_H__
#define __UKEY_VERIFY_H__

#ifdef UKEYVERIFY_EXPORTS
#define RZT_UKEYVERIFY_API extern "C" __declspec(dllexport)
#else
#define RZT_UKEYVERIFY_API extern "C" __declspec(dllimport)
#endif

#define UKEYPIN_MAX_LEN		32
#define USERNUM_MAX_LEN		64
#define USERPASSWD_MAX_LEN	64

#define USERFINGER_ENROLL_NUMBER 4
#define DEFAULT_USERPIN		"12345678"

typedef enum{
	CK_UKEYDEVEMPTYTYPE = 0,								//空类型;
	CK_UKEYDEVNORMALTYPE,									//普通UKey设备;
	CK_UKEYDEVFINGERTYPE,									//指纹Key设备;
}CK_UKEYDEVICETYPE;

typedef enum{
	CK_UKEYSTATEEMPTYTYPE = 0,								//空类型;
	CK_UKEYSTATESUCCEDTYPE,									//UKEY设备成功操作;
	CK_UKEYSTATEFAILEDTYPE,									//UKEY设备失败操作;
	CK_UKEYSTATEINSERTTYPE,									//UKEY设备插入操作;
	CK_UKEYSTATEREMOVETYPE,									//UKEY设备拔除操作;
	CK_UKEYSTATEINPUTETYPE,									//UKEY设备输入操作;
	CK_UKEYSTATEOUTPUTTYPE,									//UKEY设备输出操作;
	CK_UKEYSTATEMODIFYTYPE,									//UKEY设备修改操作;
}CK_UKEYSTATETYPE;

typedef enum{
	CK_UKEYLEFTLITTLEFINGER = 0,							//左手小拇指
	CK_UKEYLEFTRINGFINGER,									//左手无名指
	CK_UKEYLEFTMIDDLEFINGER,								//左手中指
	CK_UKEYLEFTINDEXFINGER,									//左手食指
	CK_UKEYLEFTTHUMBFINGER,									//左手大拇指
	CK_UKEYRIGHTTHUMBFINGER,								//右手大拇指
	CK_UKEYRIGHTINDEXFINGER,								//右手食指
	CK_UKEYRIGHTMIDDLEFINGER,								//右手中指
	CK_UKEYRIGHTRINGFINGER,									//右手无名指
	CK_UKEYRIGHTLITTLEFINGER,								//右手小拇指
}CK_UKEYFINGER;

typedef struct{
	bool				bExist;								//UKey设备是否存在;
	ULONG				ulSlotId;							//UKey设备ID;
	CK_UKEYDEVICETYPE	emUKeyType;							//UKey设备类型;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey设备状态;
}CK_UKEYENUM;

typedef struct{
	ULONG				ulRemainCount;						//UKey设备PIN口令剩余次数;
	char				szUserPIN[UKEYPIN_MAX_LEN];			//UKey设备原始PIN码;
	char				szNewUserPIN[UKEYPIN_MAX_LEN];		//UKey设备新的PIN码;
	HANDLE				hEvent;								//UKey设备指纹录入事件;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey设备状态;
}CK_UKEYVERIFY; 

typedef struct{
	char				szUserNum[USERNUM_MAX_LEN];			//调度登录号码;
	char				szUserPasswd[USERPASSWD_MAX_LEN];	//调度登录密码;
	HANDLE				hEvent;								//UKey设备读写事件;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey设备状态;
}CK_UKEYREADDATA;

typedef struct{
	char				szUserNum[USERNUM_MAX_LEN];			//调度登录号码;
	char				szUserPasswd[USERPASSWD_MAX_LEN];	//调度登录密码;
	HANDLE				hEvent;								//UKey设备读写事件;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey设备状态;
}CK_UKEYWRITEDATA;

typedef BOOL (*CK_UKEYENUM_CALLBACK_FUNC)(const CK_UKEYENUM *pUKeyEnum);		//UKey设备枚举回调
typedef BOOL (*CK_UKEYVERIFY_CALLBACK_FUNC)(CK_UKEYVERIFY *pUKeyVerify);		//UKey设备校验回调	
typedef BOOL (*CK_UKEYREAD_CALLBACK_FUNC)(CK_UKEYREADDATA *pUKeyRead);			//UKey设备读取信息回调
typedef BOOL (*CK_UKEYWRITE_CALLBACK_FUNC)(CK_UKEYWRITEDATA *pUKeyWrite);		//UKey设备写入信息回调

/*
 * 打开UKey设备一个处理会话
 * pfUKeyEnum : UKey设备枚举回调;
 * pfUkeyVerify : UKey设备校验回调;
 * pfUkeyReadData  : UKey设备读取信息回调;
 * pfUKeyWriteData : UKey设备写入信息回调;
 * hUKeyProc : 返回一个UKey设备处理会话;
 */
RZT_UKEYVERIFY_API bool	rzt_openUKeyProc(CK_UKEYENUM_CALLBACK_FUNC pfUKeyEnum, CK_UKEYVERIFY_CALLBACK_FUNC pfUkeyVerify, CK_UKEYREAD_CALLBACK_FUNC pfUkeyReadData, CK_UKEYWRITE_CALLBACK_FUNC pfUKeyWriteData, HANDLE &hUKeyProc);

/*
 * 关闭UKey设备一个处理会话
 * hUKeyProc : 传输一个UKey设备处理会话;
 */
RZT_UKEYVERIFY_API bool rzt_closeUKeyProc(HANDLE hUKeyProc);

#endif