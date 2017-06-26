#ifndef __PNGGRAPH_H__
#define __PNGGRAPH_H__

#define	PNG_MACROS		0

#if (PNG_MACROS == 1)
	
#include <libpng/png.h>
#pragma comment(lib, "libpng1616-static-mtdll_x86.lib")

long ReadPngData( const char *szPath, int *pnWidth, int *pnHeight, unsigned char **cbData );

#endif
#endif