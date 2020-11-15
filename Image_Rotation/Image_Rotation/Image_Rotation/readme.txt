#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <stdio.h>
#include <iostream>
#include "ctype.h"
#include "math.h"

using namespace std;

#define Center_X 0   //旋转的中心的X坐标
#define Center_Y 0   //旋转的中心的Y坐标
#define Angle  10      //旋转的角度
double Sin_Angle;
double Cos_Angle;
IplImage *image = 0;

int  main(int argc,char **argv)
{
	Sin_Angle=sin(Angle*CV_PI/180);
	Cos_Angle=cos(Angle*CV_PI/180);

//	printf("Sin_Angle=%f;\nCos_Angle=%f\n",Sin_Angle,Cos_Angle);

	CvMemStorage* storage = cvCreateMemStorage(0); //用来创建一个内存存储器，来统一管理各种动态对象的内存。
	IplImage* Object_Color=cvLoadImage("hua6.jpg");//读取目标图片

	//IplImage* Object_Result=(IplImage*)malloc(4*cvRound(fabs(Object_Color->width*Sin_Angle)+fabs(Object_Color->height*Cos_Angle))*cvRound(fabs(Object_Color->width*Sin_Angle)+fabs(Object_Color->height*Cos_Angle)));
	IplImage* Object_Result=cvCreateImage(cvSize(10*cvRound(fabs(Object_Color->width*Cos_Angle)+fabs(Object_Color->height*Sin_Angle)),10*cvRound(fabs(Object_Color->width*Sin_Angle)+fabs(Object_Color->height*Cos_Angle))),8,3); //创建一个图片
	
	//cvNamedWindow("Object Color",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Object",CV_WINDOW_AUTOSIZE);

	CvScalar Temp;
	//cvSetImageROI(Object_Result,cvRect(0,0,Object_Color->width,Object_Color->height));//设置图片的感兴趣区域

	//cvCopy(Object_Color,Object_Result);//复制目标图片到新图像中
	for(int i=0;i<Object_Color->height;i++)
	{
		for(int j=0;j<Object_Color->width;j++)
		{
			if((Center_X+(i-Center_X)*Cos_Angle-(j-Center_Y)*Sin_Angle)>0&&
				(Center_Y+(i-Center_X)*Sin_Angle+(j-Center_Y)*Cos_Angle)>0&&
				(Center_X+(i-Center_X)*Cos_Angle-(j-Center_Y)*Sin_Angle)<=Object_Color->width&&
				(Center_Y+(i-Center_X)*Sin_Angle-(j-Center_Y)*Cos_Angle)<=Object_Color->height)
			{
				//Temp=cvGet2D(Object_Color,i,j);
				//printf("R=%d;\nG=%d;\nB=%d.\n",cvRound(Temp.val[0]),cvRound(Temp.val[1]),cvRound(Temp.val[2]));
				Temp=cvGet2D(Object_Color,cvRound(Center_X+(i-Center_X)*Cos_Angle-(j-Center_Y)*Sin_Angle),cvRound(Center_Y+(i-Center_X)*Sin_Angle+(j-Center_Y)*Cos_Angle));
				cvSet2D(Object_Result,i,j,Temp);
			}
		}
	}
	cvShowImage("Object Color",Object_Color);
	cvWaitKey(0);

	cvShowImage("Object",Object_Result);
	cvWaitKey(0);

	system("pause");
	return 0;
}
