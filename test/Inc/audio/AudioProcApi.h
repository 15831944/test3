#ifndef __AUDIOPROC_API_H__
#define __AUDIOPROC_API_H__

enum AudioApi {
	UNSPECIFIED,
	WINDOWS_WAVE,
	WINDOWS_WASAPI,
	WINDOWS_ASIO,
	WINDOWS_DS,
	LINUX_PULSE,
	LINUX_ALSA,
	LINUX_OSS,
};

class IAudioProcApi
{
public:
	virtual void	initApi(AudioApi emAudioApi) = 0
	virtual void	unInitApi() = 0;
	
	virtual bool	isStreamOpen()  = 0;
	
	virtual void	getCurrentApi() = 0;
	virtual void	getEnumDevice() = 0;
	
	virtual void	openStream() = 0;
	virtual void	closeStream() = 0;
	
	virtual void	startStream() = 0;
	virtual void	stopStream()  = 0;
	virtual void	abortStream() = 0;
};

class CAudioProc : public IAudioProcApi
{
public:
	CAudioProc(AudioApi emAudioApi);
	~CAudioProc();
	
public:
	virtual void	initApi(AudioApi emAudioApi);
	virtual void	unInitApi();
	
	virtual bool	isStreamOpen();
	
	virtual void	getCurrentApi();
	virtual void	getEnumDevice();
	
	virtual void	openStream();
	virtual void 	closeStream();
	
	virtual void 	startStream();
	virtual void 	stopStream();
	virtual void 	abortStream();
	
protected:
	IAudioInterface	*m_audioInterface;
};

#endif