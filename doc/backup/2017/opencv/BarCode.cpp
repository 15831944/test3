#include <afx.h> 
#include<iostream>
#include "BarCodeRec.h"
#include "cv.h"
#include "highgui.h"
#include "string.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>




using namespace cv; 
using namespace std;
using namespace barcode;

char* Barcode(char* ImageName);//总的封装接口
double otsu(const IplImage *src_image); //大津法求阈值
double Kittler( IplImage *imge );
void OnPicSb(IplImage *imge);
BOOL PreProcess(IplImage *imge);
BOOL Recognize();
int JudgNum(int w1, int w2, int w3, int w4, double mx);

/////////////////////////////////////////////////////////////////////////////
char* Barcode(char* ImageName)
{
	// TODO: Add your command handler code here
	IplImage* pImg; //声明IplImage指针

	// Matlab风格的cvLoadImage 函数的另一种调用
	IplImage *img = cvLoadImage(ImageName,1);
	if(!img)//载入失败
	{
		printf("Can not load image %s /n",ImageName);
		return NULL;
	}
	if(!img->imageData)//载入的图像数据是否正确
		return NULL;

	/*IplImage *gray = NULL;
	IplImage *binary = NULL;
	gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);//转化成灰度图
	cvCvtColor(img, gray, CV_BGR2GRAY);
	binary = cvCreateImage(cvGetSize(gray), IPL_DEPTH_8U, 1);//创建二值图
	*/

	cvNamedWindow("image",CV_WINDOW_AUTOSIZE );//创建窗口，窗口名字为image
	cvShowImage("image",img);//在刚创建的image窗口中载入图像
	//创建一个与img相同大小的图像img1
	IplImage *img1 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	//色彩空间转换，将源彩色图像img转化成目标灰色图像imag1
	cvCvtColor(img,img1,CV_BGR2GRAY); //灰度化

	//二值化
	IplImage *img_thre = cvCreateImage(cvGetSize(img1),IPL_DEPTH_8U,1);
//	IplImage *img3 = cvCreateImage(cvGetSize(img1),IPL_DEPTH_8U,1);
	/*void cvThreshold( const CvArr* src, CvArr* dst, double threshold,
	       double max_value, int threshold_type );
	  void cvAdaptiveThreshold( dst, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C,
	       CV_THRESH_BINARY, 9, -10);*/
	cvThreshold( img1, img_thre, otsu(img1),255, CV_THRESH_BINARY);
//	cvThreshold( img1, img_thre, Kittler(img1),255, CV_THRESH_BINARY);

	IplImage* out_smooth = cvCreateImage(cvGetSize(img_thre),  
		IPL_DEPTH_8U,img_thre->nChannels);   

    cvSmooth(img_thre,out_smooth,CV_MEDIAN,3,img_thre->nChannels);
//识别
//	OnPicSb(out_smooth);
	PreProcess(out_smooth);
	Recognize();

	cvNamedWindow("ThreImage",CV_WINDOW_AUTOSIZE);//创建窗口，窗口名字GrayImage
	cvShowImage("ThreImage",img_thre);//载入转化后的图像

	cvNamedWindow("SmoothImage",CV_WINDOW_AUTOSIZE);//创建窗口，窗口名字GrayImage
	cvShowImage("SmoothImage",out_smooth);//载入转化后的图像

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&img1);
	cvReleaseImage(&img_thre);
	cvReleaseImage(&out_smooth);
	cvDestroyAllWindows( );
	return 0;


}

