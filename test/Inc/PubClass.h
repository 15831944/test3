#ifndef __PUBCLASS_H__
#define __PUBCLASS_H__

#include <io.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strsafe.h>
#include <atlimage.h>
#include <direct.h>								//_getcwd,_getdrives
#include <string.h>								//兼容C的标准库头文件
#include <sys/types.h>							//
#include <sys/stat.h>							//_stat	

// #include <regex>
// #include <devguid.h>
// #include <Dbt.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <list>
#include <queue> 
#include <vector>

#include "./common/MemDCEx.h"
//#include <opencv2/opencv.hpp>
#include "./common/Character.h"
//////////////////////////////////////////////////////////////////////////
//
#define GETSYSTEM_FONT						0x1000

#define WM_TEST1WND_CTRL					WM_USER + 0x1000
#define WM_TEST2WND_CTRL					WM_USER + 0x1001
#define WM_TEST3WND_CTRL					WM_USER + 0x1002
#define WM_TEST4WND_CTRL					WM_USER + 0x1003

//////////////////////////////////////////////////////////////////////////
//
#define countof(arr)						(sizeof(arr)/sizeof(arr[0]))
#define SIZEOF_ARRAY(array)					(sizeof(array) / sizeof(array[0]))

#define DIV_ROUND_UP(x, d)					(((x) + (d) - 1) / (d))
#define ROUND_UP(x, d)						(DIV_ROUND_UP(x, d) * (d))

#ifndef NOMINMAX
#ifndef max
#define max(a,b)							(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)							(((a) < (b)) ? (a) : (b))
#endif
#endif

//////////////////////////////////////////////////////////////////////////
//
typedef struct{
	BOOL			bFlag;
	HWND			hWnd;
	DWORD			dwParam;
	char			szValue[MAX_PATH];
	char			szAddInfo[MAX_PATH];
	LOGFONT			hLogFont;
}HT_TESTWND_MSG;

typedef struct{
	HWND			hWnd;
	DWORD			dwProcessID;
}HT_ENUM_WNDINFO;

typedef struct{
	USHORT			usTransId;
	USHORT			usFlag;
	USHORT			usQuestionCount;
	USHORT			usAnswerCount;
	USHORT			usAuthorityCount;
	USHORT			usAdditionalCount;
}HT_DNS_HEADER;

//////////////////////////////////////////////////////////////////////////
//
BOOL CALLBACK	EnumWindowsProc(HWND hwnd, LPARAM lParam);
int  CALLBACK	EnumFontProc(ENUMLOGFONTEX *lpelf,NEWTEXTMETRICEX *lpntm,DWORD nFontType,long lParam);

#endif