

//////////////////////////////////////////////////////////////////////////
//
// 获取直方图  
// 1. pImageData   图像数据  
// 2. nWidth       图像宽度  
// 3. nHeight      图像高度  
// 4. nWidthStep   图像行大小  
// 5. pHistogram   灰度直方图，大小256  
inline bool GetHistogram(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep, int *pHistogram)  
{  
	int i = 0;  
	int j = 0;  
	unsigned char *pLine = NULL;  
	// 清空直方图  
	memset(pHistogram, 0, sizeof(int) * 256);  
	for (pLine = pImageData, j = 0; j < nHeight; j++, pLine += nWidthStep)  
	{  
		for (i = 0; i < nWidth; i++)  
		{  
			pHistogram[pLine[i]]++;  
		}  
	}  
	return true;  
}  

// 大津法  
// 1. pImageData   图像数据  
// 2. nWidth       图像宽度  
// 3. nHeight      图像高度  
// 4. nWidthStep   图像行大小  
// 函数返回阈值  
inline int Otsu1(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep)  
{  
	int i = 0;  
	int j = 0;  
	int nTotal = 0;  
	int nSum = 0;  
	int A = 0;  
	int B = 0;  
	double u = 0;  
	double v = 0;  
	double dVariance = 0;  
	double dMaximum = 0;  
	int nThreshold = 0;  
	int nHistogram[256];  
	// 获取直方图  
	GetHistogram(pImageData, nWidth, nHeight, nWidthStep, nHistogram);  
	for (i = 0; i < 256; i++)  
	{  
		nTotal += nHistogram[i];  
		nSum   += (nHistogram[i] * i);  
	}  
	for (j = 0; j < 256; j++)  
	{  
		A = 0;  
		B = 0;  
		for (i = 0; i < j; i++)  
		{  
			A += nHistogram[i];  
			B += (nHistogram[i] * i);  
		}  
		if (A > 0)  
		{  
			u = B / A;  
		}  
		else  
		{  
			u = 0;  
		}  
		if (nTotal - A > 0)   
		{  
			v = (nSum - B) / (nTotal - A);  
		}  
		else  
		{  
			v = 0;  
		}  
		dVariance = A * (nTotal - A) * (u - v) * (u - v);  
		if (dVariance > dMaximum)  
		{  
			dMaximum = dVariance;  
			nThreshold = j;  
		}  
	}  
	return nThreshold;  
}  

inline int Otsu2(const IplImage *pImage)
{
	assert(NULL != pImage);

	int width  = pImage->width;
	int height = pImage->height;

	int x = 0, y = 0;
	int i = 0, j = 0;
	int threshold = 0;
	int pixelSum = width * height;

	int pixelCount[256] = {0};
	float pixelPro[256] = {0};

	uchar *data = (uchar*)pImage->imageData;

	//////////////////////////////////////////////////////////////////////////
	//
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;
		}
	}

	for (i=0; i<256; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / (float)pixelSum;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	float w0 =0, w1 = 0, u0tmp = 0, u1tmp = 0, u0 = 0, u1 = 0, u = 0,deltaTmp = 0, deltaMax = 0; 
	for (i=0; i<256; i++)
	{
		for (j=0; j<256; j++)
		{
			if(j <= i) //背景部分  
			{  
				//以i为阈值分类，第一类总的概率  
				w0 += pixelPro[j];        
				u0tmp += j * pixelPro[j];  
			}  
			else       //前景部分  
			{  
				//以i为阈值分类，第二类总的概率  
				w1 += pixelPro[j];        
				u1tmp += j * pixelPro[j];  
			}  
		}

		u0 = u0tmp / w0;        //第一类的平均灰度  
		u1 = u1tmp / w1;        //第二类的平均灰度  
		u = u0tmp + u1tmp;      //整幅图像的平均灰度  
		//计算类间方差  
		deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);  
		//找出最大类间方差以及对应的阈值  
		if(deltaTmp > deltaMax)  
		{     
			deltaMax = deltaTmp;  
			threshold = i;  
		}  
	}

	 return threshold;
}