double otsu(const IplImage *src_image) //大津法求阈值
{
	double sum = 0.0;
	double w0 = 0.0;
	double w1 = 0.0;
	double u0_temp = 0.0;
	double u1_temp = 0.0;
	double u0 = 0.0;
	double u1 = 0.0;
	double delta_temp = 0.0;
	double delta_max = 0.0;

	//src_image灰度级
	int pixel_count[256]={0};
	float pixel_pro[256]={0};
	double threshold = 0.0;
	uchar* data = (uchar*)src_image->imageData;
	//统计每个灰度级中像素的个数
	for(int i = 0; i < src_image->height; i++)
	{
		for(int j = 0;j < src_image->width;j++)
		{
			pixel_count[(int)data[i * src_image->width + j]]++;
			sum += (int)data[i * src_image->width + j];
		}
	}
	cout<<"平均灰度："<<sum / ( src_image->height * src_image->width )<<endl;
	//计算每个灰度级的像素数目占整幅图像的比例
	for(int i = 0; i < 256; i++)
	{
		pixel_pro[i] = (float)pixel_count[i] / ( src_image->height * src_image->width );
	}
	//遍历灰度级[0,255],寻找合适的threshold
	for(int i = 0; i < 256; i++)
	{
		w0 = w1 = u0_temp = u1_temp = u0 = u1 = delta_temp = 0;
		for(int j = 0; j < 256; j++)
		{
			if(j <= i)   //背景部分
			{
				w0 += pixel_pro[j];
				u0_temp += j * pixel_pro[j];
			}
			else   //前景部分
			{
				w1 += pixel_pro[j];
				u1_temp += j * pixel_pro[j];
			}
		}
		u0 = u0_temp / w0;
		u1 = u1_temp / w1;
		delta_temp = (float)(w0 *w1* pow((u0 - u1), 2)) ;
		if(delta_temp > delta_max)
		{
			delta_max = delta_temp;
			threshold = i;
		}
	}
	cout<<"ostu算法所得到的阈值为："<<threshold<<endl;
	return threshold;
}

//kittler算法
double Kittler( IplImage *imge )  
{  
	int sumGrads = 0;  
	int sumGrayGrads = 0;  
	char *pData = imge->imageData;  
	int height = imge ->height;  
	int width = imge->width;  
	int widthStep = imge->widthStep;  

	for ( int i = 1; i < height - 1; i++ )  
		for( int j = 1; j < width - 1; j++ )  
		{  
			int curPos = i * widthStep + j;  
			int leftPos = curPos - 1;   
			int rightPos = curPos + 1;  
			int upPos = curPos -widthStep;  
			int downPos =curPos + widthStep;  
			//求水平或垂直方向的最大梯度  
			int Grads=MAX(abs(pData[leftPos]-pData[rightPos]),abs(pData[upPos] -pData[downPos]));  
			sumGrads += Grads;  
			//梯度与当前点灰度的积  
			sumGrayGrads += Grads*((uchar)pData[curPos]);  

		}  
		int threshold = sumGrayGrads / sumGrads;  
		printf("Kittler算法计算出来的阈值结果为：%d\n",threshold);  
        return threshold;
}  

