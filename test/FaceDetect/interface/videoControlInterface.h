#ifndef __VIDEOCONTROL_INTERFACE_H__
#define __VIDEOCONTROL_INTERFACE_H__

#define MAX_CAMERANAME_LEN				256
typedef struct tagCameraData
{
	int					iCameraId;
	char				szFriendlyName[MAX_CAMERANAME_LEN];
	char				szMonikerName[MAX_CAMERANAME_LEN];
}VIDEO_CAMERADATA;

// typedef struct tagFrame_data_buffer{
// 	unsigned char*		pFrameData;
// 	unsigned long		uFrameDataLen;
// 	unsigned long		uPixelWidth;
// 	unsigned long		uPixelHeight;
// }FRAME_DATA_BUFFER;

using namespace std;
class CVideoControlInterface
{
public:
	virtual int				rzt_video_geterror() = 0;
	
	virtual bool			rzt_video_enumCamera(OUT std::vector<VIDEO_CAMERADATA> &vecCamera) = 0;
	virtual bool			rzt_video_isCameraOpen() = 0;

	virtual bool			rzt_video_openDevice(IN int iCameraId) = 0;
	virtual bool			rzt_video_closeDevice() = 0;
	
	virtual bool			rzt_video_getVideoImage(OUT FRAME_DATA_BUFFER *pFrameData) = 0;
	
protected:
	int						m_ierror;	
};

#endif