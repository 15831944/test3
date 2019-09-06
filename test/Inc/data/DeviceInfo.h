#ifndef __DEVICE_INFO_H__
#define __DEVICE_INFO_H__

#include "../audio/audio.h"
#include "../video/video.h"

enum DeviceType {
	DEVICE_EMPTYTYPE = 0,
	DEVICE_AUDIOTYPE = 1,
	DEVICE_VIDEOTYPE = 2,
};

enum DeviceState {
	DEVICE_EMPTYSTATE = 0,
};

enum DeviceMode {
	DEVICE_EMPTYMODE = 0,
	DEVICE_RENDERMODE = 1,
	DEVICE_CAPTUREMODE = 2,
};

struct DeviceHandle {
	DeviceType	emDeviceType;
	DeviceState	emDeviceState;
	DeviceMode  emDeviceMode;
	std::string	strDeviceId;
	std::string strDeviceName;
	
	union {
		AudioDev stAudioDev;
		VideoDev stVideoDev;
	}
	
	struct DeviceHandle() {
		emDeviceType  = DEVICE_EMPTYTYPE;
		emDeviceState = DEVICE_EMPTYSTATE;
		emDeviceMode  = DEVICE_EMPTYMODE;

		strDeviceId = "";
		strDeviceName = "";
	}
};

class CDeviceInfo
{
public:
	CDeviceInfo();	
	~CDeviceInfo();

	CDeviceInfo(const CDeviceInfo& devInfo);
	CDeviceInfo& operator=(const CDeviceInfo& devInfo);

public:
	void			SetDevType(DeviceType emDevType);
	DeviceType		GetDevType();

	void			SetDevState(DeviceState emDevState);
	DeviceState		GetDevState();

	void			SetDevMode(DeviceMode emDevMode);
	DeviceMode		GetDevMode();

	void			SetDevId(std::string strDevId);
	std::string		GetDevId();

	void			SetDevName(std::string strDevName);
	std::string		GetDevName();
	
	void			SetDevData(DeviceType emDevType, void *pDevData);
	void*			GetDevData(DeviceType emDevType);

private:
	DeviceHandle	m_stAudioDev;
};

#endif