/******************************************************************************
* 功能：对条形码图像进行预处理，得出宽度序列
******************************************************************************/
BOOL PreProcess(IplImage *imge)
{
	//freopen("a.txt","w",stdout);
	int i, j;
	int tempMax;
	int tempArray[1000];
	ImageHeight = imge->height;
	ImageWidth = imge->width;
	for (i=0;i<imge->height;i++)
	{
		for (j=0;j<imge->width;j++)
		{
			//printf("%d ", mat.data[i][j]);
			//char *imageData;    /* 指向排列的图像数据 */
			ImageArray[i][j] = imge->imageData[i * imge->widthStep + j];
			if(ImageArray[i][j] != 0)
				ImageArray[i][j] = 1;
			ImageArray[i][j] = !ImageArray[i][j];
			//if(ImageArray[i][j])
				//printf("%2d ",ImageArray[i][j]);
		}//printf("\n");
	}
	//进行水平方向和垂直方向上的直方图统计，两个方向上的像素个数统计
	
	//进行水平方向和垂直方向上的直方图统计
	for(i=0; i<ImageHeight; i++)
		arPixelV[i] = 0;
	for(i=0; i<ImageWidth; i++)
		arPixelH[i] = 0;
	for(i=0; i<ImageHeight; i++)
	{
		for(j=0; j<ImageWidth; j++)
		{
			if(ImageArray[i][j] == 1)
			{
				arPixelV[i] += 1;
				arPixelH[j] += 1;
			}
		}
	}

	
	//寻找包含条形码的区域，
	//线寻找水平方向上黑象素最大的行

	tempMax = 0;
	for(i=0; i<ImageHeight; i++)
	{
		if(arPixelV[i]>tempMax)
			tempMax = arPixelV[i];
		arMark[i] = false;
	}

	for(i=0; i<ImageHeight-1; i++)
	{
		//计算差分
		arDifference[i] = arPixelV[i+1] - arPixelV[i];

		//如果该行像素足够多且变化不大，标记为true
		if( (abs(arDifference[i])<20) && (arPixelV[i]>(0.75*tempMax)) )
			arMark[i] = true;
	}
	
	//确定包含条码的行
	int iLengthThrehold = 40;
	int iCount;
	for(i=0; i<ImageHeight-iLengthThrehold; i++)
	{
		iCount = 0;
		for(j=0; j<iLengthThrehold; j++)
		{
			if(arMark[i+j] == true)
				iCount++;
		}
		if(iCount >= 37)
		{
			ImageTop = i+10;		//确定顶部
			break;
		}
	}

	for(i=ImageHeight-1; i>=iLengthThrehold-1; i--)
	{
		iCount = 0;
		for(j=0; j<iLengthThrehold; j++)
		{
			if(arMark[i-j] == true)
				iCount++;
		}
		if(iCount >= 37)	//iLengthThrehold-3
		{
			ImageBottom = i-10;		//确定底部
			break;
		}
	}

	//寻找左边缘,为了保证鲁棒性，在已经确定的上下边界内全局搜索
	for(i=ImageTop; i<=ImageBottom; i++)
	{
		for(j=20; j<ImageWidth; j++)
		{
			if( (ImageArray[i][j-1]==0) && (ImageArray[i][j]==1) )
			{
				arLeftEdge[i] = j;
				//printf("%d %d\n",i,j);
				break;
			}
		}
	}

	//为消除噪声的干扰，下面确定准确的左边界

	tempMax = 0;
	int iMax = 0;
	for(i=ImageTop; i<=ImageBottom; i++)
	{
		if(arLeftEdge[i] > tempMax)
		{
			tempMax = arLeftEdge[i];
			iMax = i;
		}
	}


	//倾斜度不能大于1/10
	iCount = 0;
	for(i=ImageTop; i<=ImageBottom; i++)
	{
		if( abs(tempMax-arLeftEdge[i]) < abs(i-iMax)/6+1 )
		{
			iCount++;
		}
	}

	if( (iCount/(ImageBottom-ImageTop))<0.6 )
		return false;

	//调整起点
	for(i=iMax; i>ImageTop; i--)
	{
		if( abs(arLeftEdge[i]-arLeftEdge[i-1])>=2 )
		{
			if(ImageArray[i-1][arLeftEdge[i]]-ImageArray[i-1][arLeftEdge[i]-1] == 1)
				arLeftEdge[i-1] = arLeftEdge[i];
			else if(ImageArray[i-1][arLeftEdge[i]-1]-ImageArray[i-1][arLeftEdge[i]-2] == 1)
				arLeftEdge[i-1] = arLeftEdge[i]-1;
			else if(ImageArray[i-1][arLeftEdge[i]+1]-ImageArray[i-1][arLeftEdge[i]] == 1)
				arLeftEdge[i-1] = arLeftEdge[i]+1;
			else
				arLeftEdge[i-1] = arLeftEdge[i];
		}
	}

	for(i=iMax; i<ImageBottom; i++)
	{
		if(i == ImageBottom)
			break;

		if( abs(arLeftEdge[i]-arLeftEdge[i+1])>=2 )
		{
			if(ImageArray[i+1][arLeftEdge[i]]-ImageArray[i+1][arLeftEdge[i]-1] == 1)
				arLeftEdge[i+1] = arLeftEdge[i];
			else if(ImageArray[i+1][arLeftEdge[i]-1]-ImageArray[i+1][arLeftEdge[i]-2] == 1)
				arLeftEdge[i+1] = arLeftEdge[i]-1;
			else if(ImageArray[i+1][arLeftEdge[i]+1]-ImageArray[i+1][arLeftEdge[i]] == 1)
				arLeftEdge[i+1] = arLeftEdge[i]+1;
			else
				arLeftEdge[i+1] = arLeftEdge[i];
		}
	}

	int n;
	//搜索出所有的宽度
	for(n=0; n<29; n++)
	{
		//搜索条的右边缘
		for(i=ImageTop; i<=ImageBottom; i++)
		{
			for(j = arLeftEdge[i]+1; j<ImageWidth; j++)
			{
				if( (ImageArray[i][j-1]==1) && (ImageArray[i][j]==0) )
				{
					arLeftEdge1[i] = j;
					break;
				}
			}
			arDelta[i] = arLeftEdge1[i] - arLeftEdge[i];
			//printf("%d %d %d\n", arLeftEdge1[i], arLeftEdge[i], arDelta[i]);
		}

		//假定条和空的宽度最多为11
		//排序，可以认为最中间的5个宽度是平均宽度
		for(i=ImageTop; i<ImageBottom; i++)
			tempArray[i] = arDelta[i];

		for(i=ImageTop; i<ImageBottom; i++)
		{
			for(j=ImageBottom; j>i; j--)
			{
				int tempSwap;
				if(tempArray[j] < tempArray[j-1])
				{
					tempSwap = tempArray[j];
					tempArray[j] = tempArray[j-1];
					tempArray[j-1] = tempSwap;
				}
			}
		}

		if(tempArray[ImageTop+(ImageBottom-ImageTop)/2+2]-tempArray[ImageTop+(ImageBottom-ImageTop)/2-2]>1)
			return false;
		else
			arWidth[2*n] = tempArray[ImageTop+(ImageBottom-ImageTop)/2];

		//调整下一列边缘
		for(i=ImageTop; i<=ImageBottom; i++)
		{
			if(abs(arDelta[i] - arWidth[2*n])>2)
				arLeftEdge1[i] = arLeftEdge[i] + arWidth[2*n];
			arLeftEdge[i] = arLeftEdge1[i];
		}

		//搜索空的右边缘
		for(i=ImageTop; i<=ImageBottom; i++)
		{
			for(j = arLeftEdge[i]+1; j<ImageWidth; j++)
			{
				if( (ImageArray[i][j-1]==0) && (ImageArray[i][j]==1) )
				{
					arLeftEdge1[i] = j;
					break;
				}
			}
			arDelta[i] = arLeftEdge1[i] - arLeftEdge[i];
		}

		//假定条和空的宽度最多为11
		//排序，可以认为最中间的5个宽度是平均宽度
		for(i=ImageTop; i<ImageBottom; i++)
			tempArray[i] = arDelta[i];

		for(i=ImageTop; i<ImageBottom; i++)
		{
			for(j=ImageBottom; j>i; j--)
			{
				int tempSwap;
				if(tempArray[j] < tempArray[j-1])
				{
					tempSwap = tempArray[j];
					tempArray[j] = tempArray[j-1];
					tempArray[j-1] = tempSwap;
				}
			}
		}

		if(tempArray[ImageTop+(ImageBottom-ImageTop)/2+2]-tempArray[ImageTop+(ImageBottom-ImageTop)/2-2]>1)
			return false;
		else
			arWidth[2*n+1] = tempArray[ImageTop+(ImageBottom-ImageTop)/2];

		//调整下一列边缘
		for(i=ImageTop; i<=ImageBottom; i++)
		{
			if(abs(arDelta[i] - arWidth[2*n+1])>2)
				arLeftEdge1[i] = arLeftEdge[i] + arWidth[2*n+1];
			arLeftEdge[i] = arLeftEdge1[i];
		}
	}

	//搜索最后一个条的右边缘
	for(i=ImageTop; i<=ImageBottom; i++)
	{
		for(j = arLeftEdge[i]+1; j<ImageWidth; j++)
		{
			if( (ImageArray[i][j-1]==1) && (ImageArray[i][j]==0) )
			{
				arLeftEdge1[i] = j;
				break;
			}
		}
		arDelta[i] = arLeftEdge1[i] - arLeftEdge[i];
	}

	//假定条和空的宽度最多为11
	//排序，可以认为最中间的5个宽度是平均宽度
	for(int i=ImageTop; i<ImageBottom; i++)
		tempArray[i] = arDelta[i];

	for(int i=ImageTop; i<ImageBottom; i++)
	{
		for(int j=ImageBottom; j>i; j--)
		{
			int tempSwap;
			if(tempArray[j] < tempArray[j-1])
			{
				tempSwap = tempArray[j];
				tempArray[j] = tempArray[j-1];
				tempArray[j-1] = tempSwap;
			}
		}
	}

	if(tempArray[ImageTop+(ImageBottom-ImageTop)/2+2]-tempArray[ImageTop+(ImageBottom-ImageTop)/2-2]>1)
		return false;
	else
		arWidth[2*n] = tempArray[ImageTop+(ImageBottom-ImageTop)/2];

	//调整下一列边缘
	for(int i=ImageTop; i<=ImageBottom; i++)
	{
		if(abs(arDelta[i] - arWidth[2*n+1])>2)
			arLeftEdge1[i] = arLeftEdge[i] + tempArray[ImageTop+(ImageBottom-ImageTop)/2];
		arLeftEdge[i] = arLeftEdge1[i];
	}

	return true;
}


