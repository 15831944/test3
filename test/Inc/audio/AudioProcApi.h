#ifndef __AUDIOPROC_API_H__
#define __AUDIOPROC_API_H__

#include "audio.h"
#include "./interface/AudioInterface.h"

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
	virtual void	getEnumDevice(DevMode devMode, std::vector<CDevData> &vecDevData);
	
	virtual void	openStream();
	virtual void 	closeStream();
	
	virtual void 	startStream();
	virtual void 	stopStream();
	virtual void 	abortStream();
	
protected:
	IAudioInterface	*m_audioInterface;
};

#endif