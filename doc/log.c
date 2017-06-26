#include "log.h"

#include <Windows.h>

static LOG g_loging;
static LOGCONFIG g_logConfig;

static unsigned int	g_logLevel;
static bool g_bAppend = false;;
static bool g_bIssync = false;

static bool log_init(unsigned int logLevel, const char* pszLogFilePath, bool bAppend=true, bool bIssync=false)
{
	FILE* pFile = NULL;
	
	if(pszLogFilePath == NULL)
	{
		return false;
	}
	
	g_logLevel = logLevel;
	g_bAppend  = bAppend;
	g_bIssync  = bIssync;
	
	pFile = fopen(pszLogFilePath, bAppend ? "a" : "w");
	if(pFile == NULL)
	{
		return false;
	}
	
	setvbuf(pFile, NULL, _IOLBF, 0);
	return true;
}

static bool log_check_level(unsigned int logLevel)
{
	
}

bool LogInit(unsigned int logLevel, const char* pszModuleName, const char* pszLogPath)
{
	char szLogFilePath[] = {0};
	
	if(access(pszLogPath, 0) == -1)
	{
		if(mkdir(pszLogPath, S_IREAD|S_IWRITE) < 0)
		{
			return false;
		}
	}
	
	sprintf(szLogFilePath, "%s\\%s.log", pszLogPath, pszModuleName);
	log_init(logLevel, szLogFilePath);
	
	return true;
}


















static LOGCONFIG* GetLogConfig()
{
	g_logConfig.logLevel = LOGLEVEL_INFO;
	g_logConfig.uLogMaxLen = LOGFILE_MAX_SIZE;
	
	return &g_logConfig;
}

static bool splitpath(char* pszAppPath, char* pszAppName, char* pszAppTitle)
{
	int nPos = 0;
	int nLen = 0;

	char* p = NULL;

	char szAppPath[LOGPATH_MAX_LEN]  = {0};
	char szAppName[LOGPATH_MAX_LEN]  = {0};
	char szAppTitle[LOGPATH_MAX_LEN] = {0};

	char szModuleFileName[LOGPATH_MAX_LEN] = {0};

	GetModuleFileName(NULL, szModuleFileName, sizeof(szModuleFileName)/sizeof(char));

	//
	p = strrchr(szModuleFileName, '\\');
	if (p == NULL)
	{
		return false;
	}

	nLen = strlen(szModuleFileName);
	if (nLen <= 0)
	{
		return false;
	}

	nPos = p - szModuleFileName;
	if (nPos <= 0)
	{
		return false;
	}

	memcpy(szAppName, p+1, nLen-nPos-1);
	memcpy(szAppPath, szModuleFileName, nPos);

	//
	p = strstr(szAppName, ".");
	if (p == NULL)
	{
		return false;
	}

	nLen = strlen(szAppName);
	if (nLen <= 0)
	{
		return false;
	}

	nPos = p - szAppName;
	if(nPos <= 0)
	{
		return false;
	}

	memcpy(szAppTitle, szAppName, nPos);

	strcpy(pszAppPath,  szAppPath);
	strcpy(pszAppName,  szAppName);
	strcpy(pszAppTitle, szAppTitle);

	return true;
}

