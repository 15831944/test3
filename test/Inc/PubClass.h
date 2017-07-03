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


//#include <opencv2/opencv.hpp>

#include "Base64.h"
#include "CBase64.h"
#include "Character.h"

#define	DEVICE_NAME_LEN_MAX		 256

#define countof(arr)			(sizeof(arr)/sizeof(arr[0]))
#define SIZEOF_ARRAY(array)		(sizeof(array) / sizeof(array[0]))

#define DIV_ROUND_UP(x, d)		(((x) + (d) - 1) / (d))
#define ROUND_UP(x, d)			(DIV_ROUND_UP(x, d) * (d))

#ifndef NOMINMAX
#ifndef max
#define max(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)				(((a) < (b)) ? (a) : (b))
#endif
#endif

#endif