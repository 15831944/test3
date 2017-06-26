#ifndef __IMAGE_CONVER_H__
#define __IMAGE_CONVER_H__

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
class CImageConvert
{
public:
	CImageConvert();
	~CImageConvert();

public:
	static CBitmap*						IplImage2CBitmap(const IplImage *pImage);
	static IplImage*					CBitmap2IplImage(const CBitmap *pBitmap);

	static HBITMAP						IplImage2hBitmap(IplImage* pImg);
	static IplImage*					hBitmap2Ipl(HBITMAP hBmp);

	static CBitmap						HBITMAP2CBitmap(HBITMAP  hbitmap);
	static HBITMAP						CBitmap2HBITMAP(CBitmap  bitmap);

	static CBitmap						BITMAP2CBitmap(BITMAP    bmp);
	static BITMAP						HBITMAP2BITMAP(HBITMAP   hBmp);

protected:
	static void							FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin);
};

#endif