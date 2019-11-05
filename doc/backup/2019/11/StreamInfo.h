#ifndef __STREAM_INFO_H__
#define __STREAM_INFO_H__

enum StreamState {
	STREAM_EMPTYSTATE = 0,
	STREAM_RUNNING  = 1,
	STREAM_CLOSED   = 2,
	STREAM_STOPPED  = 3,
	STREAM_STOPPING = 4,
};

enum StreamMode {
	STREAM_EMPTYMODE = 0,
	STREAM_INPUTMODE = 1,
	STREAM_OUTPUTMODE = 2,
	STREAM_DUPLEXMODE = 3,
};

struct {
};

class CStreamInfo
{
public:
	CStreamInfo();
	~CStreamInfo();
	
public:
	void			SetDeviceId(std::string strDeviceId);
	std::string		GetDeviceId();
	
	void			SetStreamState(StreamState emStreamState);
	StreamState		GetStreamState();
	
	void			SetStreamMode(StreamMode emStreamMode);
	StreamMode		GetStreamMode();
	
	void			SetApiHandle(void *pApiHandle);
	void*			GetApiHandle();
	
	  
};

#endif