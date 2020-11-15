#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;

float angle;

void main()
{
	//轮廓最小外接矩形的绘制
	/*	Mat srcImg = imread("hu.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	medianBlur(srcImg, srcImg, 3);
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY_INV);//二值化
	imshow("threshold", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());//定义外接矩形的集合
	vector<RotatedRect> box(contours.size());//定义最小外接矩形集合
	Point2f rect[4];
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		box[i] = minAreaRect(Mat(contours[i]));//计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect((Mat)contours[i]);
		
		circle(dstImg, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);//绘制最小外接矩形的中心点

		box[i].points(rect);//points返回四个端点，把最小外接矩形的四个端点复制给rect数组
		x = boundRect[i].x; 
		y = boundRect[i].y; 
		w = boundRect[i].width;
		h = boundRect[i].height;
		rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);
		for (int j = 0;j < 4;j++)
		{
			line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);//绘制最小外接矩形的每条边
		}	
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/

	//粗略计算物体的像素长宽
	/*Mat srcImg = imread("1.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	medianBlur(srcImg, srcImg, 3);
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY_INV);//二值化
	imshow("threshold", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());//定义外接矩形的集合
	vector<RotatedRect> box(contours.size());//定义最小外接矩形集合
	Point2f rect[4];
	for (int i = 0;i < contours.size();i++)
	{
		box[i] = minAreaRect(Mat(contours[i]));//计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect((Mat)contours[i]);
		box[i].points(rect);

		char width[20], height[20];
		sprintf_s(width,"width=%0.2f", box[i].size.width);
		sprintf_s(height,"height=%0.2f", box[i].size.height);

		for (int j = 0;j < 4;j++)
		{
			line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);//绘制最小外接矩形的每条边
		}
		putText(dstImg, width, Point(box[i].center.x, box[i].center.y), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.85, Scalar(0, 255, 0), 2, 8);
		putText(dstImg, height, Point(box[i].center.x, box[i].center.y+25), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.85, Scalar(0, 255, 0), 2, 8);
	}

	imshow("dst", dstImg);
	waitKey(0);*/

	//倾斜物体矫正
	Mat srcImg = imread("mark.jpg");
	imshow("source", srcImg);
	Mat dstImg = srcImg.clone();
	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0);
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	Canny(srcImg, srcImg, 100, 200);//去除一些不需要的边缘
	imshow("CANNY", srcImg);
	Mat element = getStructuringElement(MORPH_RECT, Size(11, 11), Point(-1, -1));
	dilate(srcImg, srcImg, element);//膨胀操作
	erode(srcImg, srcImg, element);
	imshow("dilate and erode", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	cout << "num:" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());//定义外接矩形的集合
	vector<RotatedRect> box(contours.size());//定义最小外接矩形集合
	Point2f rect[4];
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		box[i] = minAreaRect(Mat(contours[i]));//计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect((Mat)contours[i]);
		if (box[i].size.width < 50 || box[i].size.height < 50)
		{
			continue;
		}
		//circle(dstImg, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);//绘制最小外接矩形的中心点
		angle = box[i].angle;
		box[i].points(rect);//points返回四个端点，把最小外接矩形的四个端点复制给rect数组
		for (int j = 0;j < 4;j++)
		{
			line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);//绘制最小外接矩形的每条边
		}
		//经验值
		if (0 < abs(angle) && abs(angle) < 45)//逆时针
			angle = angle;
		else if (45 < abs(angle) && abs(angle) < 90)//顺时针
			angle = 90 - abs(angle);

		Point2f center = box[i].center;//定义旋转中心坐标
		double angle0 = angle;
		double scale = 1;
		Mat roateM = getRotationMatrix2D(center, angle0, scale);
		warpAffine(dstImg, dstImg, roateM, dstImg.size());//仿射变换
		//也可以用透视变换
	}
	imshow("boundRect", dstImg);
	waitKey(0);



}




