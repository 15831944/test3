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
	CK_UKEYDEVEMPTYTYPE = 0,								//������;
	CK_UKEYDEVNORMALTYPE,									//��ͨUKey�豸;
	CK_UKEYDEVFINGERTYPE,									//ָ��Key�豸;
}CK_UKEYDEVICETYPE;

typedef enum{
	CK_UKEYSTATEEMPTYTYPE = 0,								//������;
	CK_UKEYSTATESUCCEDTYPE,									//UKEY�豸�ɹ�����;
	CK_UKEYSTATEFAILEDTYPE,									//UKEY�豸ʧ�ܲ���;
	CK_UKEYSTATEINSERTTYPE,									//UKEY�豸�������;
	CK_UKEYSTATEREMOVETYPE,									//UKEY�豸�γ�����;
	CK_UKEYSTATEINPUTETYPE,									//UKEY�豸�������;
	CK_UKEYSTATEOUTPUTTYPE,									//UKEY�豸�������;
	CK_UKEYSTATEMODIFYTYPE,									//UKEY�豸�޸Ĳ���;
}CK_UKEYSTATETYPE;

typedef enum{
	CK_UKEYLEFTLITTLEFINGER = 0,							//����СĴָ
	CK_UKEYLEFTRINGFINGER,									//��������ָ
	CK_UKEYLEFTMIDDLEFINGER,								//������ָ
	CK_UKEYLEFTINDEXFINGER,									//����ʳָ
	CK_UKEYLEFTTHUMBFINGER,									//���ִ�Ĵָ
	CK_UKEYRIGHTTHUMBFINGER,								//���ִ�Ĵָ
	CK_UKEYRIGHTINDEXFINGER,								//����ʳָ
	CK_UKEYRIGHTMIDDLEFINGER,								//������ָ
	CK_UKEYRIGHTRINGFINGER,									//��������ָ
	CK_UKEYRIGHTLITTLEFINGER,								//����СĴָ
}CK_UKEYFINGER;

typedef struct{
	bool				bExist;								//UKey�豸�Ƿ����;
	ULONG				ulSlotId;							//UKey�豸ID;
	CK_UKEYDEVICETYPE	emUKeyType;							//UKey�豸����;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey�豸״̬;
}CK_UKEYENUM;

typedef struct{
	ULONG				ulRemainCount;						//UKey�豸PIN����ʣ�����;
	char				szUserPIN[UKEYPIN_MAX_LEN];			//UKey�豸ԭʼPIN��;
	char				szNewUserPIN[UKEYPIN_MAX_LEN];		//UKey�豸�µ�PIN��;
	HANDLE				hEvent;								//UKey�豸ָ��¼���¼�;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey�豸״̬;
}CK_UKEYVERIFY; 

typedef struct{
	char				szUserNum[USERNUM_MAX_LEN];			//���ȵ�¼����;
	char				szUserPasswd[USERPASSWD_MAX_LEN];	//���ȵ�¼����;
	HANDLE				hEvent;								//UKey�豸��д�¼�;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey�豸״̬;
}CK_UKEYREADDATA;

typedef struct{
	char				szUserNum[USERNUM_MAX_LEN];			//���ȵ�¼����;
	char				szUserPasswd[USERPASSWD_MAX_LEN];	//���ȵ�¼����;
	HANDLE				hEvent;								//UKey�豸��д�¼�;
	CK_UKEYSTATETYPE	emUKeyState;						//UKey�豸״̬;
}CK_UKEYWRITEDATA;

typedef BOOL (*CK_UKEYENUM_CALLBACK_FUNC)(const CK_UKEYENUM *pUKeyEnum);		//UKey�豸ö�ٻص�
typedef BOOL (*CK_UKEYVERIFY_CALLBACK_FUNC)(CK_UKEYVERIFY *pUKeyVerify);		//UKey�豸У��ص�	
typedef BOOL (*CK_UKEYREAD_CALLBACK_FUNC)(CK_UKEYREADDATA *pUKeyRead);			//UKey�豸��ȡ��Ϣ�ص�
typedef BOOL (*CK_UKEYWRITE_CALLBACK_FUNC)(CK_UKEYWRITEDATA *pUKeyWrite);		//UKey�豸д����Ϣ�ص�

/*
 * ��UKey�豸һ������Ự
 * pfUKeyEnum : UKey�豸ö�ٻص�;
 * pfUkeyVerify : UKey�豸У��ص�;
 * pfUkeyReadData  : UKey�豸��ȡ��Ϣ�ص�;
 * pfUKeyWriteData : UKey�豸д����Ϣ�ص�;
 * hUKeyProc : ����һ��UKey�豸����Ự;
 */
RZT_UKEYVERIFY_API bool	rzt_openUKeyProc(CK_UKEYENUM_CALLBACK_FUNC pfUKeyEnum, CK_UKEYVERIFY_CALLBACK_FUNC pfUkeyVerify, CK_UKEYREAD_CALLBACK_FUNC pfUkeyReadData, CK_UKEYWRITE_CALLBACK_FUNC pfUKeyWriteData, HANDLE &hUKeyProc);

/*
 * �ر�UKey�豸һ������Ự
 * hUKeyProc : ����һ��UKey�豸����Ự;
 */
RZT_UKEYVERIFY_API bool rzt_closeUKeyProc(HANDLE hUKeyProc);

#endif