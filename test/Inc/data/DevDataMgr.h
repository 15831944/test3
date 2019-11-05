#ifndef __DEVDATA_MGR_H__
#define __DEVDATA_MGR_H__

#include "./ObjectPtr.h"

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

class CDevDataMgr : public CObjectPtr
{
public:
	CDevDataMgr();
	~CDevDataMgr();

	CDevDataMgr(const CDevDataMgr& devData);
	CDevDataMgr& operator=(const CDevDataMgr& devData);

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
	DevMode		m_devMode;
	DevType		m_devType;
	DevState	m_devState;
	
	std::string	m_strDevId;
	std::string m_strDevName;
};

#endif