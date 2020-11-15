#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int i,j,k;

void main()
{
	//动态地址访问方式
	//Mat img = imread("1.jpg");
	//imshow("src",img );
	//Mat dst = img.clone();
	//int rowNumber = img.rows;
	//int colNumber = img.cols;
	//for (i = 0;i < rowNumber;i++)
	//{
	//	for (j = 0;j < colNumber;j++)
	//	{
	//		dst.at<Vec3b>(i, j)[0] = 255;//蓝色通道
	//		dst.at<Vec3b>(i, j)[1] = 0;//绿色通道
	//		dst.at<Vec3b>(i, j)[2] = 0;//红色通道
	//	}
	//
	//}
	//imshow("dst",dst );
	//waitKey(0);

	//指针访问
	//Mat img = imread("1.jpg");
	//imshow("src",img );
	//Mat dst = img.clone();
	//int rowNumber = img.rows;//获取行数
	//int colNumber = img.cols*img.channels();//获取每一行的元素
	//for (i = 0;i < rowNumber;i++)
	//{
	//	uchar *data = dst.ptr<uchar>(i);//获取每一行的首地址
	//	for (j = 0;j < colNumber;j++)
	//	{
	//		switch (j%3)
	//		{
	//		case 0://蓝色通道
	//			data[j] = 255;
	//			break;
	//		case 1://绿色通道
	//			data[j] = 0;
	//			break;
	//		case 2://红色通道
	//			data[j] = 255;
	//			break;
	//		}
	//	}	
	//}
	//imshow("dst",dst );
	//waitKey(0);

	//减色效果
	//Mat img = imread("1.jpg");
	//imshow("src",img );
	//Mat dst = img.clone();
	//int rowNumber = img.rows;//获取行数
	//int colNumber = img.cols*img.channels();//获取每一行的元素
	//for (i = 0;i < rowNumber;i++)
	//{
	//	uchar *data = dst.ptr<uchar>(i);//获取每一行的首地址
	//	for (j = 0;j < colNumber;j++)
	//	{
	//		switch (j%3)
	//		{
	//		case 0://蓝色通道
	//			data[j] = data[j] / 64 * 64 + 64 / 2;
	//			break;
	//		case 1://绿色通道
	//			data[j] = data[j] / 64 * 64 + 64 / 2;
	//			break;
	//		case 2://红色通道
	//			data[j] = data[j] / 64 * 64 + 64 / 2;
	//			break;
	//		}
	//	}	
	//}
	//imshow("dst",dst );
	//waitKey(0);

	//雪花效果
	Mat img = imread("1.jpg");
	imshow("src",img );
	Mat dst = img.clone();
	int rowNumber = img.rows;
	int colNumber = img.cols;
	for (k = 0;k < 1000;k++)
	{
		i = rand() % rowNumber;
		j = rand() % colNumber;
		dst.at<Vec3b>(i, j)[0] = 255;//蓝色通道
		dst.at<Vec3b>(i, j)[1] = 255;//绿色通道
		dst.at<Vec3b>(i, j)[2] = 255;//红色通道
	}
	imshow("dst",dst );
	waitKey(0);

}

