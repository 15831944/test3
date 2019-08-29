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
	
	virtual bool	audio_enumDevice();
	virtual bool	audio_getDeviceInfo();
	
	virtual bool	audio_openDevice();
	
private:
	bool	m_bInitialized;
	IMMDeviceEnumerator* m_pDevEnumerator;
};

#endif