#ifndef __DEV_HANDLER_H__
#define __DEV_HANDLER_H__

#include "./DataQueue.h"
#include "../audio/audio.h"
#include "../video/video.h"

enum DevType {
	DEV_EMPTYTYPE = 0,
	DEV_AUDIOTYPE = 1,
	DEV_VIDEOTYPE = 2,
};

enum DevMode {
	DEV_EMPTYMODE = 0,
	DEV_RENDERMODE = 1,			//播放
	DEV_CAPTUREMODE = 2,		//采集
};

enum DevState {
	DEV_EMPTYSTATE = 0,
	DEV_ACTIVESTATE = 1,		//激活
	DEV_DISABLEDSTATE = 2,		//禁用
	DEV_NOTPRESENTSTATE = 3,	//不存在
	DEV_UNPLUGGEDSTATE = 4,		//未上电
};

class CDevData
{
public:
	CDevData();
	~CDevData();

	CDevData(const CDevData& devData);
	CDevData& operator=(const CDevData& devData);

public:
	void		SetDevType(DevType devType);
	DevType		GetDevType();

	void		SetDevMode(DevMode devMode);
	DevMode		GetDevMode();

	void		SetDevState(DevState devState);
	DevState	GetDevState();

	void		SetDevId(std::string strDevId);
	std::string	GetDevId();

	void		SetDevName(std::string strDevName);
	std::string GetDevName();

private:
	DevType		m_devType;
	DevState	m_devState;
	DevMode		m_devMode;

	std::string	m_strDevId;
	std::string m_strDevName;
};

class IDataFrame
{
public:
	IDataFrame();
	~IDataFrame();
};

class CDevHandler
{
public:
	CDevHandler();	
	virtual ~CDevHandler();
	
public:
	void			registerDevObj();

public:	
	void			SetDevData(CDevData devData);
	CDevData&		GetDevData();

	void			SetApiHandle(void *pApiHandle);
	void*			GetApiHandle();

	void			SetDataQueue(IDataFrame *pDataFrame);
	IDataFrame*		GetDataQueue();

private:
	void*			m_pApiHandle;

	CDevData		m_devData;
	CDataQueue<IDataFrame> m_dataQueue;
};

#endif