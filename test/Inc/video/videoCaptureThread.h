#ifndef __VIDEOCAPTURE_THREAD_H__
#define __VIDEOCAPTURE_THREAD_H__

class CVideoCaptureThread
{
public:
	CVideoCaptureThread();
	~CVideoCaptureThread();

public:
	
protected:
	HANDLE				m_hThread;
	
	HANDLE				m_hStartEvent;
	HANDLE				m_hEndEvent;
	
private:
	mutable volatile BOOL m_bExit;
};

#endif