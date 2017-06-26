#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define	LOGTIME_MAX_SIZE	64
#define LOGFILE_MAX_SIZE	2048*2

#define LOGINFO_MAX_LEN		2048
#define LOGPATH_MAX_LEN		512
#define LOGNAME_MAX_LEN		64

#define LOGLEVEL_NONE		0x00
#define LOGLEVEL_INFO		0x01
#define LOGLEVEL_DEBUG		0x02
#define LOGLEVEL_ERROR		0x04
#define LOGLEVEL_ALL		0xFF

typedef struct tagLog
{
	char			logTime[LOGTIME_MAX_SIZE+1];
	char			logPath[LOGPATH_MAX_LEN+1];
	char			logName[LOGNAME_MAX_LEN+1];
	FILE*			logFile;
}LOG;

typedef struct tagLogConfig
{
	char			logPath[LOGPATH_MAX_LEN+1];
	unsigned int	uLogMaxLen;
	unsigned char	logLevel;
}LOGCONFIG;
 
//int		WriteLog(unsigned char logLevel, char* format, ...);
int		WriteLog(const char* pszLogInfo);

#ifdef __cplusplus
}
#endif

#endif