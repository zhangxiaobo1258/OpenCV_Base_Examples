#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int i,j,k;

void main()
{
	//��̬��ַ���ʷ�ʽ
	//Mat img = imread("1.jpg");
	//imshow("src",img );
	//Mat dst = img.clone();
	//int rowNumber = img.rows;
	//int colNumber = img.cols;
	//for (i = 0;i < rowNumber;i++)
	//{
	//	for (j = 0;j < colNumber;j++)
	//	{
	//		dst.at<Vec3b>(i, j)[0] = 255;//��ɫͨ��
	//		dst.at<Vec3b>(i, j)[1] = 0;//��ɫͨ��
	//		dst.at<Vec3b>(i, j)[2] = 0;//��ɫͨ��
	//	}
	//
	//}
	//imshow("dst",dst );
	//waitKey(0);

	//ָ�����
	//Mat img = imread("1.jpg");
	//imshow("src",img );
	//Mat dst = img.clone();
	//int rowNumber = img.rows;//��ȡ����
	//int colNumber = img.cols*img.channels();//��ȡÿһ�е�Ԫ��
	//for (i = 0;i < rowNumber;i++)
	//{
	//	uchar *data = dst.ptr<uchar>(i);//��ȡÿһ�е��׵�ַ
	//	for (j = 0;j < colNumber;j++)
	//	{
	//		switch (j%3)
	//		{
	//		case 0://��ɫͨ��
	//			data[j] = 255;
	//			break;
	//		case 1://��ɫͨ��
	//			data[j] = 0;
	//			break;
	//		case 2://��ɫͨ��
	//			data[j] = 255;
	//			break;
	//		}
	//	}	
	//}
	//imshow("dst",dst );
	//waitKey(0);

	//��ɫЧ��
	//Mat img = imread("1.jpg");
	//imshow("src",img );
	//Mat dst = img.clone();
	//int rowNumber = img.rows;//��ȡ����
	//int colNumber = img.cols*img.channels();//��ȡÿһ�е�Ԫ��
	//for (i = 0;i < rowNumber;i++)
	//{
	//	uchar *data = dst.ptr<uchar>(i);//��ȡÿһ�е��׵�ַ
	//	for (j = 0;j < colNumber;j++)
	//	{
	//		switch (j%3)
	//		{
	//		case 0://��ɫͨ��
	//			data[j] = data[j] / 64 * 64 + 64 / 2;
	//			break;
	//		case 1://��ɫͨ��
	//			data[j] = data[j] / 64 * 64 + 64 / 2;
	//			break;
	//		case 2://��ɫͨ��
	//			data[j] = data[j] / 64 * 64 + 64 / 2;
	//			break;
	//		}
	//	}	
	//}
	//imshow("dst",dst );
	//waitKey(0);

	//ѩ��Ч��
	Mat img = imread("1.jpg");
	imshow("src",img );
	Mat dst = img.clone();
	int rowNumber = img.rows;
	int colNumber = img.cols;
	for (k = 0;k < 1000;k++)
	{
		i = rand() % rowNumber;
		j = rand() % colNumber;
		dst.at<Vec3b>(i, j)[0] = 255;//��ɫͨ��
		dst.at<Vec3b>(i, j)[1] = 255;//��ɫͨ��
		dst.at<Vec3b>(i, j)[2] = 255;//��ɫͨ��
	}
	imshow("dst",dst );
	waitKey(0);

}

