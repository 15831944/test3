#ifndef __THREAD_MSG_HANDLER_H__
#define __THREAD_MSG_HANDLER_H__

typedef enum {
	THREAD_MSG_EMPTYID = 0,
	
}THREAD_MSGID;

class CThreadMsgHandler
{
public:
	CThreadMsgHandler();
	~CThreadMsgHandler();
	
	static CThreadMsgHandler& Instance();
	
public:
	BOOL			InitMsgHandler();
	void			unInitMsgHandler();
};

#endif