#ifndef __AUDIO_WAVEAPI_H__
#define __AUDIO_WAVEAPI_H__

#include "AudioInterface.h"

class CAudioWaveAPi : public CAudioInterface
{
public:
	CAudioWaveAPi();
	~CAudioWaveAPi();
	
public:
	virtual	int		audio_geterror();
	
	virtual bool	audio_init();
	virtual void	audio_uninit();

	virtual bool	audio_enumDevice(std::vector<CDeviceData> &vecAudioDev);
	virtual bool	audio_openDevice();
	virtual void	audio_closeDevice();

	virtual void	audio_startStream();
	virtual void	audio_closeStream();
	virtual void	audio_stopStream();
	virtual void	audio_abortStream();
	
protected:
	static DWORD WINAPI audio_waveApiThread(void *pWaveApiPtr);
	
protected:
	bool			audio_getDevInfo(std::vector<CDeviceData> &vecAudioDev);
	void			audio_waveApiProc();
	
protected:
	HANDLE			m_hHandle;
};

#endif