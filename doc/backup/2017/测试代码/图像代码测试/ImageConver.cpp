#include "stdafx.h"
#include "ImageConver.h"

CImageConvert::CImageConvert()
{
}


CImageConvert::~CImageConvert()
{}


CBitmap* CImageConvert::IplImage2CBitmap(const IplImage *pImage)
{
	if( pImage && pImage->depth == IPL_DEPTH_8U )
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;

		int bmp_w = pImage->width, bmp_h = pImage->height;

		FillBitmapInfo( bmi, bmp_w, bmp_h, pImage->depth*pImage->nChannels, pImage->origin );

		char *pBits=NULL;
		HBITMAP hBitmap=CreateDIBSection(::GetDC(NULL),bmi,DIB_RGB_COLORS,(void**)&pBits,NULL,0);
		memcpy(pBits,pImage->imageData,pImage->imageSize);

		CBitmap *pBitmap=new CBitmap;
		pBitmap->Attach(hBitmap);

		return pBitmap;
	}
	else
	{
		return NULL;
	}
}


void CImageConvert::FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}


IplImage* CImageConvert::CBitmap2IplImage(const CBitmap *pBitmap)
{
	DIBSECTION ds;
	pBitmap->GetObject(sizeof(ds),&ds);

	int nChannels = ds.dsBmih.biBitCount/8;
	int depth = IPL_DEPTH_8U;
	
	IplImage *pImage=cvCreateImage(cvSize(ds.dsBm.bmWidth,ds.dsBm.bmHeight), depth, nChannels);
	memcpy(pImage->imageData,ds.dsBm.bmBits,pImage->imageSize);

	return pImage;
}


HBITMAP CImageConvert::IplImage2hBitmap(IplImage* pImg)
{
	BYTE tmp[sizeof(BITMAPINFO)+1024];
	BITMAPINFO *bmi = (BITMAPINFO*)tmp;
	HBITMAP hBmp;
	int i;

	memset(bmi,0,sizeof(BITMAPINFO));

	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth = pImg->width;
	bmi->bmiHeader.biHeight = pImg->height; 
	bmi->bmiHeader.biPlanes = 1;
	bmi->bmiHeader.biBitCount = pImg->nChannels * pImg->depth;
	bmi->bmiHeader.biCompression = BI_RGB;
	bmi->bmiHeader.biSizeImage = 0;//if biCompression is BI_RGB,this can be 0
	bmi->bmiHeader.biClrImportant =0 ;

	switch(pImg->nChannels * pImg->depth) 
	{ 
	case 8 : 
		for(i=0 ; i < 256 ; i++){ 
			bmi->bmiColors[i].rgbBlue = i;
			bmi->bmiColors[i].rgbGreen= i;
			bmi->bmiColors[i].rgbRed= i;
		}
		break;
	case 32:
	case 24: 
		((DWORD*) bmi->bmiColors)[0] = 0x00FF0000;
		((DWORD*) bmi->bmiColors)[1] = 0x0000FF00;
		((DWORD*) bmi->bmiColors)[2] = 0x000000FF;
		break; 
	} 
	hBmp = ::CreateDIBSection(NULL,bmi,DIB_RGB_COLORS,NULL,0,0);

	SetDIBits(NULL,hBmp,0,pImg->height,pImg->imageData,bmi,DIB_RGB_COLORS);

	return hBmp;
}


IplImage* CImageConvert::hBitmap2Ipl(HBITMAP hBmp)
{
	BITMAP bmp; 
	::GetObject(hBmp,sizeof(BITMAP),&bmp);//hBmp-->bmp

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U; 
	int nSize = bmp.bmHeight * bmp.bmWidth * nChannels;

	IplImage* pImage = cvCreateImage(cvSize(bmp.bmWidth, bmp.bmHeight), depth, nChannels);
	GetBitmapBits(hBmp, bmp.bmWidth*bmp.bmHeight*nChannels, (void*)pImage->imageData);

	return pImage;
}



/*
CBitmap CImageConvert::HBITMAP2CBitmap(HBITMAP  hbitmap)
{
	CBitmap   cbitmap;
	cbitmap.Attach(hbitmap);
	return cbitmap;
}


HBITMAP CImageConvert::CBitmap2HBITMAP(CBitmap bitmap)
{
	HBITMAP bmp = HBITMAP(bitmap);
//	bmp=(HBITMAP)bitmap.GetSafeHandle();
	return bmp;
}


CBitmap CImageConvert::BITMAP2CBitmap(BITMAP   bmp)
{
	CBitmap bitmap;
	bitmap.GetBitmap(&bmp);
	return bitmap;
}


BITMAP CImageConvert::HBITMAP2BITMAP(HBITMAP hBmp)
{
	BITMAP bmp; 
	::GetObject(hBmp,sizeof(BITMAP),&bmp);	
	return bmp;
}*/