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

#include <string>
#include <iostream>
#include <fstream>

#include <cctype>
#include <locale>
#include <algorithm>
#include <functional> 

#include <vector>
#include <queue> 
#include <list>
#include <map>

#include <regex>
#include <array>
//#include <devguid.h>
//#include <Dbt.h>

#include <winsvc.h>
#include <atlconv.h>
#include <gdiplus.h>
#include <tlhelp32.h>

#include "./internal.h"
#include "./GlobalInfo.h"

#pragma comment(lib, "gdiplus.lib")
BOOL CALLBACK	EnumWindowsProc(HWND hwnd, LPARAM lParam);
#endif