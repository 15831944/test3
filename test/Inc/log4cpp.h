#ifndef __LOG4CPP_H__
#define __LOG4CPP_H__

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>  

#define  LOG4CPLUS_STATIC
#ifdef _DEBUG
#pragma comment(lib,"log4cplus.lib")	//log4cplus
#else
#pragma comment(lib,"log4cplusS.lib")	//log4cplusS
#endif

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

typedef enum tagLevelType
{
	LOG_LEVEL_NOLEVEL = 0x00, 
	LOG_LEVEL_ALLLEVEL= 0x01,
	LOG_LEVEL_TRACE	  = 0x02,
	LOG_LEVEL_DEBUG   = 0x03,
	LOG_LEVEL_INFO    = 0x04,
	LOG_LEVEL_WARNING = 0x05,
	LOG_LEVEL_ERROR   = 0x06
}LEVELTYPE;

#define PATTERN			"[%d{%m/%d/%y %H:%M:%S}] - %m %n"
class log4cpp
{
public:
	log4cpp();
	~log4cpp();
	
public:
	BOOL		Initlog4cplus();
	void		Releaselog4cplus();
	
	void		SetLogLevel(int nLogLevel);
	void		WriteLog(int nLogLevel, const char* szLogInfo);
	
private:
	Logger 		m_pLogger;	
	int			m_nLogLevel;
	char		m_szLogName[260];
};

#endif