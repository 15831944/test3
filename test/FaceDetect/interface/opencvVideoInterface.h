#ifndef __OPENCVVIDEO_INTERFACE_H__
#define __OPENCVVIDEO_INTERFACE_H__

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "videoControlInterface.h"

using namespace std;
class COpencvVideoInterface : public CVideoControlInterface
{
public:
	COpencvVideoInterface();
	virtual ~COpencvVideoInterface();
	
public:
	virtual int				rzt_video_geterror();
	
	virtual bool			rzt_video_enumCamera(OUT std::vector<VIDEO_CAMERADATA> &vecCamera);
	virtual bool			rzt_video_isCameraOpen();

	virtual bool			rzt_video_openDevice(IN int iCameraId);
	virtual bool			rzt_video_closeDevice();
	
	virtual bool			rzt_video_getVideoImage(OUT FRAME_DATA_BUFFER *pFrameData);

private:
	CvCapture 				*m_pCamera;
};

#endif