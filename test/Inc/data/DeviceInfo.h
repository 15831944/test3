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

enum DeviceMode {
	DEVICE_EMPTYMODE = 0,
	DEVICE_RENDERMODE = 1,		//播放
	DEVICE_CAPTUREMODE = 2,		//采集
};

enum DeviceState {
	DEVICE_EMPTYSTATE = 0,
	DEVICE_ACTIVESTATE = 1,		//激活
	DEVICE_DISABLEDSTATE = 2,	//禁用
	DEVICE_NOTPRESENTSTATE = 3,	//不存在
	DEVICE_UNPLUGGEDSTATE = 4,	//未上电
};

class IDeviceInfo
{
public:
	IDeviceInfo();
	~IDeviceInfo();

	IDeviceInfo(const IDeviceInfo& devInfo);
	IDeviceInfo& operator=(const IDeviceInfo& devInfo);

public:
	void		SetDeviceType(DeviceType devType);
	DeviceType	GetDeviceType();

	void		SetDeviceMode(DeviceMode devMode);
	DeviceMode	GetDeviceMode();

	void		SetDeviceState(DeviceState devState);
	DeviceState	GetDeviceState();

	void		SetDeviceId(std::string strDevId);
	std::string	GetDeviceId();

	void		SetDeviceName(std::string strDevName);
	std::string GetDeviceName();

	IAudioData&	GetDeviceAudio();

protected:
	IAudioData	m_audioData;

private:
	DeviceType	m_devType;
	DeviceState	m_devState;
	DeviceMode  m_devMode;

	std::string	m_strDevId;
	std::string m_strDevName;
};

class IDataFrame
{
public:
	IDataFrame();
	~IDataFrame();
};

class IDeviceHandle
{
public:
	IDeviceHandle();	
	~IDeviceHandle();

public:	
	void			SetDevInfo(IDeviceInfo devInfo);
	IDeviceInfo&	GetDevInfo();

	void			SetApiHandle(void *pApiHandle);
	void*			GetApiHandle();

	void			SetDataQueue(IDataFrame *pDataFrame);
	IDataFrame*		GetDataQueue();

private:
	void*			m_pApiHandle;

	IDeviceInfo		m_devInfo;
	CDataQueue<IDataFrame> m_dataQueue;
};

#endif