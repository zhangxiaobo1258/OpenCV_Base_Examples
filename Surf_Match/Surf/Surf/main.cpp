#include"cvsurf.h"
#include<iostream>
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<stdio.h>
#include<ctype.h>
#include "levenberg_marquardt.h"
#include <cstdlib>   //不加该头文件，system()会出现不明确的错误

using namespace std;

IplImage *image = 0;


int main(int argc, char** argv)
{
	CvMemStorage* storage = cvCreateMemStorage(0); //用来创建一个内存存储器，来统一管理各种动态对象的内存。
	//cvNamedWindow("The Gray Image", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("The SURF Features Image",CV_WINDOW_AUTOSIZE);
	//CvPoint2D32f Object_Center;
	//Object_Center=cvPoint2D32f(10,10);
	cvSURFInitialize();

	static CvScalar colors[] = 
	{
		{0,0,255},
		{0,128,255},
		{0,255,255},
		{0,255,0},
		{255,128,0},
		{255,255,0},
		{255,0,0},
		{255,0,255},
		{255,255,255}
	};
	IplImage* Object = cvLoadImage( "lena7-10.png", CV_LOAD_IMAGE_GRAYSCALE );
	IplImage* Object_Color=cvLoadImage("lena7-10.png");
	CvSeq* ModelDescriptors = cvSURFDescriptor( Object, storage, 4., CV_SURF_EXTENDED );
	printf("Object Descriptors: %d\n",ModelDescriptors->total);
	IplImage* Ref_Image = cvLoadImage( "lena2.png", CV_LOAD_IMAGE_GRAYSCALE );
	IplImage* Ref_Image_Color=cvLoadImage("lena2.png");
	CvSeq* RefDescriptors=cvSURFDescriptor(Ref_Image,storage,4.,CV_SURF_EXTENDED);
	printf("Refernce Image Descriptors:%d\n",RefDescriptors->total);

	CvSeq* Match_Point=cvSURFFindPair(ModelDescriptors,RefDescriptors,storage,CV_SURF_EXTENDED);

	printf("Match Point total is:%d\n",Match_Point->total);

	IplImage * Object_Rotation=cvCreateImage(cvSize(Ref_Image->width,Ref_Image->height),8,3);
	IplImage * Object_Move=cvCreateImage(cvSize(Ref_Image->width,Ref_Image->height),8,3);

	CvPoint* result1_Point=(CvPoint*)malloc(sizeof(CvPoint)*Match_Point->total);//必须对此指针变量进行初始化
	CvPoint* result2_Point=(CvPoint*)malloc(sizeof(CvPoint)*Match_Point->total);//必须对此指针变量进行初始化

	if(Match_Point!=0)
	{
		for(int i=0;i<Match_Point->total;i++)
		{
			CvSURFDescriptor* r1 =(CvSURFDescriptor*)cvGetSeqElem( Match_Point, i*2 );
			CvSURFDescriptor* r2 =(CvSURFDescriptor*)cvGetSeqElem( Match_Point, i*2+1 );
			result1_Point[i]=cvPoint(r1->x,r1->y);
			result2_Point[i]=cvPoint(r2->x,r2->y);
			printf("The Number %d match point is:(%d,%d),(%d,%d)\n" ,i+1,result1_Point[i].x,result1_Point[i].y,result2_Point[i].x,result2_Point[i].y);
		}
	}

	CvRatation_Parameter  Ratation_Value;
	Ratation_Value=cvLevenberg_Marquardt(result1_Point,result2_Point,Ref_Image->height,Ref_Image->width,Match_Point->total);
	printf("The ratation angle is %f,center is (%f,%f)\n",Ratation_Value.Thata*180/CV_PI,Ratation_Value.Center_X,Ratation_Value.Center_Y);



	Object_Move=cvMoveImage(Object_Color,(int)Ratation_Value.Center_X,(int)Ratation_Value.Center_Y);//图片的平移

	Object_Rotation=cvRotation(Object_Move,Ratation_Value.Thata); //图片的旋转

	//Object_Rotation=cvRotationMoveImage(Object_Color,Ratation_Value );

	CvPoint points[4];
	points[0] = cvPoint( 0, 0 );
	points[1] = cvPoint( Object->width, 0 );
	points[2] = cvPoint( Object->width, Object->height );
	points[3] = cvPoint( 0, Object->height );

	//int t = 0;
	for(int  i = 0; i < ModelDescriptors->total; i++ )
	{
		CvSURFPoints* r = (CvSURFPoints*)cvGetSeqElem( ModelDescriptors, i );
		CvSURFDescriptor* r0=(CvSURFDescriptor*) cvGetSeqElem( ModelDescriptors, i );
		CvPoint center;
		int radius;
		center.x = cvRound(r->x);
		center.y = cvRound(r->y);
		radius = cvRound(r->size*1.2/9.*2);
		cvCircle( Object_Color, center, r0->mod, colors[5], 1, 8, 0 );//在图片中画圈
	}
	cvShowImage( "The SURF Features Image Obj", Object_Rotation );//显示SURF算法得出的特征点
	cvWaitKey(0);//在显示图像时必须有该指令才能进行显示

	for (int  i = 0; i < RefDescriptors->total; i++ )
	{
		CvSURFPoints* r = (CvSURFPoints*)cvGetSeqElem( RefDescriptors, i );
		CvSURFDescriptor* r0=(CvSURFDescriptor*) cvGetSeqElem( RefDescriptors, i );
		CvPoint center;
		int radius;
		center.x = cvRound(r->x);
		center.y = cvRound(r->y);
		radius = cvRound(r->size*1.2/9.*2);
		cvCircle( Ref_Image_Color, center, r0->mod, colors[6], 1, 8, 0 );//在图片中画圈
	}
	cvShowImage( "The SURF Features Image Ref", Ref_Image_Color );//显示SURF算法得出的特征点
	cvWaitKey(0);//在显示图像时必须有该指令才能进行显示
	cvDestroyWindow("The SURF Features Image Obj");
	cvDestroyWindow("The SURF Features Image Ref");
	free(result1_Point);
	free(result2_Point);

	system("pause");
	return 0;
}