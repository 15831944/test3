#ifndef __OPENCV_VIDEO_INTERFACE_H__
#define __OPENCV_VIDEO_INTERFACE_H__

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "videoInterface.h"

using namespace std;
class COpencvVideoInterface : public CVideoInterface
{
public:
	COpencvVideoInterface();
	virtual ~COpencvVideoInterface();
	
public:
	virtual int			video_getError();
	virtual bool		video_enum(OUT std::vector<VIDEO_DEVDATA> &vecDevData);
	
	virtual bool		video_isOpen();
	virtual bool		video_open(IN int nDevId);
	virtual bool		video_close();
	
	virtual bool		video_getImage(OUT void *pImageData);
	
private:
	CvCapture 			*m_pCamera;
};

#endif