static bool GetDate(int nFlag, char* date)
{
	time_t timed = 0;
	struct tm *p = NULL;
	
	timed = time(NULL);
	p = localtime(&timed);
	if(p == NULL)
	{
		return false;
	}
	
	if(nFlag == 0)
	{
		sprintf(date, "%04d%02d%02d%02d%02d%02d", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	}
	else if(nFlag == 1)
	{
		sprintf(date, "%04d-%02d-%02d %02d:%02d:%02d", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	}
	else
	{
		return false;
	}
	
	return true;
}

static bool PrintfLog(char* format, va_list args)
{
	int  d = 0;
	char c, *s;
/*
	while(*format)
	{
		switch(*format)
		{
		case 's':
			s = va_arg(args, char*);
			fprintf(g_loging.logFile, "%s", s);
			break;

		case 'd':
			d = va_arg(args, int);
			fprintf(g_loging.logFile, "%d", d);
			break;

		case 'c':
			c = (char)va_arg(args, int);
			fprintf(g_loging.logFile, "%c", c);
			break;

		default:
			if (*format != '%' && *format != '\n')
			{
				fprintf(g_loging.logFile, "%c", *format);
			}
			break;
		}

		format++;
	}
*/


	fprintf(g_loging.logFile, "%s", "]\n");
	return true;
}

static bool InitLog(unsigned char logLevel)
{
	char date[64] = {0};
	
	char szLogPath[LOGPATH_MAX_LEN+1]  = {0};
	char szLogName[LOGPATH_MAX_LEN+1]  = {0};
	char szLogTitle[LOGPATH_MAX_LEN+1] = {0};
	
	LOGCONFIG* logConfig = NULL;
	
	logConfig = GetLogConfig();
	if(logConfig == NULL)
	{
		return false;
	}
	
	if((logLevel&(logConfig->logLevel)) != logLevel)
	{
		return false;
	}
	
	memset(&g_loging, 0x0, sizeof(LOG));

	if(!GetDate(1, g_loging.logTime))
	{
		return false;
	}
	
	if(strlen(logConfig->logPath) == 0)
	{
		if(!splitpath(szLogPath, szLogName, szLogTitle))
		{
			return false;
		}
		
		if(!GetDate(0, date))
		{
			return false;
		}
		
		sprintf(logConfig->logPath, "%s\\%s_%s.log", szLogPath, szLogTitle, date);
	}
	
	memcpy(g_loging.logPath, logConfig->logPath, strlen(logConfig->logPath));

	if(g_loging.logFile == NULL)
	{
		g_loging.logFile = fopen(g_loging.logPath, "at+");
	}
	
	if(g_loging.logFile == NULL)
	{
		return false;
	}
	
	fprintf(g_loging.logFile, "[%s]:[", g_loging.logTime);
	
	return true;
}

/*
int	WriteLog(unsigned char logLevel, char* format, ...)
{
 	va_list args;
	
	if(!InitLog(logLevel))
	{
		return -1;
	}
	
	va_start(args,format);
	PrintfLog(format, args);
	va_end(args);
	
	fflush(g_loging.logFile);
	
	if(g_loging.logFile != NULL)
	{
		fclose(g_loging.logFile);
		g_loging.logFile = NULL;
	}
	
	return 0;
}
*/

int WriteLog(const char* pszLogInfo)
{
	char date[LOGTIME_MAX_SIZE+1] = {0};
	char logTime[LOGTIME_MAX_SIZE+1]  = {0};

	char szLogPath[LOGPATH_MAX_LEN+1]  = {0};
	char szLogName[LOGPATH_MAX_LEN+1]  = {0};
	char szLogTitle[LOGPATH_MAX_LEN+1] = {0};
	
	char szLogFile[LOGPATH_MAX_LEN+1] = {0};
	char szLogInfo[LOGINFO_MAX_LEN+1] = {0};

	FILE* logFile = NULL;

	if (pszLogInfo == NULL)
	{
		return -1;
	}

	if(!GetDate(1, logTime))
	{
		return -1;
	}

	if(!splitpath(szLogPath, szLogName, szLogTitle))
	{
		return -1;
	}

	if(!GetDate(0, date))
	{
		return -1;
	}

	sprintf(szLogFile, "%s\\%s_%s.log", szLogPath, szLogTitle, date);

	logFile = fopen(szLogFile, "at+");
	if (logFile == NULL)
	{
		return -1;
	}
	else
	{
		sprintf(szLogInfo, "[%s] - %s\n", logTime, pszLogInfo);

		fputs(szLogInfo, logFile);
		fflush(logFile);
	}

	fclose(logFile);
	logFile = NULL;
	return 0;
}