#ifndef __CHECKLINK_THREAD_H__
#define __CHECKLINK_THREAD_H__

#define MAX_IPADDRESS_LEN		64
#define MAX_ADAPTERNAME_LEN		64

typedef enum{
	LINK_STATEEMPTYTYPE = 0,
	LINK_STATESUCCEDTYPE,
	LINK_STATEFAILEDTYPE,
	LINK_STATEINPUTTYPE,
	LINK_STATEOUTPUTTYPE,
}LINK_STATE_TYPE;

typedef struct{
	IP_ADAPTER_INFO		stcIpAdapterInfo;
	char				szIpAddress[MAX_IPADDRESS_LEN];
	char				szAdapterName[MAX_ADAPTERNAME_LEN];
	unsigned long		ulAdapterIndex;
}LINK_ADAPTER_DATA;

typedef struct{
	BOOL				bIsConnected;
	char				szIpAddress[MAX_IPADDRESS_LEN];
	LINK_STATE_TYPE		emLinkStateType;
}LINK_ADAPTER_INFO;

typedef BOOL (*CHECK_LINK_CALLBACK_FUNC)(LINK_ADAPTER_INFO *pLinkAdapterInfo);

class CCheckLinkThread
{
public:
	CCheckLinkThread();
	~CCheckLinkThread();
	
public:
	BOOL					CreateCheckLinkProc(CHECK_LINK_CALLBACK_FUNC pfCheckLinkFunc);
	void					CloseCheckLinkProc();
	
protected:
	static DWORD WINAPI		CheckLinkProc(LPVOID lpParam);
	
protected:
	void					CheckLinkInfo();

	BOOL					GetIpAdapterInfo();
	BOOL					GetLinkAdapterInfo(LINK_ADAPTER_DATA *pLinkAdapterData, LINK_ADAPTER_INFO *pLinkAdapterInfo);
	BOOL					CheckLinkAdapterInfo(LINK_ADAPTER_DATA *pLinkAdapterData, LINK_ADAPTER_INFO *pLinkAdapterInfo);
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
private:
	BOOL					m_bExit;
	
	DWORD					m_dwThreadID;
	DWORD					m_dwProcTimeOver;
	DWORD					m_dwCloseTimeOver;

	IP_ADAPTER_INFO*		m_pIpAdapterInfo;
	CHECK_LINK_CALLBACK_FUNC m_pfCheckLinkFunc;
};

#endif