#include "stdafx.h"
#include "log4cpp.h"

log4cpp::log4cpp()
{
	m_nLogLevel = 0;
}

log4cpp::~log4cpp()
{
	Releaselog4cplus();
}

BOOL log4cpp::Initlog4cplus()
{
	Logger::getRoot().removeAllAppenders();


#ifdef _WIN32 
	TCHAR szFileName[MAX_PATH+1] ={0};
	TCHAR szFilePath[MAX_PATH+1] ={0};
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	
	char *ptr1 = strrchr(szFilePath, '\\');
	if (ptr1)
	{
		strcpy(szFileName, ++ptr1);
		*ptr1 = '\0';

 		char *ptr2 = strrchr(szFileName, '.');
		if (ptr2)
		{
			ptr2++;
			*ptr2 = '\0';
			strcat(szFileName, _T("log"));
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

#endif
	SharedAppenderPtr 		pFileAppender(new FileAppender(szFileName));
	std::auto_ptr<Layout>	pPatternLayout(new PatternLayout(PATTERN));
	pFileAppender->setLayout(pPatternLayout);
	
	m_pLogger = Logger::getRoot();
	m_pLogger.addAppender(pFileAppender);
	return TRUE;
}

void log4cpp::Releaselog4cplus()
{
	Logger::shutdown();
}

void log4cpp::SetLogLevel(int nLogLevel)
{
	m_nLogLevel = nLogLevel;
	switch(m_nLogLevel)
	{
	case LOG_LEVEL_NOLEVEL:
		m_pLogger.setLogLevel(NOT_SET_LOG_LEVEL);
		break;
	case LOG_LEVEL_ALLLEVEL:
		m_pLogger.setLogLevel(ALL_LOG_LEVEL);
		break;
	case LOG_LEVEL_TRACE:
		m_pLogger.setLogLevel(TRACE_LOG_LEVEL);
		break;
	case LOG_LEVEL_DEBUG:
		m_pLogger.setLogLevel(DEBUG_LOG_LEVEL);
		break;
	case LOG_LEVEL_INFO:
		m_pLogger.setLogLevel(INFO_LOG_LEVEL);
		break;
	case LOG_LEVEL_WARNING:
		m_pLogger.setLogLevel(WARN_LOG_LEVEL);
		break;
	case LOG_LEVEL_ERROR:
		m_pLogger.setLogLevel(ERROR_LOG_LEVEL);
		break;
	}
}

void log4cpp::WriteLog(int nLogLevel, const char* szLogInfo)
{
	switch(nLogLevel)
	{
	case LOG_LEVEL_TRACE:
		LOG4CPLUS_TRACE(m_pLogger, szLogInfo);
		break;
	case LOG_LEVEL_DEBUG:
		LOG4CPLUS_DEBUG(m_pLogger, szLogInfo);
		break;
	case LOG_LEVEL_INFO:
		LOG4CPLUS_INFO(m_pLogger, szLogInfo);
		break;
	case LOG_LEVEL_WARNING:
		LOG4CPLUS_WARN(m_pLogger, szLogInfo);
		break;
	case LOG_LEVEL_ERROR:
		LOG4CPLUS_ERROR(m_pLogger, szLogInfo);
		break;
	}
}