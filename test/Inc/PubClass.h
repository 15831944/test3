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

//#include <regex>
//#include <devguid.h>
//#include <Dbt.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <list>
#include <queue> 
#include <vector>

#include <atlconv.h>
//#include "afxshelltreeCtrl.h"
//#include <opencv2/opencv.hpp>

#include <detours.h>
#include <winsvc.h>

#include "./common/MemDCEx.h"
#include "./common/Character.h"

#include "./internal.h"
#include "../Inc/GlobalInfo.h"
#include "../Inc/audio/waveplayer.h"
#include "../Inc/video/opengl_wnd_draw_video.h"
#include "../Inc/video/gdi_wnd_draw_video.h"

#include "../Contrib/VideoWndThread.h"
#include "../Contrib/CheckLinkThread.h"
#include "../Contrib/update_file_name.h"


BOOL CALLBACK	EnumWindowsProc(HWND hwnd, LPARAM lParam);
int  CALLBACK	EnumFontProc(ENUMLOGFONTEX *lpelf,NEWTEXTMETRICEX *lpntm,DWORD nFontType,long lParam);
#endif