#ifndef __VIDEOWNDTHREAD_H__
#define __VIDEOWNDTHREAD_H__


class CVideoWndThread : public CWinThread
{
	DECLARE_DYNCREATE(CVideoWndThread)
	
public:
	CVideoWndThread();
	~CVideoWndThread();

public:
	virtual BOOL	InitInstance();
	virtual	int		ExitInstance();
	virtual int		Run();

protected:
	DECLARE_MESSAGE_MAP()	
};

#endif