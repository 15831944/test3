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

enum AudioFormat {
	AUDIO_SAMPLE_U8  = 0x1,
	AUDIO_SAMPLE_S16 = 0x2,
	AUDIO_SAMPLE_S32 = 0x4,
	AUDIO_SAMPLE_FLT32 = 0x10,
	AUDIO_SAMPLE_DBL64 = 0x20,
};

struct AudioPcmFormat {
	WORD wChannels;				//�������ݵ�������.(1,2...)
	WORD wBitsPerSample;		//������С.(16,...)	
	DWORD dwSamplesPerSec;		//����Ƶ��.(44100,...)
	DWORD dwAvgBytesPerSec;		//ָ�����ݴ���Ĵ�������.(16000,...)
	WORD wBlockAlign;			//ָ������룬�������������С��λ.(2,...)
};

class IAudioProcApi
{
public:
	virtual void	initApi(AudioApi emAudioApi) = 0
	virtual void	unInitApi() = 0;

	virtual bool	isStreamOpen()  = 0;

	virtual void	getCurrentApi() = 0;
	virtual void	getEnumDevice(DevMode devMode, std::vector<CDevData> &vecDevData) = 0;

	virtual void	openStream(CDevData devInfo, ) = 0;
	virtual void	closeStream() = 0;

	virtual void	startStream() = 0;
	virtual void	stopStream()  = 0;
	virtual void	abortStream() = 0;
};


#endif