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
	bool hDataNew = NULL; //��ֵΪNULL
	bool m_isBitmap = 0;
	//��Ա����
	int ImageArray[1000][1000];
//	BYTE ** ImageArray;				//ԭʼ��ͼ���������飭�Ҷ�
//	RGBQUAD** m_tOriPixelArray;		//ԭʼλͼ����

	long ImageHeight;				//ԭͼ��ĸ߶�
	long ImageWidth;				//ԭͼ��Ŀ��

	int arPixelH[1000] = {0,0};				//Ϊ��ˮƽ����ֱ��ͼͳ����
	int arPixelV[1000] = {0,0};				//Ϊ�˴�ֱ����ֱ��ͼͳ����
	int arWidth[200] = {0,0};				//���������У�������ߵĺ�����ʼ
	int arDifference[1000] = {0,0};			//���
	bool arMark[1000] = {0,0};				//���
	int ImageLeft;					//ͼ������ߵ�����
	int ImageRight;					//ͼ�����ұߵ�����
	int ImageTop = 0;					//�����붥��������
	int ImageBottom = 0;				//�������²�������
	int arDelta[1000] = {0,0};
	int arLeftEdge1[1000] = {0,0};
	int arLeftEdge2[1000] = {0,0};
	int arLeftEdge[1000] = {0,0};
	CString strCodeStyle;
	CString strCodeNumber;
}

