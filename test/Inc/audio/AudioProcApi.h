#ifndef __AUDIOPROC_API_H__
#define __AUDIOPROC_API_H__

#include "./interface/AudioInterface.h"

class CAudioProc : public IAudioProcApi, public IAudioNotifyHandler
{
public:
	CAudioProc(AudioApi emAudioApi);
	~CAudioProc();
	
public:
	virtual AudioApi getCurrentApi();
	
	virtual bool	initApi(AudioApi emAudioApi);
	virtual void	unInitApi();
	
	virtual bool	getEnumDevice(DevMode devMode, std::vector<CDevDataMgr> &vecDevData);
	
	virtual bool	isStreamOpen();
	virtual bool	openStream(AudioPcmFormat stAudioFormat, CDevDataMgr devData);
	virtual void 	closeStream();
	
	virtual bool 	startStream();
	virtual void 	stopStream();
	virtual void 	abortStream();
	
protected:
	IAudioInterface	*m_audioInterface;
};

#endif