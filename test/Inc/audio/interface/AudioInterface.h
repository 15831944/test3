#ifndef __AUDIO_INTERFACE_H__
#define __AUDIO_INTERFACE_H__

#include "../audio.h"

class IAudioNotifyHandler;
class IAudioInterface
{
public:
	virtual	int		audio_geterror() = 0;

	virtual bool	audio_init() = 0;
	virtual void	audio_uninit() = 0;

	virtual bool	audio_enumDevice(DevMode devMode, std::vector<CDevDataMgr> &vecDevData) = 0;
	virtual bool	audio_openDevice(AudioPcmFormat stAudioFormat, CDevDataMgr devData) = 0;
	virtual void	audio_closeDevice() = 0;

	virtual void	audio_startStream() = 0;
	virtual void	audio_closeStream() = 0;
	virtual void	audio_stopStream()  = 0;
	virtual void	audio_abortStream() = 0;

	virtual	void	audio_addBuffer(CDataFrameMgr &dataBuff) = 0;
	virtual	void	audio_openBuffer(CDataFrameMgr &dataBuff)  = 0;
	virtual void	audio_closeBuffer(CDataFrameMgr &dataBuff) = 0;

	virtual void	audio_setNotifyHandler(IAudioNotifyHandler *pNotify) = 0;
};

class IAudioNotifyHandler
{
public:
	virtual void	NotifyEvent(IAudioInterface *pAudioInterface, int nEventMsg, CDataFrameMgr *pDataBuff) = 0;
};

#endif