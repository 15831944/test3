#ifndef __AUDIO_H__
#define __AUDIO_H__

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

struct WavFormat {
	WORD wFormatTag;
	WORD wChannels;
	DWORD dwSamplesPerSec;
	DWORD dwAvgBytesPerSec;
	WORD wBlockAlign;
	WORD wBitsPerSample;
	WORD wSize;
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


#endif