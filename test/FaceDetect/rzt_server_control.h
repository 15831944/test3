#ifndef __RZT_SERVER_CONTROL_H__
#define __RZT_SERVER_CONTROL_H__

#include "camera_video_thread.h"

class CRztServerControl
{
public:
	CRztServerControl();
	virtual ~CRztServerControl();

public:
	bool						InitServer();
	void						UnInitServer();

	bool						EnumCamera();
	bool						OpenCamera(int iCameraId);
	bool						CloseCamera();

protected:
	CFaceDetectInterface		*m_pFaceDetect;
	CVideoControlInterface		*m_pVideoControl;

	CCameraVideoThread			m_CameraVideoThread;

private:
	thread_queue<FRAME_DATA_BUFFER*> m_queFrameData;
};

#endif