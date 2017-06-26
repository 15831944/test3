#include <atlstr.h>
#include<iostream>
#include "cv.h"
#include "highgui.h"
#include <vector>

using namespace std;

namespace barcode
{
	bool m_isCut  = 0;
	bool m_isLBDown = 0;
	bool hDataOld = NULL;
	bool hDataNew = NULL; //初值为NULL
	bool m_isBitmap = 0;
	//成员变量
	int ImageArray[1000][1000];
//	BYTE ** ImageArray;				//原始的图像数据数组－灰度
//	RGBQUAD** m_tOriPixelArray;		//原始位图数据

	long ImageHeight;				//原图像的高度
	long ImageWidth;				//原图像的宽度

	int arPixelH[1000] = {0,0};				//为了水平方向直方图统计用
	int arPixelV[1000] = {0,0};				//为了垂直方向直方图统计用
	int arWidth[200] = {0,0};				//保存宽度序列：从最左边的黑条开始
	int arDifference[1000] = {0,0};			//差分
	bool arMark[1000] = {0,0};				//标记
	int ImageLeft;					//图像最左边的座标
	int ImageRight;					//图像最右边的座标
	int ImageTop = 0;					//条形码顶部的座标
	int ImageBottom = 0;				//条形码下部的座标
	int arDelta[1000] = {0,0};
	int arLeftEdge1[1000] = {0,0};
	int arLeftEdge2[1000] = {0,0};
	int arLeftEdge[1000] = {0,0};
	CString strCodeStyle;
	CString strCodeNumber;
}

