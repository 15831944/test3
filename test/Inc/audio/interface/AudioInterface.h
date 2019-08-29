#ifndef __AUDIO_INTERFACE_H__
#define __AUDIO_INTERFACE_H__

class CAudioInterface
{
public:
	virtual	int		audio_geterror() = 0;
	
	virtual bool	audio_enumDevice() = 0;
	virtual bool	audio_getDeviceInfo() = 0;
	
	virtual bool	audio_openDevice() = 0;
	
protected:
	int	m_iError;
};

#endif