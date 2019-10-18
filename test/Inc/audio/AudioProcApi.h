#ifndef __AUDIOPROC_API_H__
#define __AUDIOPROC_API_H__

class IAudioProcApi
{
public:
	virtual void	getCurrentApi() = 0;
	virtual void	getEnumDevice() = 0;
	
	virtual bool	isStreamOpen()  = 0;
	
	virtual void	openStream() = 0;
	virtual void	closeStream() = 0;
	
	virtual void	startStream() = 0;
	virtual void	stopStream()  = 0;
	virtual void	abortStream() = 0;
};

class CAudioProc : public IAudioProcApi
{
public:
	CAudioProc();
	~CAudioProc();
	
public:
	virtual void	getCurrentApi();
	virtual void	getEnumDevice();
	
	virtual bool	isStreamOpen();
	
	virtual void	openStream();
	virtual void 	closeStream();
	
	virtual void 	startStream();
	virtual void 	stopStream();
	virtual void 	abortStream();
	
protected:
	IAudioInterface	*m_audioInterface;
};

#endif