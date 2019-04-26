#ifndef __ZLIBCLASS_H__
#define __ZLIBCLASS_H__

#define ZLIB_MACROS			0
#if (ZLIB_MACROS == 1)

#define ZLIB_WINAPI
#include <zlib/zlib.h>
#pragma comment(lib, "zlib128d.lib")

int					Compress(const char * DestName,	 const char *SrcName);
int					UnCompress(const char * DestName,const char *SrcName);
#endif

#endif