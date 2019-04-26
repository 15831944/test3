#ifndef __TIFFCLASS_H__
#define __TIFFCLASS_H__

#define TIFF_MACROS			0

#if (TIFF_MACROS == 1)
#include <libtiff/tiffio.h>
#pragma comment(lib, "libtiff.lib")

#ifndef   UNUSED_PARAM 
#define   UNUSED_PARAM(v)   (void)(v) 
#endif 

//定义宏从RGB888或RGB5A1像素格式数据中返回一个RGBA8888的像素格式数据。
#define CC_RGB_PREMULTIPLY_APLHA(vr, vg, vb, va) \
	(unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
	((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
	((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
	((unsigned)(unsigned char)(va) << 24))

//图片文件数据的信息结构
typedef struct 
{
	unsigned char* data;
	int size;
	int offset;
}tImageSource;

//使用LibTiff读取TIFF格式的图片数据;
bool _initWithTiffData(void* pData, int nDataLen);

//读取TIFF图片数据时的回调函数;
//参1:文件数据内存;
//参2:输出参数，读取到的图像数据复制到对应的内存地址中;
//参3:图片数据长度;
static tmsize_t _tiffReadProc(thandle_t fd, void* buf, tmsize_t size);

//将数据保存为tiff图像文件所调用的回调函数;
static tmsize_t _tiffWriteProc(thandle_t fd, void* buf, tmsize_t size);

//在对TIFF图像文件进行解析时进行重定位时调用的回调函数;
static uint64 _tiffSeekProc(thandle_t fd, uint64 off, int whence);

//取得tiff图片文件大小的回调函数;
static uint64 _tiffSizeProc(thandle_t fd);

//关闭tiff图片文件读取的回调函数;
static int _tiffCloseProc(thandle_t fd);

//将tiff图片文件映射到内存时调用的回调函数;
static int _tiffMapProc(thandle_t fd, void** pbase, toff_t* psize);

//解除tiff图片映射到内存的回调函数;
static void _tiffUnmapProc(thandle_t fd, void* base, toff_t size);

#endif

#endif