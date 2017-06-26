#include "stdafx.h"
#include "tiffClass.h"

#if (TIFF_MACROS == 1)

static bool m_bHasAlpha;
static bool m_bPreMulti;
static int  m_nWidth ;
static int  m_nHeight;
static int  m_nBitsPerComponent;
static unsigned char* m_pData;



bool _initWithTiffData(void* pData, int nDataLen)
{
	bool bRet = false;
	do 
    {
        //设置图片文件数据的信息结构
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)pData;
        imageSource.size    = nDataLen;
        imageSource.offset  = 0;
		
        //使用libtiff打开一个tif文件，设置对其进行操作的各行为的回调函数。如果成功打开文件返回一个TIFF结构指针。
        TIFF* tif = TIFFClientOpen("file.tif", "r", (thandle_t)&imageSource, 
            _tiffReadProc, _tiffWriteProc,
            _tiffSeekProc, _tiffCloseProc, _tiffSizeProc,
            _tiffMapProc,
            _tiffUnmapProc);
			
        //有效性判断。
//        CC_BREAK_IF(NULL == tif);
        
        uint32 w = 0, h = 0;
        uint16 bitsPerSample = 0, samplePerPixel = 0, planarConfig = 0;
       //定义变量nPixels存储图像数据像素数量。
        size_t npixels = 0;
        //读取相应的图片属性信息。
        //图片宽度。
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        //图片高度。
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        //图片色深。
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
        //每像素数据占的字节数
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplePerPixel);
        //图像的平面配置
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarConfig);
        //取得像素数量
        npixels = w * h;
        //设置带ALPHA通道。
        m_bHasAlpha = true;
        m_nWidth = w;
        m_nHeight = h;
        m_nBitsPerComponent = 8;
       //申请相应的内存用来存储像素数据。
        m_pData = new unsigned char[npixels * sizeof (uint32)];
       //申请临时内存进行TIFF数据读取。
        uint32* raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL) 
        {
          //读取TIFF数据
           if (TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
           {
               //下面是从TIFF数据中解析生成我们Cocos2d-x所用的图像数据。
              //这里定义指针变量指向TIFF数据
                unsigned char* src = (unsigned char*)raster;
              //这里定义指针变量指向我们Cocos2d-x所用的图像数据
                unsigned int* tmp = (unsigned int*)m_pData;

    /*
                //遍历每像素进行，对像素的RGBA值进行组合，将组合成的DWORD值写入到图像数据中。
                for(int j = 0; j < m_nWidth * m_nHeight * 4; j += 4)
                {
                    *tmp++ = CC_RGB_PREMULTIPLY_APLHA( src[j], src[j + 1], 
                        src[j + 2], src[j + 3] );
                }
    */
    //ALPHA通道有效。
                m_bPreMulti = true;
              //上面循环将组合后的DWORD值写入图像数据太慢，这里直接进行内存拷贝可以达到同样目的。
               memcpy(m_pData, raster, npixels*sizeof (uint32));
           }
          //释放临时申请的内存。
          _TIFFfree(raster);
        }
        

        //关闭TIFF文件读取。
        TIFFClose(tif);

        bRet = true;
    } while (0);
	
	return bRet;
}


tmsize_t _tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
{
	//将fd转化为图片文件数据的信息结构指针。
	tImageSource* isource = (tImageSource*)fd;
	uint8* ma;
	uint64 mb;
	//定义一次可以读取的数据长度。
	unsigned long n;
	//定义变量o统计每次循环读取的数据长度。
	unsigned long o;
	//定义变量统计读取完的数据长度。
	tmsize_t p;
	//让前面定义的uint8类型指针变量ma指向buf。用于在后面存放图像数据。
	ma=(uint8*)buf;
	//让前面定义的变量mb来统计剩余未读取的数据长度。
	mb=size;
	p=0;
	//使用while循环进行读取，判断条件为剩余未读的数据长度是否大于0。
	while (mb>0)
	{
		n=0x80000000UL;
		if ((uint64)n>mb)
			n=(unsigned long)mb;

		//如果尚未读完所有数据，则继续读取，否则出错返回0
		if((int)(isource->offset + n) <= isource->size)
		{
			memcpy(ma, isource->data+isource->offset, n);
			isource->offset += n;
			o = n;
		}
		else
		{
			return 0;
		}
		//读取完长度为o的数据，则对指针进行相应的偏移操作供下次进行读取操作。
		ma+=o;
		//更新未读取的剩余长度
		mb-=o;
		//更新读取完的数量长度
		p+=o;
		//下面这个if比较奇怪，因为是不可能为true的。在上一个if判断中已经设置了o=n。
		if (o!=n)
		{
			break;
		}
	}
	return p;
}

tmsize_t _tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
{
// 	CC_UNUSED_PARAM(fd);
//     CC_UNUSED_PARAM(buf);
//     CC_UNUSED_PARAM(size);
    return 0;
}

uint64 _tiffSeekProc(thandle_t fd, uint64 off, int whence)
{
	//将fd转化为图片文件数据的信息结构指针。
    tImageSource* isource = (tImageSource*)fd;
    uint64 ret = -1;
    do 
    {
           //如果定位方式为从头开始计算
        if (whence == SEEK_SET)
        {
//            CC_BREAK_IF(off > isource->size-1);
            ret = isource->offset = (uint32)off;
        }
        else if (whence == SEEK_CUR)
        {  //如果定位方式为从当前位置开始计算
//            CC_BREAK_IF(isource->offset + off > isource->size-1);
            ret = isource->offset += (uint32)off;
        }
        else if (whence == SEEK_END)
        {   //如果定位方工业从文件尾部开始计算
//            CC_BREAK_IF(off > isource->size-1);
            ret = isource->offset = (uint32)(isource->size-1 - off);
        }
        else
        {//其它方式也按照从头开始计算
//            CC_BREAK_IF(off > isource->size-1);
            ret = isource->offset = (uint32)off;
        }
    } while (0);

    return ret;
}

uint64 _tiffSizeProc(thandle_t fd)
{
	tImageSource* pImageSrc = (tImageSource*)fd;
    return pImageSrc->size;
}

int _tiffCloseProc(thandle_t fd)
{
//	CC_UNUSED_PARAM(fd);
    return 0;
}

int _tiffMapProc(thandle_t fd, void** pbase, toff_t* psize)
{
// 	CC_UNUSED_PARAM(fd);
//     CC_UNUSED_PARAM(pbase);
//     CC_UNUSED_PARAM(psize);
    return 0;
}

void _tiffUnmapProc(thandle_t fd, void* base, toff_t size)
{
// 	CC_UNUSED_PARAM(fd);
//     CC_UNUSED_PARAM(base);
//     CC_UNUSED_PARAM(size);
}
#endif