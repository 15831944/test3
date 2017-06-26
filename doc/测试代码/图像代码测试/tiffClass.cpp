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
        //����ͼƬ�ļ����ݵ���Ϣ�ṹ
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)pData;
        imageSource.size    = nDataLen;
        imageSource.offset  = 0;
		
        //ʹ��libtiff��һ��tif�ļ������ö�����в����ĸ���Ϊ�Ļص�����������ɹ����ļ�����һ��TIFF�ṹָ�롣
        TIFF* tif = TIFFClientOpen("file.tif", "r", (thandle_t)&imageSource, 
            _tiffReadProc, _tiffWriteProc,
            _tiffSeekProc, _tiffCloseProc, _tiffSizeProc,
            _tiffMapProc,
            _tiffUnmapProc);
			
        //��Ч���жϡ�
//        CC_BREAK_IF(NULL == tif);
        
        uint32 w = 0, h = 0;
        uint16 bitsPerSample = 0, samplePerPixel = 0, planarConfig = 0;
       //�������nPixels�洢ͼ����������������
        size_t npixels = 0;
        //��ȡ��Ӧ��ͼƬ������Ϣ��
        //ͼƬ��ȡ�
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        //ͼƬ�߶ȡ�
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        //ͼƬɫ�
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
        //ÿ��������ռ���ֽ���
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplePerPixel);
        //ͼ���ƽ������
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarConfig);
        //ȡ����������
        npixels = w * h;
        //���ô�ALPHAͨ����
        m_bHasAlpha = true;
        m_nWidth = w;
        m_nHeight = h;
        m_nBitsPerComponent = 8;
       //������Ӧ���ڴ������洢�������ݡ�
        m_pData = new unsigned char[npixels * sizeof (uint32)];
       //������ʱ�ڴ����TIFF���ݶ�ȡ��
        uint32* raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL) 
        {
          //��ȡTIFF����
           if (TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
           {
               //�����Ǵ�TIFF�����н�����������Cocos2d-x���õ�ͼ�����ݡ�
              //���ﶨ��ָ�����ָ��TIFF����
                unsigned char* src = (unsigned char*)raster;
              //���ﶨ��ָ�����ָ������Cocos2d-x���õ�ͼ������
                unsigned int* tmp = (unsigned int*)m_pData;

    /*
                //����ÿ���ؽ��У������ص�RGBAֵ������ϣ�����ϳɵ�DWORDֵд�뵽ͼ�������С�
                for(int j = 0; j < m_nWidth * m_nHeight * 4; j += 4)
                {
                    *tmp++ = CC_RGB_PREMULTIPLY_APLHA( src[j], src[j + 1], 
                        src[j + 2], src[j + 3] );
                }
    */
    //ALPHAͨ����Ч��
                m_bPreMulti = true;
              //����ѭ������Ϻ��DWORDֵд��ͼ������̫��������ֱ�ӽ����ڴ濽�����Դﵽͬ��Ŀ�ġ�
               memcpy(m_pData, raster, npixels*sizeof (uint32));
           }
          //�ͷ���ʱ������ڴ档
          _TIFFfree(raster);
        }
        

        //�ر�TIFF�ļ���ȡ��
        TIFFClose(tif);

        bRet = true;
    } while (0);
	
	return bRet;
}


tmsize_t _tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
{
	//��fdת��ΪͼƬ�ļ����ݵ���Ϣ�ṹָ�롣
	tImageSource* isource = (tImageSource*)fd;
	uint8* ma;
	uint64 mb;
	//����һ�ο��Զ�ȡ�����ݳ��ȡ�
	unsigned long n;
	//�������oͳ��ÿ��ѭ����ȡ�����ݳ��ȡ�
	unsigned long o;
	//�������ͳ�ƶ�ȡ������ݳ��ȡ�
	tmsize_t p;
	//��ǰ�涨���uint8����ָ�����maָ��buf�������ں�����ͼ�����ݡ�
	ma=(uint8*)buf;
	//��ǰ�涨��ı���mb��ͳ��ʣ��δ��ȡ�����ݳ��ȡ�
	mb=size;
	p=0;
	//ʹ��whileѭ�����ж�ȡ���ж�����Ϊʣ��δ�������ݳ����Ƿ����0��
	while (mb>0)
	{
		n=0x80000000UL;
		if ((uint64)n>mb)
			n=(unsigned long)mb;

		//�����δ�����������ݣ��������ȡ�����������0
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
		//��ȡ�곤��Ϊo�����ݣ����ָ�������Ӧ��ƫ�Ʋ������´ν��ж�ȡ������
		ma+=o;
		//����δ��ȡ��ʣ�೤��
		mb-=o;
		//���¶�ȡ�����������
		p+=o;
		//�������if�Ƚ���֣���Ϊ�ǲ�����Ϊtrue�ġ�����һ��if�ж����Ѿ�������o=n��
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
	//��fdת��ΪͼƬ�ļ����ݵ���Ϣ�ṹָ�롣
    tImageSource* isource = (tImageSource*)fd;
    uint64 ret = -1;
    do 
    {
           //�����λ��ʽΪ��ͷ��ʼ����
        if (whence == SEEK_SET)
        {
//            CC_BREAK_IF(off > isource->size-1);
            ret = isource->offset = (uint32)off;
        }
        else if (whence == SEEK_CUR)
        {  //�����λ��ʽΪ�ӵ�ǰλ�ÿ�ʼ����
//            CC_BREAK_IF(isource->offset + off > isource->size-1);
            ret = isource->offset += (uint32)off;
        }
        else if (whence == SEEK_END)
        {   //�����λ����ҵ���ļ�β����ʼ����
//            CC_BREAK_IF(off > isource->size-1);
            ret = isource->offset = (uint32)(isource->size-1 - off);
        }
        else
        {//������ʽҲ���մ�ͷ��ʼ����
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