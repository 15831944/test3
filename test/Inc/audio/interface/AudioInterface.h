#ifndef __AUDIO_INTERFACE_H__
#define __AUDIO_INTERFACE_H__

#include "../../data/DeviceInfo.h"
class CAudioInterface
{
public:
	virtual	int		audio_geterror() = 0;

	virtual bool	audio_init() = 0;
	virtual void	audio_uninit() = 0;

	virtual bool	audio_enumDevice(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecDevInfo) = 0;
	virtual bool	audio_openDevice(CDeviceInfo &devInfo, CDeviceHandle &devHandle)  = 0;
	virtual void	audio_closeDevice(CDeviceHandle &devHandle) = 0;

	virtual void	audio_startStream(CDeviceHandle &devHandle) = 0;
	virtual void	audio_closeStream(CDeviceHandle &devHandle) = 0;
	virtual void	audio_stopStream(CDeviceHandle &devHandle)  = 0;
	virtual void	audio_abortStream(CDeviceHandle &devHandle) = 0;
	
protected:
	int	m_iError;
};

#endif