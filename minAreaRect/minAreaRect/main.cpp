#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;

float angle;

void main()
{
	//������С��Ӿ��εĻ���
	/*	Mat srcImg = imread("hu.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	medianBlur(srcImg, srcImg, 3);
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY_INV);//��ֵ��
	imshow("threshold", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());//������Ӿ��εļ���
	vector<RotatedRect> box(contours.size());//������С��Ӿ��μ���
	Point2f rect[4];
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		box[i] = minAreaRect(Mat(contours[i]));//����ÿ��������С��Ӿ���
		boundRect[i] = boundingRect((Mat)contours[i]);
		
		circle(dstImg, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);//������С��Ӿ��ε����ĵ�

		box[i].points(rect);//points�����ĸ��˵㣬����С��Ӿ��ε��ĸ��˵㸴�Ƹ�rect����
		x = boundRect[i].x; 
		y = boundRect[i].y; 
		w = boundRect[i].width;
		h = boundRect[i].height;
		rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);
		for (int j = 0;j < 4;j++)
		{
			line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);//������С��Ӿ��ε�ÿ����
		}	
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/

	//���Լ�����������س���
	/*Mat srcImg = imread("1.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	medianBlur(srcImg, srcImg, 3);
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY_INV);//��ֵ��
	imshow("threshold", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());//������Ӿ��εļ���
	vector<RotatedRect> box(contours.size());//������С��Ӿ��μ���
	Point2f rect[4];
	for (int i = 0;i < contours.size();i++)
	{
		box[i] = minAreaRect(Mat(contours[i]));//����ÿ��������С��Ӿ���
		boundRect[i] = boundingRect((Mat)contours[i]);
		box[i].points(rect);

		char width[20], height[20];
		sprintf_s(width,"width=%0.2f", box[i].size.width);
		sprintf_s(height,"height=%0.2f", box[i].size.height);

		for (int j = 0;j < 4;j++)
		{
			line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);//������С��Ӿ��ε�ÿ����
		}
		putText(dstImg, width, Point(box[i].center.x, box[i].center.y), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.85, Scalar(0, 255, 0), 2, 8);
		putText(dstImg, height, Point(box[i].center.x, box[i].center.y+25), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.85, Scalar(0, 255, 0), 2, 8);
	}

	imshow("dst", dstImg);
	waitKey(0);*/

	//��б�������
	Mat srcImg = imread("mark.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	Canny(srcImg, srcImg, 100, 200);//ȥ��һЩ����Ҫ�ı�Ե
	imshow("CANNY", srcImg);
	Mat element = getStructuringElement(MORPH_RECT, Size(11, 11), Point(-1, -1));
	dilate(srcImg, srcImg, element);//���Ͳ���
	erode(srcImg, srcImg, element);
	imshow("dilate and erode", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());//������Ӿ��εļ���
	vector<RotatedRect> box(contours.size());//������С��Ӿ��μ���
	Point2f rect[4];
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		box[i] = minAreaRect(Mat(contours[i]));//����ÿ��������С��Ӿ���
		boundRect[i] = boundingRect((Mat)contours[i]);
		if (box[i].size.width < 50 || box[i].size.height < 50)
		{
			continue;
		}
		//circle(dstImg, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);//������С��Ӿ��ε����ĵ�
		angle = box[i].angle;
		box[i].points(rect);//points�����ĸ��˵㣬����С��Ӿ��ε��ĸ��˵㸴�Ƹ�rect����
		for (int j = 0;j < 4;j++)
		{
			line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);//������С��Ӿ��ε�ÿ����
		}
		//����ֵ
		if (0 < abs(angle) && abs(angle) < 45)//��ʱ��
			angle = angle;
		else if (45 < abs(angle) && abs(angle) < 90)//˳ʱ��
			angle = 90 - abs(angle);

		Point2f center = box[i].center;//������ת��������
		double angle0 = angle;
		double scale = 1;
		Mat roateM = getRotationMatrix2D(center, angle0, scale);
		warpAffine(dstImg, dstImg, roateM, dstImg.size());//����任
		//Ҳ������͸�ӱ任
	}
	imshow("boundRect", dstImg);
	waitKey(0);



}




