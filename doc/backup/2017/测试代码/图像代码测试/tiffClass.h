#ifndef __TIFFCLASS_H__
#define __TIFFCLASS_H__

#define TIFF_MACROS			0

#if (TIFF_MACROS == 1)
#include <libtiff/tiffio.h>
#pragma comment(lib, "libtiff.lib")

#ifndef   UNUSED_PARAM 
#define   UNUSED_PARAM(v)   (void)(v) 
#endif 

//������RGB888��RGB5A1���ظ�ʽ�����з���һ��RGBA8888�����ظ�ʽ���ݡ�
#define CC_RGB_PREMULTIPLY_APLHA(vr, vg, vb, va) \
	(unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
	((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
	((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
	((unsigned)(unsigned char)(va) << 24))

//ͼƬ�ļ����ݵ���Ϣ�ṹ
typedef struct 
{
	unsigned char* data;
	int size;
	int offset;
}tImageSource;

//ʹ��LibTiff��ȡTIFF��ʽ��ͼƬ����;
bool _initWithTiffData(void* pData, int nDataLen);

//��ȡTIFFͼƬ����ʱ�Ļص�����;
//��1:�ļ������ڴ�;
//��2:�����������ȡ����ͼ�����ݸ��Ƶ���Ӧ���ڴ��ַ��;
//��3:ͼƬ���ݳ���;
static tmsize_t _tiffReadProc(thandle_t fd, void* buf, tmsize_t size);

//�����ݱ���Ϊtiffͼ���ļ������õĻص�����;
static tmsize_t _tiffWriteProc(thandle_t fd, void* buf, tmsize_t size);

//�ڶ�TIFFͼ���ļ����н���ʱ�����ض�λʱ���õĻص�����;
static uint64 _tiffSeekProc(thandle_t fd, uint64 off, int whence);

//ȡ��tiffͼƬ�ļ���С�Ļص�����;
static uint64 _tiffSizeProc(thandle_t fd);

//�ر�tiffͼƬ�ļ���ȡ�Ļص�����;
static int _tiffCloseProc(thandle_t fd);

//��tiffͼƬ�ļ�ӳ�䵽�ڴ�ʱ���õĻص�����;
static int _tiffMapProc(thandle_t fd, void** pbase, toff_t* psize);

//���tiffͼƬӳ�䵽�ڴ�Ļص�����;
static void _tiffUnmapProc(thandle_t fd, void* base, toff_t size);

#endif

#endif