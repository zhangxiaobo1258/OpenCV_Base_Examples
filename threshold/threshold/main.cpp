#include "opencv2/opencv.hpp"

using namespace cv;
	
Mat srcImg,dstImg;
int threshold_min = 0;
int block_size = 3;
int C0=3;
//void OnThreshold(int, void *)
//{
//	threshold(srcImg, dstImg, threshold_min, 255, CV_THRESH_BINARY);
//	imshow("src",srcImg);
//	imshow("固定阈值", dstImg);
//}

void OnAdaptiveThreshold(int, void *)
{
	if (block_size % 2 == 0)
	{
		block_size++;
	}
	adaptiveThreshold(srcImg,dstImg,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,block_size,C0);
	imshow("src",srcImg);
	imshow("自适应阈值", dstImg);
}

void main()
{
	srcImg = imread("1.jpg",0);//灰度图像

	namedWindow("固定阈值", CV_WINDOW_AUTOSIZE);
	namedWindow("自适应阈值", CV_WINDOW_AUTOSIZE);

	//threshold(srcImg, dstImg, 100, 255, CV_THRESH_BINARY);
	//threshold(srcImg, dstImg, 100, 255, CV_THRESH_BINARY_INV);
	//threshold(srcImg, dstImg, 100, 255, CV_THRESH_TRUNC);
	//threshold(srcImg, dstImg, 100, 255, CV_THRESH_TOZERO);
	//threshold(srcImg, dstImg, 100, 255, CV_THRESH_TOZERO_INV);

	//adaptiveThreshold(srcImg,dstImg,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,11,5);
	//adaptiveThreshold(srcImg,dstImg,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,11,5);
	//createTrackbar("Threshold", "固定阈值", &threshold_min, 255,OnThreshold , 0);
	//OnThreshold(threshold_min,0);

	createTrackbar("adaptiveThreshold", "自适应阈值", &block_size, 255,OnAdaptiveThreshold , 0);
	createTrackbar("C", "自适应阈值", &C0, 255,OnAdaptiveThreshold , 0);
	OnAdaptiveThreshold(block_size,0);
	OnAdaptiveThreshold(C0,0);
	//imshow("src",srcImg);
	//imshow("dst", dstImg);

	waitKey(0);
}