#ifndef __AUDIO_INTERFACE_H__
#define __AUDIO_INTERFACE_H__

#include "../../data/DeviceInfo.h"
#include "../../data/DataBuffer.h"

class IAudioNotifyHandler;
class IAudioInterface
{
public:
	virtual	int		audio_geterror() = 0;

	virtual bool	audio_init() = 0;
	virtual void	audio_uninit() = 0;

	virtual bool	audio_enumDevice(DevMode devMode, std::vector<CDevData> &vecDevInfo) = 0;
	virtual bool	audio_openDevice(CDevData devInfo) = 0;
	virtual void	audio_closeDevice() = 0;

	virtual void	audio_startStream() = 0;
	virtual void	audio_closeStream() = 0;
	virtual void	audio_stopStream()  = 0;
	virtual void	audio_abortStream() = 0;

	virtual	void	audio_addBuffer(IDataBuffer &dataBuff) = 0;
	virtual	void	audio_openBuffer(IDataBuffer &dataBuff)  = 0;
	virtual void	audio_closeBuffer(IDataBuffer &dataBuff) = 0;

	virtual void	audio_setNotifyHandler(IAudioNotifyHandler *pNotify) = 0;

protected:
	int	m_nError;
	CDevHandle*	m_devHandle;
};

class IAudioNotifyHandler
{
public:
	virtual void	NotifyEvent(IAudioInterface *pAudioInterface, int nEventMsg, IDataBuffer *pDataBuff) = 0;
};

#endif