/******************************************************************************
* 识别
******************************************************************************/
BOOL Recognize()
{
	//总共有7×12＋3×2＋5＝ 95个单位宽度
	//有4×12＋3×2＋5＝59个宽度，
	int i;
	int result[12];
	double mx = 0.0;        //平均宽度

	for(int i=0; i<59; i++)
	{
		mx += (double)arWidth[i];
		//printf("%d ",arWidth[i]);
	}
	mx /= 95.0;

	//起始条文
	for(i=0; i<3; i++)
	{
		double dTemp = (double)arWidth[i]/mx;
		if( dTemp<0.6 || dTemp>1.4 )
			break;
	}
	//起始码不符合要求
	//if(i<3)
	//        return false;

	//识别前6个
	for(i=0; i<6; i++)
	{
		result[i] = JudgNum(arWidth[i*4+3], arWidth[i*4+4], arWidth[i*4+5], arWidth[i*4+6], mx);
	}
	//识别后6个
	for(i=6; i<12; i++)
	{
		result[i] = JudgNum(arWidth[i*4+8], arWidth[i*4+9], arWidth[i*4+10], arWidth[i*4+11], mx);
	}

	//判断码制
	if( result[0]==7 && result[1]==7 )
	{
		strCodeStyle = "ISSN";
	}
	else if( result[0]==7 && result[1]==8 )
	{
		strCodeStyle = "ISBN";
	}
	else
		strCodeStyle = "Unknown!";

	//判断是否全部识别出来
	for(i=0; i<12; i++)
		if(result[i] == -1)
			return false;

	char out[125];
	for(i=0; i<12; i++)
	{
		if(result[i] >= 0 && result[i] <= 9)
			out[i] = result[i] + '0';
		else
			out[i] = result[i] + 'A';
	}
	out[i] = '\0';
	cout<<"strCodeNumber:"<<out<<endl;
	return true;
}
//CString hex = _T("FAB");
//CString decimal = _T("4011");ASSERT(_tcstoul(hex,0,16） == _ttoi(decimal));

