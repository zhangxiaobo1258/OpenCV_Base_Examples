#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;


void main()
{
	//��С���Բ
/*	Mat srcImg = imread("mark.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	Canny(srcImg, srcImg, 100, 200);//ȥ��һЩ����Ҫ�ı�Ե
	imshow("CANNY", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	Point2f center;
	float radius;
	for (int i = 0;i < contours.size();i++)
	{
		minEnclosingCircle(Mat(contours[i]), center, radius);
		drawContours(dstImg, contours, i, Scalar(0, 0, 255), 2, 8);
		circle(dstImg, center, radius, Scalar(0, 255, 0), 2, 8);//���Ƶ�i��������С���Բ
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/

	//������Բ���
	/*Mat srcImg = imread("mark.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	Canny(srcImg, srcImg, 100, 200);//ȥ��һЩ����Ҫ�ı�Ե
	imshow("CANNY", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<RotatedRect> box(contours.size());
	Point2f rect[4];
	for (int i = 0;i < contours.size();i++)
	{
		box[i] = fitEllipse(Mat(contours[i]));
		ellipse(dstImg, box[i], Scalar(0, 255, 0), 2, 8);
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/
	//��������αƽ�
	/*Mat srcImg = imread("1.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	Canny(srcImg, srcImg, 100, 200);//ȥ��һЩ����Ҫ�ı�Ե
	imshow("CANNY", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<vector<Point>> contours_poly(contours.size());
	for (int i = 0;i < contours.size();i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 15, true);
		//drawContours(dstImg, contours, i, Scalar(0, 255, 0), 2, 8);
		drawContours(dstImg, contours_poly, i, Scalar(0, 255, 255), 2, 8);
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/
	//��������������ͳ���
	/*Mat srcImg = imread("1.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	Mat dstImg2(srcImg.size(), CV_8UC3, Scalar::all(0));
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	Canny(srcImg, srcImg, 100, 200);//ȥ��һЩ����Ҫ�ı�Ե
	imshow("CANNY", srcImg);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;

	for (int i = 0;i < contours.size();i++)
	{
		double area = contourArea(contours[i]);
		cout << "area<" << i << ">  =" << area << endl;
		//double length = arcLength(contours[i], true);
		//drawContours(dstImg, contours, i, Scalar(0, 255, 0), 2, 8);
		//if(length>300)
			drawContours(dstImg, contours, i, Scalar(0, 255, 255), 2, 8);
	}
	imshow("dst", dstImg);
	waitKey(0);*/

	//��ȡ����������
	/*Mat srcImg = imread("1.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	Mat tempImg = srcImg.clone();
	Mat tempImg2(srcImg.size(), CV_8UC3, Scalar::all(0));
	Mat draw(srcImg.size(), CV_8UC3, Scalar::all(0));
	Mat tempImg3(srcImg.size(), CV_8UC3, Scalar::all(0));
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	Canny(srcImg, srcImg, 100, 200);//ȥ��һЩ����Ҫ�ı�Ե
	imshow("CANNY", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	while (1)
	{
		for (int i = 0;i < contours.size();i++)
		{
			tempImg2.copyTo(draw);//ÿ�ν���֮�����drawΪȫ��
			tempImg2.copyTo(tempImg3);//ÿ�ν���֮�����tempImg3Ϊȫ��
			drawContours(draw, contours, i, Scalar(255, 255, 255), -1, 8);
			
			Mat mask;
			cvtColor(draw, mask, CV_BGR2GRAY);//����������ת��Ϊ����
			tempImg.copyTo(tempImg3, mask);//��tempImg���Ƶ�tempImg3��ֻ�����벿�ֱ����ơ�
			imshow("dst", draw);
			imshow("result", tempImg3);
			char key=waitKey();
			if (key == 27)
				break;
		}
		break;
	}*/
	//����ƥ��
	Mat srcImg = imread("1.jpg");//��ȡģ��ͼ��
	imshow("source", srcImg);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	threshold(srcImg, srcImg, 100, 255,CV_THRESH_BINARY);//ȥ��һЩ����Ҫ�ı�Ե
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat srcImg2 = imread("2.jpg");//��ȡ������ͼ��
	Mat dstImg = srcImg2.clone();
	imshow("dst", srcImg2);
	cvtColor(srcImg2, srcImg2, CV_BGR2GRAY);
	threshold(srcImg2, srcImg2, 100, 255,CV_THRESH_BINARY);//ȥ��һЩ����Ҫ�ı�Ե
	vector<vector<Point>> contours2;
	vector<Vec4i> hierarcy2;
	findContours(srcImg2, contours2, hierarcy2, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);

	while (1)
	{
		for (int i = 0;i < contours2.size();i++)
		{
			double matchRate = matchShapes(contours[0], contours2[i], CV_CONTOURS_MATCH_I1, 0.0);
			cout << "index=" << i << "----" << setiosflags(ios::fixed) << matchRate << endl;//setiosflags�޶�Ϊ�������
			if (matchRate <= 0.1)
			{
				drawContours(dstImg, contours2, i, Scalar(0, 255, 0), 2, 8);
			}
			imshow("dst", dstImg);
			char key=waitKey();
			if (key == 27)
				break;
		} 
		break;
	}
	waitKey(0);
}
