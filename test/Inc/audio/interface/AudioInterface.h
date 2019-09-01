#ifndef __AUDIO_INTERFACE_H__
#define __AUDIO_INTERFACE_H__

class CAudioInterface
{
public:
	virtual	int		audio_geterror() = 0;

	virtual bool	audio_init() = 0;
	virtual void	audio_uninit() = 0;

	virtual bool	audio_enumDevice(std::vector<CDeviceData> &vecAudioDev) = 0;
	virtual bool	audio_openDevice()  = 0;
	virtual void	audio_closeDevice() = 0;

	virtual void	audio_startStream() = 0;
	virtual void	audio_closeStream() = 0;
	virtual void	audio_stopStream()  = 0;
	virtual void	audio_abortStream() = 0;
	
protected:
	int	m_iError;
};

#endif