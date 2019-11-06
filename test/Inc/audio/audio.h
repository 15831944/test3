#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "../data/DevDataMgr.h"
#include "../data/DataFrameMgr.h"

enum AudioApi {
	UNSPECIFIED,
	WINDOWS_WAVE,
	WINDOWS_ASIO,
	WINDOWS_DS,
	WINDOWS_WASAPI,
	LINUX_PULSE,
	LINUX_ALSA,
	LINUX_OSS,
};

enum AudioMsgId {
	MSG_PTR_EMPTY_ID,
	MSG_DEV_DATA_ID,
	MSG_FRAME_DATA_ID,
	MSG_API_HANDLE_ID,
};

enum AudioSampleFormat {
	AUDIO_SAMPLE_U8  = 0x1,
	AUDIO_SAMPLE_S16 = 0x2,
	AUDIO_SAMPLE_S32 = 0x4,
	AUDIO_SAMPLE_FLT32 = 0x10,
	AUDIO_SAMPLE_DBL64 = 0x20,
};

struct AudioPcmFormat {
	unsigned short sChannels;			//�������ݵ�������.(1,2...)
	unsigned short sBitsPerSample;		//������С.(16,...)	
	unsigned int   nSamplesPerSec;		//����Ƶ��.(44100,...)
	unsigned int   nAvgBytesPerSec;		//ָ�����ݴ���Ĵ�������.(16000,...)
	unsigned short sBlockAlign;			//ָ������룬�������������С��λ.(2,...)
	unsigned int   nBufferSize;			//
};

class IAudioProcApi
{
public:
	virtual AudioApi getCurrentApi() = 0;
	
	virtual bool	initApi(AudioApi emAudioApi) = 0;
	virtual void	unInitApi() = 0;

	virtual bool	getEnumDevice(DevMode devMode, std::vector<CDevDataMgr> &vecDevData) = 0;
	
	virtual bool	isStreamOpen()  = 0;
	virtual bool	openStream(AudioPcmFormat stAudioFormat, CDevDataMgr devData) = 0;
	virtual void	closeStream() = 0;

	virtual bool	startStream() = 0;
	virtual void	stopStream()  = 0;
	virtual void	abortStream() = 0;
};


#endif