#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat srcImg;
Mat hvsImg;
int h_min = 0, s_min = 0, v_min = 0;
int h_max = 180, s_max = 255, v_max = 46;

void OnChange(int, void * param)
{
	Scalar hsv_min(h_min, s_min, v_min);
	Scalar hsv_max(h_max, s_max, v_max);
	Mat dstImg = Mat::zeros(srcImg.rows, srcImg.cols, CV_8U);
	inRange(hvsImg, hsv_min, hsv_max, dstImg);

	imshow("HSV", dstImg);

}

void main()
{
	//识别图片中的颜色
	srcImg = imread("1.jpg");
	imshow("src", srcImg);
	cvtColor(srcImg, hvsImg, CV_BGR2HSV);//转换到HSV颜色空间
	namedWindow("HSV", CV_WINDOW_AUTOSIZE);//创建滑动条
	createTrackbar("H_MIN", "HSV", &h_min, 180, OnChange, 0);
	createTrackbar("S_MIN", "HSV", &s_min, 255, OnChange, 0);
	createTrackbar("V_MIN", "HSV", &v_min, 255, OnChange, 0);
	createTrackbar("H_MAX", "HSV", &h_max, 180, OnChange, 0);
	createTrackbar("S_MAX", "HSV", &s_max, 255, OnChange, 0);
	createTrackbar("V_MAX", "HSV", &v_max, 255, OnChange, 0);

	OnChange(h_min, 0);
	OnChange(s_min, 0);
	OnChange(v_min, 0);
	OnChange(h_max, 0);
	OnChange(s_max, 0);
	OnChange(v_max, 0);

	waitKey(0);
}