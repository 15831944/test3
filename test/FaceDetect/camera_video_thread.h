#ifndef __CAMERA_VIDEO_THREAD_H__
#define __CAMERA_VIDEO_THREAD_H__

class CCameraVideoThread
{
public:
	CCameraVideoThread();
	~CCameraVideoThread();
	
public:
	static DWORD WINAPI		CameraVideoProc(LPVOID lpParam);
	
public:
	bool					CreateCameraVideoProc(CVideoControlInterface *pFaceDetect);
	void					CloseCameraVideoProc();

	void					SetProcState(BOOL bState);
	
protected:
	void					CameraVideoInfo();
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
private:
	mutable volatile bool	m_bExit;
	
	DWORD					m_dwThreadId;
	
	DWORD					m_dwProcTimeOver;
	DWORD					m_dwCloseTimeOver;

	CVideoControlInterface	*m_pFaceDetect;
};

#endif