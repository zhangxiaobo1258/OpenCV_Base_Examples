#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	//计算点与轮廓的距离以及位置关系
/*	Mat srcImg = imread("1.jpg");
	imshow("src", srcImg);
	Mat dstImg = srcImg.clone();
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY);
	imshow("Threshold", srcImg);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
	
	for (int i = 0;i < contours.size();i++)
	{
		for (int j = 0;j < contours[i].size();j++)
		{
			cout << "(" << contours[i][j].x << "," << contours[i][j].y << ")" << endl;
		}
	}
	double a0 = pointPolygonTest(contours[0], Point(306, 96), true);
	double b0 = pointPolygonTest(contours[0], Point(306, 96), false);
	cout << "a0:" << a0 << endl;
	cout << "b0:" << b0 << endl;
	waitKey(0);*/

	

}
