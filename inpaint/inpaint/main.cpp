#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/photo/photo.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	//Í¼ÏñÐÞ²¹
	Mat srcImg = imread("2.jpg");
	imshow("src", srcImg);
	Mat mask(srcImg.size(), CV_8U, Scalar(0));
	rectangle(mask, Point(357, 967), Point(669, 984), Scalar(255), -1, 8);

	Mat Result;
	inpaint(srcImg, mask, Result, 5, CV_INPAINT_NS);
	imshow("result", Result);
	imwrite("2.jpg", Result);
	waitKey(0);
}