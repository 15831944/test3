#ifndef __VIDEO_INTERFACE_H__
#define __VIDEO_INTERFACE_H__

#define MAX_DEVNAME_LEN	 256
typedef struct {
	int		nDevId;
	char	szFriendlyName[MAX_DEVNAME_LEN];
	char	szMonikerName[MAX_DEVNAME_LEN];
}VIDEO_DEVDATA;

using namespace std;
class CVideoInterface
{
public:
	virtual int			video_getError() = 0;
	virtual bool		video_enum(OUT std::vector<VIDEO_DEVDATA> &vecDevData) = 0;
	
	virtual bool		video_isOpen() = 0;
	virtual bool		video_open(IN int nDevId) = 0;
	virtual bool		video_close() = 0;
	
	virtual bool		video_getImage(OUT void *pImageData) = 0;
	
protected:
	int					m_nError;	
};

#endif