#ifndef __DEVICE_INFO_H__
#define __DEVICE_INFO_H__

#include "./DataQueue.h"
#include "../audio/audio.h"
#include "../video/video.h"

enum DeviceType {
	DEVICE_EMPTYTYPE = 0,
	DEVICE_AUDIOTYPE = 1,
	DEVICE_VIDEOTYPE = 2,
};

enum DeviceState {
	DEVICE_EMPTYSTATE = 0,
	DEVICE_ACTIVESTATE = 1,		//激活
	DEVICE_DISABLEDSTATE = 2,	//禁用
	DEVICE_NOTPRESENTSTATE = 3,	//不存在
	DEVICE_UNPLUGGEDSTATE = 4,	//未上电
};

enum DeviceMode {
	DEVICE_EMPTYMODE = 0,
	DEVICE_RENDERMODE = 1,		//播放
	DEVICE_CAPTUREMODE = 2,		//采集
};

class CDeviceInfo
{
public:
	CDeviceInfo();
	~CDeviceInfo();

	CDeviceInfo(const CDeviceInfo& devInfo);
	CDeviceInfo& operator=(const CDeviceInfo& devInfo);

public:
	void		SetDeviceType(DeviceType emDeviceType);
	DeviceType	GetDeviceType();

	void		SetDeviceState(DeviceState emDeviceState);
	DeviceState	GetDeviceState();

	void		SetDeviceMode(DeviceMode emDeviceMode);
	DeviceMode	GetDeviceMode();

	void		SetDeviceId(std::string strDeviceId);
	std::string	GetDeviceId();

	void		SetDeviceName(std::string strDeviceName);
	std::string GetDeviceName();

	CAudioData*	GetDeviceAudio();

protected:
	CAudioData	m_audioData;

private:
	DeviceType	m_emDeviceType;
	DeviceState	m_emDeviceState;
	DeviceMode  m_emDeviceMode;

	std::string	m_strDeviceId;
	std::string m_strDeviceName;
};

class CDataFrame
{
public:
	CDataFrame();
	~CDataFrame();
};

class CDeviceHandle
{
public:
	CDeviceHandle();	
	~CDeviceHandle();

public:	
	void			SetDevInfo(CDeviceInfo *pDevInfo);
	CDeviceInfo*	GetDevInfo();

	void			SetApiHandle(void *pApiHandle);
	void*			GetApiHandle();

	void			SetDataQueue(CDataFrame *pDataFrame);
	CDataFrame*		GetDataQueue();

private:
	void*			m_pApiHandle;

	CDeviceInfo		m_deviceInfo;
	CDataQueue<CDataFrame> m_dataQueue;
};

#endif