/******************************************************************************
* 根据宽度数组判断数字
******************************************************************************/
int JudgNum(int w1, int w2, int w3, int w4, double mx)
{
	double a1, a2, a3;
	int ia1, ia2, ia3;
	a1 = (double)(w1+w2)/mx;
	a2 = (double)(w2+w3)/mx;
	a3 = (double)(w3+w4)/mx;
	ia1 = (int)(a1+0.5);
	ia2 = (int)(a2+0.5);
	ia3 = (int)(a3+0.5);

	//判断该码值
	if( (ia1==5 && ia2==3 && ia3==2) || (ia1==2 && ia2==3 && ia3==5) )
		return 0;

	if( (ia1==4 && ia2==4 && ia3==3) || (ia1==3 && ia2==4 && ia3==4) )
	{
		if(ia1 == 4)
		{
			double dw2 = (double)w2/mx;
			if(dw2 < 2.4)
				return 1;
			else if(dw2 > 2.6)
				return 7;
			else return -1;
		}

		if(ia1 == 3)
		{
			double dw3 = (double)w3/mx;
			if(dw3 < 2.4)
				return 1;
			else if(dw3 > 2.6)
				return 7;
			else return -1;
		}
	}

	if( (ia1==3 && ia2==3 && ia3==4) || (ia1==4 && ia2==3 && ia3==3) )
	{
		if(ia1 == 3)
		{
			double dw4 = (double)w4/mx;
			if(dw4 < 2.4)
				return 2;
			else if(dw4 > 2.6)
				return 8;
			else return -1;
		}

		if(ia1 == 4)
		{
			double dw1 = (double)w1/mx;
			if(dw1 < 2.4)
				return 2;
			else if(dw1 > 2.6)
				return 8;
			else return -1;
		}
	}

	if( (ia1==5 && ia2==5 && ia3==2) || (ia1==2 && ia2==5 && ia3==5) )
		return 3;

	if( (ia1==2 && ia2==4 && ia3==5) || (ia1==5 && ia2==4 && ia3==2) )
		return 4;

	if( (ia1==3 && ia2==5 && ia3==4) || (ia1==4 && ia2==5 && ia3==3) )
		return 5;

	if( (ia1==2 && ia2==2 && ia3==5) || (ia1==5 && ia2==2 && ia3==2) )
		return 6;

	if( (ia1==4 && ia2==2 && ia3==3) || (ia1==3 && ia2==2 && ia3==4) )
		return 9;

	return false;
}
