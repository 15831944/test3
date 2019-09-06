#ifndef __AUDIO_WASAPI_H__
#define __AUDIO_WASAPI_H__

#include <Mmdeviceapi.h>
#include "./AudioInterface.h"

class CAudioWasApi : public CAudioInterface
{
public:
	CAudioWasApi();
	~CAudioWasApi();
	
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
	static DWORD WINAPI audio_wasApiThread(void *pWasApiPtr);

protected:
	bool			audio_getDevInfo(EDataFlow emdataFlow, UINT uiDevState, std::vector<CDeviceData> &vecAudioDev);
	void			audio_wasApiProc();

protected:
	HANDLE			m_hHandle;
	
private:
	bool	m_bInitialized;
	IMMDeviceEnumerator* m_pDevEnumerator;
};

#endif