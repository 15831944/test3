#ifndef __AUDIO_INTERFACE_H__
#define __AUDIO_INTERFACE_H__

#include "../../data/DeviceInfo.h"
#include "../../data/DataBuffer.h"

class IAudioNotifyHandler;
class CAudioInterface
{
public:
	virtual	int		audio_geterror() = 0;

	virtual bool	audio_init() = 0;
	virtual void	audio_uninit() = 0;

	virtual bool	audio_enumDevice(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecDevInfo) = 0;
	virtual bool	audio_openDevice(CDeviceInfo *pDevInfo) = 0;
	virtual void	audio_closeDevice() = 0;

	virtual void	audio_startStream() = 0;
	virtual void	audio_closeStream() = 0;
	virtual void	audio_stopStream()  = 0;
	virtual void	audio_abortStream() = 0;

	virtual	void	audio_addBuffer(CDataBuffer &dataBuff) = 0;
	virtual	void	audio_openBuffer(CDataBuffer &dataBuff)  = 0;
	virtual void	audio_closeBuffer(CDataBuffer &dataBuff) = 0;

	virtual void	audio_setNotifyHandler(IAudioNotifyHandler *pNotify) = 0;

protected:
	int	m_nError;
	CDeviceHandle*	m_pDevHandle;
};

class IAudioNotifyHandler
{
public:
	virtual void	NotifyEvent(CAudioInterface *pAudioInterface, int nEventMsg, CDataBuffer *pDataBuff) = 0;
};

#endif